CC = clang++
WARNINGS = -Weverything -Wpedantic -Wno-c++98-compat -Wno-old-style-cast -Wno-unused-parameter -Wno-padded -Werror
DEBUG_CFLAGS = -g $(WARNINGS) -O0
RELEASE_CFLAGS = -g $(WARNINGS) -Ofast -flto -fomit-frame-pointer -fno-rtti -ffast-math -ffp-model=fast -ffp-contract=fast -funsafe-math-optimizations -freciprocal-math -ffinite-math-only -fno-trapping-math -fno-math-errno -fveclib=libmvec -fno-signed-zeros -fno-complete-member-pointers -ffunction-sections -march=x86-64-v3
STD = -std=c++20
SRC_DIR := ./src/
LIBS_DIR := ./libs/
SRCS := $(wildcard $(SRC_DIR)*.cpp) $(wildcard $(SRC_DIR)**/*.cpp)
INCLUDE = -I"$(LIBS_DIR)"
LINK_FLAGS = -lSDL2 -lGLEW -lGL
OUTDIR = ./bin/
DEBUG_OBJ_NAME = $(OUTDIR)gltest-debug
RELEASE_OBJ_NAME = $(OUTDIR)gltest-release

debug: CFLAGS = $(DEBUG_CFLAGS)
debug: OBJ_NAME = $(DEBUG_OBJ_NAME)
debug: build

release: CFLAGS = $(RELEASE_CFLAGS)
release: OBJ_NAME = $(RELEASE_OBJ_NAME)
release: build

build:
	$(CC) $(CFLAGS) $(STD) $(INCLUDE) $(SRCS) $(LINK_FLAGS) -o $(OBJ_NAME)

run-debug: BUILD_TYPE = $(DEBUG_OBJ_NAME)
run-debug:
	$(BUILD_TYPE)

run-release: BUILD_TYPE = $(RELEASE_OBJ_NAME)
run-release:
	$(BUILD_TYPE)

clean:
	rm $(DEBUG_OBJ_NAME) $(RELEASE_OBJ_NAME)
