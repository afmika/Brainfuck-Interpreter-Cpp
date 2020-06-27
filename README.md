# Brainfuck-Interpreter-Cpp
```
A simple optimized Brainfuck interpreter written in C++14

Commands :

info | -i : Information    [arg :    none ]
test | -t : Run tests      [arg :    none ]
run  | -r : Execute a file [arg : filepath]
```
[[ Basic use of the run command ]]
```
Example :
$ afbf run myfile.bf
```
[[ Advanced uses of the run command ]]

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
You can combine the two latest mode along with the run command
Examples:
$ afbf run io=ic memory=8 myfile.bf
$ afbf run memory=16 io=ii myfile.bf
$ afbf run memory=16 io=ci myfile.bf
```
