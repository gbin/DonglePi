
![pcb](http://gbin.github.io/DonglePi/images/pcb.png)

DonglePi is a device that gives you a Raspberry Pi P1 compatible connector for your PC.

The RPi P1 is a defacto standard connector on all models with pins for:
- GPIO
- I2C
- SPI
- Serial
- PWM

[More info about it.](http://elinux.org/RPi_Low-level_peripherals)

On the software side, it exposes APIs that are compatible with the ones you can use on the Raspberry Pi like [GPIO](https://pypi.python.org/pypi/RPi.GPIO/) and [smbus](http://www.raspberry-projects.com/pi/programming-in-python/i2c-programming-in-python/using-the-i2c-interface-2) under python.

## Why ?

With DonglePi:
- you can use your familiar PC or Mac to directly develop & debug your RPi embedded applications (with an IDE for example)
- you can use peripherals made for the Raspberry Pi ecosystem on your PC. You can find a list of those [here](http://elinux.org/RPi_VerifiedPeripherals).

DonglePi can also be used to easily setup students for an embedded development course in a preexisting computer lab.

The fidelity with the original hardware won't be 100% but it doesn't need to be to be useful.

## Current Status

It is a prototype working on a breadboard.
The goal is to make a PCB like the one above.

On the software side:
- GPIOs are working.
- I2C is working.
- SPI is in progress.
- PWM needs to be done.
- Serial is currently used for debug but should be easy after all that :).

## Hardware

It can be plugged on any USB2 port.

You can build a prototype on a breadboard using an Atmel SAMD21 development board.
I recommend [this one](http://www.ebay.com/itm/131296219501?_trksid=p2060778.m2749.l2649&var=430589049056&ssPageName=STRK%3AMEBIDX%3AIT). Be sure to take the one with the bootloader if you are not comfortable playing with an Arm/JTAG probe.

For the RPi connector, you can use a [Pi cobbler kit](https://learn.adafruit.com/adafruit-pi-cobbler-kit/overview) from adafruit.

![breadboard](http://gbin.github.io/DonglePi/images/breadboard.jpg)

You can find [here](http://gbin.github.io/DonglePi/video/i2c_demo.mp4) a little demo of it driving an i2c device.

## Software

### Dependencies

Get the [asf sdk](http://www.atmel.com/System/GetBinary.ashx?target=tcm:26-49230&type=soft&actualTarget=tcm:26-65233)
It needs to be linked to the asf directory in the root of the project (by default it looks for the latest version in your home directory).

Sync the submodule:

    git submodule update --init firmware/nanopb

Install google protobuf:

    pip install -user protobuf
    #(or under arch for example)
    sudo pacman -S protobuf python2-protobuf

Install gcc for arm bare metal:

    # (under arch)
    sudo pacman -S arm-none-eabi-gcc
    # Otherwise you can always get the binaries there:
    https://launchpad.net/gcc-arm-embedded/+download

Install pyserial:

    pip install pyserial

### Building the firmware

From firmware/:

    make

It will produce a flash.bin file.

### Installing the fimware

If you have the recommanded dev board, connect it on USB, press Reset + Button B. It will appear as a standard USB storage device.
The red led from the board should slowly blink.
Copy flash.bin to the root of it.
Unmount the disk.
Reset it, the led should switch to solid red = it has correctly initialized and wait for commands.

### Building the client side support.

From python/:

    make

This should generate some files from tht protobuf so the bindings could talk to the hardware.

Here you have a test.py showing how to use the bindings.

### Debug logs.

For the moment, I have dedicated the serial output to that, you can connect it for example to a real RPi at 115200 BPS and use minicom to see the debug logs.

## Help Wanted !

Any contribution or contributor is welcomed !
If you want to contribute some code, feel free to open a pull request.
If you have some experience developing PCBs, feel free to contribute to the Eagle project.

## And the license ?

DonglePi is released under a dual license.
It is GPL for educational and personal use.

It is under a commercial license if you want to produce it and sell it. The money would be reused to develop the v2 of the product under the same dual license.
Contact me if you are interested to produce it: gbin@gootz.net

