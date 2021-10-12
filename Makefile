INC=-Iinclude
SRCPATH=src/

decoder-example: decoder-test decoder data-structures file-handling
	g++ -std=c++17 -o bin/decoder bin/HuffmanDecoderTest.o bin/HuffmanDecoder.o bin/DataStructures.o bin/file_stream_handling.o
decoder-test: make-bin
	g++ -std=c++17 $(INC) -c $(SRCPATH)HuffmanDecoderTest.cpp -o bin/HuffmanDecoderTest.o
decoder: make-bin
	g++ -std=c++17 $(INC) -c $(SRCPATH)HuffmanDecoder.cpp -o bin/HuffmanDecoder.o
encoder-example: encoder-test encoder data-structures file-handling
	g++ -std=c++17 -o bin/encoder bin/HuffmanEncoderTest.o bin/HuffmanEncoder.o bin/DataStructures.o bin/file_stream_handling.o
encoder-test: make-bin
	g++ -std=c++17 $(INC) -c $(SRCPATH)HuffmanEncoderTest.cpp -o bin/HuffmanEncoderTest.o
encoder: make-bin
	g++ -std=c++17 $(INC) -c $(SRCPATH)HuffmanEncoder.cpp -o bin/HuffmanEncoder.o
data-structures: make-bin
	g++ -std=c++17 $(INC) -c $(SRCPATH)DataStructures.cpp -o bin/DataStructures.o
file-handling: make-bin
	g++ -std=c++17 $(INC) -c $(SRCPATH)file_stream_handling.cpp -o bin/file_stream_handling.o
make-bin:
	[ -d "./bin" ] || mkdir bin
clean:
	[ ! -d "./bin" ] || rm -r bin