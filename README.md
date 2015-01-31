
Dependencies.

Get the asf sdk:
http://www.atmel.com/System/GetBinary.ashx?target=tcm:26-49230&type=soft&actualTarget=tcm:26-65233

Sync the submodule:
git submodule update --init firmware/nanopb

Install google protobuf:
sudo pacman -S protobuf python2-protobuf

Install gcc for arm bare metal:
sudo pacman -S arm-none-eabi-gcc

Make sure python is python2 not 3.

mkvirtualenv -p `which python2` donglepi
pip install pyserial
pip install protobuf
