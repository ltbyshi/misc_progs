#! /bin/bash

MAX=300000

g++ -Wall --param max-unroll-times=$MAX \
    --param max-peel-times=$MAX \
    -funroll-all-loops -fpeel-loops \
    -finline-limit=10000000 \
    --param max-inline-insns-single=$MAX \
    --param max-inline-recursive-depth=$MAX \
    --param max-inline-insns-recursive=$MAX \
    --param max-unrolled-insns=$MAX \
    --param max-peeled-insns=$MAX \
    --param large-function-growth=$MAX \
    --param large-unit-insns=$MAX \
    --param large-function-insns=$MAX \
    --param inline-unit-growth=$MAX \
    --param large-unit-insns=$MAX \
    --param max-peel-branches=$MAX \
    --param max-completely-peeled-insns=$MAX \
    --param max-completely-peel-times=$MAX \
    --param max-completely-peel-loop-nest-depth=10 \
    --param max-unswitch-insns=$MAX \
    --param max-unswitch-level=$MAX \
    -Ofast -o bin/bloat bloat.cpp
wc -l bin/bloat.S

