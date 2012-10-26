#!/bin/bash

#really simple to install
echo "- copy binary /usr/bin/edn"
sudo cp -vf out/Linux/release/staging/usr/bin/edn /usr/bin/edn
sudo mkdir -p /usr/share/edn
echo "- Remove the folder /usr/share/edn/*"
sudo rm -rf /usr/share/edn/*
sudo cp -rfv out/Linux/release/staging/usr/share/* /usr/share/
