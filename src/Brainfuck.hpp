/*
 * @author afmika
 * Brainfuck interpreter
 * June 2020
 **/

#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <stack>
#include <cassert>
#include <sstream>

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

// constants
const CLUSTER_SIZE ONE_BYTE   = { 8};
const CLUSTER_SIZE TWO_BYTES  = {16};
const CLUSTER_SIZE FOUR_BYTES = {32};
const std::string symbols = ".,+-<>[]#";

enum IO_MODE {
    CHAR, INT // INPUT/OUTPUT MODE
};

std::map<uint8_t, uint32_t> UINT_MASK {
    {  8, 0x000000FF}, // 1 x FF
    { 16, 0x0000FFFF}, // 2 x FF
    { 32, 0xFFFFFFFF}  // 4 x FF
};

std::map<char, int> sign = {
    {'-', -1}, {'+', 1},
    {'<', -1}, {'>', 1}
};


// helpers

int str_to_int(std::string str) {
    std::istringstream iss(str);
    int f = 0;
    iss >> f;
    return f;
}

bool isValidSymbol(char c) {
    bool found = false;
    for (auto symbol : symbols) found = found || (symbol == c );
    return found;
}

std::string removeInvalidSymbols(std::string source) {
    std::string result = "";
    for (char c : source) {
        if ( isValidSymbol(c) ) {
            result += c;
        }
    }
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
            i++;
            while (source.at(i) != ')') {
                tmp += source.at(i);
                i++;
            }
        }
        tokens.push_back(tmp);
    }
    return tokens;
}

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
                // => same sign
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

Source UncompressCode(Source source) {
    std::vector<std::string> tokens = tokenizeSource(source);
    Source result("", false);
    for (size_t i = 0; i < tokens.size(); i++ ) {
        std::string token = tokens[i];
        char symbol = token[0];
        if ( token.size() > 1 ) {
            std::string str_number = token.substr(1, token.size());
            int n_times = str_to_int(str_number);
            while ( n_times > 0 ) {
                result.value += symbol;
                n_times--;
            }
        } else {
            result.value += symbol;
        }
    }
    return result;
}

// adv optimizations
// detects [+] and [-]
/*
int detectClearLoop (std::vector<string> tokens, int pos ) {
    int init_pos = pos;
    std::string type_a = "[+]";
    std::string type_b = "[-]";
    if ( pos + 3 < (int) tokens.size() ) {
        for (char c : type_a) {
            std::string current(c);
            if ( tokens[pos].compare(current) != 0 ) { found = false; break; }
            pos++;
            count = 0;
        }
        if ( found ) return pos;
        pos = init_pos;
        for (char c : type_b) {
            std::string current(c);
            if ( tokens[pos].compare(current) != 0 ) { found = false; break; }
            pos++;
        }
        if ( found ) return pos;
    }
    return -1;
}
*/

std::string NumberFormat(std::string base, uint32_t value) {
    static char str[100];
    sprintf(str, base.c_str(), value);
    std::string res(str);
    return str;
}

