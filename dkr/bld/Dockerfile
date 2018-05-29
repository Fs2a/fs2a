# @author Simon de Hartog <simon@fs2a.pro>
# @copyright Fs2a Ltd. (c) 2018
# vim:set ts=4 sw=4 noexpandtab:

FROM gentoo/stage3-amd64-nomultilib

ADD l /usr/local/bin/l
ADD p /usr/local/bin/p
RUN chmod 755 /usr/local/bin/[lp]
RUN eselect news purge
ADD ../make.conf /etc/portage/make.conf
RUN echo 'DISTDIR="/zfs/distfiles"' >> /etc/portage/make.conf

# Emerge or Re-emerge needed software
RUN emerge \
	app-shells/bash \
	app-portage/gentoolkit \
	sys-libs/ncurses

# Install custom configurations
ADD bashrc /etc/bash/bashrc

# Create binary packages from installed files
RUN quickpkg --include-config=y \
	app-shells/bash \
	sys-libs/glibc \
	sys-libs/ncurses \
	sys-libs/readline
