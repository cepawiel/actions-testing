/* KallistiOS ##version##

   dc/net/broadband_adapter.h
   Copyright (C) 2001-2002 Megan Potter

*/

/** \file   dc/net/broadband_adapter.h
    \brief  Broadband Adapter support.

    This file contains declarations related to support for the HIT-0400
    "Broadband Adapter". There's not really anything that users will generally
    have to deal with in here.

    \author Megan Potter
*/

#ifndef __DC_NET_BROADBAND_ADAPTER_H
#define __DC_NET_BROADBAND_ADAPTER_H

#include <sys/cdefs.h>
__BEGIN_DECLS

/** \defgroup bba_regs  RTL8139C Register Definitions

    The default assumption is that these are all RW at any aligned size unless 
    otherwise noted. ex (RW 32bit, RO 16/8) indicates read/write at 32bit and 
    read-only at 16 or 8bits.

    @{
*/
#define RT_IDR0             0x00    /**< \brief MAC address 0 (RW 32bit, RO 16/8) */
#define RT_IDR1             0x01    /**< \brief MAC address 1 (Read-only) */
#define RT_IDR2             0x02    /**< \brief MAC address 2 (Read-only) */
#define RT_IDR3             0x03    /**< \brief MAC address 3 (Read-only) */
#define RT_IDR4             0x04    /**< \brief MAC address 4 (RW 32bit, RO 16/8) */
#define RT_IDR5             0x05    /**< \brief MAC address 5 (Read-only) */
#define RT_RES06            0x06    /**< \brief Reserved */
#define RT_RES07            0x07    /**< \brief Reserved */
#define RT_MAR0             0x08    /**< \brief Multicast filter 0 (RW 32bit, RO 16/8) */
#define RT_MAR1             0x09    /**< \brief Multicast filter 1 (Read-only) */
#define RT_MAR2             0x0A    /**< \brief Multicast filter 2 (Read-only) */
#define RT_MAR3             0x0B    /**< \brief Multicast filter 3 (Read-only) */
#define RT_MAR4             0x0C    /**< \brief Multicast filter 4 (RW 32bit, RO 16/8) */
#define RT_MAR5             0x0D    /**< \brief Multicast filter 5 (Read-only) */
#define RT_MAR6             0x0E    /**< \brief Multicast filter 6 (Read-only) */
#define RT_MAR7             0x0F    /**< \brief Multicast filter 7 (Read-only) */
#define RT_TXSTATUS0        0x10    /**< \brief Transmit status 0 (32bit only) */
#define RT_TXSTATUS1        0x14    /**< \brief Transmit status 1 (32bit only) */
#define RT_TXSTATUS2        0x18    /**< \brief Transmit status 2 (32bit only) */
#define RT_TXSTATUS3        0x1C    /**< \brief Transmit status 3 (32bit only) */
#define RT_TXADDR0          0x20    /**< \brief Tx descriptor 0 (32bit only) */
#define RT_TXADDR1          0x24    /**< \brief Tx descriptor 1 (32bit only) */
#define RT_TXADDR2          0x28    /**< \brief Tx descriptor 2 (32bit only) */
#define RT_TXADDR3          0x2C    /**< \brief Tx descriptor 3 (32bit only) */
#define RT_RXBUF            0x30    /**< \brief Receive buffer start address (32bit only) */
#define RT_RXEARLYCNT       0x34    /**< \brief Early Rx byte count (RO 16bit) */
#define RT_RXEARLYSTATUS    0x36    /**< \brief Early Rx status (RO) */
#define RT_CHIPCMD          0x37    /**< \brief Command register */
#define RT_RXBUFTAIL        0x38    /**< \brief Current address of packet read (queue tail) (16bit only) */
#define RT_RXBUFHEAD        0x3A    /**< \brief Current buffer address (queue head) (RO 16bit) */
#define RT_INTRMASK         0x3C    /**< \brief Interrupt mask (16bit only) */
#define RT_INTRSTATUS       0x3E    /**< \brief Interrupt status (16bit only) */
#define RT_TXCONFIG         0x40    /**< \brief Tx config (32bit only) */
#define RT_RXCONFIG         0x44    /**< \brief Rx config (32bit only) */
#define RT_TIMER            0x48    /**< \brief A general purpose counter, any write clears (32bit only) */
#define RT_RXMISSED         0x4C    /**< \brief 24 bits valid, write clears (32bit only) */
#define RT_CFG9346          0x50    /**< \brief 93C46 command register */
#define RT_CONFIG0          0x51    /**< \brief Configuration reg 0 */
#define RT_CONFIG1          0x52    /**< \brief Configuration reg 1 */
#define RT_RES53            0x53    /**< \brief Reserved */
#define RT_TIMERINT         0x54    /**< \brief Timer interrupt register (32bit only) */
#define RT_MEDIASTATUS      0x58    /**< \brief Media status register */
#define RT_CONFIG3          0x59    /**< \brief Config register 3 */
#define RT_CONFIG4          0x5A    /**< \brief Config register 4 */
#define RT_RES5B            0x5B    /**< \brief Reserved */
#define RT_MULTIINTR        0x5C    /**< \brief Multiple interrupt select (32bit only) */
#define RT_RERID            0x5E    /**< \brief PCI Revision ID (10h) (Read-only) */
#define RT_RES5F            0x5F    /**< \brief Reserved */
#define RT_MII_TSAD         0x60    /**< \brief Transmit status of all descriptors (RO 16bit) */
#define RT_MII_BMCR         0x62    /**< \brief Basic Mode Control Register (16bit only) */
#define RT_MII_BMSR         0x64    /**< \brief Basic Mode Status Register (RO 16bit) */
#define RT_AS_ADVERT        0x66    /**< \brief Auto-negotiation advertisement reg (16bit only) */
#define RT_AS_LPAR          0x68    /**< \brief Auto-negotiation link partner reg (RO 16bit) */
#define RT_AS_EXPANSION     0x6A    /**< \brief Auto-negotiation expansion reg (RO 16bit) */

