# Blinds/shades automation

## Over engineering things into oblivion (^:

## How it works 

It works in the assumption that your shades motor have a switch to close and switch to open.
In my home I'll be using a mix of raspberry pi activating relay and a usb relay board.

## How to find in what usb is your relay

```bash
$ lsusb
> Bus 003 Device 010: ID 0519:2018 Star Micronics Co., Ltd HIDRelay
$ sudo dmesg | grep -i '0519:2018'
> [ 2450.629455] hid-generic 0003:0519:2018.0014: hiddev96,hidraw1: USB HID v1.00 Device [Ucreatefun.com HIDRelay] on usb-0000:04:00.3-2/input0
```
in this example the ID is usb-0000:04:00.3-2 

Doing this this way allows me to use two boards with the same ID.
You have to use always the same usb port