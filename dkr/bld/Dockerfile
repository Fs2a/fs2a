# @author Simon de Hartog <simon@fs2a.pro>
# @copyright Fs2a Ltd. (c) 2019
# vim:set ts=4 sw=4 noexpandtab:

FROM gentoo/stage3-amd64-nomultilib

ADD l /usr/local/bin/l
ADD p /usr/local/bin/p
RUN chmod 755 /usr/local/bin/[lp]
ADD make.conf /etc/portage/make.conf
RUN echo 'DISTDIR="/zfs/distfiles"' >> /etc/portage/make.conf
ADD bashrc /etc/bash/bashrc
ADD bashrc /etc/bash/bashrc.new
CMD source /bldpkgs.sh
