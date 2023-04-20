#include <string.h>
#include <bitset>

class Interpreter{

public:
std::string interpret_instruction(std::bitset<36> instruction, int address);

private:
int address;
std::bitset<36> full_instruction;
std::bitset<9> next_address;
std::bitset<3> JAM;
std::bitset<8> ALU;
std::bitset<9> C_bus;
std::bitset<3> memory;
std::bitset<4> B_bus;
void set_sub_instructions();
std::string ALU_decode();
std::string ALU_operation();
std::string b_bus_selection();
std::string c_bus_selection();
std::string chain_for_c_bus(std::string to_check);
std::string memory_check();
std::string special_location();

//here is all the hex turned into ints to use with bitset.to_ulong();
const int bipush = 16;
const int dup = 89;
const int GOTO = 167;
const int iadd = 96;
const int iand = 126;
const int ifeq = 99;
const int iflt = 153;
const int if_icmpeq = 159;
const int iinc = 132;
const int iload = 21;
const int invokevirtual = 182;
const int ior = 128;
const int ireturn = 172;
const int istore = 54;
const int isub = 100;
const int ldc_w = 19;
const int nop = 0;
const int pop = 87;
const int swap = 95;
const int wide = 196;


//more consts for b bus
const int MDR = 0;
const int PC = 1;
const int MBR = 2;
const int MBRU = 3;
const int SP = 4;
const int LV = 5;
const int CPP = 6;
const int TOS = 7;
const int OPC = 8;


};