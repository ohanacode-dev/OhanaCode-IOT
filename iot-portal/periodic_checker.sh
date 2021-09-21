#!/bin/bash

# This is a helper script to periodically check ambient temperature and weather

SCRIPT_FULL_PATH="$(realpath -s $0)"
SCRIPT_DIR="$(dirname $SCRIPT_FULL_PATH)"

cd $SCRIPT_DIR

# Initial check of weather and temperature
./weather.py
./temperature_ds18b20.py
COUNTER=0

while :
do
  sleep 60
  
  ./temperature_ds18b20.py
	
  if [ "$COUNTER" -eq "0" ]; then
    ./weather.py
  fi
	
  let COUNTER++

  if [ "$COUNTER" -eq "14" ]; then
    COUNTER=0
  fi
	
done
