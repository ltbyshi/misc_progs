#! /usr/bin/env python
import requests
from lxml import etree

'''
https://www.ncbi.nlm.nih.gov/books/NBK3827/#pubmedhelp.Filters

'''

def build_query(filters):
    query = []
    for key, val in filters.items():
        query.append(val.replace(' ', '+') + '[' + key + ']')
    query = '+AND+'.join(query)
    return query

if __name__ == '__main__':
    query = build_query({
        'TI': 'Three-dimensional RNA structure refinement by hydroxyl radical probing',
        'PDAT': '2012',
        '1AU': 'Ding F',
        'VI': '9',
        'TA': 'Nat Methods'
    })
    params = {'db': 'pubmed', 'term': query}
    url = 'https://eutils.ncbi.nlm.nih.gov/entrez/eutils/esearch.fcgi?db=pubmed&term=' + query
    sess = requests.Session()
    r = sess.get(url)
    if r.status_code == 200:
        tree = etree.fromstring(r.content)
        IdList = tree.findall('IdList')
        if len(IdList) > 0:
            pmids = [e.text for e in IdList[0].findall('Id')]
            for pmid in pmids:
                r = sess.get('https://eutils.ncbi.nlm.nih.gov/entrez/eutils/esummary.fcgi?db=pubmed&id=%s'%pmid)
                if r.status_code == 200:
                    tree = etree.fromstring(r.content)
                    docsum = tree.findall('DocSum')[0]
                    for item in docsum.findall('Item'):
                        if item.attrib['Name'] == 'Title':
                            print(pmid, item.text)

        