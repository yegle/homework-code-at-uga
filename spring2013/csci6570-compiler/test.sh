#!/usr/bin/env bash

#for f in tests/*
#do
    f=tests/Sort.java
    echo "Parsing $f"
    ./tjc $f 2>/dev/null
#done
