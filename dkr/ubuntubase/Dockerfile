# vim:set ts=4 sw=4 noexpandtab tw=120:
FROM ubuntu:bionic
LABEL copyright="Fs2a Ltd. (c) 2019"
LABEL maintainer="Simon de Hartog <simon@fs2a.pro>"
ENV DEBIAN_FRONTEND "noninteractive"
ENV APT_OPTIONS "-y --no-install-recommends --no-install-suggests -q"
ENV TERM "screen"

# Copy Automatic APT cacher config script and make it executable
ADD auto_apt_cacher.sh /10_auto_apt_cacher.sh
RUN chmod 0755 /10_auto_apt_cacher.sh

# Determine presence of APT cacher automatically
RUN /10_auto_apt_cacher.sh

# Install basic packages
RUN apt-get ${APT_OPTIONS} update && \
	apt-get ${APT_OPTIONS} install apt-utils && \
	apt-get ${APT_OPTIONS} install locales && \
	locale-gen en_US.UTF-8 en_DK.UTF-8 && \
	apt-get ${APT_OPTIONS} install \
	dnsutils \
	iputils-ping \
	iproute2 \
	less \
	net-tools \
	openssh-server \
	runit \
	vim-tiny \
	&& apt-get ${APT_OPTIONS} autoremove && \
	apt-get ${APT_OPTIONS} clean && \
	rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

# Set Locale properly. update-locale does not work. Copied from:
# http://jaredmarkell.com/docker-and-locales/
ENV LANG en_US.UTF-8
ENV LANGUAGE en_US:en
ENV LC_COLLATE POSIX
ENV LC_CTYPE POSIX
ENV LC_MEASUREMENT en_DK.UTF-8
ENV LC_MESSAGES POSIX
ENV LC_TIME en_DK.UTF-8

# Start runit by default
ENTRYPOINT ["/sbin/runit"]

# Add SdH user to run unprivileged processes
RUN useradd -c "Simon de Hartog" -d /home/sdh \
	-m -k /dev/null -N -g 100 -u 1000 \
	-s /bin/bash sdh && \
	install -o sdh -g users -m 0700 -d /home/sdh/.ssh && \
	usermod -p '*' sdh && passwd -u sdh

# Setup initialization scripts
RUN install -o root -g root -m 0755 -d \
	/etc/runit \
	/etc/service/sshd \
	/var/run/sshd

# Configure SSH authorized keys
RUN install -o root -g root -m 0700 -d /root/.ssh
ADD sdh.authkeys /root/.ssh/authorized_keys
ADD sdh.authkeys /home/sdh/.ssh/authorized_keys
RUN chown root:root /root/.ssh/authorized_keys
RUN chown sdh:users /home/sdh/.ssh/authorized_keys

ADD 1 /etc/runit/1
ADD 2 /etc/runit/2
ADD 3 /etc/runit/3
RUN chmod 755 /etc/runit/[123]

# Set up SSH server
ADD sshd_config /etc/ssh/sshd_config
RUN rm -f /etc/ssh/ssh_host*_key*
ADD sshd-run /etc/service/sshd/run
RUN chmod 755 /etc/service/sshd/run
