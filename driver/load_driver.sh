#!/bin/bash
ifconfig sn0 down
rmmod hellosnic.ko
rm /dev/scull0
rm /dev/scull_ctl

make
insmod hellosnic.ko

major=$(awk "\$2==\"scull\" {print \$1}" /proc/devices)
mknod /dev/scull0 c $major 0
mknod /dev/scull_ctl c $major 1
chmod o+w /dev/scull0
chmod o+w /dev/scull_ctl

ifconfig sn0 192.168.13.10