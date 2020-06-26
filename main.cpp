#include <bits/stdc++.h>
#include "./Brainfuck.hpp"

void runTests () {
	/*
	TODO

	1- MAKE A DECOMPRESSER USING THE SAME TOKENIZATION ALOGIRHTM !!!

	*/
    std::vector<std::string> tests {
        // Hello World
        "++++++++++[>+++++++>++++++++++>+++>+<<<<-]>++.>+.+++++++..+++.>++.<<+++++++++++++++.>.+++.------.--------.>+.>.",
        // AFMIKA
        "+++[>+++++[>+++++<-]<-]>>----------.+++++.+++++++.----.++.----------.",
        // ASCII Chars
        ".+[.+]",

        // [!] SLOW

        // serpinski
        ">++++[<++++++++>-]>++++++++[>++++<-]>>++>>>+>>>+<<<<<<<<<<[-[->+<]>[-<+>>>.<<]>>>[[->++++++++[>++++<-]>.<<[->+<]+>[->++++++++++<<+>]>.[-]>]]+<<<[-[->+<]+>[-<+>>>-[->+<]++>[-<->]<<<]<<<<]++++++++++.+++.[-]<]+++++",
    };

    for (auto source : tests ) {
        Brainfuck::Parser parser( source, true );
        // parser.SetOutputMode(Brainfuck::OUTPUT_MODE::INT);
        parser.SetOutputMode(Brainfuck::OUTPUT_MODE::CHAR); // default
        printf("Source : %s\n", parser.GetSource().value.c_str() );
        parser.SetClusterSize( Brainfuck::ONE_BYTE );
        parser.Run();
        printf("\n----------------------\n");
    }
}

int main() {
    runTests();
    return 0;
}
