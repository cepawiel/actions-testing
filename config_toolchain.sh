#!/bin/bash

JOBS=$(nproc)

function raw {
    # 1. increase build core count
    # 2. disable newlib patches
    # 3. disable threading
    sed -i \
        -e "s/makejobs=-j2/makejobs=-j$JOBS/g" \
        -e '/auto_fixup_sh4_newlib/s/^#//' \
        -e "s/thread_model=kos/thread_model=single/" \
        -e '/use_kos_patches/s/^#//' \
        /opt/toolchains/dc/kos/utils/dc-chain/config.mk
}

function kos {
    sed -i "s/makejobs=-j2/makejobs=-j$JOBS/g" \
      /opt/toolchains/dc/kos/utils/dc-chain/config.mk
}

if [ $# -eq 0 ]; then
  echo "Build type must be passed as first parameter"
  exit 1
fi

case "$1" in
  "kos")
    kos
    ;;
  "raw")
    raw
    ;;
  *)
    echo "Invalid Build Type"
    exit 1
    ;;
esac
