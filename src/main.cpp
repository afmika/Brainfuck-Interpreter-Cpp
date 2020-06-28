#include <bits/stdc++.h>
#include "./Brainfuck.hpp"

using namespace std;

// opt execution
// Cluster size
Brainfuck::CLUSTER_SIZE CLUSTER = Brainfuck::ONE_BYTE.value;
// IO
Brainfuck::IO_MODE INPUT_MODE   = Brainfuck::IO_MODE::CHAR;
Brainfuck::IO_MODE OUTPUT_MODE  = Brainfuck::IO_MODE::CHAR;

// debug
bool D_PRINT_INSTR  = false;
bool D_STEP_BY_STEP = true;

// Tests
vector<string> tests {
    // Hello World
    "++++++++++[>+++++++>++++++++++>+++>+<<<<-]>++.>+.+++++++..+++.>++.<<+++++++++++++++.>.+++.------.--------.>+.",
    // AFMIKA
    "+++[>+++++[>+++++<-]<-]>>----------.+++++.+++++++.----.++.----------."
};

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
        uparser.SetOutputMode (Brainfuck::IO_MODE::CHAR); // default
        uparser.SetInputMode  (Brainfuck::IO_MODE::CHAR); // default
        uparser.SetClusterSize ( Brainfuck::ONE_BYTE );
        uparser.Run();

        // compressed
        printf("\n[RUNNING : COMPRESSED SRC] : ");
        Brainfuck::Parser cparser ( source, true );
        cparser.SetOutputMode (Brainfuck::IO_MODE::CHAR); // default
        cparser.SetInputMode  (Brainfuck::IO_MODE::CHAR); // default
        cparser.SetClusterSize ( Brainfuck::ONE_BYTE );
        cparser.Run();
    }
}

void runAllTests (string str) {
    printf("\n[[ TEST MODE ]]\n");
    runCompressionTest ();
    runExecutionTests  ();
    printf("\n");
}

void printInfos (string str) {
    printf("#####################################################\n");
    printf("# Optimized brainfuck-interpreter written by afmika #\n");
    printf("#                     JUNE 2020                     #\n");
    printf("#                [github.com/afmika]                #\n");
    printf("#####################################################\n\n");
    printf("info | -i : Information    [arg :    none ]\n");
    printf("test | -t : Run tests      [arg :    none ]\n");
    printf("run  | -r : Execute a file [arg : filepath]\n");
    printf("\n[[ Basic use of the run command ]]\n");

    printf("Example :\n$ afbf run myfile.bf\n");

    printf("\n[[ Advanced uses of the run command ]]\n");
    printf("\n## Debug mode with the debug/-d command\n");
    printf("[usage] $ afbf run debug=value myfile.bf\n");
    printf("[usage] $ afbf run debug myfile.bf\n");
    printf("[!] value can be replaced by print, end or step\n");
    printf("\t* step  : debug at each instruction\n");
    printf("\t* print : debug each time the print instruction is executed\n");
    printf("\t* end   : display the final memory state after the program execution\n");
    printf("\nExamples:\n");
    printf("$ afbf run -d myfile.bf\n");
    printf("$ afbf run debug myfile.bf\n");
    printf("$ afbf run -d=print myfile.bf\n");
    printf("$ afbf run debug=end myfile.bf\n");

    printf("\n## Customized cluster size (8, 16 or 32)\n");
    printf("[usage] $ afbf run memory=size myfile.bf\n");
    printf("\nExample : \nafbf run memory=8 myfile.bf\n");
    printf("\n## Customized input/output mode ('c' : char or 'i' : integer)\n");
    printf("[usage] $ afbf run io=xy myfile.bf\n");
    printf("[!] x represents the input mode, y represents the output mode\n");
    printf("[!] x and y can be replaced by c or i\n");
    printf("\nExamples:\n");
    printf("1. integer as input, char as output\n");
    printf("$ afbf run io=ic myfile.bf\n");
    printf("2. char as input, char as output\n");
    printf("$ afbf run io=cc myfile.bf\n");

    printf("\n## Combined version\n");
    printf("You can combine all of them along with the run command\n");
    printf("Examples:\n");
    printf("$ afbf run io=ic memory=8 myfile.bf\n");
    printf("$ afbf run memory=16 io=ii myfile.bf\n");
    printf("$ afbf run memory=16 io=ci myfile.bf\n");
    printf("$ afbf run memory=16 io=ci debug myfile.bf\n");
    printf("$ afbf run debug memory=32 io=ci myfile.bf\n");
    printf("$ afbf run -d=print io=ci myfile.bf\n");
    printf("$ afbf run debug=step io=ci myfile.bf\n");
}

