#!/bin/sh
g++-10 -Wall -Wextra -Wpedantic -DOP="*" test.cpp -o test && echo COMPILED with "*" && ./test
g++-10 -Wall -Wextra -Wpedantic -DOP="/" test.cpp -o test && echo COMPILED with / && ./test
g++-10 -Wall -Wextra -Wpedantic -DOP="-" test.cpp -o test && echo COMPILED with - && ./test
g++-10 -Wall -Wextra -Wpedantic -DOP="+" test.cpp -o test && echo COMPILED with + && ./test
