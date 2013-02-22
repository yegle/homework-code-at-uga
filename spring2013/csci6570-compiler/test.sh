#!/usr/bin/env bash

for f in tests/*
do
    echo "Parsing $f"
    ./tjc $f 2>/dev/null
done
