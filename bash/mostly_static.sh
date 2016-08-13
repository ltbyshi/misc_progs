#! /bin/bash
if [ "$#" -eq 0 ]; then
    echo "Link to static libraries as many as possible"
    echo "Usage: $0 gcc|g++ [--exclude <lib_name>]. . . <link_command>"
    exit 1
fi

if [ "$1" = "gcc" ] || [ "$1" = "g++" ];then
    cmd=$1
else
    echo "Error: linker should be either gcc or g++"
    exit 1
fi
shift

exclude=()
lib_path=()

{
echo "-static-libgcc"
echo "-static-libstdc++"

while [ "$#" -ne 0 ]; do
    case "$1" in
        -L*)
            if [ "$1" == -L ]; then
                shift
                LPATH="-L$1"
            else
                LPATH="$1"
            fi

            lib_path+=("$LPATH")
            echo "$LPATH"
            ;;

        -l*)
            NAME="$(echo $1 | sed 's/-l\(.*\)/\1/')"

            if echo "${exclude[@]}" | grep " $NAME " >/dev/null; then
                echo "$1"
            else
                LIB="$(gcc $lib_path -print-file-name=lib"$NAME".a)"
                if [ "$LIB" == lib"$NAME".a ]; then
                    echo "$1"
                else
                    echo "$LIB"
                fi
            fi
            ;;

        --exclude)
            shift
            exclude+=(" $1 ")
            ;;

        *) echo "$1"
    esac

    shift
done
} | xargs -t -d '\n' "$cmd"
