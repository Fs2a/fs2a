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

#include "Singleton.h"

namespace $namespace {

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
		~$1() = default;

		protected:

		public:

	};

} // $namespace namespace
EOF

cat > $1.cpp << EOF
// vim:set ts=4 sw=4 noexpandtab:

#include "$1.h"

namespace $namespace {

	$1::$1()
	: Fs2a::Singleton<$1>::Singleton()
	{

	}

} // $namespace namespace
EOF
