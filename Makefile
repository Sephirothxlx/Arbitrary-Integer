arb_test:arb_test.c arb_int.c arb_int.h
	gcc -std=c11 -Wall -Werror -pedantic -o arb_test arb_test.c arb_int.c

check:arb_int.c arb_int.h arb_unittest.c
	gcc -std=c11 -Wall -Werror -pedantic -o arb_unittest arb_int.c arb_unittest.c -lcunit -lgmp

clean:
	rm -f *.o arb_test arb_unittest
