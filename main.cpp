#include <iostream>

#include "parser/parser.h"
#include "cpu/cpu.h"

int main(int argc,char** argv) {
    if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " <input.s> \n";
		return 1;
	}
    const char *input_file = argv[1];
    parser_t parser;
    CPU      cpu;
    cpu.load_program(parser.parse_program(std::string(input_file)));
    while (!cpu.halt()) {
        cpu.execute();
    }
	cpu.log(std::cout);
}