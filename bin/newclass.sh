#!/bin/bash

if [ $# -ne 1 ]
then
	echo "Please provide a classname to create files for"
	exit 1
fi

email="simon@fs2a.pro"
if pwd | grep -q 'hyn'
then
	copy="Hear-Your-News LLC"
	ns="HYN"
else
	copy="Fs2a Ltd."
	ns="Fs2a"
fi
copy="${copy} (c) `date +%Y`"

cat > $1.h << EOF
/** @author   Simon de Hartog <$email>
 * @copyright $copy
 * vim:set ts=4 sw=4 noexpandtab: */

#pragma once

namespace $ns {

	class $1
	{
		protected:

		public:
		$1();
		~$1();
	};

} // $ns namespace
EOF

cat > $1.cpp << EOF
/** @author   Simon de Hartog <$email>
 * @copyright $copy
 * vim:set ts=4 sw=4 noexpandtab: */

#include "$1.h"

namespace $ns {



} // $ns namespace
EOF
