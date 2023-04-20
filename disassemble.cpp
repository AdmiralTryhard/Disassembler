#include <fstream>
#include <bitset>
#include <iostream>
#include "interpreter.h"



std::ifstream generate_stream();
std::bitset<36> read_instruction(std::ifstream& input);
void interpret_instruction(std::bitset<36> instruction, int address);
void print_bit_map();
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

    int i = 1;
    //read instructions
    print_bit_map();
    while(!input.eof() && i < 200){
    std::bitset<36> current_instruction = read_instruction(input);
    interpret_instruction(current_instruction, i);
    ++i;
    }
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
    return bits;
}

void interpret_instruction(std::bitset<36> instruction, int address){
    Interpreter interpretty{};
    std::string readable_instruction = interpretty.interpret_instruction(instruction, address);
    std::cout << readable_instruction << std::endl;
}

void print_bit_map(){
    std::cout << "ADR  NXT J J J  S S F F E E I I  H O T C L S P M M  W R F  B \n"
    << "         M A A  L R 0 1 N N N N    P O P V P C D A  R E E  - \n"
    << "         P M M  L A     A B V C    C S P       R R  I A T  B \n"
    << "         C N Z  8 1         A                       T D C  U \n"
    << "                                                    E   H  S \n";
}