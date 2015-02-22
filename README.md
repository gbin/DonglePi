DonglePi is a device that gives you a Raspberry Pi P1 compatible connector for your PC.

P1 is a connector with:
- GPIO
- I2C
- SPI
- Serial
- PWM

More info [about the connector](http://elinux.org/RPi_Low-level_peripherals)

Hardware
--------

It can be plugged to any USB2 port.

You can build a prototype on a breadboard using an Atmel SAMD21 development board.
I recommend [this one](http://www.ebay.com/itm/131296219501?_trksid=p2060778.m2749.l2649&var=430589049056&ssPageName=STRK%3AMEBIDX%3AIT)


Software
--------


## Dependencies

Get the [asf sdk](http://www.atmel.com/System/GetBinary.ashx?target=tcm:26-49230&type=soft&actualTarget=tcm:26-65233)

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
