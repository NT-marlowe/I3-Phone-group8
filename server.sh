#!/bin/bash

if [ $# -ne 3 ]; then
  echo "The number of arguments you passed is $#." 1>&2
  echo "You need to pass 3 arguments, \"-l\", Port_number and number_of_clients to execute." 1>&2
  echo "example: ./server.sh -l 50000 4" 1>&2
  exit 1
fi

# ./bin/main 10.213.150.58 50000 | play -t raw -b 16 -c 1 -e s -r 44100 -
./bin/main $1 $2 $3 
