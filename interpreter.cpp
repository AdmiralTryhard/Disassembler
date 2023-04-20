#include "interpreter.h"
#include <iostream>



std::string Interpreter::interpret_instruction(std::bitset<36> instruction, int address){
    std::string final_output;
    full_instruction = instruction;
    address = address;
    set_sub_instructions();
    final_output += ALU_operation();
    final_output += memory_check();
    if(!special_location().empty()){
        if(!final_output.empty()){
            final_output += "; " + special_location();
        }
        else {
            final_output = special_location();
        }
    }
    std::cout << std::hex << address << "    " 
    << std::hex << next_address.to_ulong() << "   " << JAM << "    " << ALU << "         " 
    << C_bus << "          " << memory << "    " << std::hex << B_bus.to_ulong() << '\n';
    return final_output + '\n';
}
//filling in all the smaller bitsets this way means I have to read them backwards for all subsequent functions because I wanted them to appear nice in the output
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
//This really only works with adding, subtracting, << 8, AND, OR, NOT. I did not find the shift to the right operator in my feeble attempt to scan the book.
//I assumed that when the ALU is in operation, it must output SOMETHING, so I only checked while C_bus is active. 
//I filled out what I thought would be common cases and then proceeded to fill in cases I noticed were missing in the output either by broken equations or ones I knew could be done but had nothing.
std::string Interpreter::ALU_operation(){
    std::string operation;
    operation += c_bus_selection();
    if(!operation.empty()){
        if(ALU[7]){
            operation += b_bus_selection() + " << 8 ";
        }
        if(ALU[3]){
            if(ALU[2]){ //operations requiring both buses being on is AND, OR, + 
            operation += "H " + ALU_decode() + b_bus_selection();
            }
            else if(ALU[1]){
                operation += "NOT H";
            }
            else if(ALU[0]){
                operation += "H + 1";
            }
            else if(ALU_decode() == "OR "){
                operation += "H ";
            }
        }
        else if(ALU[2]){
            if(ALU[0]){
                operation += b_bus_selection() + " + 1";
            }
            else { //if you are not using H or incrementing, the only ALU_decode() option should be NOT
                if(ALU_decode() == "OR "){
                    operation += b_bus_selection();
                }
                else{
                    if(ALU[1]){
                        operation += b_bus_selection() + " - " + "H";
                    }
                    else{
                        operation += ALU_decode() + b_bus_selection();

                    }
                }
            }
        }
        else {
            operation += "NOT H";
        }
    }
    return operation;
}
//I could not remember what decode value meant what, so I looked it up and made a helper function
std::string Interpreter::ALU_decode(){
    if(!ALU[5] &! ALU[4]){
        return "AND ";
    }
    if(!ALU[5] && ALU[4]){
        return "OR ";
    }
    if(ALU[5] &! ALU[4]){
        return "NOT ";
    }
    else{
        return "+ ";
    }

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
    if(C_bus[8]){
        output = "H";
    }
    if(C_bus[7]){
        output += chain_for_c_bus(output);
        output += "OPC";
    }
    if(C_bus[6]){
        output += chain_for_c_bus(output);
        output += "TOS";
    }
    if(C_bus[5]){
        output += chain_for_c_bus(output);
        output += "CPP";
    }
    if(C_bus[4]){
        output += chain_for_c_bus(output);
        output += "LV";
    }
    if(C_bus[3]){
        output += chain_for_c_bus(output);
        output += "SP";
    }
    if(C_bus[2]){
        output += chain_for_c_bus(output);
        output += "PC";
    }
    if(C_bus[1]){
        output += chain_for_c_bus(output);
        output += "MDR";
    }
    if(C_bus[0]){
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

//I have fetch before wr and rd because not a single mal instruction I see in the book has fetch after wr or rd
std::string Interpreter::memory_check(){
    std::string output;
    if(memory[0]){
        output += "; fetch";
        if(!memory[1] &! memory[2]){
            output += ";"; //fetch always has a semi colon
        }
    }
    if(memory[1]){
        output +=   "; rd";
    }
    if (memory[2]){

        output += "; wr";
    }

    return output;
    
}

//basically check to see if I'm going to the start of another instruction. Beyond that, I do not have time to figure things out.
std::string Interpreter::special_location(){
    if(next_address.to_ulong() == 2){
        return "goto main1;";
    }
    if(address == 2){
        return "goto (MBR);";
    }
    if(next_address.to_ulong() == bipush){
        return "goto bipush1;";
    }
    if(next_address.to_ulong() == dup){
        return "goto dup1;";
    }
    if(next_address.to_ulong() == iadd){
        return "goto iadd1;";
    }
    if(next_address.to_ulong() == iand){
        return "goto iand1;";
    }
    if(next_address.to_ulong() == ifeq){
        return "goto ifeq;";
    }
    if(next_address.to_ulong() == iflt){
        return "goto iflt1;";
    }
    if(next_address.to_ulong() == if_icmpeq){
        return "goto if_icmpeq;";
    }
    if(next_address.to_ulong() == iinc){
        return "goto iinc1;";
    }
    if(next_address.to_ulong() == iload){
        return "goto iload1;";
    }
    if(next_address.to_ulong() == invokevirtual){
        return "goto invokevirtual;";
    }
    if(next_address.to_ulong() == ior){
        return "goto ior1;";
    }
    if(next_address.to_ulong() == ireturn){
        return "goto ireturn1;";
    }
    if(next_address.to_ulong() == istore){
        return "goto istore1;";
    }
    if(next_address.to_ulong() == isub){
        return "goto isub1;";
    }
    if(next_address.to_ulong() == ldc_w){
        return "goto ldc_w1;";
    }
    if(next_address.to_ulong() == nop){
        return "goto nop1;";
    }
    if(next_address.to_ulong() == pop){
        return "goto pop1;";
    }
    if(next_address.to_ulong() == swap){
        return "goto swap1;";
    }
    if(next_address.to_ulong() == wide){
        return "goto wide1;";
    }
    else {
        return "";
    }
}