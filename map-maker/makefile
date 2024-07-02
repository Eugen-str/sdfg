LIBS=-lraylib -lm
SRC=src/main.c

BUILD_DIR=build
MAPS_DIR=saved_maps
OUT_DIR=$(BUILD_DIR)
OUT_NAME=sdfg-map-maker

sdfg: $(SRC) | $(BUILD_DIR)
	gcc $(SRC) -o $(OUT_DIR)/$(OUT_NAME) $(LINK) $(LIBS)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	mkdir -p $(MAPS_DIR)
