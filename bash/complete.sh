#! /bin/bash

dummy(){
    echo $0 $@
}

_dummy()   #  By convention, the function name
{                 #+ starts with an underscore.
    local cur
    # Pointer to current completion word.
    # By convention, it's named "cur" but this isn't strictly necessary.

    COMPREPLY=()   # Array variable storing the possible completions.
    cur=${COMP_WORDS[COMP_CWORD]}

    case "$cur" in
        --*)
            COMPREPLY=( $(compgen -W '--a1 --b2 --c3 --d4 --e5' -- $cur ) );;
        -*)
            COMPREPLY=( $( compgen -W '-a -b -c -d -e -f -g -h' -- $cur ) );;
            #   Generate the completion matches and load them into $COMPREPLY array.
            #   xx) May add more cases here.
            #   yy)
            #   zz)
        *)
            COMPREPLY=( $(compgen -W 'abc def ghi jkl mno pqr stu' -- $cur) );;
    esac
    return 0
}

complete -F _dummy -o filenames dummy
