/********************************************************************************
* alu.hpp: Contains function definitions for implementation of an ALU
*          (Aritmetic Logic Unit) for performing calculations and updating
*          status bits SNZVC as described below:
*
*          S (Signed)  : Set if result is negative with overflow considered*.
*          N (Negative): Set if result is negative, i.e. N = result[7].
*          Z (Zero)    : Set if result is zero, i.e. Z = result == 0 ? 1 : 0.
*          V (Overflow): Set if signed overflow occurs**.
*          C (Carry)   : Set if result contains a carry bit, i.e. C = result[8]***.
*
*          * Signed flag is set if result is negative (N = 1) while
*            overflow hasn't occured (V = 0) or result is positive (N = 0)
*            while overflow has occured (V = 1), i.e. S = N ^ V.
*
*            For instance, consider the subtracting two 8-bit numbers -100 and 50.
*            The result is -100 - 50 = -150, but since only 8 bits are used, the
*            result is implemented as -150 + 2^8 = -150 + 256 = 106, i.e.
*            0110 0110. Since most significant bit is cleared, the N-flag is
*            cleared and the number if positive. However, overflow occured,
*            since the two numbers -100 and 50 have different signs and the
*            result has the same sign as the subtrahend 50. Hence the V-flag
*            is set. Since N = 0 and V = 1, the S-flag is also set.
*            Therefore the number is correctly intepreted as negative.
*
*          ** Signed overflow occurs:
*
*             a) During addition (+) if the operands A and B are of the
*                same sign and the result is of the opposite sign, i.e.
*
*                V = (A[7] == B[7]) && (A[7] != result[7]) ? 1 : 0
*
*             b) During subtraction (-) if the operands A and B are of the
*                opposite sign and the result has the same sign as B, i.e.
*
*                V = (A[7] != B[7]) && (B[7] == result[7]) ? 1 : 0
*
*          *** One instance when the carry bit is set is when unsigned overflow
*              occurs, for instance when adding two numbers 255 and 1 into an
*              8 bit destination. The result is equal to 0 (with carry set),
*              since 1111 1111 + 1 = 1 0000 0000, which gets truncated to
*              0000 0000. Since result[8] == 1, the carry bit is set.
*              Unsigned overflow occurs for the timer circuits of microcontroller
*              ATmega328P when counting up in Normal Mode.
********************************************************************************/
#ifndef ALU_HPP_
#define ALU_HPP_

/* Include directives: */
#include "cpu.hpp"

/********************************************************************************
* alu: Namespace containing ALU functions.
********************************************************************************/
namespace alu
{
   using namespace cpu; /* Brings all content of the cpu namespace into current scope. */

   /********************************************************************************
   * calculate: Performs calculation with specified operands and returns the 
   *            result. The status flags SNZVC of the referenced status register 
   *            are updated in accordance with the result.
   *
   *            - operation: The operation to perform (OR, AND, XOR, ADD or SUB).
   *            - a        : First operand.
   *            - b        : Second operand.
   *            - sr       : Reference to status register containing SNZVC flags.
   ********************************************************************************/
   static std::uint8_t calculate(const std::uint8_t operation,
                                 const std::uint8_t a,
                                 const std::uint8_t b,
                                 std::uint8_t& sr)
   {
      std::uint16_t result = 0x00;
      sr &= ~((1 << S) | (1 << N) | (1 << Z) | (1 << V) | (1 << C));

      switch (operation)
      {
      case OR:
      {
         result = a | b;
         break;
      }
      case AND:
      {
         result = a & b;
         break;
      }
      case XOR:
      {
         result = a ^ b;
         break;
      }
      case ADD:
      {
         result = a + b;

         if ((read(a, 7) == read(b, 7)) && (read(a, 7) != read(result, 7)))
         {
            set(sr, V);
         }

         break;
      }
      case SUB:
      {
         result = a - b;

         if (a < b) result += 256;

         if ((read(a, 7) != read(b, 7)) && (read(b, 7) == read(result, 7)))
         {
            set(sr, V);
         }

         break;
      }
      }

      if (read(result, 8)) set(sr, C);
      if (result == 0) set(sr, Z);
      if (read(result, 7)) set(sr, N);
      if (static_cast<bool>(read(sr, N)) != static_cast<bool>(read(sr, V))) set(sr, S);

      return static_cast<std::uint8_t>(result);
   }

   /********************************************************************************
   * print: Performs calculation with specified operands and prints the result
   *        and corresponding status flags. The output is printed to the standard
   *        output, i.e. the terminal, as default.
   *
   *        - operation: The operation to perform (OR, AND, XOR, ADD or SUB).
   *        - a        : First operand.
   *        - b        : Second operand.
   *        - ostream  : Reference to output stream (default = std::cout).
   ********************************************************************************/
   static void print(const std::uint8_t operation,
                     const std::uint8_t a,
                     const std::uint8_t b,
                     std::ostream& ostream = std::cout)
   {
      std::uint8_t sr = 0x00;
      const auto result = calculate(operation, a, b, sr);
      ostream << "--------------------------------------------------------------------------------\n";
      ostream << "Instruction: " << get_instruction_name(operation) << "\n";
      ostream << "Decimal\t   : " << get_signed(a) << get_operator(operation)
         << get_signed(b) << " = " << get_signed(result, sr) << "\n";
      ostream << "Binary\t   : " << std::bitset<8>(a) << get_operator(operation)
         << std::bitset<8>(b) << " = " << std::bitset<8>(result) << "\n";
      ostream << "Status bits: SNZVC = " << std::bitset<5>(sr) << "\n";
      ostream << "--------------------------------------------------------------------------------\n\n";
      return;
   }

   /********************************************************************************
   * calculate_by_input: Reads an operation and two operands to calculate with
   *                     the ALU. The result is printed in the terminal along
   *                     with status bits SNZVC.
   ********************************************************************************/
   void calculate_by_input(void)
   {
      std::cout << "Enter an operation to perform (OR, AND, XOR, ADD och SUB):\n";
      const auto op_code = read_op_code();

      std::cout << "Enter the first operand (0 - 255):\n";
      const auto op1 = read_byte();

      std::cout << "Enter the second operand (0 - 255):\n";
      const auto op2 = read_byte();

      print(op_code, op1, op2);
      return;
   }
}

#endif /* ALU_HPP_ */