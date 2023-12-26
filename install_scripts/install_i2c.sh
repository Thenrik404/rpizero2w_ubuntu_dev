# on ubuntu 23.04 arm64 server

wget https://archive.raspberrypi.org/debian/pool/main/r/raspi-config/raspi-config_20231216_all.deb -P /tmp
sudo apt-get install -fy
# (sudo dpkg -i /tmp/raspi-config_20231216_all.deb)
sudo apt install alsa-utils lua5.1
sudo apt-get install libnewt0.52 whiptail parted triggerhappy lua5.1 alsa-utils -y

sudo apt --fix-broken install
sudo dpkg -i /tmp/raspi-config_20231216_all.deb
sudo mount /dev/mmcblk0p1 /boot
sudo raspi-config
sudo reboot
sudo apt install libi2c0 libi2c-dev i2c-tools

sudo i2cdetect -y 1

git clone https://github.com/alex-mous/MPU6050-C-CPP-Library-for-Raspberry-Pi.git