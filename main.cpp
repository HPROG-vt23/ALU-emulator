/********************************************************************************
* main.cpp: ALU emulator, where the used is able to enter and operation and
*           two operands two perform. The result is printed in the terminal. 
*           A few calculation examples are printed in the terminal before
*           user input commences.
********************************************************************************/
#include "alu.hpp"

using namespace cpu; /* Brings all content of the cpu namespace into current scope. */

/********************************************************************************
* main: Prints five examples of ALU calculations in the terminal. Then the
*       user is able to perform ALU calculations by entering OP code and
*       operands in the terminal. The program is running continuously.
********************************************************************************/
int main(void)
{
   std::cout << "Five examples of ALU calculations are printed below!\n\n";
   alu::print(ADD, 100, 50);
   alu::print(SUB, -100, 50);
   alu::print(AND, 0x24, (1 << 5));
   alu::print(OR, 0x20, (1 << 0));
   alu::print(ADD, -5, 10);
   
   while (1)
   {
      alu::calculate_by_input();
   }

   return 0;
}