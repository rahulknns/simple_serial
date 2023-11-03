#include "../src/simple_serial.hpp"
#include <iostream>

int main() {
    SimpleSerial serial("/dev/ttyACM0", 9600, 1);
    while (true)
    {
        std::string data = serial.readFromSerial(1);
        std::cout<<data;
    }
    return 0;
}