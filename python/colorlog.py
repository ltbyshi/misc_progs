#! /usr/bin/env python
import logging
import os

class ColorStreamHandler(logging.StreamHandler):
    def emit(self, record):
        if os.isatty(self.stream.fileno()):
            color_code = ''
            if record.levelno <= logging.DEBUG:
                color_code = '34'
            elif record.levelno <= logging.INFO:
                color_code = '32'
            elif record.levelno <= logging.WARNING:
                color_code = '33'
            elif record.levelno <= logging.ERROR:
                color_code = '31'
            elif record.levelno <= logging.CRITICAL:
                color_code = '1;31'
            record.levelname = '\x1B[%sm%s\x1B[0m'%(color_code, record.levelname)
        super(ColorStreamHandler, self).emit(record)

logger = logging.getLogger('colorlog')
logger.setLevel(logging.DEBUG)
color_handler = ColorStreamHandler()
color_handler.setFormatter(logging.Formatter('[%(asctime)s] [%(levelname)s] %(name)s: %(message)s'))
logger.addHandler(color_handler)

logger.debug('don\'t painc, just debugging')
logger.info('first logging message')
logger.warn('this is dangerous')
logger.error('oops! we are dying')
logger.critical('this is the end of the world!')
