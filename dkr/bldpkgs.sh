#!/bin/bash
# @author Simon de Hartog <simon@fs2a.pro>
# @copyright Fs2a Ltd. (c) 2018
# vim:set ts=4 sw=4 noexpandtab:

# Build all packages here instead of in the Dockerfile, because during
# build, the volumes are not available.

# Mark all news as read
eselect news read >/dev/null

# Emerge or re-emerge needed software
emerge -NDuv \
	app-shells/bash \
	app-portage/gentoolkit \
	sys-libs/ncurses || exit 1

# Install custom configurations again
mv /etc/bash/bashrc.new /etc/bash/bashrc

# Create binary packages from installed files
quickpkg --include-config=y \
	app-shells/bash \
	sys-libs/glibc \
	sys-libs/ncurses \
	sys-libs/readline
