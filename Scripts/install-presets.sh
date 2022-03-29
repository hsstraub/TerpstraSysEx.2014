#!/bin/bash

# Please run in the same directory as the Presets folder

PRESETS=`pwd`/Presets

cd ~/Documents

[[ -d "Lumatone Editor" ]] || mkdir "Lumatone Editor" && chmod 777 "Lumatone Editor"

cp -n -r ${PRESETS}/* "Lumatone Editor"

chmod -R 777 "Lumatone Editor"

exit 0
