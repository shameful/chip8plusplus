#include "CPU.h"
#include "Memory.h"
#include "Display.h"
#include <iostream>
#include <bitset>
#include <string>
#include <fstream>
#define SDL_MAIN_HANDLED //without this SDL tries to change the default main() entry point
#include <SDL2/SDL.h>


int main(int numArgs, char* args[])
{
	std::cout << "not fully implemented\n";
	Mem::Chip8Mem mem;
	Display::Chip8Display display;
	CPU::Chip8CPU cpu;
	const Uint8* key_buffer = NULL;
	cpu.Load_Sprites(mem);

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
	
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow("Chip8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 256, 128, 0);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_RenderSetScale(renderer, 4, 4);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_Rect test_rect = { 0, 0, 400, 400 };
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_RenderPresent(renderer); //swap back buffer to screen
	SDL_Event event_SDL;

	//run the instructions
	CPU::RETURN_CODES status = CPU::RETURN_CODES::OK;
	while (status == CPU::RETURN_CODES::OK)
	{
		status = cpu.Execute_Step(mem, display);
		SDL_PollEvent(&event_SDL);

		//handle inputs
		key_buffer = SDL_GetKeyboardState(NULL);
		cpu.keystates[0x0] = key_buffer[SDL_SCANCODE_1]; //row 1
		cpu.keystates[0x1] = key_buffer[SDL_SCANCODE_2];
		cpu.keystates[0x2] = key_buffer[SDL_SCANCODE_3];
		cpu.keystates[0x3] = key_buffer[SDL_SCANCODE_4];
		cpu.keystates[0x4] = key_buffer[SDL_SCANCODE_Q]; //row 2
		cpu.keystates[0x5] = key_buffer[SDL_SCANCODE_W];
		cpu.keystates[0x6] = key_buffer[SDL_SCANCODE_E];
		cpu.keystates[0x7] = key_buffer[SDL_SCANCODE_R];
		cpu.keystates[0x8] = key_buffer[SDL_SCANCODE_A]; //row 3
		cpu.keystates[0x9] = key_buffer[SDL_SCANCODE_S];
		cpu.keystates[0xA] = key_buffer[SDL_SCANCODE_D];
		cpu.keystates[0xB] = key_buffer[SDL_SCANCODE_F];
		cpu.keystates[0xC] = key_buffer[SDL_SCANCODE_Z]; //row 4
		cpu.keystates[0xD] = key_buffer[SDL_SCANCODE_X];
		cpu.keystates[0xE] = key_buffer[SDL_SCANCODE_C];
		cpu.keystates[0xF] = key_buffer[SDL_SCANCODE_V];

		//check if framebuffer got changed since last instruction
		if (display.flag_dirty == true)
		{
			//update the screen
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear(renderer);
			SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
			for (std::uint16_t i = 0; i < 32; i++)
			{
				for (std::uint16_t j = 0; j < 8; j++)
				{
					std::uint8_t pixels = display.read(i * 8 + j);
					if (pixels & 0b10000000) { SDL_RenderDrawPoint(renderer, j * 8, i); }
					if (pixels & 0b01000000) { SDL_RenderDrawPoint(renderer, j * 8 + 1, i); }
					if (pixels & 0b00100000) { SDL_RenderDrawPoint(renderer, j * 8 + 2, i); }
					if (pixels & 0b00010000) { SDL_RenderDrawPoint(renderer, j * 8 + 3, i); }
					if (pixels & 0b00001000) { SDL_RenderDrawPoint(renderer, j * 8 + 4, i); }
					if (pixels & 0b00000100) { SDL_RenderDrawPoint(renderer, j * 8 + 5, i); }
					if (pixels & 0b00000010) { SDL_RenderDrawPoint(renderer, j * 8 + 6, i); }
					if (pixels & 0b00000001) { SDL_RenderDrawPoint(renderer, j * 8 + 7, i); }
				}
			}
			SDL_RenderPresent(renderer);
			display.flag_dirty = false; //display has been updated
		}
	}
	
	
	SDL_Quit();
	return 0;
}
