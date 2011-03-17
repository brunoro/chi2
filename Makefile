CC = gcc 
CFLAGS = -Wall -g -lm -O3
RM = rm -f
SRC = $(wildcard src/*.c)
OBJS = $(SRC:.c=.o)
MAIN = chi2 
RUN_ARGS = -i test/0.arff -o test/test.txt -c comp.sys -n 80 -s . -r

all: $(MAIN)

$(MAIN): $(OBJS)
	@$(CC) $(CFLAGS) $^ -o $@

%.o: src/%.c
	@$(CC) $(CFLAGS) $< -c

clean:
	$(RM) $(MAIN) src/*.o
	clear

run: $(MAIN)
	./$(MAIN) $(RUN_ARGS) 

valgrind: $(MAIN)
	valgrind ./$(MAIN) $(RUN_ARGS)

gdb: $(MAIN)
	echo "run $(RUN_ARGS)" > "/tmp/temp_args"
	gdb $(MAIN) < "/tmp/temp_args"
	rm "/tmp/temp_args"
