#! /bin/bash
LD_PRELOAD=$PWD/lib/libjail.so LIBJAIL_DEBUG=1 LIBJAIL_ROOT=/tmp $@
