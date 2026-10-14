CC      := gcc
CFLAGS  := -Wall -Wextra -Wpedantic -std=c23 -pthread

BUILD   := build
INCLUDE := internal/public
PRIVATE := internal/private
PRIVATE_SRC := $(wildcard $(PRIVATE)/*.c)
CODE    := code

MAIN    := $(BUILD)/vmsim
TEST    := $(BUILD)/test

ASM_SRC := $(CODE)/vmsim/launch.c $(PRIVATE_SRC)
ASM_OBJ := $(patsubst %.c,$(BUILD)/%.s,$(ASM_SRC))

.PHONY: build test run asm clean

build: $(MAIN)

test: $(TEST)
	./$(TEST)

run: $(MAIN)
	./$(MAIN)

asm: $(ASM_OBJ)

$(BUILD)/%.s: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -masm=intel -I$(INCLUDE) -S $< -o $@

$(MAIN): $(CODE)/vmsim/launch.c $(PRIVATE_SRC)
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -I$(INCLUDE) $^ -o $@

$(TEST): $(CODE)/tests/Main.c
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -I$(INCLUDE) $^ -o $@

clean:
	rm -f $(MAIN) $(TEST)
	rm -rf $(BUILD)/$(CODE) $(BUILD)/$(PRIVATE)
