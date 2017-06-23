#pragma once

namespace CPU
{
	enum CPU_STATES
	{
		CPU_INIT, //initial state
		CPU_PAUSE, //ready to run
		CPU_RUN, //running
		CPU_STOP //halted
	};

	enum OPCODE_CLASSES
	{
		CLASS_0 = 0x0000, //CallRCA, ClearDisplay, Return
		CLASS_1 = 0x1000, //Jump
		CLASS_2 = 0x2000, //CallSub
		CLASS_3 = 0x3000, //Skip next if equal (register and constant)
		CLASS_4 = 0x4000, //Skip next if unequal (register and constant)
		CLASS_5 = 0x5000, //Skip next if equal (two registers)
		CLASS_6 = 0x6000, //Load constant into register
		CLASS_7 = 0x7000, //Add constant to register
		CLASS_8 = 0x8000, //9 opcodes
		CLASS_9 = 0x9000, //Skip next if unqeual (two registers)
		CLASS_A = 0xA000, //Load constant into IR
		CLASS_B = 0xB000, //Jump to [V0 + constant]
		CLASS_C = 0xC000, //Load bitwise AND result into register (random number and constant)
		CLASS_D = 0xD000, //Draw sprite
		CLASS_E = 0xE000, //2 opcodes (input)
		CLASS_F = 0xF000  //9 opcodes
	};

	const std::array<std::uint16_t, 0x10> OP_CLASS_MASKS =
	{
		0xF000, //CLASS_0
		0xF000, //CLASS_1
		0xF000, //CLASS_2
		0xF000, //CLASS_3
		0xF000, //CLASS_4
		0xF000, //CLASS_5
		0xF000, //CLASS_6
		0xF000, //CLASS_7
		0xF00F, //CLASS_8
		0xF000, //CLASS_9
		0xF000, //CLASS_A
		0xF000, //CLASS_B
		0xF000, //CLASS_C
		0xF000, //CLASS_D
		0xF0FF, //CLASS_E
		0xF0FF, //CLASS_F
	};

