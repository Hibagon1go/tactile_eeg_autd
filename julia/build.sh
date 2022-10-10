#!/bin/bash

CMDNAME=`basename $0`
script_dir="$( dirname -- "$( readlink -f -- "$0"; )"; )"

pushd $script_dir
cd ..
mkdir -p build
cd build
cmake .. -DBUILD_ALL=ON
cmake --build . --parallel 8
cd ..
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
  dst="julia/src/NativeMethods/bin/linux-x64"
  ext="so"
elif [[ "$OSTYPE" == "darwin"* ]]; then
  dst="julia/src/NativeMethodspyautd3/bin/macos-universal"
  ext="dylib"
fi
mkdir -p $dst

cp build/bin/*.$ext $dst

popd
