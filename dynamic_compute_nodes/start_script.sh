#!/bin/bash

echo "hi" >> /home/pi/output.txt

i="1"
while [ i -eq 1]
do
	sleep 30
	echo `date` >> /home/pi/output.txt
done