string getFileContent(string filename) {
    string content;
    ifstream file(filename.c_str(),  ios::in);
    if ( ! file.is_open() ) {
        return content;
    }
    while ( !file.eof() ) {
        string tmp = "";
        file >> tmp;
        content += tmp;
    }
    file.close();
    return content;
}

void runFile (string filename) {
    printf("\n--[ EXECUTION ]-------------------------\n\n");
    string content = getFileContent(filename);
    if ( content.compare("") == 0 ) {
        cerr << "\nError opening source file" << "\n";
        return;
    }
    Brainfuck::Parser parser ( content, true );
    parser.SetOutputMode ( OUTPUT_MODE ); // default
    parser.SetInputMode ( INPUT_MODE  ); // default
    parser.SetClusterSize ( CLUSTER );
    parser.Run();
}

void runDebugMode (string filename) {
    string content = getFileContent(filename);
    if ( content.compare("") == 0 ) {
        cerr << "\nError opening source file" << "\n";
        return;
    }
    Brainfuck::Parser parser ( content, true );
    parser.SetOutputMode ( OUTPUT_MODE ); // default
    parser.SetInputMode ( INPUT_MODE  ); // default
    parser.SetClusterSize ( CLUSTER );
    int margin = 7;
    if ( D_STEP_BY_STEP ) {
        while ( parser.Next() ) {
            string token = parser.GetCurrentToken();
            int   center = parser.GetPtr();
            int left     = center - margin;
            int right    = center + margin;
            string memo  = parser.GetMemoryIntervalAsString(left, right);

            left  &= Brainfuck::UINT_MASK[CLUSTER.value];
            right &= Brainfuck::UINT_MASK[CLUSTER.value];
            if ( D_PRINT_INSTR && token.compare(".") == 0) {
                printf("\n\nVRAM $%08x - $%08x [token = \"%s\"]", left, right, token.c_str());
                printf("\n---------------------------------------------------\n");
                printf("%s", memo.c_str());
                printf("\n-------------[ ENTER : CONTINUE ]------------------");
                getchar();
            }
            if ( ! D_PRINT_INSTR ) {
                printf("\n\nVRAM $%08x - $%08x [token = \"%s\"]", left, right, token.c_str());
                printf("\n---------------------------------------------------\n");
                printf("%s", memo.c_str());
                printf("\n-------------[ ENTER : CONTINUE ]------------------");
                getchar(); // at each Next(token)
            }
        }
    } else {
        parser.Run();
        string token = parser.GetCurrentToken();
        int   center = parser.GetPtr();
        int left     = center - margin;
        int right    = center + margin;
        string memo  = parser.GetMemoryIntervalAsString(left, right);
        left  &= Brainfuck::UINT_MASK[CLUSTER.value];
        right &= Brainfuck::UINT_MASK[CLUSTER.value];
        printf("\n\nVRAM $%08x - $%08x [token = \"%s\"]", left, right, token.c_str());
        printf("\n---------------------------------------------------\n");
        printf("%s\n", memo.c_str());
    }
}

// Command map
map<string, void (*)(string) > cmd {
    {"info", &printInfos },
    {"-i"  , &printInfos },
    {""    , &printInfos },
    {"test", &runAllTests},
    {"-t"  , &runAllTests},
    {"run" , &runFile    },
    {"-r"  , &runFile    },

    {"io"    , nullptr},
    {"memory", nullptr},
    {"debug" , nullptr},
    {"-d" , nullptr},
};

bool isValidCommand (string com) {
    return cmd.find(com) != cmd.end();
}