#define RT_CONFIG5          0xD8    /**< \brief Config register 5 */
/** @} */

/** \defgroup bba_miicb RTL8139C MII (media independent interface) control bits
    @{
*/
#define RT_MII_RESET       0x8000  /**< \brief Reset the MII chip */
#define RT_MII_RES4000     0x4000  /**< \brief Reserved */
#define RT_MII_SPD_SET     0x2000  /**< \brief 1 for 100 0 for 10. Ignored if AN enabled. */
#define RT_MII_AN_ENABLE   0x1000  /**< \brief Enable auto-negotiation */
#define RT_MII_RES0800     0x0800  /**< \brief Reserved */
#define RT_MII_RES0400     0x0400  /**< \brief Reserved */
#define RT_MII_AN_START    0x0200  /**< \brief Start auto-negotiation */
#define RT_MII_DUPLEX      0x0100  /**< \brief 1 for full 0 for half. Ignored if AN enabled. */

/** @} */

/** \defgroup bba_miisb RTL8139C MII (media independent interface) status bits
    @{
*/
#define RT_MII_LINK         0x0004  /**< \brief Link is present */
#define RT_MII_AN_CAPABLE   0x0008  /**< \brief Can do auto negotiation */
#define RT_MII_AN_COMPLETE  0x0020  /**< \brief Auto-negotiation complete */
#define RT_MII_10_HALF      0x0800  /**< \brief Can do 10Mbit half duplex */
#define RT_MII_10_FULL      0x1000  /**< \brief Can do 10Mbit full */
#define RT_MII_100_HALF     0x2000  /**< \brief Can do 100Mbit half */
#define RT_MII_100_FULL     0x4000  /**< \brief Can do 100Mbit full */
/** @} */

/** \defgroup bba_cbits RTL8139C Command Bits

    OR appropriate bit values together and write into the RT_CHIPCMD register to
    execute the command.

    @{
*/
#define RT_CMD_RESET        0x10 /**< \brief Reset the RTL8139C */
#define RT_CMD_RX_ENABLE    0x08 /**< \brief Enable Rx */
#define RT_CMD_TX_ENABLE    0x04 /**< \brief Enable Tx */
#define RT_CMD_RX_BUF_EMPTY 0x01 /**< \brief Empty the Rx buffer */
/** @} */

