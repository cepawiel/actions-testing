/* KallistiOS ##version##

   kernel/net/net_ipv6.c
   Copyright (C) 2010 Lawrence Sebald

*/

#include <string.h>
#include <netinet/in.h>
#include <kos/net.h>
#include <errno.h>

#include "net_ipv6.h"
#include "net_icmp6.h"
#include "net_ipv4.h"

static net_ipv6_stats_t ipv6_stats = { 0 };
const struct in6_addr in6addr_any = IN6ADDR_ANY_INIT;
const struct in6_addr in6addr_loopback = IN6ADDR_LOOPBACK_INIT;

/* These few aren't in the IEEE 1003.1-2008 spec, but do appear on (at least)
   Mac OS X (in not strictly compliant mode) and are useful for us... */
const struct in6_addr in6addr_linklocal_allnodes =
    {{{ 0xff, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 }}};
const struct in6_addr in6addr_linklocal_allrouters =
    {{{ 0xff, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02 }}};

static int is_in_network(netif_t *net, const struct in6_addr *ip) {
    int i;

    /* Make sure its not trivially link-local */
    if(IN6_IS_ADDR_LINKLOCAL(ip)) {
        return 1;
    }

    /* Go through each prefix, and see if any match */
    for(i = 0; i < net->ip6_addr_count; ++i) {
        if(!memcmp(ip, &net->ip6_addrs[i], 8)) {
            return 1;
        }
    }

    return 0;
}

/* Send a packet on the specified network adapter */
int net_ipv6_send_packet(netif_t *net, ipv6_hdr_t *hdr, const uint8 *data,
                         int data_size) {
    uint8 pkt[data_size + sizeof(ipv6_hdr_t) + sizeof(eth_hdr_t)];
    uint8 dst_mac[6];
    int err;
    struct in6_addr dst = hdr->dst_addr;
    eth_hdr_t *ehdr;

    if(!net) {
        net = net_default_dev;

        if(!net) {
            return -1;
        }
    }

    /* Are we sending a packet to loopback? */
    if(IN6_IS_ADDR_LOOPBACK(&hdr->dst_addr)) {
        memcpy(pkt, hdr, sizeof(ipv6_hdr_t));
        memcpy(pkt + sizeof(ipv6_hdr_t), data, data_size);

        ++ipv6_stats.pkt_sent;

        /* Send the packet "away" */
        net_ipv6_input(NULL, pkt, sizeof(ipv6_hdr_t) + data_size);
        return 0;
    }
    else if(IN6_IS_ADDR_MULTICAST(&hdr->dst_addr)) {
        dst_mac[0] = dst_mac[1] = 0x33;
        dst_mac[2] = hdr->dst_addr.__s6_addr.__s6_addr8[12];
        dst_mac[3] = hdr->dst_addr.__s6_addr.__s6_addr8[13];
        dst_mac[4] = hdr->dst_addr.__s6_addr.__s6_addr8[14];
        dst_mac[5] = hdr->dst_addr.__s6_addr.__s6_addr8[15];
    }
    else {
        if(!is_in_network(net, &dst)) {
            dst = net->ip6_gateway;
        }

        err = net_ndp_lookup(net, &dst, dst_mac, hdr, data, data_size);

        if(err == -1) {
            errno = ENETUNREACH;
            ++ipv6_stats.pkt_send_failed;
            return err;
        }
        else if(err == -2) {
            ++ipv6_stats.pkt_send_failed;
            return err;
        }
    }

    /* Fill in the ethernet header */
    ehdr = (eth_hdr_t *)pkt;
    memcpy(ehdr->dest, dst_mac, 6);
    memcpy(ehdr->src, net->mac_addr, 6);
    ehdr->type[0] = 0x86;
    ehdr->type[1] = 0xDD;

    /* Put the IP header / data into our ethernet packet */
    memcpy(pkt + sizeof(eth_hdr_t), hdr, sizeof(ipv6_hdr_t));
    memcpy(pkt + sizeof(eth_hdr_t) + sizeof(ipv6_hdr_t), data, data_size);

    ++ipv6_stats.pkt_sent;

    /* Send it away */
    net->if_tx(net, pkt, sizeof(ipv6_hdr_t) + data_size + sizeof(eth_hdr_t),
               NETIF_BLOCK);

    return 0;
}

