#include <boost/system/error_code.hpp>
#include <iostream>

int main() {
    for (int i = 0; i <= 125; ++i) { // Примерный диапазон кодов ошибок
        boost::system::error_code ec(i, boost::system::generic_category());
        std::cout << "Code: " << ec.value() << ", Message: " << ec.message() << std::endl;
    }
    return 0;
}
