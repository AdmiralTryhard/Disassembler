#include <fstream>
#include <bitset>
#include <iostream>

int main(){
    std::string filename{"mic1ijvm.mic1"};
    std::ifstream input{filename, std::ios::binary};

    if(!input){
        throw std::runtime_error("Cannot open file in binary mode: " + filename);
    }

    input.seekg(0, input.end);
    int length = input.tellg();
    input.seekg(0, input.beg);
    std::cout << length << '\n';
    if (length != 2564) {
        throw std::runtime_error("File size should be 2564 bytes, but is " + std::to_string(length));

    }

    char bytes[5];
    input.read(bytes, 4);
    if(bytes[0] != 0x12 || 
    bytes[1] != 0x34 ||
    bytes[2] != 0x56 ||
    bytes[3] != 0x78) {
        throw std::runtime_error("Bad magic bytes");
    }

    //read one instruction
    input.read(bytes, 5);
    std::bitset<36> bits;
    bits |= (bytes[0] & 0xFF);
    bits <<= 28;
    bits |= (bytes[1] & 0xFF) << 20;
    bits |= (bytes[2] & 0xFF) << 12;
    bits |= (bytes[3] & 0xFF) << 4;
    bits |= (bytes[4] & 0xFF) >> 4;


    std::cout << bits << '\n';

    return 0;
}