#!/bin/bash
# Super simple script to open serial connection to raspi and order a large coffee

clear

exec 2<> /dev/ttyACM0
sleep 1
cat <&2
echo "2" >&2
cat <&2
exec 2>&-
