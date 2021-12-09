#!/bin/bash

cd day$1
make -B && \
echo -e "\n" && \
./out/day$1.out ./input.txt
