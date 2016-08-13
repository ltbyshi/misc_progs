import pybedtools

def expand_bedgraph(feature, strand=None):
    return pybedtools.Interval(feature.chrom, feature.start, feature.end, 
                 name='.', score=feature.fields[3], strand=strand)
    
def extend_ends(feature, left, right):
    '''extend each interval on both ends
    '''
    feature.start -= left
    feature.end += right
    return feature

def reorder(feature, fields):
    '''select a subset of features (start from the 4th field) in fields from feature
    '''
    selected = feature.fields[:3]
    for i in fields:
        selected.append(feature.fields[i])
    return pybedtools.create_interval_from_list(selected)