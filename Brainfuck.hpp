/*
 * @author afmika
 * Brainfuck interpreter
 * June 2020
 **/

#pragma once
#include <bits/stdc++.h>

namespace Brainfuck {
typedef struct CLUSTER_SIZE {
    int value;
    CLUSTER_SIZE(int value) : value(value) {}
} CLUSTER_SIZE;
const CLUSTER_SIZE ONE_BYTE   = { 8};
const CLUSTER_SIZE TWO_BYTES  = {16};
const CLUSTER_SIZE FOUR_BYTES = {32};


class Parser {
public:
    Parser(std::string source) {
        this->source = source;
        init();
    };

    Parser(const char * source) {
        std::string tmp(source);
        this->source = tmp;
        init();
    }

    virtual ~Parser() {};

    bool verifyLoop () {
        std::stack<uint32_t> loops;
        for (uint32_t i = 0; i < source.size(); ++i) {
            if ( source[i] == '[' ) {
                loops.push( i );
            } else if ( source[i] == ']') {
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
        if ( BREAK ) return false;
        if ( cursor == source.size() ) {
            BREAK = true;
            return false;
        }

        char token = source[cursor];
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

    char GetCurrentToken () const
    {
        return source[cursor];
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
    std::string source  = "";
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
        // ".,+-<>[]#"
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

	void truncateCluster()
	{
        // (*cluster) %= size_cluster
		switch (clust_size) {
			case 8:
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
        printf("[Error] %s at cursor=%d, char. \"%c\"", error.c_str(), pos, source[pos]);
    }
};

}
