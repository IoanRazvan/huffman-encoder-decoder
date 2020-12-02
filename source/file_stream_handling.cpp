#include <cstring>
#include <utility>
#include "../headers/file_stream_handling.h"

using namespace std;

ifstream openFileInputStream(const string & fileName, ios_base::openmode mode)
{
    ifstream fin(fileName, mode);
    if (!fin.is_open())
        throw file_error(fileName);
    return fin;
}

ofstream openFileOutputStream(const string & fileName, ios_base::openmode mode)
{
    ofstream fout(fileName, mode);
    if (!fout.is_open())
        throw file_error(fileName);
    return fout;
}

file_error::file_error(const string & fileName) 
    : runtime_error(fileName + ": " + strerror(errno)) 
{
}

file_error::file_error(streamsize expected, streamsize actual)
    : runtime_error("Requested " + to_string(expected) + " but was able to move only " + to_string(actual))
{
}