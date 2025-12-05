# Needs to support:
# - make program
# - make test
# - make clean
# - make all
# - make docs
# Should compile with -Wall and -Werror at the minimum
# Cheers Dan for help with the makefile, never knew you could do "wildcard" and such


MAIN_NAME = chal4
OUT_NAME = chal4

INCLUDES = include
CFLAGS = -Wall -Werror -I$(INCLUDES)
DEBUG_FLAGS = -DDEBUG -g

CC = gcc
BIN = bin
SRC = src

SRCS = $(wildcard $(SRC)/**/*.c $(SRC)/*.c)
OBJS = $(patsubst $(SRC)/%.c, $(BIN)/%.o, $(SRCS))
DEPS = $(wildcard $(INCLUDES)/*.h)


.PHONY: test debug clean clean-objs run check-complexity docs

all: clean $(OBJS) $(BIN)/$(OUT_NAME)
all: clean-objs

debug: CFLAGS += -g -DDEBUG
debug: all

$(BIN)/$(OUT_NAME): $(OBJS)
	$(CC) $^ -o $@ $(CFLAGS) $(LINKS)

$(BIN)/%.o: $(SRC)/%.c $(DEPS)
	@mkdir -p $(@D)
	$(CC) -c -o $@ $< $(CFLAGS) $(LINKS)

clean:
	@echo "[i] Cleaning up build artifacts..."
	@rm -rf $(BIN)
	@rm -rf $(TEST_EXECUTABLES)
	@find . -name "*.o" -type f -delete
	@find . -name "*.log" -type f -delete

clean-objs:
	@echo "[i] Cleaning up object files..."
	@find $(BIN) -name "*.o" -type f -delete
	@find $(BIN) -type d -empty -delete
	@echo "[i] Cleanup complete"

run: all
	@echo "[i] Running program..."
	@./$(BIN)/$(OUT_NAME)
	@echo "[i] Program complete"

check-complexity:
	@echo "[i] Running complexity..."
	@$(foreach src, $(SRCS), \
	complexity --horrid-threshold=15 --threshold=0 $(src); \
	)
	@echo "[i] Complexity complete"

check-memory:
	@echo "[i] Running memory check..."
	@valgrind --tool=memcheck --track-origins=yes --leak-check=full --show-leak-kinds=all ./$(BIN)/$(OUT_NAME)
	@echo "[i] Memory check complete"

check-format:
	@echo "[i] Running clang format check..."
	@clang-format --style=file --Werror $(SRC)/*.c $(INCLUDES)/*.h
	@echo "[i] Format check complete"

check-tidy:
	@echo "[i] Running tidy check..."
	@clang-tidy --checks="cert*, bugprone*, readability*" $(SRC)/*.c -- -I$(INCLUDES) -std=gnu99
	@echo "[i] Tidy check complete"

check-threading:
	@echo "[i] Running threading check..."
	@valgrind --tool=helgrind ./$(BIN)/$(OUT_NAME)
	@echo "[i] Threading check complete"