// Main class
class Parser {
public:
    Parser(std::string source) {
        source = removeInvalidSymbols(source);
        this->source = {source, false};
        tokens = tokenizeSource(this->source);
        init();
    };
    Parser(std::string source, bool optimize) {
        source = removeInvalidSymbols(source);
        if ( optimize ) {
            this->source = CompressCode(source);
        } else {
            this->source = {source, false};
        }
        tokens = tokenizeSource(this->source);
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

    bool Next () {
        if ( BREAK ) return false;
        if ( source.isOptimized ) {
            return RunTokens ();
        }
        return RunAsString ();
    }

    void Run () {
        while ( Next() ) /*naa--thing*/;
    }

    // Getters
    Source GetSource () const {
        return source;
    }

    uint32_t GetCursor () const
    {
        return cursor;
    }

    uint32_t GetPtr    () const
    {
        return ptr;
    }


    std::string GetCurrentToken () // [NOTE] : not const
    {
        return tokens[cursor];
    }

    std::string GetTokenAt (int pos) // [NOTE] : not const
    {
        return tokens[pos];
    }

    std::vector<uint32_t> GetMemory () const
    {
        return *ram;
    }

    uint8_t GetMemoryAt (uint32_t p) {
        assert(p >= 0 && p < total_mem);
        return (*ram)[p];
    }

    std::string GetMemoryIntervalAsString (int min_val, int max_val) // [NOTE] int can be negative
    {
        std::string result = "";
        for (int i = min_val; i <= max_val; i++) {
            int pos = i & UINT_MASK[clust_size];
            std::string cur = NumberFormat("%03d", (i < 0 ? 0 : GetMemoryAt(i)) );
			if ( pos == ptr ) cur = "[" + cur + "]";
            result += " " + cur + " ";
        }
        return result;
    }

    std::string GetMemoryDumpAsString (uint32_t limit) {
        std::string result = "";
        // 00 - 11 : m1 m2 ..... m12
        int interval = 12;
        uint32_t begin = 0, end = interval - 1;
        for (uint32_t i = 0; i <= std::min(limit, GetMemorySize()); i++) {
            if ( i % interval == 0 ) {
                static char temp[200];
                sprintf(temp, "\n%05d - %05d | ", begin, end);
                std::string t(temp);
                result += t;
                begin += interval;
                end += interval;
            }
            std::string cur = NumberFormat("%03d", GetMemoryAt(i) );
            if ( i == ptr ) result += "[" + cur + "]";
            else result += " " + cur + " ";
        }
        return result;
    }

    uint32_t GetMemorySize () const
    {
        return total_mem;
    }


    // Setters
    void SetClusterSize (CLUSTER_SIZE c_size)
    {
        clust_size = c_size.value;
    }

    void SetOutputMode (IO_MODE m) {
        OUTPUT = m;
    }
    void SetInputMode (IO_MODE m) {
        INPUT  = m;
    }

private:
    Source      source  = {"", false};
    std::vector<std::string> tokens;
    uint8_t OUTPUT      = IO_MODE::CHAR;
    uint8_t INPUT       = IO_MODE::CHAR;

    uint32_t    cursor  = 0;
    bool        BREAK   = false;
    uint8_t clust_size  = 8;
    int       total_mem = 30000;
    int       ptr       = 0;

    std::vector<uint32_t>* ram = new std::vector<uint32_t>(total_mem, 0); // virtual memory
    std::map<char, bool (Brainfuck::Parser::*) (uint32_t) > instr_map; // instruction map
    std::stack<uint32_t> open_loop_pos;

    // instructions
    bool ValDecr  (uint32_t value) // -
    {
        assert(ptr >= 0 && ptr < total_mem);
        (*ram)[ptr] -= value;
        truncateCluster();
        return true;
    }

    bool ValIncr  (uint32_t value) // +
    {
        assert(ptr >= 0 && ptr < total_mem);
        (*ram)[ptr] += value;
        truncateCluster();
        return true;
    }

    bool PtrDecr  (uint32_t value) // <
    {
        ptr -= value;
        if (ptr < 0)
            ptr += total_mem;
        truncatePtr();
        return true;
    }

    bool PtrIncr  (uint32_t value) // >
    {
        ptr += value;
        if (ptr >= total_mem) ptr %= total_mem;
        truncatePtr();
        return true;
    }

    bool ValPrint (uint32_t value) // .
    {
        assert(ptr >= 0 && ptr < total_mem);
        if ( OUTPUT == IO_MODE::CHAR ) {
			printf("%c", (*ram)[ptr]);
        } else {
			printf("%d", (*ram)[ptr]);
        }
        return true;
    }

    bool Input (uint32_t value) // ,
    {
        assert(ptr >= 0 && ptr < total_mem);
        if ( INPUT == IO_MODE::INT ) {
            int val;
            std::cin >> val;
            (*ram)[ptr] = val;
        } else {
            char charact;
            std::cin >> charact;
            (*ram)[ptr] = (uint32_t) charact;
        }
        return true;
    }

    bool LoopOpen (uint32_t value) // [
    {
        open_loop_pos.push(cursor);
        return true;
    }

    bool LoopClose(uint32_t value) // ]
    {
        assert(ptr >= 0 && ptr < total_mem);
        if ( (*ram)[ptr] != 0 ) {
            cursor = open_loop_pos.top();
        } else {
            if ( open_loop_pos.empty() ) {
                return false;
            }
            open_loop_pos.pop();
        }
        return true;
    }

    bool Break    (uint32_t value) // #
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
        assert(ptr >= 0 && ptr < total_mem);
        (*ram)[ptr] &= UINT_MASK[ clust_size ];
    }

    void truncatePtr() {
        // (cluster) %= size_cluster
        ptr         &= UINT_MASK[ clust_size ];
    }


    bool RunAsString () {
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
            bool all_ok = (this->*instr_map[token]) ( 1 );
            if ( !all_ok ) {
                DisplayError("Program stopped", cursor);
                return false;
            }
        }
        ++cursor;
        return true;
    }

    bool RunTokens () {
        if ( cursor == tokens.size() ) {
            BREAK = true;
            return false;
        }
        bool all_ok = false;
        std::string token = tokens[cursor];
        char symbol = token[0];
        if ( token.size() == 1 ) {
            if (  instr_map.find(symbol) == instr_map.end() ) {
                DisplayError("Program stopped", cursor);
                return false;
            } else {
                all_ok = (this->*instr_map[symbol]) ( 1 );
            }
        } else {
            std::string str_number = token.substr(1, token.size());
            int n_times = str_to_int(str_number);
            // printf("%c x %d => %d\n", symbol, n_times, sign[symbol] * n_times);
            all_ok =  (this->*instr_map[symbol]) ( n_times );
        }
        if ( !all_ok ) {
            DisplayError("Program stopped", cursor);
            return false;
        }
        ++cursor;
        return true;
    }

    void DisplayError(std::string error, int pos) {
        printf("[Error] %s at cursor=%d, token = \"%s\"", error.c_str(), pos, GetTokenAt(pos).c_str() );
    }
};

}
