#!/bin/bash

if [ $# -ne 1 ]
then
	echo "Please provide a classname to create files for"
	exit 1
fi

source $(readlink -f $(dirname ${BASH_SOURCE[0]}))/lib.sh

cat > $1.h << EOF
$(header)

#pragma once

namespace $namespace {

	class $1
	{
		protected:

		public:
		$1();
		~$1();
	};

} // $namespace namespace
EOF

cat > $1.cpp << EOF
$(header)
#include "$1.h"

namespace $namespace {



} // $namespace namespace
EOF
