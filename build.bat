rem We define the includes which are not specific to the project to avoid compilation errors
rem Wall means show warnings

g++ -std=c++14 -Wall -c *.hpp
g++ -std=c++14 -Wall -c *.cpp

rem We define the result of the build + obj files to link + the libraries included in \ lib
g++ -std=c++14 -o ".\bin\afbf" *.o

rem starts the application
.\bin\afbf
