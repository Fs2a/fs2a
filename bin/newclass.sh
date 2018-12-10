#!/bin/bash

if [ $# -ne 1 ]
then
	echo "Please provide a classname to create files for"
	exit 1
fi

source $(readlink -f $(dirname ${BASH_SOURCE[0]}))/lib.sh

setns

cat > $1.h << EOF
// vim:set ts=4 sw=4 noexpandtab:

#pragma once

namespace $namespace {

	class $1
	{
		protected:

		public:
			/// Constructor
			$1();

			/// Destructor
			~$1();
	};

} // $namespace namespace
EOF

cat > $1.cpp << EOF
// vim:set ts=4 sw=4 noexpandtab:

#include "$1.h"

namespace $namespace {



} // $namespace namespace
EOF
