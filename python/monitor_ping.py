#! /usr/bin/env python
import sys
import os
import subprocess
import re

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print('Usage: {} ping_args ...'.format(sys.argv[0]), file=sys.stderr)
        sys.exit(1)

    pat = re.compile(r'(?P<size>[0-9]+) bytes from (?P<dest_ip>[0-9\.]+): icmp_seq=(?P<icmp_seq>[0-9]+) ttl=(?P<ttl>[0-9]+) time=(?P<time>[0-9]+) ms')
    proc = subprocess.Popen(['ping'] + sys.argv[1:], stdout=subprocess.PIPE)
    print('size\tdest_ip\ticmp_seq\tttl\ttime')
    for line in proc.stdout:
        m = pat.match(str(line.strip(), encoding='utf-8'))
        if m is not None:
            print('{size}\t{dest_ip}\t{icmp_seq}\t{ttl}\t{time}'.format(**m.groupdict()))
            sys.stdout.flush()
