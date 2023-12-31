#include "simple_serial.hpp"
/**
 * @brief Construct a new Simple Serial:: Simple Serial object
 * @param port - port name (e.g. /dev/ttyACM0)
 * @param baudrate - baudrate (e.g. 9600)
 * @param timeout - timeout in seconds (e.g. 1)
*/
SimpleSerial::SimpleSerial(std::string port, int baudrate,double timeout) {
    fd = open(port.c_str(), O_RDWR);
    this->timeout = timeout;
    if (fd == -1) {
        throw std::runtime_error("open_port: Unable to open " + port + " - " + std::string(std::strerror(errno)) + "\n");
    }
    struct termios options;
    if(tcgetattr(fd, &options) != 0) {
        throw std::runtime_error("tcgetattr: Unable to get " + port + " attributes - " + std::string(std::strerror(errno)) + "\n");
    }
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    options.c_cflag &= ~CRTSCTS;
    options.c_cflag |= CREAD | CLOCAL;
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    options.c_iflag &= ~(IXON | IXOFF | IXANY);
    options.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); 
    options.c_oflag &= ~OPOST;
    options.c_oflag &= ~ONLCR;
    options.c_cc[VTIME] = timeout*10;
    options.c_cc[VMIN] = 0;
    cfsetispeed(&options, baudrate);
    cfsetospeed(&options, baudrate);
    if(tcsetattr(fd, TCSANOW, &options) != 0) {
        throw std::runtime_error("tcsetattr: Unable to set " + port + " attributes - " + std::string(std::strerror(errno)) + "\n");
    }
    setBufferSize(buffer_size);
}

/**
 * @brief Destroy the Simple Serial:: Simple Serial object
*/
SimpleSerial::~SimpleSerial() {
    closePort();
}

/**
 * @brief Set buffer size
 * @param size - buffer size
 * @return int - 0 if successful else -1
*/
int SimpleSerial::setBufferSize(int size) {
    buffer_size = size;
    buffer = new char[size];
    return 0;
}

/**
 * @brief Check if data is available for reading
 * @return int - number of bytes available for reading
*/
int SimpleSerial::availableForRead() {
    int bytes_available;
    ioctl(fd, FIONREAD, &bytes_available);
    return bytes_available;
}
/**
 * @brief Write data to serial port
 * @param data - data to be written
 * @return int - number of bytes written
*/
int SimpleSerial::writeToSerial(std::string data) {
    return write(fd, data.c_str(), data.size());
}

/**
 * @brief Read data from serial port
 * @param size - number of bytes to be read
 * @return std::string - data read
*/
std::string SimpleSerial::readFromSerial( int size) {
    std::string data;
    if(size > buffer_size) {
        setBufferSize(size);
    }
    int n = read(fd, buffer, size);
    data = std::string(buffer, n);
    return data;
}

/**
 * @brief Read all data from serial port
 * @return std::string - data read
*/
std::string SimpleSerial::readAll() {
    std::string data;
    int n = availableForRead();
    data = readFromSerial(n);
    return data;
}
/**
 * @brief Read until a character is found
 * @param  sequence- the sequence till which to be read
 * */   
std::string SimpleSerial::readUntil(std::string sequence){
    std::string line;
    int sequence_length = sequence.length();
    auto start = std::chrono::steady_clock::now();
    auto now = std::chrono::steady_clock::now();
    while(std::chrono::duration_cast<std::chrono::seconds>(now - start).count() < timeout){
        line += readFromSerial(sequence_length);
        int sequence_index = line.find(sequence);
        if(sequence_index != std::string::npos){
            return line.substr(0, sequence_index + sequence_length);
        }
        now = std::chrono::steady_clock::now();
    }
    line = "";
    return line;
}

/**
 * @brief Read a line from serial port
 * @return std::string - line read
*/
std::string SimpleSerial::readline() {
    return readUntil("\n");
}
/**
 * @brief Close serial port
 * @return int - 0 if successful else -1
 * @note This function is called by the destructor
*/
int SimpleSerial::closePort() {
    return close(fd);
}