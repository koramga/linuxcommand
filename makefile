command_exe : INIT TEST CHAPTER

INIT : init.c
	gcc -Wall -W -O2 -lrt -g -o init init.c queue.c

TEST : test.c
	gcc -o test test.c

CHAPTER : chapter08.c
	gcc -o chapter08 chapter08.c

clean :
	rm test
	rm init
	rm chapter08
