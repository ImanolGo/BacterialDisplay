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

cd ../bin

if pgrep $PROCESS
	then
		echo '-> BacterialDisplay process already exit'
	else
		echo '-> Starting BacterialDisplay process...'
		./BacterialDisplay &
fi
