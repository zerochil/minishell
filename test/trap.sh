#!/bin/bash

trap 'echo xx' SIGINT
trap 'echo ahh' SIGTSTP 
read $line
# echo "$_"
# echo "1: $1"
# echo "2: $2"
# echo "3: $3"
# echo "4: $4"
#
