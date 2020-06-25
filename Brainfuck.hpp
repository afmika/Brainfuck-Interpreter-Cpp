/*
 * @author afmika
 * Brainfuck interpreter
 * June 2020
 **/

#pragma once
#include <bits/stdc++.h>

namespace Brainfuck {
// data structures
typedef struct CLUSTER_SIZE {
    int value;
    CLUSTER_SIZE(int value) :
        value(value) {}
} CLUSTER_SIZE;

typedef struct Source {
    std::string value;
    bool isOptimized;

    char at (int pos) {
        return value[pos];
    }

    Source (std::string value) : value(value) {
        this->isOptimized = false;
    }

    Source (std::string value, bool isOptimized) :
        value(value), isOptimized(isOptimized) {}
} Source;

const CLUSTER_SIZE ONE_BYTE   = { 8};
const CLUSTER_SIZE TWO_BYTES  = {16};
const CLUSTER_SIZE FOUR_BYTES = {32};

std::map<char, int> sign = {
    {'-', -1}, {'+', 1},
    {'<', -1}, {'>', 1}
};


// helpers
std::string CompressCodeUsing(std::map<char, int> chars, std::string source) {
    source += "#";
    std::string out = "";
    for (size_t i = 0; i < source.size(); i++) {
        char current = source[i];
        if ( chars.find(current) != chars.end() ) {
            chars[current]++;
        } else {
            int total = 0;
            std::string next_str = "";
            // Compute the exact amount we need
            // (ex : ++++-- <=> ++ (+2), +---- <=> --- (-3) )
            for (auto p : chars) {
                total += sign[p.first] * chars[p.first];
                chars[p.first] = 0;
            }
            for (auto p : chars) {
                // same sign
                // => same nature
                // => the product should be greater than zero
                if ( sign[p.first] * total > 1 ) {
                    using namespace std;
                    next_str += "("s;
                    next_str += p.first + to_string(abs(total));
                    next_str += ")"s;
                } else if ( sign[p.first] * total == 1 ) {
                    next_str += p.first;
                }
            }

            // update the current value
            out += next_str + current;
        }
    }
    return out.substr(0, out.size() - 1);
}

Source CompressCode(std::string source) {
    // ex : +++++-- => +5-2 => +3
    source = CompressCodeUsing({{'-', 0},{'+', 0}}, source);
    source = CompressCodeUsing({{'<', 0},{'>', 0}}, source);
    Source result = {source, true};
    return result;
}

std::vector<std::string> tokenizeSource(Source source) {
    std::vector<std::string> tokens;
    for (size_t i = 0; i < source.value.size(); i++) {
        char c = source.at(i);
        std::string tmp = "";
        if ( c != '(' ) {
            tmp += c;
        } else {
            if ( c == '(') {
                i++;
                while (source.at(i) != ')') {
                    tmp += source.at(i);
                    i++;
                }
            }
        }
        tokens.push_back(tmp);
    }
    return tokens;
}

int str_to_int(std::string str) {
    std::istringstream iss(str);
    int f = 0;
    iss >> f;
    return f;
}



// Main class
class Parser {
public:
    Parser(std::string source) {
        this->source = {source, false};
        init();
    };
    Parser(std::string source, bool optimize) {
        if ( optimize ) {
            this->source = CompressCode(source);
        } else {
            this->source = {source, false};
        }
        init();
    };

    virtual ~Parser() {};

    bool verifyLoop () {
        std::stack<uint32_t> loops;
        for (uint32_t i = 0; i < source.value.size(); ++i) {
            if ( source.at(i) == '[' ) {
                loops.push( i );
            } else if ( source.at(i) == ']') {
                if ( loops.empty() ) {
                    DisplayError("Invalid use of loop", i);
                    return false;
                }
                loops.pop();
            }
        }
        if ( !loops.empty() ) {
            DisplayError("Invalid use of loop", loops.top());
            return false;
        }
        return true;
    }

    bool Next() {
        if ( source.isOptimized ) {
            DisplayError("The source has been compressed/optimized. Use \"Run\" instead", 0);
            return false;
        }

        if ( BREAK ) return false;
        if ( cursor == source.value.size() ) {
            BREAK = true;
            return false;
        }

        char token = source.at(cursor);
        if ( instr_map.find(token) == instr_map.end() ) {
            DisplayError("Program stopped", cursor);
            return false;
        } else {
            // printf("%c", token);
            bool all_ok = (this->*instr_map[token]) ();
            if ( !all_ok ) {
                DisplayError("Program stopped", cursor);
                return false;
            }
        }
        ++cursor;
        return true;
    }

