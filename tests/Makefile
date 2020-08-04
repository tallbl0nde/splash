# Default target is 'compile' (compiles all test related files)
.DEFAULT_GOAL := compile

# Variables for file + output locations
BUILD		:=	build
OBJDIR		:=	build/objs
DEPDIR		:=	build/deps
EXE			:=  run-tests
INCLUDE		:=	include ../include
SOURCE		:=	source
LIBDIR		:=	../lib
LIBNAME		:=	Splash

# Flags to pass to the compiler
CXXFLAGS	:=	-std=c++11 -Wall -O3 $(foreach dir, $(INCLUDE), -I$(dir))

# Variables which store file locations
CPPFILES 	:= $(shell find $(SOURCE)/ -name "*.cpp")
OBJS     	:= $(CPPFILES:$(SOURCE)/%.cpp=$(OBJDIR)/%.o)
DEPS     	:= $(CPPFILES:$(SOURCE)/%.cpp=$(DEPDIR)/%.d)
TREE     	:= $(sort $(patsubst %/,%,$(dir $(OBJS))))
LIB			:= $(LIBDIR)/lib$(LIBNAME).a

# Include dependency files if they already exist
ifeq "$(MAKECMDGOALS)" ""
-include $(DEPS)
endif

# Define virtual make targets
.PHONY: compile run clean

# 'compile' compiles all related files for testing
compile: $(EXE)
$(EXE): $(LIB) $(OBJS)
	@echo "Compiling test executable..."
	@$(CXX) -o $(EXE) $(OBJS) -L$(LIBDIR) -l$(LIBNAME)

# 'run' runs the tests (and compiles the executable if necessary)
run: compile
	@echo "Running all tests..."
	@$(CURDIR)/$(EXE)

# 'clean' removes all build files
clean:
	@echo "Removing test build files..."
	@rm -rf $(BUILD) $(EXE)

# Compiles each object file
.SECONDEXPANSION:
$(OBJDIR)/%.o: $(SOURCE)/%.cpp | $$(@D)
	@echo Compiling $*.o...
	@$(CXX) -MMD -MP -MF $(@:$(OBJDIR)/%.o=$(DEPDIR)/%.d) $(CXXFLAGS) -o $@ -c $<

# Creates a directory for each object/dependency file
$(TREE): %:
	@mkdir -p $@
	@mkdir -p $(@:$(OBJDIR)%=$(DEPDIR)%)