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


while true; do
	PROCESS=fcserver-osx
	if pgrep $PROCESS
		then
			echo '-> fcserver-osx process already exit'
		else
			cd ~/c-lab/BacterialDisplay/fcserver
			./fcserver-osx &
			echo '-> Starting fcserver-osx'
	fi

	PROCESS=BacterialDisplay
	if pgrep $PROCESS
		then
			echo '-> BacterialDisplay process already exit'
		else
			cd ~/c-lab/fcserver
			open ~/c-lab/BacterialDisplay/bin/BacterialDisplay.app 
			echo '-> Starting fcserver-osx'
	fi


	sleep 60
done
