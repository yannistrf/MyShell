# our compiler
CC = gcc

# where to look for header files
INCLUDE = ./include

# compiler flags
CFLAGS = -I$(INCLUDE) -Wall -g #-Werror

# linker flags
LDFLAGS = -g

# executable name
EXEC = mysh

# binary folder
BIN = ./bin

# source code folder
SRC = ./src

# all of our object files
OBJS = 	$(BIN)/main.o $(BIN)/shell.o $(BIN)/parser.o $(BIN)/util.o \
		$(BIN)/sys_cmd.o $(BIN)/pipes.o $(BIN)/proc.o $(BIN)/alias.o \
		$(BIN)/history.o $(BIN)/sig.o $(BIN)/wildchar.o $(BIN)/help.o

# rule to build executable
$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) $(LDFLAGS)

# rule to build our object files
$(BIN)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $^ -o $@

# run executable
run: $(EXEC)
	./$(EXEC) $(ARGS)

# clean all binary files
clean:
	rm -f $(OBJS) $(EXEC)

val: $(EXEC)
	valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(EXEC)