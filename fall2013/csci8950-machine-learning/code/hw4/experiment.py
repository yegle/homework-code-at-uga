#!/usr/bin/env python
# vim: set fileencoding=utf-8 ts=4 sw=4 tw=72 :
# pylint: disable-msg=C0103,C0111

from __future__ import (unicode_literals, absolute_import,
                        division, print_function)

import csv
import sys
from pybrain.datasets import ClassificationDataSet
from pybrain.tools.shortcuts import buildNetwork
from pybrain.supervised.trainers import BackpropTrainer
from pybrain.structure import LinearLayer, SigmoidLayer
from pybrain.tools.validation import CrossValidator, ModuleValidator

translation = {
    'x': 0,
    'o': 1,
    'b': 2,
}


def row_preprocess(row):
    return [translation[x] for x in row]


if __name__ == "__main__":
    raw_data = list(csv.reader(open("tic-tac-toe.data")))
    targets = [1 if x[-1] == 'positive' else 0 for x in raw_data]
    inputs = [row_preprocess(x[:-1]) for x in raw_data]
    alldata = ClassificationDataSet(
        9, class_labels=['negative', 'positive'])

    for (i, t) in zip(inputs, targets):
        alldata.addSample(i, [t])

    network = buildNetwork(9, int(sys.argv[1]), 1,
                           hiddenclass=SigmoidLayer,
                           outclass=LinearLayer)

    trainer = BackpropTrainer(network, weightdecay=0.001)

    validator = CrossValidator(trainer, alldata, n_folds=2,
                               valfunc=ModuleValidator.MSE)
    validator.setArgs(max_epochs=500)
    ret = validator.validate()
    print(ret)