/** \defgroup bba_ibits RTL8139C Interrupt Status bits
    @{
*/
#define RT_INT_PCIERR           0x8000  /**< \brief PCI Bus error */
#define RT_INT_TIMEOUT          0x4000  /**< \brief Set when TCTR reaches TimerInt value */
#define RT_INT_RXFIFO_OVERFLOW  0x0040  /**< \brief Rx FIFO overflow */
#define RT_INT_RXFIFO_UNDERRUN  0x0020  /**< \brief Packet underrun / link change */
#define RT_INT_LINK_CHANGE      0x0020  /**< \brief Packet underrun / link change */
#define RT_INT_RXBUF_OVERFLOW   0x0010  /**< \brief Rx BUFFER overflow */
#define RT_INT_TX_ERR           0x0008  /**< \brief Tx error */
#define RT_INT_TX_OK            0x0004  /**< \brief Tx OK */
#define RT_INT_RX_ERR           0x0002  /**< \brief Rx error */
#define RT_INT_RX_OK            0x0001  /**< \brief Rx OK */

/** \brief  Composite RX bits we check for while doing an RX interrupt. */
#define RT_INT_RX_ACK (RT_INT_RXFIFO_OVERFLOW | RT_INT_RXBUF_OVERFLOW | RT_INT_RX_OK)
/** @} */

/** \defgroup bba_tbits RTL8139C transmit status bits
    @{
*/
#define RT_TX_CARRIER_LOST  0x80000000  /**< \brief Carrier sense lost */
#define RT_TX_ABORTED       0x40000000  /**< \brief Transmission aborted */
#define RT_TX_OUT_OF_WINDOW 0x20000000  /**< \brief Out of window collision */
#define RT_TX_STATUS_OK     0x00008000  /**< \brief Status ok: a good packet was transmitted */
#define RT_TX_UNDERRUN      0x00004000  /**< \brief Transmit FIFO underrun */
#define RT_TX_HOST_OWNS     0x00002000  /**< \brief Set to 1 when DMA operation is completed */
#define RT_TX_SIZE_MASK     0x00001fff  /**< \brief Descriptor size mask */
/** @} */

/** \defgroup bba_rbits RTL8139C receive status bits
    @{
*/
#define RT_RX_MULTICAST     0x00008000  /**< \brief Multicast packet */
#define RT_RX_PAM           0x00004000  /**< \brief Physical address matched */
#define RT_RX_BROADCAST     0x00002000  /**< \brief Broadcast address matched */
#define RT_RX_BAD_SYMBOL    0x00000020  /**< \brief Invalid symbol in 100TX packet */
#define RT_RX_RUNT          0x00000010  /**< \brief Packet size is <64 bytes */
#define RT_RX_TOO_LONG      0x00000008  /**< \brief Packet size is >4K bytes */
#define RT_RX_CRC_ERR       0x00000004  /**< \brief CRC error */
#define RT_RX_FRAME_ALIGN   0x00000002  /**< \brief Frame alignment error */
#define RT_RX_STATUS_OK     0x00000001  /**< \brief Status ok: a good packet was received */
/** @} */

/** \defgroup bba_config1bits RTL8139C Config Register 1 bits

    From RTL8139C(L) datasheet v1.4

    @{
*/
#define RT_CONFIG1_LED1     0x80 /**< \brief XXX DC bba has no LED, maybe repurposed. */
#define RT_CONFIG1_LED0     0x40 /**< \brief XXX DC bba has no LED, maybe repurposed. */
#define RT_CONFIG1_DVRLOAD  0x20 /**< \brief Sets the Driver as loaded. */
#define RT_CONFIG1_LWACT    0x10 /**< \brief LWAKE active mode. Default 0. */
#define RT_CONFIG1_MEMMAP   0x08 /**< \brief Registers mapped to PCI mem space. Read Only */
#define RT_CONFIG1_IOMAP    0x04 /**< \brief Registers mapped to PCI I/O space. Read Only */
#define RT_CONFIG1_VPD      0x02 /**< \brief Enable Vital Product Data. */
#define RT_CONFIG1_PMEn     0x01 /**< \brief Power Management Enable */
/** @} */

