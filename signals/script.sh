#!/bin/bash

function write_to_file() { date >> file; echo "WOW"; }

trap write_to_file INT

#./signals
sleep 60
#
