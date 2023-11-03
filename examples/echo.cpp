#include "../src/simple_serial.hpp"
#include <iostream>

int main() {
    SimpleSerial serial("/dev/ttyACM0", 9600, 1);
    std::string data;
    while (true)
    {
        int no_of_bytes = serial.availableForRead();
        std::cout<<"No of Bytes:"<<no_of_bytes<<std::endl;
        if (no_of_bytes > 0) {
            std::string data = serial.readFromSerial(no_of_bytes);
            serial.writeToSerial(data);
        }
        //Wait for 1 second
        usleep(1000000);
    }
    return 0;
}