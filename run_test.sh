mkdir -p build
cd build
rm -rf *
cmake -DBUILD_TESTING=ON ..
cmake --build .
# TODO: тесты не запускаются 
ctest

# только в ручную
./lib/egts/crc/tests/crc_tests