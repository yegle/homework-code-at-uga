#!/usr/bin/env python
# vim: set fileencoding=utf-8 :

from __future__ import (unicode_literals, absolute_import,
                        division, print_function)


import csv
import time
from sklearn import tree
from sklearn.feature_extraction import DictVectorizer
from sklearn.cross_validation import KFold

def remove_key(d, key):
    ret = dict(d)
    del ret[key]
    return ret

if __name__ == '__main__':
    with open("tic-tac-toe.data") as f:
        data = list(csv.DictReader(f, fieldnames=[
            'top-left-square', 'top-middle-square', 'top-right-square',
            'middle-left-square', 'middle-middle-square', 'middle-right-square',
            'bottom-left-square', 'bottom-middle-square', 'bottom-right-square',
            'class'
        ]))

        training_set = [remove_key(x, 'class') for x in data]
        target_set = [1 if x['class'] == 'positive' else 0 for x in data]

        vec = DictVectorizer()
        vectorized_training_set = vec.fit_transform(training_set)

        clf = tree.DecisionTreeClassifier(criterion='entropy',
                                          random_state=1)
        clf.fit(vectorized_training_set.toarray(),
                target_set)

        s = clf.score(vectorized_training_set.toarray(), target_set)
        print("score on training set: %s" % (s))

        with open("p2.dot", "w") as output_file:
            tree.export_graphviz(clf, out_file=output_file,
                                 feature_names=vec.get_feature_names())

        kf = KFold(len(training_set), n_folds=10)

        for split in range(2, 11):
            #print("split=%d" % (split))
            score = 0
            start = time.time()
            for train_indices, test_indices in kf:
                clf = tree.DecisionTreeClassifier(criterion='entropy',
                                                  random_state=1, min_samples_split=split)

                vec = DictVectorizer()
                X_train = [training_set[i] for i in train_indices]
                Y_train = [target_set[i] for i in train_indices]

                vectorized_training_set = vec.fit_transform(X_train)

                X_test = [training_set[i] for i in test_indices]
                vectorized_X_test = vec.transform(X_test)
                Y_test = [target_set[i] for i in test_indices]

                clf.fit(vectorized_training_set.toarray(), Y_train)
                s = clf.score(vectorized_X_test.toarray(), Y_test)
                score += s
            end = time.time()
            #print("time cost = %s" % (end-start))
            print(score/10)
