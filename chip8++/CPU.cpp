#include "CPU.h"
#include <iostream>

namespace CPU
{
	void Chip8CPU::Fetch_and_IncPC(Mem::Chip8Mem &memory)
	{
		opcode = memory.read(PC) << 8;
		opcode = opcode | memory.read(PC + 1);
		std::cout << std::hex << PC << " " << opcode << std::endl;
		PC += 2;
	}

	void Chip8CPU::Decode_and_execute(Mem::Chip8Mem &memory, Display::Chip8Display &display)
	{
		std::uint8_t VX = 0x00;
		std::uint8_t VY = 0x00;
		switch (opcode & 0xF000)
		{
		case CLASS_0:
			Decode_Class_0(memory, display);
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
			if (GPR[VX] == (opcode & 0x00FF)) { PC += 2; }
			break;
		case CLASS_4:
			//Skip the next instruction if VX != NN. format 4XNN
			VX = (opcode & 0x0F00) >> 8;
			if (GPR[VX] != (opcode & 0x00FF)) { PC += 2; }
			break;
		case CLASS_5:
			//Skip the next instruction if VX == VY. format 5XY0
			VX = (opcode & 0x0F00) >> 8;
			VY = (opcode & 0x00F0) >> 4;
			if (GPR[VX] == GPR[VY]) { PC += 2; }
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
			Decode_Class_8(memory, display);
			break;
		case CLASS_9:
			//Skip the next instruction if VX != VY. format 9XY0
			VX = (opcode & 0x0F00) >> 8;
			VY = (opcode & 0x00F0) >> 4;
			if (GPR[VX] != GPR[VY]) { PC += 2; }
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
			VX = (opcode & 0x0F00) >> 8;
			GPR[VX] = rand() & (opcode & 0x00FF);
			break;
		case CLASS_D:
			//Draw a sprite at position (VX,VY) 8 pixels wide and N pixels high. format DXYN
			//sets VF to 1 if pixels are changed from on to off
			VX = (opcode & 0x0F00) >> 8;
			VY = (opcode & 0x00F0) >> 4;
			{
				std::uint8_t length = opcode & 0x000F;
				std::array<std::uint8_t, 15> sprite;
				sprite.fill(0x00);
				for (std::uint16_t i = 0; i < length; i++)
				{
					sprite[i] = memory.read(IR + i);
				}
				GPR[0xF] = display.DrawSprite(GPR[VX], GPR[VY], sprite, length);
			}
			break;
		case CLASS_E:
			Decode_Class_E(memory, display);
			break;
		case CLASS_F:
			Decode_Class_F(memory, display);
			break;
		default:
			//invalid opcode
			execution_status = FAULT;
			break;
		}
	}

	//RCA 1802 calls, Clear_screen, and Return from subroutine
	void Chip8CPU::Decode_Class_0(Mem::Chip8Mem &memory, Display::Chip8Display &display)
	{
		std::uint8_t VX = 0x00;
		std::uint8_t VY = 0x00;
		switch (opcode)
		{
		case CLEAR_SCREEN:
			//clear the screen
			display.Clear();
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
	void Chip8CPU::Decode_Class_8(Mem::Chip8Mem &memory, Display::Chip8Display &display)
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
			//invalid opcode
			execution_status = FAULT;
			break;
		}
	}

	//skip if key is held, skip if key is not held
	void Chip8CPU::Decode_Class_E(Mem::Chip8Mem &memory, Display::Chip8Display &display)
	{
		std::uint8_t VX = 0x00;
		std::uint8_t VY = 0x00;
		switch (opcode & 0xF0FF)
		{
		case SKIP_KEY_T:
			//Skip the next instruction if the key in VX is pressed. format EX9E
			VX = (opcode & 0x0F00) >> 8;
			if (keystates[(GPR[VX] & 0x0F)]) { PC += 2; } //zero top bits from GPR to prevent out-of-bounds
			break;
		case SKIP_KEY_F:
			//SKip the next instruction if the key in VX is not pressed. format EXA1
			VX = (opcode & 0x0F00) >> 8;
			if (!keystates[(GPR[VX] & 0x0F)]) { PC += 2; } //zero top bits from GPR to prevent out-of-bounds
			break;
		default:
			//invalid opcode
			execution_status = FAULT;
			break;
		}
	}

	void Chip8CPU::Decode_Class_F(Mem::Chip8Mem &memory, Display::Chip8Display &display)
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
			{
			//limiting scope of key_found variable
				VX = (opcode & 0x0F00) >> 8;
				bool key_found = false;
				for (std::uint8_t i = 0; i < 0x10; i++)
				{
					if (keystates[i]) { GPR[VX] = i; key_found = true; break; }
				}
				if (!key_found) { PC -= 2; }
			}
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
			VX = (opcode & 0x0F00) >> 8; //GPR[VX] contains offset of specific sprite to load (0 through F)
			IR = sprites_addr + (GPR[VX] & 0x0F) * SPRITE_LENGTH;
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
			memory.write(IR, tempa);
			memory.write(IR + 1, tempb);
			memory.write(IR + 2, tempc);
		}
		break;
		case REG_STO:
			//store V0 through VX in memory starting at IR. format FX55
			VX = (opcode & 0x0F00) >> 8;
			for (std::uint8_t i = 0; i <= VX; i++)
			{
				memory.write(IR + i, GPR[i]);
			}
			break;
		case REG_LOAD:
			//load memory starting at IR into V0 through VX. format FX65
			VX = (opcode & 0x0F00) >> 8;
			for (std::uint8_t i = 0; i <= VX; i++)
			{
				GPR[i] = memory.read(IR + i);
			}
			break;
		default:
			//invalid opcode
			execution_status = FAULT;
			break;
		}
	}

	void Chip8CPU::Dec_Timers()
	{
		if (DT > 0) { DT--; }
		if (ST > 0) { ST--; }
	}

	Chip8CPU::Chip8CPU()
	{
		Cpu_initialize();
	}

	Chip8CPU::~Chip8CPU()
	{

	}

	void Chip8CPU::Cpu_initialize()
	{
		GPR.fill(0x00);
		DT = 0;
		ST = 0;
		IR = 0;
		PC = PROG_START;
		SP = 0;
		cpu_stack.fill(0x0000);
		opcode = 0x0000;
		execution_status = OK;
		keystates.fill(false);
	}

	void CPU::Chip8CPU::Load_Sprites(Mem::Chip8Mem &memory)
	{
		for (std::uint16_t i = 0; i < SPRITEMAP_LENGTH; i++)
		{
			memory.write(sprites_addr + i, character_sprites[i]);
		}
		memory.setSpritesLoaded(true);
	}

	RETURN_CODES CPU::Chip8CPU::Execute_Step(Mem::Chip8Mem &memory, Display::Chip8Display &display)
	{
		if (!memory.checkSpritesLoaded()) { Load_Sprites(memory); }
		execution_status = OK;
		Dec_Timers();
		Fetch_and_IncPC(memory);
		Decode_and_execute(memory, display);
		return execution_status;
	}

}
