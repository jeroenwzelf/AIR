# This makefile compiles sources from multiple subdirectories
# Highly inspired by Manzill0,
# https://stackoverflow.com/questions/231229/how-to-generate-a-makefile-with-source-in-sub-directories-using-just-one-makefil

CC=g++-7
CPPFLAGS=-Wall -ggdb `sdl-config --cflags` -std=c++14
LDFLAGS=`sdl-config --libs` -Llib -lcurlpp -lcurl -ljsoncpp

MODULES   := Composer Performer RadioHost
SRC_DIR   := $(addprefix src/,$(MODULES))
BUILD_DIR := $(addprefix build/,$(MODULES))

SRC       := $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.cpp))
OBJ       := $(patsubst src/%.cpp,build/%.o,$(SRC))
INCLUDES  := $(addprefix -I,$(SRC_DIR))

vpath %.cpp $(SRC_DIR)

define make-goal
$1/%.o: %.cpp
	$(CC) $(INCLUDES) -c $$< $(CPPFLAGS) -o $$@
endef

.PHONY: all checkdirs clean

MKDIR_P = mkdir -p

.PHONY: directories

all: directories AIR

directories: $(BUILD_DIR)

${OUT_DIR}:
		${MKDIR_P} ${OUT_DIR}

AIR: $(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

checkdirs: $(BUILD_DIR)

$(BUILD_DIR):
	@mkdir -p $@

clean:
	@rm -rf $(BUILD_DIR)

$(foreach bdir,$(BUILD_DIR),$(eval $(call make-goal,$(bdir))))
