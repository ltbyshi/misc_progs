#! /bin/bash

inotifywait -e 'CREATE,DELETE,MODIFY,MOVED_FROM,MOVED_TO' \
    -m -r --format '%:e %f' /tmp/junk