bool includes(string a, char b) {
    for (char t : a) if ( t == b ) return true;
    return false;
}

// example io=cc => io, cc
pair<string, string> extractCmd ( string param ) {
    string com = "", value = "";
    bool switched = false;
    for (char c : param) {
        if ( c != ' ' ) {
            if ( c == '=' ) switched = true;
            else
                if ( !switched ) {
                    com   += c;
                } else {
                    value += c;
                }
        }
    }
    return make_pair(com, value);
}

int main(int argc, const char* argv[]) {
    if ( argc == 1 ) {
        printInfos ("");
        return 0;
    } else {
        string main_com(argv[1]);
        if ( ! isValidCommand( main_com )  ){
            printf("[ERROR] INVALID COMMAND !\n");
            return 0;
        }

        // afbf run filename
        string filename(argv[argc-1]);
        if ( argc == 3 ) {
            (*cmd[main_com]) (filename);
            return 0;
        }

        // main_com is assumed to be equal to -r or run

        // the latest arg is assumed to be the filepath
        for (int i = 2; i < argc - 1; i++) {
            string temp(argv[i]);
            pair<string, string> p = extractCmd(temp);
            if ( ! isValidCommand(  p.first ) ) {
                printf("\n[ERROR] INVALID COMMAND : %s !", p.first.c_str());
                return 0;
            }
            string cname = p.first, cvalue = p.second;
            if ( cname.compare("memory") == 0 ) {
                int cur = Brainfuck::str_to_int(cvalue);
                if ( cur == 8 ) {
                    CLUSTER = Brainfuck::ONE_BYTE;
                } else if ( cur == 16 ) {
                    CLUSTER = Brainfuck::TWO_BYTES;
                } else if ( cur == 32) {
                    CLUSTER = Brainfuck::FOUR_BYTES;
                } else {
                    printf("\n[ERROR] INVALID MEMORY VALUE : %d != 8, 16, 32", cur);
                    return 0;
                }
            } else if ( cname.compare("io") == 0 ) {
                map<char, bool> valids { {'c', true}, {'i', true }};
                if ( cvalue.size() != 2 ) {
                    printf("\n[ERROR] You must provide a string of length = 2");
                    return 0;
                }
                for (auto c : cvalue) {
                    if ( valids.find(c) == valids.end() ) {
                        printf("\n[ERROR] INVALID VALUE : %c != c, i", c);
                        return 0;
                    }
                }
                char input  = cvalue[0];
                char output = cvalue[1];
                if ( input == 'i' ) {
                    INPUT_MODE = Brainfuck::IO_MODE::INT;
                    printf("\n[INPUT  : MODE_INTEGER]");
                } else {
                    printf("\n[INPUT  : DEFAULT_CHAR]");
                }
                if ( output == 'i') {
                    OUTPUT_MODE = Brainfuck::IO_MODE::INT;
                    printf("\n[OUTPUT : MODE_INTEGER]");
                } else {
                    printf("\n[OUTPUT : DEFAULT_CHAR]");
                }
            } else if ( cname.compare("debug") == 0 || cname.compare("-d") == 0 ) {
                if ( cvalue.compare("") == 0 ) {
                    printf("\n[DEBUG MODE : DEFAULT (STEP BY STEP)]\n");
                } else if ( cvalue.compare("step") == 0 ) {
                    printf("\n[DEBUG MODE : STEP BY STEP]\n");
                    D_STEP_BY_STEP = true;
                } else if ( cvalue.compare("end") == 0  ) {
                    printf("\n[DEBUG MODE : DISPLAY MEMORY AT THE END]\n");
                    D_STEP_BY_STEP = false;
                } else if ( cvalue.compare("print") == 0 ) {
                    printf("\n[DEBUG MODE : DISPLAY MEMORY AT EACH PRINT INSTR]\n");
                    D_STEP_BY_STEP = true;
                    D_PRINT_INSTR  = true;
                } else {
                    printf("\n[ERROR] INVALID DEBUG OPTION, %s != step,end,print", cvalue.c_str());
                }
                runDebugMode( filename );
                return 0;
            }
        }
        (*cmd[main_com]) (filename);
    }
    return 0;
}
