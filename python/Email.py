#! /usr/bin/env python

from smtplib import SMTP
import email.utils
from email.mime.text import MIMEText
import argparse, sys

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-u', '--user', type=str, required=True)
    parser.add_argument('-p', '--password', type=str, required=True)
    parser.add_argument('-x', '--server', type=str, required=True)
    parser.add_argument('-a', '--attachment', type=str, required=False)
    parser.add_argument('-f', '--sender', type=str, required=False)
    parser.add_argument('-t', '--receipient', type=str, required=False)
    parser.add_argument('-s', '--subject', type=str, required=False, default='No subject')
    parser.add_argument('--content-type', type=str, required=False, default='text/html')
    parser.add_argument('content', type=str, nargs='?')
    args = parser.parse_args()

    if args.content:
        msg = MIMEText(open(args.content, 'r').read())
    else:
        msg = MIMEText(sys.stdin.read())

    #msg.set_unixfrom('author')
    msg['To'] = email.utils.formataddr(('Binbin Shi', 'ltbyshi@gmail.com'))
    msg['From'] = email.utils.formataddr(('Binbin Shi', 'ltbyshi@outlook.com'))
    msg['Subject'] = args.subject
    msg['Content-Type'] = args.content_type
    server = SMTP(server_name)
    server.set_debuglevel(True)
    server.ehlo()
    if server.has_extn('STARTTLS'):
        server.starttls()
        server.ehlo()
    server.login(username, password)
    server.sendmail('ltbyshi@outlook.com', 
                   ['ltbyshi@gmail.com'],
                   msg.as_string())
    server.quit()
