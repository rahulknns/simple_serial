#ifndef SIMPLE_SERIAL_HPP
#define SIMPLE_SERIAL_HPP
#include <unistd.h>
#include <stdio.h>
#include <termios.h>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <string>

class SimpleSerial {
private:
    int fd;
    int buffer_size = 256;
    char* buffer;
public:
    SimpleSerial(std::string port, int baudrate,int timeout);
    ~SimpleSerial();
    int setBufferSize(int size);
    int writeToSerial(std::string data);
    std::string readFromSerial(int size);
    int closePort();

};

#endif // SIMPLE_SERIAL_HPP