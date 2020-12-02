# Huffman Encoder & Decoder

## Introduction

This repo contains classes that are able to encode and decode files using Huffman's algorithm as well as demo programs to test the results of the encoding and decoding processes to make sure that the contents are unchanged after an encoding and a later decoding, and to check if there is any memory benefit gained from encoding a file.

The answer to the latter is that it depends on the file. If the file is big enough and it contains a small group of very frequent characters, then by encoding it, the size of the file is reduced, otherwise, because of the need to store the structure of the tree used to encode the characters, the file might actually end up bigger.


## How to Build and Run

The project was developed and tested on the WSL(Windows Subsytem for Linux) Ubuntu distribution. For building the project you will need GNU's g++ compiler, which can be installed on Ubuntu systems using the command:

```bash
$ sudo apt install g++
```

For automatically compiling the test programs into executables GNU's make can be used, which can be installed on Ubuntu using:

```bash
$ sudo apt install make
```

The following instructions on how to build the test programs asume you have g++ and make installed on your system. 
To test the classes `HuffmanEncoder` and `HuffmanDecoder` use the programs `HuffmanEncoderTest.cpp` and `HuffmanDecoderTest.cpp`.

To compile the program `HuffmanEncoderTest.cpp` into an executable named `HuffmanEncoderTest` you can run:

```bash
make encoder-example
```

and to run this executable you can use:

```bash
./bin/HuffmanEncoderTest <input_filename> <encoded_output_filename>
```

Where `input_filename` is the name of the file to be encoded and `encoded_output_filename` is the name of the file that will be used to output the result of the encoding.

To compile the program `HuffmanDecoderTest.cpp` into an executable named `HuffmanDecoderTest` you can run:

```bash
make decoder-example
```

and to run this executable you can use:

```bash
./bin/HuffmanDecoderTest <encoded_input_filename> <decoded_output_filename>
```

Where `encoded_input_filename` is the name of a file previously encoded using `HuffmanEncoder`'s member function `encodeFile` and `decoded_output_filename` is the name of the file that will be used to output the result of decoding the former file.

## Exceptions and Error Checking

The classes provide minimal error checking regarding file existence and permisions, throwing exceptions when files can't be opened or created. The `HuffmanDecoder` class does not perform any checks on the file it tries to decode and asumes that it came from a previous call to `HuffmanEncoder::encodeFile`. If this is not true the program will crash or have unpredictable results.