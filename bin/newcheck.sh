#!/bin/bash

if [ $# -ne 1 ]
then
	echo "Please provide a classname to create a check for"
	exit 1
fi

source $(readlink -f $(dirname ${BASH_SOURCE[0]}))/lib.sh

class=$(echo $1 | tr '[:upper:]' '[:lower:]')

cat > $class.cpp << EOF
$(header)

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <$1.h>

#define CHECKNAME ${class}Check

class CHECKNAME;

CPPUNIT_TEST_SUITE_REGISTRATION(CHECKNAME);

class CHECKNAME : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(CHECKNAME);
	CPPUNIT_TEST(firstTest);
	CPPUNIT_TEST_SUITE_END();

	public:
		void setUp() {
		}

		void tearDown() {
		}

		void firstTest() {

		}

};
EOF
