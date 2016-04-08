from lxml.etree import Element, ElementTree, SubElement

def _to_xml(obj, tag, root, create_sub=True):
    #print tag
    if isinstance(obj, dict):
        e = SubElement(root, tag) if tag != 'root' else root
        for key, value in obj.iteritems():
            #e = SubElement(root, key)
            _to_xml(value, key, e, False)
    elif isinstance(obj, str) or isinstance(obj, unicode):
        if create_sub:
            SubElement(root, tag).text = obj
        else:
            root.text = obj
    elif hasattr(obj, '__iter__') or hasattr(obj, '__getitem__'):
        e = SubElement(root, tag)
        for value in obj:
            _to_xml(value, 'item', e)
    else:
        #root.text = unicode(obj)
        if create_sub:
            SubElement(root, tag).text = unicode(obj)
        else:
            root.text = unicode(obj)

def to_xml(obj, filename):
    root = Element('root')
    _to_xml(obj, 'root', root)
    tree = ElementTree(root)
    tree.write(filename, pretty_print=True, standalone=True)

if __name__ == '__main__':
    import sys
    import argparse, json
    '''
    to_xml(d, sys.stdout)
    '''
    parser = argparse.ArgumentParser('Convert JSON to XML')
    parser.add_argument('input_file', type=str, nargs='?',  help='JSON file')
    parser.add_argument('--example', action='store_true', default=False, required=False)
    args = parser.parse_args()
    if args.example:
        d = {'html': {
                'head': {'title': 'XML Utils'},
                'body': {
                    'numbers': [1, 2, 3], 
                    'strings': ['a', 'b', 'c']
                    }
                }
            }
        to_xml(d, sys.stdout)
    else:
        obj = json.load(open(args.input_file, 'r'))
        to_xml(obj, sys.stdout)

