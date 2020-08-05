# Default target is 'help' (lists valid options/rules)
.DEFAULT_GOAL := help

# Variables for file + output locations
TARGET		:=  Splash
BUILD		:=	build
LIB			:=	lib
OUTPUT		:=	$(LIB)/lib$(TARGET).a
OBJDIR		:=	build/objs
DEPDIR		:=	build/deps
INCLUDE		:=	include
SOURCE		:=	source

# Flags to pass to the compiler
CXXFLAGS	:=	-std=c++11 -Wall -O3 -I$(INCLUDE)

# Variables which store file locations
CPPFILES	:=	$(shell find $(SOURCE)/ -name "*.cpp")
OBJS		:=	$(CPPFILES:$(SOURCE)/%.cpp=$(OBJDIR)/%.o)
DEPS		:=	$(CPPFILES:$(SOURCE)/%.cpp=$(DEPDIR)/%.d)
TREE		:=	$(sort $(patsubst %/,%,$(dir $(OBJS))))

# Include dependency files if they already exist
ifeq "$(MAKECMDGOALS)" ""
-include $(DEPS)
endif

# Define virtual make targets
.PHONY: all clean-all library clean-library tests clean-tests run-tests help

# 'help' displays the available targets
help:
	@echo "----------------------------------------------------------------"
	@echo "The following targets are available:"
	@echo "----------------------------------------------------------------"
	@echo "all: compile the library, tests and run the tests"
	@echo "library: compile the library"
	@echo "tests: compile (but do not run) the test cases"
	@echo "run-tests: run (and compile if necessary) the test cases"
	@echo "----------------------------------------------------------------"
	@echo "clean-all: clean all build files"
	@echo "clean-library: clean library build files"
	@echo "clean-tests: clean test build files"
	@echo "----------------------------------------------------------------"

# 'all' compiles the library, tests and runs the tests
all: run-tests

# 'library' compiles the library
library: $(OUTPUT)

# 'tests' compiles the tests (in other Makefile)
tests:
	@$(MAKE) -s -C tests/ compile

# 'run-tests' compiles and runs the tests (in other Makefile)
run-tests: library
	@$(MAKE) -s -C tests/ run

# 'clean-all' removes all build files
clean-all: clean-library clean-tests

# 'clean-library' only removes library related build files
clean-library:
	@echo "Removing library build files..."
	@rm -rf $(BUILD) $(LIB)

# 'clean-tests' only removes test related build files (in other Makefile)
clean-tests:
	@$(MAKE) -s -C tests/ clean

# Compiles each object file
.SECONDEXPANSION:
$(OBJDIR)/%.o: $(SOURCE)/%.cpp | $$(@D)
	@echo Compiling $*.o...
	@$(CXX) -MMD -MP -MF $(@:$(OBJDIR)/%.o=$(DEPDIR)/%.d) $(CXXFLAGS) -o $@ -c $<

# Builds the library archive from all .o files
$(OUTPUT): $(OBJS)
	@[ -d $(LIB) ] || mkdir -p $(LIB)
	@rm -rf $(OUTPUT)
	@echo Creating $(TARGET) library archive at $(OUTPUT)...
	@$(AR) -rc $(OUTPUT) $(OBJS)

# Creates a directory for each object/dependency file
$(TREE): %:
	@mkdir -p $@
	@mkdir -p $(@:$(OBJDIR)%=$(DEPDIR)%)