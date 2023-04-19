#include <fstream>
#include <bitset>
#include <iostream>
#include "interpreter.h"



std::ifstream generate_stream();
std::bitset<36> read_instruction(std::ifstream& input);
void interpret_instruction(std::bitset<36> instruction);
int main(){
    //check length and magic bytes
    std::ifstream input = generate_stream();
    input.seekg(0, input.end);
    int length = input.tellg();
    input.seekg(0, input.beg);
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

    //read instructions
   // while(!input.eof()){ // save this loop for the end. It is tested already
    std::bitset<36> current_instruction = read_instruction(input);
    interpret_instruction(current_instruction);
 //   }
    return 0;
}


std::ifstream generate_stream() {
    std::string filename{"mic1ijvm.mic1"};
    std::ifstream input{filename, std::ios::binary};

    if(!input){
        throw std::runtime_error("Cannot open file in binary mode: " + filename);
    }

    return input;
}

std::bitset<36>  read_instruction(std::ifstream& input){
    //read one instruction
    char bytes[5];
    input.read(bytes, 5);
    std::bitset<36> bits;
    bits |= (bytes[0] & 0xFF);
    bits <<= 28;
    bits |= (bytes[1] & 0xFF) << 20;
    bits |= (bytes[2] & 0xFF) << 12;
    bits |= (bytes[3] & 0xFF) << 4;
    bits |= (bytes[4] & 0xFF) >> 4;
    std::cout << bits << '\n';
    return bits;
}

void interpret_instruction(std::bitset<36> instruction){
    Interpreter interpretty{};
    std::string readable_instruction = interpretty.interpret_instruction(instruction);
    std::cout << readable_instruction << std::endl;
}