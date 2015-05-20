#/bin/bash

ARCH=i386
#VARIANT="--variant=minibase"
DIST=utopic
ROOT_DIR=$DIST-$ARCH

mkdir $ROOT_DIR

sudo debootstrap --verbose --arch $ARCH $VARIANT --foreign $DIST $ROOT_DIR http://mirrors.sohu.com/ubuntu/


sudo cp /usr/bin/qemu-i386-static $ROOT_DIR/usr/bin/

#sudo mkdir -p $ROOT_DIR/dev/pts
#sudo mount --bind /dev/pts $ROOT_DIR/dev/pts
#sudo mount --bind /proc $ROOT_DIR/proc

sudo cp debootstrap_init.sh $ROOT_DIR/root/
sudo chroot $ROOT_DIR /root/debootstrap_init.sh

