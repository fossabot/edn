#!/bin/bash

#really simple to install
echo "- copy binary /usr/bin/edn"
sudo cp -vf linux/bin/release/edn /usr/bin/edn
sudo chmod 777 /usr/bin/edn
sudo mkdir -p /usr/share/edn
echo "- Remove the folder /usr/share/edn/*"
sudo rm -rf /usr/share/edn/*
sudo cp -rfv assets/* /usr/share/edn/
