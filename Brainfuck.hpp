/*
 * @author afmika
 * Brainfuck interpreter
 * June 2020
 **/

#pragma once
#include <bits/stdc++.h>

class Brainfuck {
public:
    Brainfuck(std::string source) {
        this->source = source;
        init();
    };

    Brainfuck(const char * source) {
        std::string tmp(source);
        this->source = tmp;
        init();
    }

    virtual ~Brainfuck() {};

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

    std::map<uint32_t, uint8_t> GetMemory() const
    {
        return ram;
    }

    uint32_t GetUsedMemorySize() const
    {
        return ram.size();
    }

private:
    std::string source  = "";
    std::string symbols = ".,+-<>[]#";
    uint32_t    cursor  = 0;
    uint32_t    ptr     = 0;
    bool        BREAK   = false;

    std::map<uint32_t, uint8_t> ram; // virtual memory
    std::map<char, bool (Brainfuck::*) () > instr_map; // instruction map
    std::stack<uint32_t> open_loop_pos;

    // instructions
    bool ValDecr  () // -
    {
        ram[ptr]--;
        return true;
    }
    bool ValIncr  () // +
    {
        ram[ptr]++;
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
        std::cout << ram[ptr];
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
        instr_map['.'] = &Brainfuck::ValPrint  ;
        instr_map[','] = &Brainfuck::Input     ;
        instr_map['+'] = &Brainfuck::ValIncr   ;
        instr_map['-'] = &Brainfuck::ValDecr   ;
        instr_map['<'] = &Brainfuck::PtrDecr   ;
        instr_map['>'] = &Brainfuck::PtrIncr   ;
        instr_map['['] = &Brainfuck::LoopOpen  ;
        instr_map[']'] = &Brainfuck::LoopClose ;
        instr_map['#'] = &Brainfuck::Break     ;
    }

    void DisplayError(std::string error, uint32_t pos) {
        printf("[Error] %s at cursor=%d, char. \"%c\"", error.c_str(), pos, source[pos]);
    }
};
