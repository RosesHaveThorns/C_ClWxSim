$(info .========= Preparing ClWxSim =========.)
CC=gcc
CFLAGS=-g -Wall -lgdi32

DIRS=core ui sim utils

SOURCEDIR:=./src
BUILDDIR:=./obj
DEPSDIR:=./include

IDIRS:=$(foreach dir,$(DIRS),$(addprefix $(DEPSDIR)/,$(dir)))
SDIRS:=$(foreach dir,$(DIRS),$(addprefix $(SOURCEDIR)/,$(dir)))

$(info SOURCE FILES [from $(SDIRS)])
SRC:=$(foreach sdir,$(SDIRS),$(wildcard $(sdir)/*.c))
$(info $(SRC))

$(info )
$(info DEPENDENCIES [from $(IDIRS)])
DEPS:=$(foreach idir,$(IDIRS),$(wildcard $(idir)/*.h))
$(info $(DEPS))

DEPSFLAGS=$(patsubst ./%,-I./%,$(IDIRS))
$(info Directory Flags:)
$(info $(DEPSFLAGS))

_OBJ:=$(patsubst %.c,%.o,$(SRC))
OBJ:=$(patsubst ./src/%,$(BUILDDIR)/%,$(_OBJ))

$(info )
$(info COMPILING [to $(OBJ)])

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.c $(DEPS)
	@echo --- Compiling $@ from $<
	$(CC) -c $< -o $@ $(CFLAGS) $(DEPSFLAGS)

clwxsim: $(OBJ)
	@echo --- Building $@ from $^
	$(CC) $^ -o $@ $(CFLAGS)
