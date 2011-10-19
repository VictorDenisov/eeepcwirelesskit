#!/bin/bash

rmmod usbhid
./mouse_charge
modprobe usbhid
