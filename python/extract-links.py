#! /usr/bin/env python
import argparse
import os
import requests
from bs4 import BeautifulSoup

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='extract links from web pages')
    parser.add_argument('url', type=str)
    args = parser.parse_args()

    res = requests.get(args.url)
    if res.status_code == 200:
        soup = BeautifulSoup(res.text, 'html.parser')
        tables = soup.find_all('table')
        if tables:
            rows = tables[0].find_all('tr')
            for row in rows[3:]:
                link = row.find('a')
                if link is not None:
                    print(args.url + '/' + link.get('href'))
                