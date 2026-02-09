#!/bin/bash

function write_to_file() { date >> file; echo "WOW"; }

trap write_to_file INT

echo $$

ssh -i ~/.ssh/id_ed25519 yoav@localhost 'sleep 30'

#while $true; do
#    sleep 10
#done

#./signalssleep 60
#
