#include "parser/parser.h"
#include "cpu/cpu.h"

#include "gui/gui_renderer.h"
#include "gui/gui_elements.h"


int main(int argc, char** argv) {

    parser_t parser;
    auto cli_args  = parser.parse_cli(argc, argv);

    if (!cli_args.valid)
        return 1;

    CPU cpu(cli_args.predictor);
    auto parse_result = parser.parse_program(cli_args.input_file);

    cpu.load_program(std::move(parse_result.first));

    if (cli_args.enable_gui) gui::run(cpu,parse_result);
    else {
        while (!cpu.endofprogram()) cpu.execute();
		if (cli_args.log_dest == "cout") cpu.log(std::cout);
		else {
			std::ofstream os(cli_args.log_dest);
			cpu.log(os);
		}
    }
}
