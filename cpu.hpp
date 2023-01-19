/********************************************************************************
* cpu.hpp: Contains definitions and declarations for implementation of an 
*          8-bit ALU (Aritmetic Logic Unit).
********************************************************************************/
#ifndef CPU_HPP_
#define CPU_HPP_

/* Include directives: */
#include <iostream>
#include <cstdint>
#include <bitset>
#include <string>

/********************************************************************************
* cpu: Namespace containing generic constants and functions for performing
*      calculations with an ALU.
********************************************************************************/
namespace cpu
{
   /********************************************************************************
   * Aritmetic and logic operations:
   ********************************************************************************/
   static constexpr std::uint8_t NOP = 0x00; /* No operation. */
   static constexpr std::uint8_t OR  = 0x01; /* Bitwise OR. */
   static constexpr std::uint8_t AND = 0x02; /* Bitwise AND. */
   static constexpr std::uint8_t XOR = 0x03; /* Bitwise XOR. */
   static constexpr std::uint8_t ADD = 0x04; /* Addition. */
   static constexpr std::uint8_t SUB = 0x05; /* Subtraction. */

   /********************************************************************************
   * Status flags:
   ********************************************************************************/
   static constexpr std::uint8_t S = 4; /* Signed flag, indicates if ALU result is negative. */
   static constexpr std::uint8_t N = 3; /* Negative flag, indicates if MSB of ALU result is set. */
   static constexpr std::uint8_t Z = 2; /* Zero flag, indicates if ALU result is equal to zero. */
   static constexpr std::uint8_t V = 1; /* Overflow flag, indicates overflow. */
   static constexpr std::uint8_t C = 0; /* Carry-flag, set if result is higher than 255. */


   /********************************************************************************
   * set: Sets bit in specified register without affecting other bits.
   *
   *      - reg: Reference to the register.
   *      - bit: The bit to be set in the referenced register.
   ********************************************************************************/
   template<class T = std::uint8_t>
   static void set(T& reg, const std::uint8_t bit)
   {
      reg |= (1 << bit);
      return;
   }

   /********************************************************************************
   * clr: Clears bit in specified register without affecting other bits.
   *
   *      - reg: Reference to the register.
   *      - bit: The bit to be cleared in the referenced register.
   ********************************************************************************/
   template<class T = std::uint8_t>
   static void clr(T& reg, const std::uint8_t bit)
   {
      reg &= ~(1 << bit);
      return;
   }

   /********************************************************************************
   * read: Reads bit from specified register. The return value is not equal to
   *       zero if the bit is high. If the bit is low the return value is zero.
   *
   *       - reg: Reference to the register.
   *       - bit: The bit to be read in the referenced register.
   ********************************************************************************/
   template<class T = std::uint8_t>
   static T read(const T reg, const std::uint8_t bit)
   {
      return reg & (1 << bit);
   }

   /********************************************************************************
   * get_operator: Returns the operator of specified instruction as text.
   *
   *               - op_code: OP code of the specified CPU instruction.
   ********************************************************************************/
   static const char* get_operator(const std::uint8_t op_code)
   {
      if (op_code == OR)       return " | ";
      else if (op_code == AND) return " & ";
      else if (op_code == XOR) return " ^ ";
      else if (op_code == ADD) return " + ";
      else if (op_code == SUB) return " - ";
      else return "Unknown";
   }

   /********************************************************************************
   * get_instruction_name: Returns the name of the specified instruction.
   *
   *                       - op_code: OP code of the specified instruction.
   ********************************************************************************/
   static const char* get_instruction_name(const std::uint8_t op_code)
   {
      if (op_code == OR)       return "OR";
      else if (op_code == AND) return "AND";
      else if (op_code == XOR) return "XOR";
      else if (op_code == ADD) return "ADD";
      else if (op_code == SUB) return "SUB";
      else return "Unknown";
   }

   /********************************************************************************
   * get_op_code: Returns the OP code of specified instruction.
   *
   *              - instruction_name: The specified CPU instruction in text form.
   ********************************************************************************/
   static std::uint8_t get_op_code(const std::string& instruction_name)
   {
      if (instruction_name == "OR")       return OR;
      else if (instruction_name == "AND") return AND;
      else if (instruction_name == "XOR") return XOR;
      else if (instruction_name == "ADD") return ADD;
      else if (instruction_name == "SUB") return SUB;
      else                                return NOP;
   }

   /********************************************************************************
   * get_signed: Returns the signed equivalent of specified number by checking
   *             the passed status flags.
   *
   *             - num: Unsigned number to be converted to signed form.
   *             - sr : Status flags from status register.
   ********************************************************************************/
   static int get_signed(const std::uint8_t num,
      const std::uint8_t sr)
   {
      if (read(sr, S))
      {
         return num - 256;
      }
      else
      {
         return num;
      }
   }

   /********************************************************************************
   * get_signed: Returns the signed equivalent of specified number.
   *
   *             - num: Unsigned number to be converted to signed form.
   ********************************************************************************/
   static int get_signed(const std::uint8_t num)
   {
      if (read(num, 7))
      {
         return static_cast<int>(num - 256);
      }
      else
      {
         return static_cast<int>(num);
      }
   }

   /********************************************************************************
   * readline: Reads a string of text from the terminal into referenced string.
   *
   *           - s: Reference to string for storage of entered text.
   ********************************************************************************/
   static void readline(std::string& s)
   {
      std::getline(std::cin, s);
      std::cout << "\n";
   }

   /********************************************************************************
   * read_byte: Returns an unsigned 8-bit integer entered from the terminal.
   *            If an invalid character is entered, input is done again.
   ********************************************************************************/
   static std::uint8_t read_byte(void)
   {
      while (1)
      {
         std::string s;
         readline(s);

         try
         {
            return static_cast<std::uint8_t>(std::stoi(s));
         }
         catch (std::invalid_argument&)
         {
            std::cerr << "Invalid input, try again!\n\n";
         }
      }
   }

   /********************************************************************************
   * read_op_code: Returns an OP code entered from the terminal. If an invalid
   *               OP code is entered, the OP code has to be entered again.
   ********************************************************************************/
   static std::uint8_t read_op_code(void)
   {
      while (1)
      {
         std::string s;
         readline(s);
         const auto op_code = get_op_code(s);

         if (op_code == NOP)
         {
            std::cout << "Invalid instruction, try again!\n\n";
         }
         else
         {
            return  op_code;
         }
      }
   }
}

#endif /* CPU_HPP_ */