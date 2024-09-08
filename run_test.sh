mkdir -p build
cd build
# rm -rf *
cmake -DBUILD_TESTING=ON ..
cmake --build .
# TODO: тесты не запускаются 
ctest

cd ..
# только в ручную
# ./build/lib/egts/crc/tests/crc_tests
./build/lib/egts/transport/tests/transport_tests 