int net_ipv6_send(netif_t *net, const uint8 *data, int data_size, int hop_limit,
                  int proto, const struct in6_addr *src,
                  const struct in6_addr *dst) {
    ipv6_hdr_t hdr;

    if(!net) {
        net = net_default_dev;

        if(!net) {
            return -1;
        }
    }

    hdr.version_lclass = 0x60;
    hdr.hclass_lflow = 0;
    hdr.lclass = 0;
    hdr.length = ntohs(data_size);
    hdr.next_header = proto;

    if(hop_limit) {
        hdr.hop_limit = hop_limit;
    }
    else if(net->hop_limit) {
        hdr.hop_limit = net->hop_limit;
    }
    else {
        hdr.hop_limit = 255;
    }

    hdr.src_addr = *src;
    hdr.dst_addr = *dst;

    /* XXXX: Handle fragmentation... */
    return net_ipv6_send_packet(net, &hdr, data, data_size);
}

int net_ipv6_input(netif_t *src, const uint8 *pkt, int pktsize) {
    ipv6_hdr_t *ip;
    uint8 next_hdr;
    int pos, len;

    if(pktsize < sizeof(ipv6_hdr_t)) {
        /* This is obviously a bad packet, drop it */
        ++ipv6_stats.pkt_recv_bad_size;
        return -1;
    }

    ip = (ipv6_hdr_t *)pkt;
    len = ntohs(ip->length);

    if(pktsize < len + sizeof(ipv6_hdr_t)) {
        /* The packet is of size less than the payload length + the size of a
           minimal IPv6 header; it must be bad, drop it */
        ++ipv6_stats.pkt_recv_bad_size;
        return -1;
    }

    /* Parse the header to find the payload */
    pos = sizeof(ipv6_hdr_t);
    next_hdr = ip->next_header;

    /* XXXX: Parse options and deal with fragmentation */
    switch(next_hdr) {
        case IPV6_HDR_ICMP:
            return net_icmp6_input(src, ip, pkt + sizeof(ipv6_hdr_t), len);

        default:
            /* XXXX: Send a parameter problem */
            return -1;
    }

    return 0;
}

net_ipv6_stats_t net_ipv6_get_stats() {
    return ipv6_stats;
}

uint16 net_ipv6_checksum_pseudo(const struct in6_addr *src,
                                const struct in6_addr *dst,
                                uint32 upper_len, uint8 next_hdr) {
    ipv6_pseudo_hdr_t ps = {{{{ 0 }}}};

    /* Since the src and dst addresses aren't necessarily aligned when we send
       them in from header processing, do this the hard way. */
    memcpy(&ps.src_addr, src, sizeof(struct in6_addr));
    memcpy(&ps.dst_addr, dst, sizeof(struct in6_addr));
    ps.upper_layer_len = htonl(upper_len);
    ps.next_header = next_hdr;

    return ~net_ipv4_checksum((uint8 *)&ps, sizeof(ipv6_pseudo_hdr_t), 0);
}

int net_ipv6_init() {
    /* Make sure we're registered to get "All nodes" multicasts from the
       ethernet layer. */
    uint8 mac[6] = { 0x33, 0x33, 0x00, 0x00, 0x00, 0x01 };
    net_multicast_add(mac);

    /* Also register for the one for our link-local address' solicited nodes
       group (which will do the same for all our other addresses too). */
    mac[2] = 0xFF;
    mac[3] = net_default_dev->ip6_lladdr.s6_addr[13];
    mac[4] = net_default_dev->ip6_lladdr.s6_addr[14];
    mac[5] = net_default_dev->ip6_lladdr.s6_addr[15];
    net_multicast_add(mac);

    return 0;
}

void net_ipv6_shutdown() {
    uint8 mac[6] = { 0x33, 0x33, 0x00, 0x00, 0x00, 0x01 };

    /* Remove from the all nodes multicast group */
    net_multicast_del(mac);

    /* ... and our solicited nodes multicast group */
    mac[2] = 0xFF;
    mac[3] = net_default_dev->ip6_lladdr.s6_addr[13];
    mac[4] = net_default_dev->ip6_lladdr.s6_addr[14];
    mac[5] = net_default_dev->ip6_lladdr.s6_addr[15];
    net_multicast_del(mac);
}
