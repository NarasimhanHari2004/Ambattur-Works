mkdir -p ~/iso/boot/isolinux
cp ~/Downloads/linux-6.14.7/arch/x86/boot/bzImage ~/iso/boot/vmlinuz
cp ~/Downloads/linux-6.14.7/busybox-1.36.1/initramfs.cpio.gz ~/iso/boot/initramfs.cpio.gz
sudo apt-get install syslinux-utils
cp /usr/lib/ISOLINUX/isolinux.bin ~/iso/boot/isolinux/
cp /usr/lib/syslinux/modules/bios/ldlinux.c32 ~/iso/boot/isolinux/
nano ~/iso/boot/isolinux/isolinux.cfg
DEFAULT linux
LABEL linux
  KERNEL /boot/vmlinuz
  APPEND initrd=/boot/initramfs.cpio.gz console=tty0
  TIMEOUT 50
  PROMPT 1
genisoimage -o ~/linux-busybox.iso \ -b boot/isolinux/isolinux.bin \ -c boot/isolinux/boot.cat \ -no-emul-boot -boot-load-size 4 -boot-info-table \ -J -R -V "LINUX_BUSYBOX" ~/iso
