#!/bin/bash

if [ $# -ne 1 ]
then
	echo "Please provide a classname to create files for"
	exit 1
fi

EMAIL="simon@fs2a.pro"
if pwd | grep -q 'hyn'
then
	COPY="Hear-Your-News LLC"
else
	COPY="Fs2a Ltd."
fi
COPY="${COPY} (c) `date +%Y`"

cat > $1.h << EOF
/** @author   Simon de Hartog <$EMAIL>
 * @copyright $COPY
 * vim:set ts=4 sw=4 noexpandtab: */

#pragma once

class $1
{
	protected:

	public:
	$1();
	~$1();
};
EOF

cat > $1.cpp << EOF
/** @author   Simon de Hartog <$EMAIL>
 * @copyright $COPY
 * vim:set ts=4 sw=4 noexpandtab: */

#include "$1.h"

EOF
