#include "interpreter.h"
#include <iostream>



std::string Interpreter::interpret_instruction(std::bitset<36> instruction){
    full_instruction = instruction;
    set_sub_instructions();
    std::cout << next_address << " " << JAM << " " << ALU << " " << C_bus << " " << memory << " " << B_bus << " \n";
    return next_address.to_string();
}

void Interpreter::set_sub_instructions(){
int i = 35;

while(i >= 0){
    if(i >= 27){
        next_address[8-(35 - i)] = full_instruction[i];
    }
    else if (i >= 24){
        JAM[2-(26- i)] = full_instruction[i];

    }
    else if (i >= 16){
        ALU[7-(23 - i)] = full_instruction[i];

    }
    else if (i >= 7){
        C_bus[8-(15 - i)] = full_instruction[i];

    }
    else if (i >= 4){
        memory[2-(6 - i)] = full_instruction[i];

    }
    else {
        B_bus[i] = full_instruction[i];

    }

    --i;
}

}