    void Run () {
        if ( ! source.isOptimized ) {
            while ( Next() ) /*naa--thing*/;
            return;
        }
        printf("%s\n", source.value.c_str());

        std::vector<std::string> tokens = tokenizeSource(source);
        for (size_t i = 0; i < tokens.size(); i++ ) {
            std::string token = tokens[i];
            printf("%s : ", token.c_str());
            char symbol = token[0];
            if ( token.size() > 1 ) {
                std::string str_number = token.substr(1, token.size());
                int n_times = str_to_int(str_number);
                printf("%c x %d => %d", symbol, n_times, sign[symbol] * n_times);
                if ( symbol == '-' || symbol == '+' ) {
                    ram[ptr] += sign[symbol] * n_times;
                } else {
                    ptr += sign[symbol] * n_times;
                }
            } else {
                if ( symbol == '[' || symbol == ']' ) {
                    if ( symbol == '[') {
                        open_loop_pos.push(i);
                    } else {
                        if ( ram[ptr] > 0 ) {
                            i = open_loop_pos.top();
                        } else {
                            open_loop_pos.pop();
                        }
                    }
                } else {
                    // , or . or single(<>+-)
                    (this->*instr_map[symbol]) ();
                }
            }
            printf("\n");
        }
    }

    // Getters
    uint32_t GetCursor () const
    {
        return cursor;
    }

    uint32_t GetPtr    () const
    {
        return ptr;
    }

    uint8_t GetMemoryAt(uint32_t p) {
        return ram[p];
    }

    char GetCurrentToken () // [NOTE] : not const
    {
        return source.at(cursor);
    }

    std::map<uint32_t, uint32_t> GetMemory() const
    {
        return ram;
    }

    uint32_t GetUsedMemorySize() const
    {
        return ram.size();
    }

    void SetClusterSize(CLUSTER_SIZE c_size)
    {
        clust_size = c_size.value;
    }

private:
    Source      source  = {"", false};
    std::string symbols = ".,+-<>[]#";
    uint32_t    cursor  = 0;
    uint32_t    ptr     = 0;
    bool        BREAK   = false;
    uint8_t clust_size  = 8;

    std::map<uint32_t, uint32_t> ram; // virtual memory
    std::map<char, bool (Brainfuck::Parser::*) () > instr_map; // instruction map
    std::stack<uint32_t> open_loop_pos;

    // instructions
    bool ValDecr  () // -
    {
        ram[ptr]--;
        truncateCluster();
        return true;
    }

    bool ValIncr  () // +
    {
        ram[ptr]++;
        truncateCluster();
        return true;
    }

    bool PtrDecr  () // <
    {
        ptr++;
        return true;
    }

    bool PtrIncr  () // >
    {
        ptr--;
        return true;
    }

    bool ValPrint () // .
    {
        std::cout << (char) (ram[ptr] & 0xFF);
        return true;
    }

    bool Input () // ,
    {
        int value;
        std::cin >> value;
        ram[ptr] = value;
        return true;
    }

    bool LoopOpen () // [
    {
        open_loop_pos.push(cursor);
        return true;
    }

    bool LoopClose() // ]
    {
        if ( ram[ptr] > 0 ) {
            cursor = open_loop_pos.top();
        } else {
            if ( open_loop_pos.empty() ) {
                return false;
            }
            open_loop_pos.pop();
        }
        return true;
    }

    bool Break    () // #
    {
        BREAK = true;
        return true;
    }

    void init () {
        if ( ! verifyLoop() ) BREAK = true;
        instr_map['.'] = &Brainfuck::Parser::ValPrint  ;
        instr_map[','] = &Brainfuck::Parser::Input     ;
        instr_map['+'] = &Brainfuck::Parser::ValIncr   ;
        instr_map['-'] = &Brainfuck::Parser::ValDecr   ;
        instr_map['<'] = &Brainfuck::Parser::PtrDecr   ;
        instr_map['>'] = &Brainfuck::Parser::PtrIncr   ;
        instr_map['['] = &Brainfuck::Parser::LoopOpen  ;
        instr_map[']'] = &Brainfuck::Parser::LoopClose ;
        instr_map['#'] = &Brainfuck::Parser::Break     ;
    }

    void truncateCluster() {
        // (*cluster) %= size_cluster
        switch (clust_size) {
            case  8:
                ram[ptr] &= 0x000000FF;
                break;
            case 16:
                ram[ptr] &= 0x0000FFFF;
                break;
            default:
                // do nothing
                break;
        }
    }

    void DisplayError(std::string error, uint32_t pos) {
        printf("[Error] %s at cursor=%d, char. \"%c\"", error.c_str(), pos, source.at(pos));
    }
};

}
