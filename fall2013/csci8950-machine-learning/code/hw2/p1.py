#!/usr/bin/env python
# vim: set fileencoding=utf-8 :

from __future__ import (unicode_literals, absolute_import,
                        division, print_function)

import csv
from functools import partial
from sklearn import tree
from sklearn.feature_extraction import DictVectorizer
from sklearn.cross_validation import LeaveOneOut


def remove_key(d, key):
    ret = dict(d)
    del ret[key]
    return ret

if __name__ == "__main__":
    remove_play_tennis_field = partial(remove_key, key="play_tennis")
    with open("p1_training.csv") as f:
        reader = list(csv.DictReader(f))
        training_set = [remove_play_tennis_field(x) for x in reader]
        target_set = [x['play_tennis'] for x in reader]
        vec = DictVectorizer()
        vectorized = vec.fit_transform(training_set)
        clf = tree.DecisionTreeClassifier(criterion='entropy',
                                          random_state=1)
        clf.fit(vectorized.toarray(), target_set)

        with open("p1.dot", "w") as output_file:
            tree.export_graphviz(clf, out_file=output_file,
                                 feature_names=vec.get_feature_names())

        loo = LeaveOneOut(len(training_set))
        score = 0
        count = 0
        for train_indices, test_index in loo:
            clf = tree.DecisionTreeClassifier(criterion='entropy',
                                             random_state=1)

            vec = DictVectorizer()
            X_train = [training_set[i] for i in train_indices]
            Y_train = [target_set[i] for i in train_indices]
            vectorized_training_set = vec.fit_transform(X_train)

            X_test = [training_set[test_index]]
            vectorized_X_test = vec.transform(X_test)
            Y_test = [target_set[test_index]]

            clf.fit(vectorized_training_set.toarray(), Y_train)
            score += clf.score(vectorized_X_test.toarray(), Y_test)
            count += 1
        print(score)
        print(score/count)
