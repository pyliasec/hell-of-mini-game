# Makefile for Hell of Mini Game
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude -I/opt/homebrew/include
LDFLAGS = -L/opt/homebrew/lib -lssl -lcrypto

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
DATA_DIR = data

# Source files
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Target executable
TARGET = $(BUILD_DIR)/hellgame

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Build object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)/*.o $(TARGET)

# Clean all generated files
cleanall: clean
	rm -rf $(DATA_DIR)/*.csv

# Run the game
run: $(TARGET)
	./$(TARGET)

# Install dependencies (macOS)
install-deps:
	@echo "Installing OpenSSL..."
	@if command -v brew >/dev/null 2>&1; then \
		brew install openssl; \
	else \
		echo "Please install Homebrew first: https://brew.sh/"; \
	fi

# Create necessary directories
dirs:
	mkdir -p $(SRC_DIR) $(INCLUDE_DIR) $(BUILD_DIR) $(DATA_DIR)

.PHONY: all clean cleanall run install-deps dirs