#!/bin/bash
/debootstrap/debootstrap --second-stage

echo "coobocEmu" > /etc/hostname

cat <<END > /etc/apt/sources.list
deb http://mirrors.sohu.com/ubuntu utopic main multiverse restricted universe
END
