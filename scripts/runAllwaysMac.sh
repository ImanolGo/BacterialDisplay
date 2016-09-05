#!/bin/bash

#-----------------------------------------------------------
#
# start Bacterial Display Application
#
#-----------------------------------------------------------

echo '-> Bacterial Display Script'

dir=$(dirname $0)
cd $dir


echo '-> Starting Bacterial Display Application'

cd ~/c-lab/BacterialDisplay/fcserver
./fcserver-osx &
echo '-> Starting fcserver-osx'

sleep 30

while true; do
	PROCESS=fcserver-osx
	if pgrep $PROCESS
		then
			echo '-> fcserver-osx process already exit'
		else
			cd ~/BacterialDisplay/fcserver
			./fcserver-osx &
			echo '-> Starting fcserver-osx'
	fi

	sleep 10

	PROCESS=BacterialDisplay
	if pgrep $PROCESS
		then
			echo '-> BacterialDisplay process already exit'
		else
			open ~/BacterialDisplay/bin/BacterialDisplay.app
			echo '-> Opening BacterialDisplay'
	fi

	sleep 50
done
