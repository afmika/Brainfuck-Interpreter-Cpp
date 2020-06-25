#include <bits/stdc++.h>
#include "./Brainfuck.hpp"

int main() {
    // Hello World
    // std::string source = "++++++++++[>+++++++>++++++++++>+++>+<<<<-]>++.>+.+++++++..+++.>++.<<+++++++++++++++.>.+++.------.--------.>+.>.";

    // AFMIKA
    std::string source = "+++[>+++++[>+++++<-]<-]>>----------.+++++.+++++++.----.++.----------.";
    
	// chars
	//std::string source = ".+[.+]"; 
	
	// serpinski
	// std::string source = ">++++[<++++++++>-]>++++++++[>++++<-]>>++>>>+>>>+<<<<<<<<<<[-[->+<]>[-<+>>>.<<]>>>[[->++++++++[>++++<-]>.<<[->+<]+>[->++++++++++<<+>]>.[-]>]]+<<<[-[->+<]+>[-<+>>>-[->+<]++>[-<->]<<<]<<<<]++++++++++.+++.[-]<]+++++";
	// Fibonnacci
	// std::string source = "+++++++++++>+>>>>++++++++++++++++++++++++++++++++++++++++++++>++++++++++++++++++++++++++++++++<<<<<<[>[>>>>>>+>+<<<<<<<-]>>>>>>>[<<<<<<<+>>>>>>>-]<[>++++++++++[-<-[>>+>+<<<-]>>>[<<<+>>>-]+<[>[-]<[-]]>[<<[>>>+<<<-]>>[-]]<<]>>>[>>+>+<<<-]>>>[<<<+>>>-]+<[>[-]<[-]]>[<<+>>[-]]<<<<<<<]>>>>>[++++++++++++++++++++++++++++++++++++++++++++++++.[-]]++++++++++<[->-<]>++++++++++++++++++++++++++++++++++++++++++++++++.[-]<<<<<<<<<<<<[>>>+>+<<<<-]>>>>[<<<<+>>>>-]<-[>>.>.<<<[-]]<<[>>+>+<<<-]>>>[<<<+>>>-]<<[<+>-]>[<+>-]<<<-]";
    // Test input
    // std::string source = ",.";
    Brainfuck::Parser parser( source );
	parser.SetClusterSize( Brainfuck::ONE_BYTE );
	
    while ( parser.Next() ) {
        if ( parser.GetCurrentToken() == '.' ) {
			// is writting
            // std::cout << "\n[ENTER : Next instruction ] : " << (int) parser.GetMemoryAt(parser.GetPtr()) << std::endl;
            // getchar();
        }
    }
    return 0;
}
