@echo off

IF NOT EXIST build mkdir build
pushd build
cmake ..
cmake --build .
popd
