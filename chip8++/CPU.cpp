#include <array>
#include <stack>
#include "CPU.h"
#include "Memory.cpp"

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

		//fetched opcode
		std::uint16_t opcode;

		//memory device pointer
		Mem::Chip8Mem* memory;

		CPU_STATES cpu_status;

		void Fetch_and_IncPC()
		{
			opcode = memory->read(PC);
			PC++;
		}

		void Decode_and_execute()
		{
			std::uint8_t VX = 0x00;
			std::uint8_t VY = 0x00;

			switch (opcode & 0xF000)
			{
			case CLASS_0:
				//3 opcodes
				//implement decoder for class_0 instructions
				break;
			case CLASS_1:
				//Jump to address NNN. format 1NNN
				PC = opcode & (OP_CLASS_MASKS[0x1] ^ 0xFFFF);
				break;
			case CLASS_2:
				//Call subroutine at address NNN. format 2NNN
				cpu_stack[SP] = PC;
				SP++;
				PC = opcode & (OP_CLASS_MASKS[0x2] ^ 0xFFFF);
				break;
			case CLASS_3:
				//Skip the next instruction if VX == NN. format 3XNN
				VX = (opcode & 0x0F00) >> 8;
				if (GPR[VX] == (opcode & 0x00FF)) { PC++; }
				break;
			case CLASS_4:
				//Skip the next instruction if VX != NN. format 4XNN
				VX = (opcode & 0x0F00) >> 8;
				if (GPR[VX] != (opcode & 0x00FF)) { PC++; }
				break;
			case CLASS_5:
				//Skip the next instruction if VX == VY. format 5XY0
				VX = (opcode & 0x0F00) >> 8;
				VY = (opcode & 0x00F0) >> 4;
				if (GPR[VX] == GPR[VY]) { PC++; }
				break;
			case CLASS_6:
				//Load NN into VX. format 6XNN
				VX = (opcode & 0x0F00) >> 8;
				GPR[VX] = opcode & 0x00FF;
				break;
			case CLASS_7:
				//Add NN to VX. format 7XNN
				VX = (opcode & 0x0F00) >> 8;
				GPR[VX] += opcode & 0x00FF;
				break;
			case CLASS_8:
				//9 opcodes
				//implement decoder for class_8 instructions
				break;
			case CLASS_9:
				//Skip the next instruction if VX != VY. format 9XY0
				VX = (opcode & 0x0F00) >> 8;
				VY = (opcode & 0x00F0) >> 4;
				if (GPR[VX] != GPR[VY]) { PC++; }
				break;
			case CLASS_A:
				//Load NNN into IR. format ANNN
				IR = opcode & 0x0FFF;
				break;
			case CLASS_B:
				//Jump to address [V0 + NNN]. format BNNN
				PC = GPR[0x0] + (opcode & 0x0FFF);
				break;
			case CLASS_C:
				//Load (random & NN) into VX. format CXNN
				//need a RNG first
				break;
			case CLASS_D:
				//Draw a sprite at position (VX,VY) 8 pixels wide and N pixels high. format DXYN
				//need to implement display interface first
				break;
			case CLASS_E:
				//2 opcodes
				//implement decoder for class_e instructions
				break;
			case CLASS_F:
				//9 opcodes
				//implement decoder for class_f instructions
				break;
			default:
				//should be unreachable
			}
		}


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
			opcode = 0x0000;
			cpu_status = CPU_INIT;
		}

		void Set_Memory_Device(Mem::Chip8Mem &mem)
		{
			//don't change memory device while running
			if (cpu_status != CPU_RUN) { memory = &mem; }
		}
	};
}