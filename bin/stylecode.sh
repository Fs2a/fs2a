#!/bin/bash

# This script styles all C++ source files

cd $(git rev-parse --show-toplevel); \
astyle --options=cfg/astylerc -r -q "chk/*.cpp" \
"src/*.cpp" "src/*.h"
