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
}
