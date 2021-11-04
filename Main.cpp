#define WIN32_LEAN_AND_MEAN
#include "./ProcessMemory.h"
#include "./ProcessModule.h"
#include <iostream>
#include <regex>

ProcessMemory memory(L"Crab Game.exe");
ProcessModule game(L"GameAssembly.dll");

int main() {
#ifdef _DEBUG
	std::cout << "Binary built without optimizations. Regex performance will suffer." << std::endl;
#endif
	
	if (int error = memory.open()) {
		std::cerr << "Unable to open process: " << error << std::endl;
		return EXIT_FAILURE;
	}
	
	
	if (int error = game.open(memory)) {
		std::cerr << "Unable to open module: " << error << std::endl;
		return EXIT_FAILURE;
	}

	std::string ass;
	ass.resize(game.size);
	
	if (int error = memory.read(game, game.size, &ass[0])) {
		std::cout << "Unable to read image: " << error << std::endl;
		return EXIT_FAILURE;
	}

	{
		std::smatch match;
		std::regex signature(R"(\xC6\x43\x24\x00\x48\x8B\xCB)");
		if (std::regex_search(ass, match, signature)) {
			uintptr_t address = game + match.position(0) + 3;
			memory.write<unsigned char>(address, 1);
		}
		else std::cerr << "Unable to match cooldown" << std::endl;
	}

	{
		std::smatch match;
		std::regex signature(R"(\x80\xBB.....\x74\x09\x80\xBB.....)");
		if (std::regex_search(ass, match, signature)) {
			uintptr_t address = game + match.position(0);
			memory.write(address, 9, "\x90\x90\x90\x90\x90\x90\x90\x90\x90");
		}
		else std::cerr << "Unable to match ground check" << std::endl;
	}

	{
		std::smatch match;
		std::regex signature(R"(\x40\x53\x48\x83\xEC\x20\x48\x8B\xD9\x48\x85\xC9\x74\x71)");
		if (std::regex_search(ass, match, signature)) {
			uintptr_t address = game + match.position(0);
			memory.write(address, 2, "\xC3\x90");
		}
		else std::cerr << "Unable to match anticheat" << std::endl;
	}
	
	return EXIT_SUCCESS;
}