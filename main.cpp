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
        //".+[.+]",
        // [!] SLOW
        // serpinski
        // ">++++[<++++++++>-]>++++++++[>++++<-]>>++>>>+>>>+<<<<<<<<<<[-[->+<]>[-<+>>>.<<]>>>[[->++++++++[>++++<-]>.<<[->+<]+>[->++++++++++<<+>]>.[-]>]]+<<<[-[->+<]+>[-<+>>>-[->+<]++>[-<->]<<<]<<<<]++++++++++.+++.[-]<]+++++",
    };

    for (auto source : tests ) {
        Brainfuck::Parser parser( source, false );
        parser.SetClusterSize( Brainfuck::ONE_BYTE );
        /*
        do {
            std::string token = parser.GetCurrentToken();
            printf("%s", token.c_str());
            getchar();
        } while ( parser.Next() );
        */
        parser.Run();

        printf("\n");
    }
}

int main() {
    runTests();
    return 0;
}
