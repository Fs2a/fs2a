#!/bin/bash

if [ $# -ne 1 ]
then
	echo "Please provide a classname to create files for"
	exit 1
fi

cat > $1.h << EOF
/** @author   Simon de Hartog <simon@fs2a.pro>
 * @copyright Fs2a Ltd. (c) `date +%Y`
 * vim:set ts=4 sw=4 noexpandtab: */

#pragma once

#include "Singleton.h"

namespace Fs2a {

	class $1 : public Fs2a::Singleton<$1>
	{
		/// Singleton template as friend for construction
		friend class Fs2a::Singleton<$1>;

		private:
			/// Default constructor
			$1();

			/// Copy constructor
			$1(const $1 & obj_i) = delete;

			/// Assignment constructor
			$1 & operator=(const $1 & obj_i) = delete;

			/// Destructor
			~$1();

		protected:

		public:

	};

} // Fs2a namespace
EOF

cat > $1.cpp << EOF
/** @author   Simon de Hartog <simon@fs2a.pro>
 * @copyright Fs2a Ltd. (c) `date +%Y`
 * vim:set ts=4 sw=4 noexpandtab: */

#include "$1.h"

namespace Fs2a {

	$1::$1()
	{
	}

	$1::~$1()
	{
	}

} // Fs2a namespace
EOF
