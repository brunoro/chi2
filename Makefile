CC = gcc 
CFLAGS = -Wall -g -lm -O3
RM = rm -f
SRC = $(wildcard src/*.c)
OBJS = $(SRC:.c=.o)
MAIN = chi2 
RUN_ARGS = -i test/0.arff -o /dev/stdout -c comp.sys -n 80 

ifeq ($(OSTYPE), darwin10.0)
	CFLAGS := -fnested-functions
endif

all: $(MAIN)

$(MAIN): $(OBJS)
	@echo " :: linkando"
	@$(CC) $(CFLAGS) $^ -o $@

%.o: src/%.c
	@echo " :: compilando \"$@\""
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
