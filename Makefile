.PHONY: clean test

test:
	clang++ test/ft.cpp -I include -I ../boost/libs/di/include/ -I. -std=c++1y -Wall -Wextra -pedantic -fno-exceptions -Werror -pedantic-errors -include test/common/test.hpp && ./a.out
	@echo ---------
	g++ test/ft.cpp -I include -I ../boost/libs/di/include/ -I. -std=c++1y -Wall -Wextra -pedantic -fno-exceptions -Werror -pedantic-errors -include test/common/test.hpp && ./a.out

all:
	time clang++ test/pt.cpp -I include -I ../boost/libs/di/include/ -I. -std=c++1y -Wall -Wextra -pedantic -fno-exceptions -Werror -pedantic-errors -include test/common/test.hpp && ./a.out
	@echo ---------
	time g++ test/pt.cpp -I include -I ../boost/libs/di/include/ -I. -std=c++1y -Wall -Wextra -pedantic -fno-exceptions -Werror -pedantic-errors -include test/common/test.hpp && ./a.out

clean:
	rm -f a.out
