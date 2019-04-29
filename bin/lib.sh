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

	namespace=$(awk '
	BEGIN {
		cnt["Fs2a"] = 0
	}

	/^namespace / {
		if ($2 in cnt) cnt[$2]++;
		else cnt[$2] = 1
	}

	END {
		winner = "Fs2a";
		wincount = 0;

		for (ns in cnt) {
			if (cnt[ns] > wincount) {
				winner = ns;
				wincount = cnt[ns];
			}
		}

		printf "%s\n", winner
	}' *.cpp *.h)
}
