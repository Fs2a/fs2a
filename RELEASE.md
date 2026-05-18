# Fs2a utilities release notes

This project follows (Semantic Versioning v2.0.0)[https://semver.org/spec/v2.0.0.html].

## v2.1.1
- fix: Prevent fork looping in `Fs2a::Child` and added unitcheck.

## v2.1.0
- feature: Add `Fs2a::Child` class to execute processes with configurable environment and
  optional stdout/stderr capturing.

## v2.0.1
- fix: Incorrect tagging and merging order.

## v2.0.0
- breaking change: Renamed all header files from .h to .hpp. Easily convert all source files with:
  `sed -r -i -e 's/^#include <fs2a\/([a-zA-Z0-9-]+).h>/#include <fs2a\/\1.hpp>/g' *`

## v1.1.0

- feature: Implement substraction of two `Fs2a::NaiveTime` objects.

## v1.0.0

- feature: Add comparison operators for `Fs2a::NaiveTime`
- breaking change: Now using justfile to build release or debug versions in different build
  directories.

## v0.1.0

- feature: Allow adding minutes to `Fs2a::NaiveTime`
- feature: Add minute adding unitcheck for `Fs2a::NaiveTime`

## v0.0.0

- Decided to start using versioning. So this is just the first version that is officially tagged as
  such.
