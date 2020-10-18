IDIRS=./include/ui ./include/core
ODIR=./obj
SDIRS=./src/ui
CC=gcc
CFLAGS=-g -Wall

SRC=$(wildcard $(SDIRS)/*.c)

DEPS=$(wildcard $(IDIRS)/*.h)

DEPSFLAGS=$(patsubst ./%,-I./%,$(IDIRS))

_OBJ=$(SRC:.c=.o)
OBJ=$(patsubst ./src/%,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SRC) $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(DEPSFLAGS)

clwxsim: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
