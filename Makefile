# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++23 -Iinclude -I/opt/homebrew/opt/sfml/include

# Linker flags using Homebrew SFML 
LDFLAGS = -L/opt/homebrew/opt/sfml/lib -lsfml-graphics -lsfml-window -lsfml-system -Wl,-rpath,/opt/homebrew/opt/sfml/lib -lcurl

# Source files organized by module
CORE_SRC = src/core/http_client.cpp src/core/html_parser.cpp
UI_SRC = src/ui/window.cpp src/ui/searchbar.cpp src/ui/content_view.cpp
APP_SRC = src/browser/browser.cpp src/main.cpp

ALL_SRC = $(CORE_SRC) $(UI_SRC) $(APP_SRC)
TARGET = bin/main

# Test files
TEST_SRC = test/test_main.cpp test/test_html_parser.cpp
TEST_TARGET = bin/test

# Default rule to build executable
all: $(TARGET)

$(TARGET): $(ALL_SRC)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) $(ALL_SRC) -o $(TARGET) $(LDFLAGS)

# Test target (only core modules, no UI/SFML dependencies)
$(TEST_TARGET): $(CORE_SRC) $(TEST_SRC)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) -Itest $(CORE_SRC) $(TEST_SRC) -o $(TEST_TARGET) -lcurl

test: $(TEST_TARGET)
	./$(TEST_TARGET)

# Clean rule to remove output binary
clean:
	rm -f $(TARGET) $(TEST_TARGET)
