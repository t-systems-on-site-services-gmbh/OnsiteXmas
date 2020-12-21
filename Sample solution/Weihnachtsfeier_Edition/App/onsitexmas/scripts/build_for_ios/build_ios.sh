#!/bin/sh

if [ -z "$1" ]
then
	echo "Choose argument:\nuopen	update and open in Xcode \ninit	run first time";
	echo "Don't forget to add the BLE Strings to your Info.plist or simply exchange the Info.plist with the file in this directory!";

else
	
	
	if [ $1 == "open" ]
	then
		cd ../../
		ionic build
		npx cap sync
		npx cap open ios
		
	fi

	if [ $1 == "init" ]
	then
		cd ../../
		ionic build
		npx cap add ios
		npm install cordova-res --save-dev
		npm run resources
		echo "Don't forget to add the BLE Strings to your Info.plist or simply exchange the Info.plist with the file in this directory!";
		npx cap open ios
	fi
fi