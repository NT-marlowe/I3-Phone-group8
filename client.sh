#!/bin/bash

if [ $# -ne 2 ]; then
  echo "The number of arguments you passed is $#." 1>&2
  echo "You need to pass 2 arguments, IP_address and Port_number to execute." 1>&2
  echo "example: ./client.sh 127.0.0.1 50000" 1>&2
  exit 1
fi

# ./bin/main 10.213.150.58 50000 | play -t raw -b 16 -c 1 -e s -r 44100 -
./bin/main $1 $2 | play -t raw -b 16 -c 1 -e s -r 44100 -
