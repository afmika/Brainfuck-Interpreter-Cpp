#include <iostream>
#include "./Brainfuck.hpp"

int main() {
    // Hello World
    // std::string source = "++++++++++[>+++++++>++++++++++>+++>+<<<<-]>++.>+.+++++++..+++.>++.<<+++++++++++++++.>.+++.------.--------.>+.>.";

    // AFMIKA
    std::string source = "+++[>+++++[>+++++<-]<-]>>----------.+++++.+++++++.----.++.----------.";

    // Test input
    // std::string source = ",.";
    Brainfuck parser( source );
    while ( parser.Next() ) {
        // printf("%c\n", parser.GetCurrentToken() );
    }
    return 0;
}
