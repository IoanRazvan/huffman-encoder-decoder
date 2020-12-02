#ifndef FILE_STREAM_HANDLING_H
#define FILE_STREAM_HANDLING_H

#include <fstream>
#include <stdexcept>

std::ifstream openFileInputStream(const std::string & fileName, std::ios_base::openmode mode = std::ios::in);
std::ofstream openFileOutputStream(const std::string & fileName, std::ios_base::openmode mode = std::ios::out);

class file_error : public std::runtime_error {
public:
    file_error(const std::string & fileName);
    file_error(std::streamsize expected, std::streamsize actual);
};

#endif