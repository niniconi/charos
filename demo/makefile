build:
	gcc -o ptmxtest ptmxtest.c
	mkfifo /tmp/p
	nc -l 2333 </tmp/p | ./ptmxtest >/tmp/p
clean:
	rm ptmxtest -f

