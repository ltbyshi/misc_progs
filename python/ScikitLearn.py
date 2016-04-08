#! /usr/bin/env python

import argparse, sys, os, time
import numpy as np
from sklearn.cross_validation import train_test_split
 
def timeit(f):
    def wrapper(*args, **kwargs):
        start = time.clock()
        r = f(*args, **kwargs)
        elapsed = time.clock() - start
        print 'timeit', elapsed
        return r
    return wrapper

def GaussianDataset(n_samples, n_dim, min_sd=1.5, max_sd=3):
    y = np.random.choice(2, size=n_samples)
    X = np.random.randn(n_samples, n_dim)
    sd = np.random.uniform(min_sd, max_sd, size=n_dim)
    for i in xrange(n_samples):
        mean = 1.0 if y[i] == 1 else -1.0
        X[i, :] = sd*X[i, :] - mean
    return (X, y)

def RandomDataset(n_samples, n_dim, class_weight=0.5):
    y = np.random.choice(2, size=n_samples, p=(1 - class_weight, class_weight))
    X = np.random.randn(n_samples, n_dim)
    return (X, y)

@timeit
def EvalModel(X_train, X_test, y_train, y_test, 
              classifier='SVM', eval_metric='Accuracy', **kwargs):
    from sklearn import metrics
    if classifier == 'SVM':
        from sklearn.svm import SVC as Classifier
    elif classifier == 'RandomForest':
        from sklearn.ensemble import RandomForestClassifier as Classifier
    elif classifier == 'LogRegL2':
        from sklearn.linear_model import LogisticRegression as Classifier
    else:
        raise ValueError('Invalid classifier: {0}'.format(classifier))
    
    model = Classifier(**kwargs)
    model.set_params(class_weight='balanced')
    if args.eval_metric == 'AUC':
        model.set_params(probability=True)
    model.fit(X_train, y_train)
    if eval_metric in ('Accuracy', 'WeightedAccuracy'):
        if eval_metric == 'WeightedAccuracy':
            classes, counts = np.unique(y_test, return_counts=True)
            weights = 1.0/counts.astype('float')
            sample_weight = np.empty(len(y_test))
            for c, w in zip(classes, weights):
                sample_weight[y_test == c] = w
            score = model.score(X_test, y_test, sample_weight=sample_weight)
        else:
            score = model.score(X_test, y_test)
    elif eval_metric == 'AUC':
        scores = model.predict_proba(X_test)[:, 1]
        fpr, tpr, thresholds = metrics.roc_curve(y_test, scores)
        score = metrics.auc(fpr, tpr)

    print '{0}: {1}'.format(eval_metric, score)

@timeit
def TestSVM(X_train, X_test, y_train, y_test, kernel='linear'):
    from sklearn.svm import SVC
    clf = SVC()
    #print X_train
    #print y_train
    clf.fit(X_train, y_train)
    score = clf.score(X_test, y_test)
    print 'SVM score:',score

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-n', '--nsamples', type=int,
            default=1000, required=False)
    parser.add_argument('-k', '--ndim', type=int,
            default=100, required=False)
    parser.add_argument('--min-sd', type=float, default=1.5)
    parser.add_argument('--max-sd', type=float, default=3.0)
    parser.add_argument('-d', '--dataset', type=str, choices=('Gaussian', 'Random'), 
            required=False, default='Gaussian')
    parser.add_argument('-w', '--class-weight', type=float, default=0.5, required=False,
            help='Weight of positive classes for random dataset')
    parser.add_argument('-c', '--classifier', type=str, default='SVM', required=False)
    parser.add_argument('-e', '--eval-metric', type=str,
            choices=('Accuracy', 'WeightedAccuracy', 'AUC'), default='Accuracy', required=False)
    args = parser.parse_args()

    print 'Generate dataset: {0}'.format(args.dataset)
    X = None
    y = None
    if args.dataset == 'Gaussian':
        X, y = GaussianDataset(args.nsamples, args.ndim, args.min_sd, args.max_sd)
    elif args.dataset == 'Random':
        X, y = RandomDataset(args.nsamples, args.ndim, args.class_weight)
    X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.1)
    print 'Run {0}'.format(args.classifier)
    EvalModel(X_train, X_test, y_train, y_test, args.classifier, args.eval_metric)


