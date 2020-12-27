#!/bin/bash
# Copyright (c) 2020 Simon de Hartog <simon@dehartog.name>
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
# 1. Redistributions of source code must retain the above copyright notice,
# this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
# notice, this list of conditions and the following disclaimer in the
# documentation and/or other materials provided with the distribution.
# 3. Neither the name of the copyright holder nor the names of its
# contributors may be used to endorse or promote products derived from this
# software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
# IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
# THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
# PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# vim:set ts=4 sw=4 noet tw=120:

# Installs Ubuntu in separate dir when not there yet, including all packages
# needed to install Zoiper in it afterwards.

function fail() {
	echo $*
	exit 1
}

base="/opt/zoiper"

command -v debootstrap &>/dev/null || \
	fail "Please install debootstrap with: ACCEPT_KEYWORDS=\"~amd64\" sudo emerge -av debootstrap"
[ -r /usr/share/debootstrap/scripts/bionic ] || fail "Please install a beta version of debootstrap"
[ -d $base ] || sudo install -o root -g root -d $base || fail "Failed creating $base"
sudo debootstrap bionic /opt/zoiper http://ftp.halifax.rwth-aachen.de/ubuntu/
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
/dev/shm        /dev/shm        none    rw,bind         0       0
/home/sdh       /home/sdh       none    rw,bind         0       0
/tmp            /tmp            none    rw,bind         0       0
/run/user/1000  /run/user/1000  none    rw,bind         0       0

# For PulseAudio and other desktop-related things
/var/lib/dbus    /var/lib/dbus  none    rw,bind         0       0
EOF
sudo install -o root -g root -m 0644 $tmpfile /etc/schroot/ubuntu/fstab
fi

sudo cp /dev/null /etc/schroot/ubuntu/nssdatabases

tmpfile=$(mktemp)
cat >$tmpfile <<EOF
[bionic]
type=directory
description=Ubuntu Bionic Beaver for Zoiper
root-users=$USER,root
directory=$base
preserve-environment=true
profile=ubuntu
EOF
sudo install -o root -g root -m 0644 $tmpfile /etc/schroot/chroot.d/bionic
sudo bash -c "echo deb http://ftp.halifax.rwth-aachen.de/ubuntu bionic main universe > $base/etc/apt/sources.list"

if ! sudo schroot -c bionic -- dpkg -l | grep -q "^ii *zoiper5" ; then
	sudo schroot -c bionic -- apt-get update
	sudo schroot -c bionic -- apt-get install -y libnotify4 libv4l-0 libnss3
	sudo schroot -c bionic -- apt-get install -y libgtk2.0-0 libxss1 libpulse0
	sudo schroot -c bionic -- apt-get install -y syslog-ng-core libasound2-dev libgl1
	sudo schroot -c bionic -- apt-get install -y  libcanberra-gtk-module libcanberra-gtk3-module
	sudo schroot -c bionic -- dpkg -i /home/sdh/Cloud2com/Relations/Zoiper/zoiper5*.deb
	sudo schroot -c bionic -- install -o root -g root -m 1777 -d /run/user
fi

exit 0
