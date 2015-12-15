#! /usr/bin/env python
import urlparse

url = 'http://username:password@example.com:8080/path/query?id=id1&id=id2&key=mykey&blank=&list=a,b,c#bookmark'
parsed = urlparse.urlparse(url)
data = urlparse.parse_qs(parsed.query)
print 'URL:',url
print 'urlparse(URL):',parsed
print 'ParseResult(URL):', '\n'.join(['%s=%s'%(key,getattr(parsed, key)) for key in dir(parsed) if not key.startswith('_') ])
print 'parse_qs(query):',data

