# our compiler
CC = gcc

# where to look for header files
INCLUDE = ./include

# compiler flags
CFLAGS = -I$(INCLUDE) -Wall -Werror

# linker flags
LDFLAGS = 

# executable name
EXEC = mysh

# binary folder
BIN = ./bin

# source code folder
SRC = ./src

# all of our object files
OBJS = $(BIN)/hello.o $(BIN)/foo.o

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