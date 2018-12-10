#!/bin/bash

# @author    Simon de Hartog <simon@fs2a.pro>
# @copyright Fs2a Ltd. (c) 2018

namespace=""

function setns() {
	if [ -n "${namespace}" ]
	then
		# Already done
		return
	fi

	if pwd | grep -q 'hyn'
	then
		namespace="HYN"
	fi

	if pwd | grep -q 'vwapi'
	then
		namespace="Cloud2com"
	fi

	if [ -z "${namespace}" ]
	then
		namespace="Fs2a"
	fi
}
