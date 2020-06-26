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
        // ">++++[<++++++++>-]>++++++++[>++++<-]>>++>>>+>>>+<<<<<<<<<<[-[->+<]>[-<+>>>.<<]>>>[[->++++++++[>++++<-]>.<<[->+<]+>[->++++++++++<<+>]>.[-]>]]+<<<[-[->+<]+>[-<+>>>-[->+<]++>[-<->]<<<]<<<<]++++++++++.+++.[-]<]+++++",
    };

    for (auto source : tests ) {
		Brainfuck::Source compressed   = Brainfuck::CompressCode( source );
		Brainfuck::Source uncompressed = Brainfuck::UncompressCode( compressed );
		printf("COMPRESSION : %s\n", (uncompressed.value.compare(source) == 0 ? "SUCCESS" : "FAIL") );

        Brainfuck::Parser parser( source, false );
        parser.SetClusterSize( Brainfuck::ONE_BYTE );
        parser.Run();
        printf("\n");
    }
}

int main() {
    runTests();
    return 0;
}
