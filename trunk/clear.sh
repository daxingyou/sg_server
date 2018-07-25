#!/bin/sh

find ./ -name "core*"  | xargs rm -f
find ./ -name "*.log"  | xargs rm -f
find ./ -name "*.asan"  | xargs rm -f
find ./ -name "*.csv"  | xargs rm -f
find ./sql/logs -name "*.txt"  | xargs rm -f