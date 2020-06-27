#include <bits/stdc++.h>
#include "./Brainfuck.hpp"

std::vector<std::string> tests {
    // Hello World
    "++++++++++[>+++++++>++++++++++>+++>+<<<<-]>++.>+.+++++++..+++.>++.<<+++++++++++++++.>.+++.------.--------.>+.",
    // AFMIKA
    "+++[>+++++[>+++++<-]<-]>>----------.+++++.+++++++.----.++.----------."
};

// Tests
void runCompressionTest () {
    printf("\n## COMPRESSION TESTS");
    int i = 1;
    for (auto source : tests) {
        printf("\n\n### Test Case %d", i++);
        Brainfuck::Source result = Brainfuck::CompressCode ( source );
        Brainfuck::Source uncompr = Brainfuck::UncompressCode (result.value);
        printf("\n[UNCOMPRESSED] : %s\n* [COMPRESSED] : %s", source.c_str(), result.value.c_str());
        printf("\n[UNCOMPRESSION (COMPRESSED)] %s", uncompr.value.c_str());
    }
}

void runExecutionTests () {
    printf("\n\n\n## EXECUTION TESTS");
    int i = 1;
    for (auto source : tests ) {
        // uncompressed
        printf("\n\n### Test Case %d", i++);
        printf("\n[RUNNING : UNCOMPRESSED SRC] : ");
        Brainfuck::Parser uparser ( source, false );
        uparser.SetOutputMode (Brainfuck::OUTPUT_MODE::CHAR); // default
        uparser.SetClusterSize ( Brainfuck::ONE_BYTE );
        uparser.Run();

        // compressed
        printf("\n[RUNNING : COMPRESSED SRC] : ");
        Brainfuck::Parser cparser ( source, true );
        cparser.SetOutputMode (Brainfuck::OUTPUT_MODE::CHAR); // default
        cparser.SetClusterSize ( Brainfuck::ONE_BYTE );
        cparser.Run();
    }
}

void runAllTests (std::string str) {
    printf("\n[[ TEST MODE ]]\n");
    runCompressionTest ();
    runExecutionTests  ();
    printf("\n");
}

void printInfos (std::string str) {
    printf("#####################################################\n");
    printf("# Optimized brainfuck-interpreter written by afmika #\n");
    printf("#                     JUNE 2020                     #\n");
    printf("#                [github.com/afmika]                #\n");
    printf("#####################################################\n");
    printf("\ninfo | -i : Information    [arg :    none ]\n");
    printf("\ntest | -t : Run tests      [arg :    none ]\n");
    printf("\nrun  | -r : Execute a file [arg : filepath]\n");
}

void runFile (std::string filename) {
    std::string content;
    std::ifstream file(filename.c_str(),  std::ios::in);
    if ( ! file.is_open() ) {
        std::cerr << "Error opening source file\n" << "\n";
        return;
    }
    while ( !file.eof() ) {
        std::string tmp = "";
        file >> tmp;
        content += tmp;
    }

    Brainfuck::Parser parser ( content, true );
    parser.SetOutputMode (Brainfuck::OUTPUT_MODE::CHAR); // default
    parser.SetClusterSize ( Brainfuck::ONE_BYTE );
    parser.Run();
    // printf("content \n%s\n", content.c_str());
}

// Command map
std::map<std::string, void (*)(std::string) > cmd {
    {"info", &printInfos },
    {"-i"  , &printInfos },
    {""    , &printInfos },
    {"test", &runAllTests},
    {"-t"  , &runAllTests},
    {"run" , &runFile    },
    {"-r"  , &runFile    },
};

int main(int argc, const char* argv[]) {
    if ( argc == 1 ) {
        printInfos ("");
        return 0;
    } else {
        std::string com(argv[1]);
        if ( cmd.find(com) == cmd.end() ) {
            printf("[ERROR] INVALID COMMAND !\n");
            printInfos("");
            return 0;
        }
        if ( argc >= 3 ) {
            std::string argm(argv[2]);
            (*cmd[com]) (argm);
        } else {
            (*cmd[com]) ("");
        }
    }
    return 0;
}
