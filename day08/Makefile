CC := gcc
CFLAGS := -g -c -Wall
LFLAGS := 
BUILD_DIR := ./out
SRC_DIR := ./src
OBJ_DIR := ./obj
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))
TARGET	:= day08.out

$(BUILD_DIR)/$(TARGET): ../lib/common.o $(OBJ_FILES)
	@mkdir -p $(@D)
	$(CC) $(LFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -std=c18 -c -o $@ $<

clean:
	rm -f $(OBJS) $(OUT)
