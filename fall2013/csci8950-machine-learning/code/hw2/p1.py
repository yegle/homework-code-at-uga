#!/usr/bin/env python
# vim: set fileencoding=utf-8 :

from __future__ import (unicode_literals, absolute_import,
                        division, print_function)

import csv
from functools import partial
from sklearn import tree
from sklearn.feature_extraction import DictVectorizer


def remove_specified_key(d, key):
    ret = dict(d)
    del ret[key]
    return ret

if __name__ == "__main__":
    remove_play_tennis_field = partial(remove_specified_key, key="play_tennis")
    with open("p1_training.csv") as f:
        reader = list(csv.DictReader(f))
        training_set = [remove_play_tennis_field(x) for x in reader]
        labels = [x['play_tennis'] for x in reader]
        vec = DictVectorizer()
        vectorized = vec.fit_transform(training_set)
        clf = tree.DecisionTreeClassifier()
        clf.fit(vectorized.toarray(), labels)

        with open("p1.dot", "w") as output_file:
            tree.export_graphviz(clf, out_file=output_file, feature_names=vec.get_feature_names())
