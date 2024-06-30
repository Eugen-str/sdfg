LIBS=-lraylib
SRC=src/main.c src/map.c src/player.c src/collision.c

BUILD_DIR=build
OUT_DIR=$(BUILD_DIR)
OUT_NAME=sdfg

sdfg: $(SRC) | $(BUILD_DIR)
	gcc $(SRC) -o $(OUT_DIR)/$(OUT_NAME) $(LIBS)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
