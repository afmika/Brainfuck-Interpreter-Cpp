# We define the includes which are not specific to the project to avoid compilation errors
# Wall means show warnings

g++ -std=c++14 -Wall -c src/*.hpp
g++ -std=c++14 -Wall -c src/*.cpp

# We define the result of the build + obj files to link + the libraries included in / lib
g++ -std=c++14 -o "./bin/afbf" *.o

# starts the application
./bin/afbf
