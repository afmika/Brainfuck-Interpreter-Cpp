#include <bits/stdc++.h>
#include "./Brainfuck.hpp"

void runTests () {
    std::vector<std::string> tests {
        // Hello World
        "++++++++++[>+++++++>++++++++++>+++>+<<<<-]>++.>+.+++++++..+++.>++.<<+++++++++++++++.>.+++.------.--------.>+.>.",
        // AFMIKA
        "+++[>+++++[>+++++<-]<-]>>----------.+++++.+++++++.----.++.----------.",

        // ASCII Chars
        // ".+[.+]",

        // [!] SLOW
        // serpinski
        // ">++++[<++++++++>-]>++++++++[>++++<-]>>++>>>+>>>+<<<<<<<<<<[-[->+<]>[-<+>>>.<<]>>>[[->++++++++[>++++<-]>.<<[->+<]+>[->++++++++++<<+>]>.[-]>]]+<<<[-[->+<]+>[-<+>>>-[->+<]++>[-<->]<<<]<<<<]++++++++++.+++.[-]<]+++++",
    };
    for (auto source : tests ) {
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
