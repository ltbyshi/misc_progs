#! /bin/bash

hello(){
    echo "Hello: $@"
}
export -f hello
seq 10 | xargs -l bash -c 'hello "$@"'

