#!/bin/bash
# @author    Simon de Hartog <simon@fs2a.pro>
# @copyright Fs2a Ltd. (c) 2019
# vim:set ts=4 sw=4 noexpandtab ft=sh:

# Ugly workaround because Ubuntu chose mawk as default AWK which does not
# support the strtonum() function.
gw=$(awk 'function hextonum(h) {
	hex["0"] = 0; hex["1"] = 1; hex["2"] = 2; hex["3"] = 3;
	hex["4"] = 4; hex["5"] = 5; hex["6"] = 6; hex["7"] = 7;
	hex["8"] = 8; hex["9"] = 9; hex["A"] = 10; hex["B"] = 11;
	hex["C"] = 12; hex["D"] = 13; hex["E"] = 14; hex["F"] = 15;
	return hex[substr(h,1,1)] * 16 + hex[substr(h,2,1)];
}
$2 == "00000000" {
	printf "%d.%d.%d.%d\n",
	hextonum(substr($3,7,2)),
	hextonum(substr($3,5,2)),
	hextonum(substr($3,3,2)),
	hextonum(substr($3,1,2))
}' /proc/net/route)

if [ -z "$gw" ]
then
	echo "Auto APT cacher: No default gateway found, unable to determine APT cacher presence"
else
	if echo -n "" 2>/dev/null >/dev/tcp/$gw/3142
	then
		echo "Auto APT cacher: Found proxy at $gw:3142, setting as APT cacher"
		echo "Acquire::http::Proxy \"http://$gw:3142\";" > \
			/etc/apt/apt.conf.d/00_proxy.conf
	else
		echo "Auto APT cacher: No APT cacher found, removing Proxy config if present"
		rm -f /etc/apt/apt.conf.d/00_proxy.conf
	fi
fi