	enum OPCODES_FULL
	{
		//format N=constant, NN=constant, NNN=address, X=register, Y=register
		CALL_RCA1802 = 0x0000, //0NNN
		CLEAR_SCREEN = 0x00E0, //00E0
		RETURN_OP = 0x00EE, //00EE
		JUMP = 0x1000, //1NNN
		CALL = 0x2000, //2NNN
		SKIP_EQ_VX = 0x3000, //3XNN
		SKIP_UNEQ_VX = 0x4000, //4XNN
		SKIP_EQ_VXVY = 0x5000, //5XY0
		LOAD_VX = 0x6000, //6XNN
		ADD_VX = 0x7000, //7XNN
		LOAD_VXVY = 0x8000, //8XY0
		BIT_OR_VXVY = 0x8001, //8XY1
		BIT_AND_VXVY = 0x8002, //8XY2
		BIT_XOR_VXVY = 0x8003, //8XY3
		ADD_VXVY = 0x8004, //8XY4
		SUB_VXVY = 0x8005, //8XY5
		BIT_SHIFTR_VX = 0x8006, //8XY6
		SUB_VYVX = 0x8007, //8XY7
		BIT_SHIFTL_VX = 0x800E, //8XYE
		SKIP_UNEQ_VXVY = 0x9000, //9XY0
		LOAD_IR = 0xA000, //ANNN
		JUMP_V0_REL = 0xB000, //BNNN
		RAND_VX = 0xC000, //CXNN
		DRAW_SPRITE = 0xD000, //DXYN
		SKIP_KEY_T = 0xE09E, //EX9E
		SKIP_KEY_F = 0xE0A1, //EXA1
		LOAD_DT_VX = 0xF007, //FX07
		KEY_WAIT = 0xF00A, //FX0A
		LOAD_VX_DT = 0xF015, //FX15
		LOAD_VX_ST = 0xF018, //FX18
		ADD_VXIR = 0xF01E, //FX1E
		LOAD_IR_SPRITE = 0xF029, //FX29
		STORE_BCD_VXIR = 0xF033, //FX33
		REG_STO = 0xF055, //FX55
		REG_LOAD = 0xF065, //FX65
	};

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
				Decode_Class_0();
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
				Decode_Class_8();
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
				Decode_Class_E();
				break;
			case CLASS_F:
				Decode_Class_F();
				break;
			default:
				//should be unreachable
				break;
			}
		}

		//RCA 1802 calls, Clear_screen, and Return from subroutine
		void Decode_Class_0()
		{
			std::uint8_t VX = 0x00;
			std::uint8_t VY = 0x00;
			switch (opcode)
			{
			case CLEAR_SCREEN:
				//clear the screen
				break;
			case RETURN_OP:
				//pop return address off the stack and point PC there
				SP--;
				PC = cpu_stack[SP];
				break;
			default:
				//unimplemented RCA 1802 call. format 0NNN
				break;
			}
		}

		//arithmetic and bitwise operations
		void Decode_Class_8()
		{
			std::uint8_t VX = 0x00;
			std::uint8_t VY = 0x00;
			switch (opcode & 0xF00F)
			{
			case LOAD_VXVY:
				//load VY into VX. format 8XY0
				VX = (opcode & 0x0F00) >> 8;
				VY = (opcode & 0x00F0) >> 4;
				GPR[VX] = GPR[VY];
				break;
			case BIT_OR_VXVY:
				//load (VX | VY) into VX. format 8XY1
				VX = (opcode & 0x0F00) >> 8;
				VY = (opcode & 0x00F0) >> 4;
				GPR[VX] = GPR[VX] | GPR[VY];
				break;
			case BIT_AND_VXVY:
				//load (VX & VY) into VX. format 8XY2
				VX = (opcode & 0x0F00) >> 8;
				VY = (opcode & 0x00F0) >> 4;
				GPR[VX] = GPR[VX] & GPR[VY];
				break;
			case BIT_XOR_VXVY:
				//load (VX ^ VY) into VX. format 8XY3
				VX = (opcode & 0x0F00) >> 8;
				VY = (opcode & 0x00F0) >> 4;
				GPR[VX] = GPR[VX] ^ GPR[VY];
				break;
			case ADD_VXVY:
				//add VY to VX. set VF to 1 if overflow, set to 0 if not . format 8XY4
				VX = (opcode & 0x0F00) >> 8;
				VY = (opcode & 0x00F0) >> 4;
				if (GPR[VX] + GPR[VY] > UINT8_MAX) { GPR[0xF] = 1; }
				else { GPR[0xF] = 0; }
				GPR[VX] += GPR[VY];
				break;
			case SUB_VXVY:
				//subtract VY from VX. set VF to 0 if underflow, set to 1 if not. format 8XY5
				VX = (opcode & 0x0F00) >> 8;
				VY = (opcode & 0x00F0) >> 4;
				if (GPR[VX] - GPR[VY] < 0) { GPR[0xF] = 0; }
				else { GPR[0xF] = 1; }
				GPR[VX] -= GPR[VY];
				break;
			case BIT_SHIFTR_VX:
				//shift VX right 1 position. set VF to the LSB before the shift. format 8XY6
				VX = (opcode & 0x0F00) >> 8;
				GPR[0xF] = GPR[VX] & 0x01;
				GPR[VX] = GPR[VX] >> 1;
				break;
			case SUB_VYVX:
				//load (VY - VX) into VX. set VF to 0 if underflow, set to 1 otherwise. format 8XY7
				VX = (opcode & 0x0F00) >> 8;
				VY = (opcode & 0x00F0) >> 4;
				if (GPR[VY] - GPR[VX] < 0) { GPR[0xF] = 0; }
				else { GPR[0xF] = 1; }
				GPR[VX] = GPR[VY] - GPR[VX];
				break;
			case BIT_SHIFTL_VX:
				//shift VX left 1 position. set VF to the MSB before the shift. format 8XYE
				VX = (opcode & 0x0F00) >> 8;
				GPR[0xF] = GPR[VX] & 0b10000000;
				GPR[VX] = GPR[VX] << 1;
				break;
			default:
				//should be unreachable
				break;
			}
		}

		//skip if key is held, skip if key is not held
		void Decode_Class_E()
		{
			std::uint8_t VX = 0x00;
			std::uint8_t VY = 0x00;
			switch (opcode)
			{
			case SKIP_KEY_T:
				//need to implement input first
				break;
			case SKIP_KEY_F:
				//need to implement input first
				break;
			default:
				//should be unreachable
				break;
			}
		}

		void Decode_Class_F()
		{
			std::uint8_t VX = 0x00;
			std::uint8_t VY = 0x00;
			switch (opcode & 0xF0FF)
			{
			case LOAD_DT_VX:
				//load the delay timer into VX. format FX07
				VX = (opcode & 0x0F00) >> 8;
				GPR[VX] = DT;
				break;
			case KEY_WAIT:
				//wait until a key is pressed and store the key in VX. format FX0A
				//need to implement input first
				break;
			case LOAD_VX_DT:
				//load VX into the delay timer. format FX15
				VX = (opcode & 0x0F00) >> 8;
				DT = GPR[VX];
				break;
			case LOAD_VX_ST:
				//load VX into the sound timer. format FX18
				VX = (opcode & 0x0F00) >> 8;
				ST = GPR[VX];
				break;
			case ADD_VXIR:
				//add VX to IR. format FX1E
				VX = (opcode & 0x0F00) >> 8;
				IR += GPR[VX];
				break;
			case LOAD_IR_SPRITE:
				//load the sprite location for the character in VX into IR. format FX29
				//need more info on where sprites are stored
				break;
			case STORE_BCD_VXIR:
				//store VX as a BCD representation starting at the location in IR. format FX33
			{
				VX = (opcode & 0x0F00) >> 8;
				std::uint8_t tempa = GPR[VX] / 100;
				std::uint8_t tempb = GPR[VX] / 10;
				std::uint8_t tempc = GPR[VX];
				tempb -= tempa * 10;
				tempc -= tempa * 100 + tempb * 10;
				memory->write(IR, tempa);
				memory->write(IR + 1, tempb);
				memory->write(IR + 2, tempc);
			}
			break;
			case REG_STO:
				//store V0 through VX in memory starting at IR. format FX55
				VX = (opcode & 0x0F00) >> 8;
				for (std::uint8_t i = 0; i <= VX; i++)
				{
					memory->write(IR + i, GPR[i]);
				}
				break;
			case REG_LOAD:
				//load memory starting at IR into V0 through VX. format FX65
				VX = (opcode & 0x0F00) >> 8;
				for (std::uint8_t i = 0; i <= VX; i++)
				{
					GPR[i] = memory->read(IR + i);
				}
				break;
			default:
				//should be unreachable
				break;
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
