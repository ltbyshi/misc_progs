#! /usr/bin/env python
from SocketServer import TCPServer, ThreadingTCPServer, StreamRequestHandler
from threading import BoundedSemaphore, Lock, Thread
import json, argparse, time, select, socket
import logging

def detect_disconnect(sock):
    r, w, e = select.select([sock], [], [], 0)
    if len(r) > 0:
        data = ''
        try:
            data = r[0].recv(1, socket.MSG_PEEK)
        except IOError as e:
            if e.errno == 32:
                pass
        if len(data) == 0:
            return True
    return False

class LockServerHandler(StreamRequestHandler):
    def setup(self):
        StreamRequestHandler.setup(self)

    def handle(self):
        logger = self.server.logger
        self.socket_name = '{0}:{1}-{2}'.format(self.client_address[0], self.client_address[1], '%.6f'%time.time())
        logger.info('Client connected: {0}:{1}'.format(*self.client_address))
        response = {'status': True}
        try:
            method, name = self.rfile.readline().strip().split()
            self.method = method
            self.name = name
            if method == 'sema_create':
                logger.info('sema_create')
                if not name in self.server.sems:
                    self.server.sems[name] = BoundedSemaphore()
                else:
                    response['status'] = False
                    response['message'] = 'semaphore {0} already exists'.format(name)
            elif method == 'sema_acquire':
                logger.info('sema_acquire')
                while not self.server.sems[name].acquire(False):
                    if detect_disconnect(self.request):
                        logger.debug('Disconnected from {0}:{1}'.format(**self.client_address))
                        return
                    time.sleep(self.server.spinlock_time)   
            elif method == 'sema_release':
                self.server.sems[name].release()
                logger.info('sema_release')
            elif method == 'lock_acquire':
                logger.info('lock_acquire')
                while not self.server.locks[name].acquire(False):
                    #print 'Spinlock from {0}:{1}'.format(*self.client_address)
                    if detect_disconnect(self.request):
                        logger.debug('Disconnected from {0}:{1}'.format(*self.client_address))
                        return
                    time.sleep(self.server.spinlock_time)
            elif method == 'lock_release':
                logger.info('lock_release')
                self.server.locks[name].release()
            elif method == 'barrier_create':
                if not name in self.server.barriers:
                    self.server.barriers[name] = None
                
            elif method == 'barrier_destroy':
                pass
            elif method == 'barrier_enter':
                pass
            else:
                response['status'] = False
                response['message'] = 'invalid method: ' + method
        except Exception as e:
            response['status'] = False
            response['message'] = '{0}: {1}'.format(repr(e.__class__), e.args)
        try:
            self.wfile.write(json.dumps(response))
        except IOError as e:
            if e.errno == 32:
                pass

    def finish(self):
        try:
            StreamRequestHandler.finish(self)
        except IOError as e:
            if e.errno == 32:
                logger.debug('Connection from {0}:{1} interrupted'.format(*self.client_address))

class LockServer(ThreadingTCPServer):
    def __init__(self, server_address, default_sem, spinlock_time):
        self.allow_reuse_address = True
        self.sems = {}
        self.sems['default'] = BoundedSemaphore(default_sem)
        self.locks = {}
        self.locks['default'] = Lock()
        self.barriers = {}
        self.spinlock_time = spinlock_time
        self.logger = logging.getLogger('LockServer')
        ThreadingTCPServer.__init__(self, server_address, LockServerHandler)
        

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-a', '--addr', type=str, default='127.0.0.1', required=False)
    parser.add_argument('-p', '--port', type=int, default=8780, required=False)
    subparsers = parser.add_subparsers(dest='subcommand')

    parser_server = subparsers.add_parser('server')
    parser_server.add_argument('--sem', type=int, default=1, required=False, help='initial value for semaphore')
    parser_server.add_argument('-t', '--spinlock-time', type=float, default=0.5, required=False)
    parser_server.add_argument('-l', '--logfile', type=str, required=False)

    parser_client = subparsers.add_parser('client')
    parser_client.add_argument('method', type=str)
    parser_client.add_argument('--name', type=str, default='default', required=False)
    args = parser.parse_args()
    
    if args.subcommand == 'server':
        if args.logfile:
            logging.basicConfig(level=logging.DEBUG, filename=args.logfile)
        else:
            logging.basicConfig(level=logging.DEBUG)
        logger = logging.getLogger('LockServer')
        server_address = (args.addr, args.port)
        # a dict of socket_name: (socket, is_disconnected)
        server = LockServer(server_address, args.sem, args.spinlock_time)
        try:
            logger.info('Server started {0}:{1}'.format(*server_address))
            server.serve_forever()
        except KeyboardInterrupt:
            pass
        logger.info('Server shutdown')
        logging.shutdown()

    elif args.subcommand == 'client':
        import socket
        import time
        try:
            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            sock.connect((args.addr, args.port))
            req = {'method': args.method, 'name': args.name}
            sock.sendall('{0} {1}\nEOF\n'.format(args.method, args.name))
            #time.sleep(30)
            response = json.loads(sock.recv(1024))
            sock.close()
            if response.get('status'):
                sys.exit(0)
        except KeyboardInterrupt:
            pass
        sys.exit(1)
    

