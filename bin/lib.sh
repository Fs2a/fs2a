#!/bin/bash

# @author    Simon de Hartog <simon@fs2a.pro>
# @copyright Fs2a Ltd. (c) 2018

copy="Fs2a Ltd."
email="simon@fs2a.pro"
namespace="Fs2a"

function setorg() {
	if [ "${copy}" = *" (c) "* ]
	then
		# Already done
		return
	fi

	if pwd | grep -q 'hyn'
	then
		copy="Hear-Your-News LLC"
		ns="HYN"
	fi

	copy="${copy} (c) `date +%Y`"
}

function header() {
setorg

cat << EOF
/** @author   Simon de Hartog <$email>
 * @copyright $copy
 * vim:set ts=4 sw=4 noexpandtab: */

EOF
}
