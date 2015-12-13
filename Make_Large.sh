#!/bin/bash

clear

exec 2<> /dev/ttyACM0
sleep 1
echo "2" >&2
cat <&2
exec 2>&-
