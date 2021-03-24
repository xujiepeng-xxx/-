CC=gcc
STD=-std=gnu99
FLAG=-Wall -Werror
BIN=open destory unlock login query save take transfer repass
TARGE=server client
TOOLS=msgq.c tools.c

all:$(BIN) $(TARGE)
	mv $(BIN) ./bin/
	./server
	
%:%.c tools.c tools.h msgq.c msgq.h
	$(CC) $(STD) $(FLAG) $(TOOLS) $< -o $@
	
clean:
	cd ./bin/ && rm -rf $(BIN) 
	rm -rf *.o 
	rm -rf *.h.gch 
	rm -rf *~
