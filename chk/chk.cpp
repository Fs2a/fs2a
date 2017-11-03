/** @author   Simon de Hartog <simon@fs2a.eu>
 * @copyright Fs2a Ltd. (c) 2017 */

#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

using namespace std;

/** Runs the check application.
 * @returns 0 on success, 1 if a check failed. */
int main(void)
{
	CppUnit::TextUi::TestRunner runner;
	bool retval = false;

	// Set up test suite
	CppUnit::TestFactoryRegistry & registry =
		CppUnit::TestFactoryRegistry::getRegistry();
	runner.addTest(registry.makeTest());

	// Actually run all tests
	retval = runner.run();

	// Convert the C-style true (1) to Shell-style true (0) and v.v.
	return retval?0:1;
}
