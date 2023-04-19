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

std::string Interpreter::ALU_operation(){
    return "not done with ALU_operation \n";
}

std::string Interpreter::b_bus_selection(){
    if(B_bus.to_ulong() == MDR) {
        return "MDR";
    }
    else if (B_bus.to_ulong() == PC ){
        return "PC";
    }
    else if (B_bus.to_ulong() == MBR){
        return "MBR";
    }
    else if (B_bus.to_ulong() == MBRU){
        return "MBRU";
    }
    else if (B_bus.to_ulong() == SP){
        return "SP";
    }
    else if (B_bus.to_ulong() == LV){
        return "LV";
    }
    else if (B_bus.to_ulong() == CPP){
        return "CPP";
    }
    else if (B_bus.to_ulong() == TOS){
        return "TOS";
    }
    else if (B_bus.to_ulong() == OPC){
        return "OPC";
    }
    else {
        return "none";
    }

}

std::string Interpreter::c_bus_selection(){
    std::string output;
    if(C_bus[0]){
        output = "H";
    }
    if(C_bus[1]){
        output += chain_for_c_bus(output);
        output += "OPC";
    }
    if(C_bus[2]){
        output += chain_for_c_bus(output);
        output += "TOS";
    }
    if(C_bus[3]){
        output += chain_for_c_bus(output);
        output += "CPP";
    }
    if(C_bus[4]){
        output += chain_for_c_bus(output);
        output += "LV";
    }
    if(C_bus[5]){
        output += chain_for_c_bus(output);
        output += "SP";
    }
    if(C_bus[6]){
        output += chain_for_c_bus(output);
        output += "PC";
    }
    if(C_bus[7]){
        output += chain_for_c_bus(output);
        output += "MDR";
    }
    if(C_bus[8]){
        output += chain_for_c_bus(output);
        output += "MAR";
    }
    //gotta have this here because the chain comes before adding the register
    output += chain_for_c_bus(output);

    return output;

}

std::string Interpreter::chain_for_c_bus(std::string to_check){
    if(!to_check.empty()){
        return " = ";
    }
    else {
        return "";
    }
}
