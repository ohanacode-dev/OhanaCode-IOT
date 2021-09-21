#!/bin/bash
# This is a helper script to periodically check ambient temperature and weather

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
	
  ((COUNTER++))

  if [ "$COUNTER" -eq "14" ]; then
    COUNTER=0
  fi
	
done
