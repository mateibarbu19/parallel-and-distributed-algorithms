JC = javac
JFLAGS = # -g
BIN_DIR = bin

# Packages we should compile
PACKAGES = \
	mapReduce \
	data \
	utils

# All the (x)java files in a package
PACKAGE_SRC	= $(shell find $(PACKAGES) \( -name '*.java' \) )

# All the classes to build in a package
PACKAGE_OBJS = $(patsubst %.java,$(BIN_DIR)/%.class,$(PACKAGE_SRC))

# All the .java source files:
JAVA_SRC := $(wildcard *.java)

JAVA_OBJS := $(patsubst %.java,$(BIN_DIR)/%.class,$(JAVA_SRC))

$(BIN_DIR)/%.class: %.java
	$(JC) $(JFLAGS) -d $(BIN_DIR) $<

build: $(PACKAGE_OBJS) $(JAVA_OBJS)
	@(echo "Build finised.")

internal_test/output.txt: build
	java -cp $(BIN_DIR) Tema2 3 internal_test/input.txt internal_test/output.txt

run: internal_test/output.txt
	@(echo "The program halted.")

#package targets
$(PACKAGES) : % : $(call PACKAGE_OBJS,%)

clean:
	rm -rf $(BIN_DIR)
	# find . -name '*.class' -exec rm -f {} \;
