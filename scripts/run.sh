#!/bin/bash

#-----------------------------------------------------------
# 
# start Halo Light Application
#
#-----------------------------------------------------------

echo '-> Halo Light Script'

dir=$(dirname $0)
cd $dir

echo '-> Starting Fade Candy Server'

PROCESS=fcserver-osx

if pgrep $PROCESS
	then
		echo '-> fcserver-osx process already exit'
	else
		cd ../fcserver
		./fcserver-osx ServerConfig.json &
		echo '-> Starting fcserver-osx'
fi



echo '-> Starting Halo Light Application'

PROCESS=HaloLight

if pgrep $PROCESS
	then
		echo '-> killing HaloLight process...'
		killall -15 HaloLight
		
fi

echo '-> starting HaloLight process...'

cd ../bin/HaloLight.app/Contents/MacOS/
./HaloLight

