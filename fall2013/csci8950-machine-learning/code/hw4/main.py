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

    network = buildNetwork(9, 3, 1,
                           hiddenclass=SigmoidLayer,
                           outclass=LinearLayer)

    #validation_data, training_data = alldata.splitWithProportion(0.25)

    trainer = BackpropTrainer(network, verbose=True, weightdecay=0.001,
                             learningrate=0.1)
    trainer.setData(alldata)

    trainer.trainUntilConvergence(maxEpochs=6000)

    validator = CrossValidator(trainer, alldata, n_folds=10,
                               valfunc=ModuleValidator.MSE)
    ret = validator.validate()
    print(ret)
