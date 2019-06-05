#!/bin/bash

# vim:set ts=4 sw=4 noexpandtab tw=120:

# Installs Ubuntu in separate dir when not there yet, including all packages
# needed to install Zoiper in it afterwards.

function fail() {
	echo $*
	exit 1
}

base="/opt/zoiper"

[ -d $base ] || sudo install -o root -g root -d $base || fail "Failed creating $base"
command -v debootstrap &>/dev/null || fail "Please install debootstrap with: sudo emerge -av debootstrap"
sudo debootstrap xenial /opt/zoiper http://ftp.halifax.rwth-aachen.de/ubuntu/
command -v schroot &>/dev/null || fail "Please install schroot with: sudo emerge -av schroot"
[ -d /etc/schroot/ubuntu ] || sudo install -o root -g root -d /etc/schroot/ubuntu

[ -r /etc/schroot/ubuntu/copyfiles ] || sudo cp /etc/schroot/desktop/copyfiles /etc/schroot/ubuntu/copyfiles

if ! [ -r /etc/schroot/ubuntu/fstab ]; then
tmpfile=$(mktemp)
cat >$tmpfile <<EOF
# <file system> <mount point>   <type>  <options>       <dump>  <pass>
/proc           /proc           none    rw,bind         0       0
/sys            /sys            none    rw,bind         0       0
/dev            /dev            none    rw,bind         0       0
/dev/pts        /dev/pts        none    rw,bind         0       0
/home/sdh       /home/sdh       none    rw,bind         0       0
/tmp            /tmp            none    rw,bind         0       0

# For PulseAudio and other desktop-related things
/var/lib/dbus    /var/lib/dbus  none    rw,bind         0       0
EOF
sudo install -o root -g root -m 0644 $tmpfile /etc/schroot/ubuntu/fstab
fi

sudo cp /dev/null /etc/schroot/ubuntu/nssdatabases

tmpfile=$(mktemp)
cat >$tmpfile <<EOF
[xenial]
type=directory
description=Ubuntu Xenial Xerus for Zoiper
root-users=$USER,root
directory=$base
profile=ubuntu
EOF
sudo install -o root -g root -m 0644 $tmpfile /etc/schroot/chroot.d/xenial

if ! sudo schroot -c xenial -- getent passwd _apt &>/dev/null; then
	sudo schroot -c xenial -- adduser --quiet --system --home /nonexistent --shell /bin/false \
	--no-create-home --uid 168 --gid 65534 --disabled-password --force-badname _apt
fi

if ! sudo schroot -c xenial -- getent group staff &>/dev/null; then
	sudo schroot -c xenial -- addgroup --quiet --system staff
fi

if ! sudo schroot -c xenial -- grep -q "de\.archive.ubuntu.com" /etc/apt/sources.list; then
	sudo schroot -c xenial -- sed -i -e 's/\/\/archive\.ubuntu/de.archive.ubuntu/g' /etc/apt/sources.list
fi

if ! sudo schroot -c xenial -- dpkg -l | grep -q "^ii.*zoiper5" ; then
	sudo schroot -c xenial -- apt-get install -y libnotify4 libv4l-0 libnss3
	sudo schroot -c xenial -- dpkg -i /home/sdh/Cloud2com/Relations/Zoiper/*.deb
fi

exit 0
