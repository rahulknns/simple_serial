#ifndef SIMPLE_SERIAL_HPP
#define SIMPLE_SERIAL_HPP
#include <unistd.h>
#include <stdio.h>
#include <termios.h>
#include <sys/ioctl.h>      /* ioctl */
#include <cstring>
#include <chrono>
#include <fcntl.h>
#include <iostream>
#include <string>

class SimpleSerial {
private:
    int fd;
    int buffer_size = 256;
    char* buffer;
    double timeout;
public:
    SimpleSerial(std::string port, int baudrate,double timeout);
    ~SimpleSerial();
    int setBufferSize(int size);
    int availableForRead();
    int writeToSerial(std::string data);
    std::string readFromSerial(int size);
    std::string readAll();
    std::string readUntil(std::string sequence);
    std::string readline();
    int closePort();

};

#endif // SIMPLE_SERIAL_HPP