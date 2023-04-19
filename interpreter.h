#include <string.h>
#include <bitset>

class interpreter{

public:
std::string interpret_instruction(std::bitset<36> instruction);


private:
std::bitset<36> full_instruction;
std::bitset<9> next_address;
std::bitset<3> JAM;
std::bitset<8> ALU;
std::bitset<9> C_bus;
std::bitset<3> memory;
std::bitset<4> B_bus;
void set_sub_instructions();
};