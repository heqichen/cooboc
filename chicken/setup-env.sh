#!/bin/bash

echo BB-UART1 > /sys/devices/bone_capemgr.*/slots
echo BB-UART2 > /sys/devices/bone_capemgr.*/slots

stty -F /dev/ttyO1 cs8 9600 ignbrk -brkint -icrnl -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echoke noflsh -ixon -crtscts
stty -F /dev/ttyO2 cs8 9600 ignbrk -brkint -icrnl -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echoke noflsh -ixon -crtscts
