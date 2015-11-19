#! /usr/bin/env python

import numpy as np
import random
import itertools

def RandomKmer(size):
    return np.random.choice((-0.999, 0.999), size=size)

def AllKmers(k):
    for i in xrange(2**k):
        x = np.zeros(k)
        for j in xrange(k):
            x[k - 1 - j] = 1 if ((i >> j) & 1) > 0 else -1
        yield x

def HyperplaneHash(x, hp):
    W, Wc = hp
    hashsize = W.shape[0]
    pow2 = [2**i for i in xrange(hashsize)]
    code = np.dot((np.dot(x, W.transpose()) > Wc), pow2)
    return code

def RandomHyperplane2(k, hashsize):
    print 'K=%d, hashsize=%d'%(k, hashsize)
    W = []
    Wc = []
    for i in xrange(hashsize):
        x = RandomKmer(size=(1000, k+1))
        x[:,-1] = -1
        if i == 0:
            print x
        u, w, v = np.linalg.svd(x)
        W.append(list(v[-1,:-1]))
        Wc.append(v[-1,-1])
    return (np.array(W), np.array(Wc))

def RandomHyperplane(k, hashsize):
    print 'K=%d, hashsize=%d'%(k, hashsize)
    W = np.random.randn(hashsize, k)
    Wc = np.random.randn(hashsize)
    return (W, Wc)

def TestCollisions(k, hashsize):
    hp = RandomHyperplane(k, hashsize)
    #print hp[0]
    #print hp[1]
    t = np.zeros(2**k, dtype=bool)
    collisions = 0
    colist = {}
    for x in AllKmers(k):
        hashed = HyperplaneHash(x, hp)
        if t[hashed]:
            collisions += 1
            #colist[hashed].append(i)
        else:
            t[hashed] = True
            #colist[hashed] = [i]
    '''
    for hashed in colist.iterkeys():
        if len(colist[hashed]) > 1:
            print '%d => %s'%(hashed, ','.join(map(lambda x: '{:06b}'.format(x), colist[hashed])))
    '''
    print 'CollisionRatio (k={:d}, hashsize={:d}): {:f} ({:d}/{:d})'.format(k,
            hashsize, float(collisions)/(2**k), collisions, 2**k)

def KmerStatistics(k, hashsize):
    Xall = np.stack(list(AllKmers(k)), axis=0)
    print 'X.T*X, all kmers'
    print np.dot(Xall.T, Xall)
    Xran = RandomKmer((100, k))
    print 'X.T*X, random kmers'
    print np.dot(Xran.T, Xran)
    W, Wc = RandomHyperplane(k, hashsize)
    Y = np.dot(Xall, W.T) - np.stack([Wc]*Xall.shape[0])
    #Y = np.dot(Xall, W.T)
    print 'Wc'
    print Wc
    print 'Y.T*Y, all kmers'
    print np.dot(Y.T, Y)
    print 'mean Y'
    print Y.mean(axis=0)
    print 'Y'
    print (Y > 0).astype(int)

if  __name__ == '__main__':
    k = 18
    hashsize = 18
    '''
    hp = RandomHyperplane(k, hashsize)
    print hp[0]
    print hp[1]
    x = RandomKmer(k)
    print x
    code = HyperplaneHash(x, hp)
    print 'code=%d'%code
    '''
    
    TestCollisions(k, hashsize)
    #KmerStatistics(k, hashsize)
