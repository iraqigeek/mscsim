#!/bin/bash

################################################################################

APP_NAME=mscsim

################################################################################

APP_DIR=${APP_NAME}_*_focal_amd64

sudo rm -R $APP_DIR
sudo rm $APP_DIR.deb

################################################################################

read -p "Press any key to continue..." -n1 -s
