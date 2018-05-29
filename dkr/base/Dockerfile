# @author Simon de Hartog <simon@fs2a.pro>
# @copyright Fs2a Ltd. (c) 2018
# vim:set ts=4 sw=4 noexpandtab:

FROM scratch
ADD libc-2.25.so /lib64/libc.so.6
ADD libiniparser.so.0.0.0 /usr/lib64/libiniparser.so.0
ADD ld-2.25.so /lib64/ld-linux-x86-64.so.2
ADD q /usr/bin/qtbz2
WORKDIR /
RUN ["/usr/bin/qtbz2","-t","/opt/binpkgs/sys-libs/glibc-2.25-r11.tbz2"]

#ADD ../make.conf /etc/portage/make.conf
#RUN echo 'EMERGE_DEFAULT_OPTS="--usepkgonly"' >> /etc/portage/make.conf
