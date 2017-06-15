#include <array>
#include <stack>
#include "CPU.h"

namespace CPU
{
	class Chip8CPU
	{

		//registers
		std::array<std::uint8_t, 0x10> GPR;  //V0 through VF
		std::uint8_t DT; //delay timer
		std::uint8_t ST; //sound timer
		std::uint16_t IR; //memory pointer
		std::uint16_t PC; //program counter
		std::uint8_t SP; //stack pointer

		//return address stack
		std::array<uint16_t, 0xFF> cpu_stack;

		CPU_STATES cpu_status;

	public:

		Chip8CPU()
		{
			Cpu_initialize();
		}

		~Chip8CPU()
		{

		}

		void Cpu_initialize()
		{
			GPR.fill(0x00);
			DT = 0;
			ST = 0;
			IR = 0;
			PC = 0;
			SP = 0;
			cpu_stack.fill(0x0000);
			cpu_status = CPU_INIT;
		}
	};
}