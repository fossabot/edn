#!/bin/bash

#really simple to install
sudo cp linux/bin/release/edn /usr/bin/edn
sudo chmod 777 /usr/bin/edn
sudo mkdir -p /usr/share/edn
sudo rm -r /usr/share/edn/*
sudo cp -r assets/* /usr/share/edn/
