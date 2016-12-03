#! /usr/bin/env python
import socket
import argparse
import sys

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='')
    parser.add_argument('-p', '--source-port', type=int, required=False,
            help='Specify source port to use')
    parser.add_argument('-s', '--source-addr', type=str, required=False,
            help='Specify source address to use')
    parser.add_argument('hostname', type=str)
    parser.add_argument('port', type=int)
    parser.add_argument('--reuse-addr', action='store_true', required=False,
            help='reuse address in TCP connection')
    parser.add_argument('--buffer-size', type=int, required=False,
            default=1024, help='buffer size in bytes for transfering data')
    args = parser.parse_args()

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    if args.reuse_addr:
        s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    if (args.source_addr is not None) and (args.source_port is not None):
        s.bind((args.source_addr, args.source_port))
    s.connect((args.hostname, args.port))
    data = sys.stdin.read(args.buffer_size)
    while data:
        s.sendall(data)
    s.close()

