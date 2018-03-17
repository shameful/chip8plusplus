#include "CPU.h"
#include "Memory.h"
#include "Display.h"
#include <iostream>
#include <bitset>
#include <string>
#include <fstream>


int main(int numArgs, char* args[])
{
	std::cout << "not fully implemented\n";
	Mem::Chip8Mem mem;
	Display::Chip8Display display;
	CPU::Chip8CPU cpu;
	cpu.Set_Memory_Device(mem);
	cpu.Set_Display(display);
	cpu.Load_Sprites();

	if (numArgs > 1)
	{
		std::ifstream program(args[1], std::ios::binary);
		if (program.is_open())
		{
			char junk = 'a';
			char* byte = &junk;
			for (std::uint16_t i = CPU::PROG_START; i < 0x1000; i++)
			{
				if (program.eof()) { program.close(); break; }
				program.read(byte, 1);
				mem.write(i, *byte);
			}
		}
	}
	
	//run the instructions
	CPU::RETURN_CODES status = CPU::RETURN_CODES::OK;
	while (status == CPU::RETURN_CODES::OK)
	{
		status = cpu.Execute_Step();

	}
	
	//print the framebuffer
	for (std::uint16_t i = 0; i < 32; i++)
	{
		for (std::uint16_t j = 0; j < 8; j++)
		{
			std::string eight_pixels = "";
			std::uint8_t bits(display.read(i * 8 + j));
			if ((bits & 0b10000000) > 0) { eight_pixels += "0"; }
			else { eight_pixels += "_"; }
			if ((bits & 0b01000000) > 0) { eight_pixels += "0"; }
			else { eight_pixels += "_"; }
			if ((bits & 0b00100000) > 0) { eight_pixels += "0"; }
			else { eight_pixels += "_"; }
			if ((bits & 0b00010000) > 0) { eight_pixels += "0"; }
			else { eight_pixels += "_"; }
			if ((bits & 0b00001000) > 0) { eight_pixels += "0"; }
			else { eight_pixels += "_"; }
			if ((bits & 0b00000100) > 0) { eight_pixels += "0"; }
			else { eight_pixels += "_"; }
			if ((bits & 0b00000010) > 0) { eight_pixels += "0"; }
			else { eight_pixels += "_"; }
			if ((bits & 0b00000001) > 0) { eight_pixels += "0"; }
			else { eight_pixels += "_"; }
			std::cout << eight_pixels;
		}
		std::cout << "\n";
	}
	
}