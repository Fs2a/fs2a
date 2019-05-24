# vim:set ts=4 sw=4 noexpandtab tw=120:
FROM fs2a/ubuntubase
LABEL copyright="Fs2a Ltd. (c) 2019"
LABEL maintainer="Simon de Hartog <simon@fs2a.pro>"
ENV DEBIAN_FRONTEND "noninteractive"
ENV APT_OPTIONS "-y --no-install-recommends --no-install-suggests -q"
ENV TERM "screen"

# Install basic packages
RUN apt-get ${APT_OPTIONS} update && \
	apt-get ${APT_OPTIONS} install \
	libasound2 \
	libgtk2.0 \
	libnotify4 \
	libnss3 \
	libpulse0 \
	libv4l-0 \
	libx11-xcb1 \
	libxss1 \
	libxtst6 \
	&& apt-get ${APT_OPTIONS} autoremove && \
	apt-get ${APT_OPTIONS} clean && \
	rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*
