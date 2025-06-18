wget https://cdn.kernel.org/pub/linux/kernel/v6.x linux-6.14.7.tar.xz
tar xf linux-6.14.7.tar.xz
cd linux-6.14.7
sudo apt install build-essential flex libncurses5-dev bc libelf-dev bison
make defconfig
make -j $(nproc) #Operation NOT PERMITTED ERROR 2
sudo apt install libssl-dev
scripts/config --disable SYSTEM_TRUSTED_KEYS
scripts/config --disable SYSTEM_REVOCATION_KEYS
make -j $(nproc) #Operation NOT PERMITTED ERROR 2
make defconfig
sudo make install #ERROR 2
make clean
make -j $(nproc)
wget https://busybox.net/downloads/busybox-1.34.1.tar.bz2
tar xf busybox-1.34.1.tar.bz2
cd busybox-1.34.1
make defconfig
gedit .config #CONFIG_STATIC=y, CONFIG_TC=n
make -j $(nproc)
file busybox
sudo make install
cd _install
tree -d
mkdir dev proc sys
gedit init #Paste material in medium.com
chmod +x init
find . -print0 | cpio --null -ov --format=newc | gzip -9 > ../initramfs.cpio.gz
qemu-system-x86_64 -kernel ../../arch/x86_64/boot/bzImage -initrd ../initramfs.cpio.gz
