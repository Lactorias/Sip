CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -g -Iinclude
LDFLAGS = 

SRC_DIR = src
SOURCES = $(SRC_DIR)/errlog.cpp \
          $(SRC_DIR)/scanner.cpp \
          $(SRC_DIR)/token.cpp \
		  $(SRC_DIR)/parser.cpp \
		  $(SRC_DIR)/ast_printer.cpp \
		  $(SRC_DIR)/interpreter.cpp \
		  $(SRC_DIR)/runtimeerror.cpp \
		  



OBJ_DIR = build
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

# To add a new target::
#
#
# 1. Add the `.cpp` file name on the RHS
MAIN = main
GENERATE = mainAST
# AST_PRINTER = ast_printer
INSPECT = inspect

# 2. Make a name for your target file 
MAIN_TARGET = compiler
GENERATE_TARGET = generate
# AST_PRINTER_TARGET = printer

# 3. Add that target to the `all` rule
all:$(OBJ_DIR) $(MAIN_TARGET) $(GENERATE_TARGET) $(INSPECT_TARGET)
#$(AST_PRINTER_TARGET)

# obj files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# check if this works
# 4. Add a rule for the exe's object file
$(OBJ_DIR)/$(MAIN).o: $(SRC_DIR)/$(MAIN).cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/$(GENERATE).o: $(SRC_DIR)/$(GENERATE).cpp
	$(CXX) $(CXXFLAGS) -IoutputTests -c $< -o $@

#$(OBJ_DIR)/$(AST_PRINTER).o: $(SRC_DIR)/$(AST_PRINTER).cpp
#	$(CXX) $(CXXFLAGS) -IoutputTests -c $< -o $@

# Link all targets into a final exec
# $@: The target name
# $<: The first prerequisite
# $^: All prerequisites
# 5. Add a rule to link sources to the target
$(MAIN_TARGET): $(OBJECTS) $(OBJ_DIR)/$(MAIN).o
	$(CXX) $(LDFLAGS) $^ -o $@ 

$(GENERATE_TARGET): $(OBJECTS) $(OBJ_DIR)/$(GENERATE).o
	$(CXX) $(LDFLAGS) $^ -o $@

#$(AST_PRINTER_TARGET): $(OBJECTS) $(OBJ_DIR)/$(AST_PRINTER).o  
#	$(CXX) $(LDFLAGS) $^ -o $@ 

############################################################################

# make `/build` if it dont exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# clean
clean:
	rm -rf $(OBJ_DIR) $(MAIN_TARGET) $(AST_PRINTER_TARGET) $(GENERATE_TARGET)

.PHONY: all clean