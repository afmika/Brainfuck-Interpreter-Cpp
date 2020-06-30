# Brainfuck-Interpreter-Cpp
```
A simple optimized Brainfuck interpreter written in C++14

Commands :

info | -i : Information    [arg :    none           ]
test | -t : Run tests      [arg :    none           ]
run  | -r : Execute a file [arg : [options] filepath]
```
[[ Basic use of the run command ]]
```
Example :
$ afbf run myfile.bf
```
[[ Advanced uses of the run command ]]

## Debug mode with the debug/-d
```
[usage] $ afbf run debug=value myfile.bf
[usage] $ afbf run debug myfile.bf
[!] value can be replaced by print, end or step
        * step  : debug at each instruction
        * print : debug each time the print instruction is executed
        * end   : display the final memory state after the program execution

Examples:
$ afbf run -d myfile.bf
$ afbf run debug myfile.bf
$ afbf run -d=print myfile.bf
$ afbf run debug=end myfile.bf
```

## Show memory dump with dump=mem_limit
```
[usage] $ afbf run dump=size myfile.bf
[usage] $ afbf run dump myfile.bf

Examples:
$ afbf run dump myfile.bf
$ afbf run dump=128 myfile.bf
```

## Customized cluster size (8, 16 or 32)
```
[usage] $ afbf run memory=size myfile.bf

Example :
afbf run memory=8 myfile.bf
```
## Customized input/output mode ('c' : char or 'i' : integer)
```
[usage] $ afbf run io=xy myfile.bf
[!] x represents the input mode, y represents the output mode
[!] x and y can be replaced by c or i

Examples:
1. integer as input, char as output
$ afbf run io=ic myfile.bf
2. char as input, char as output
$ afbf run io=cc myfile.bf
```
## Combined version
```
You can combine all of them along with the run command
Examples:
$ afbf run io=ic memory=8 myfile.bf
$ afbf run memory=16 io=ii myfile.bf
$ afbf run memory=16 io=ci myfile.bf
$ afbf run memory=16 io=ci debug myfile.bf
$ afbf run debug memory=32 io=ci myfile.bf
$ afbf run -d=print io=ci myfile.bf
$ afbf run debug=step io=ci myfile.bf
```
