PROGRAM_OBJ=server1.o client1.o
PROGRAM_OUT=server1 client1
PROGRAM_C=server1.c client1.c

program: $(PROGRAM_OUT)
	@echo done!

server1:
	gcc -Wall -Wextra --std=gnu99 -pedantic -Werror server1.c -o server1 

client1:
	gcc -Wall -Wextra --std=gnu99 -pedantic -Werror client1.c -o client1

clean:
	rm -f $(PROGRAM_OUT)
