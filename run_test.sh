mkdir -p build
cd build
rm -rf *
cmake -DBUILD_TESTING=ON ..
cmake --build .
ctest -rerun-failed --output-on-failure
