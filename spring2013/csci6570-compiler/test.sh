#!/usr/bin/env bash

for f in tests/*
do
    echo "Parsing $f"
    ./parser $f 2>/dev/null
done