/** \defgroup bba_config4bits RTL8139C Config Register 4 bits

    From RTL8139C(L) datasheet v1.4. Only RT_CONFIG4_RxFIFIOAC is used.

    @{
*/
#define RT_CONFIG4_RxFIFIOAC 0x80 /**< \brief Auto-clear the Rx FIFO overflow. */
#define RT_CONFIG4_AnaOff    0x40 /**< \brief Turn off analog power. Default 0. */
#define RT_CONFIG4_LongWF    0x20 /**< \brief Long Wake-up Frames. */
#define RT_CONFIG4_LWPME     0x10 /**< \brief LWake vs PMEB. */
#define RT_CONFIG4_RES08     0x08 /**< \brief Reserved. */
#define RT_CONFIG4_LWPTN     0x04 /**< \brief LWAKE Pattern. */
#define RT_CONFIG4_RES02     0x02 /**< \brief Reserved. */
#define RT_CONFIG4_PBWake    0x01 /**< \brief Disable pre-Boot Wakeup. */
/** @} */

/** \defgroup bba_config5bits RTL8139C Config Register 5 bits

    From RTL8139C(L) datasheet v1.4. Only RT_CONFIG5_LDPS is used.

    @{
*/
#define RT_CONFIG5_RES80    0x80 /**< \brief Reserved. */
#define RT_CONFIG5_BWF      0x40 /**< \brief Enable Broadcast Wakeup Frame. Default 0. */
#define RT_CONFIG5_MWF      0x20 /**< \brief Enable Multicast Wakeup Frame. Default 0. */
#define RT_CONFIG5_UWF      0x10 /**< \brief Enable Unicast Wakeup Frame. Default 0. */
#define RT_CONFIG5_FIFOAddr 0x08 /**< \brief Set FIFO address pointer. For testing only. */
#define RT_CONFIG5_LDPS     0x04 /**< \brief Disable Link Down Power Saving mode. */
#define RT_CONFIG5_LANW     0x02 /**< \brief Enable LANWake signal. */
#define RT_CONFIG5_PME_STS  0x01 /**< \brief Allow PCI reset to set PME_Status bit. */
/** @} */

/** \brief  Retrieve the MAC Address of the attached BBA.

    This function reads the MAC Address of the BBA and places it in the buffer
    passed in. The resulting data is undefined if no BBA is connected.

    \param  arr             The array to read the MAC into.
*/
void bba_get_mac(uint8 *arr);

/** \brief  Receive packet callback function type.

    When a packet is received by the BBA, the callback function will be called
    to handle it.

    \param  pkt             A pointer to the packet in question.
    \param  len             The length, in bytes, of the packet.
*/
typedef void (*eth_rx_callback_t)(uint8 *pkt, int len);

/** \brief  Set the ethernet packet receive callback.

    This function sets the function called when a packet is received by the BBA.
    Generally, this inputs into the network layer.

    \param  cb              A pointer to the new callback function.
*/
void bba_set_rx_callback(eth_rx_callback_t cb);

/** \defgroup bba_txrv  Return values from bba_tx().
    @{
*/
#define BBA_TX_OK       0   /**< \brief Transmit success */
#define BBA_TX_ERROR    -1  /**< \brief Transmit error */
#define BBA_TX_AGAIN    -2  /**< \brief Retry transmit again */
/** @} */

#define BBA_TX_NOWAIT   0   /**< \brief Don't block waiting for the transfer. */
#define BBA_TX_WAIT     1   /**< \brief Wait, if needed on transfer. */

/** \brief  Transmit a single packet.

    This function transmits a single packet on the bba, waiting for the link to
    become stable, if requested.

    \param  pkt             The packet to transmit.
    \param  len             The length of the packet, in bytes.
    \param  wait            BBA_TX_WAIT if you don't mind blocking for the
                            all clear to transmit, BBA_TX_NOWAIT otherwise.

    \retval BBA_TX_OK       On success.
    \retval BBA_TX_ERROR    If there was an error transmitting the packet.
    \retval BBA_TX_AGAIN    If BBA_TX_NOWAIT was specified and it is not ok to
                            transmit right now.
*/
int bba_tx(const uint8 *pkt, int len, int wait);

/* \cond */
/* Initialize */
int bba_init(void);

/* Shutdown */
int bba_shutdown(void);
/* \endcond */

__END_DECLS

#endif  /* __DC_NET_BROADBAND_ADAPTER_H */

