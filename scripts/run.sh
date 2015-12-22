#!/bin/bash

#-----------------------------------------------------------
#
# start Bacterial Display Application
#
#-----------------------------------------------------------

echo '-> Bacterial Display Script'

dir=$(dirname $0)
cd $dir

echo '-> Starting Fade Candy Server'

PROCESS=fcserver-rpi

if pgrep $PROCESS
	then
		echo '-> fcserver-rpi process already exit'
	else
		cd ../fcserver
		./fcserver-rpi &
		echo '-> Starting fcserver-rpi'
fi



echo '-> Starting Bacterial Display Application'

PROCESS=BacterialDisplay

if pgrep $PROCESS
	then
		echo '-> killing BacterialDisplay process...'
		killall -15 BacterialDisplay

fi

echo '-> starting BacterialDisplay process...'

cd ../bin
./BacterialDisplay
