# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++23 -Iapp/include -I/opt/homebrew/opt/sfml/include

# Linker flags using Homebrew SFML 
LDFLAGS = -L/opt/homebrew/opt/sfml/lib -lsfml-graphics -lsfml-window -lsfml-system -Wl,-rpath,/opt/homebrew/opt/sfml/lib -lcurl

# Source files and output binary
APP = app/src/main.cpp app/src/window.cpp app/src/browser.cpp app/src/searchbar.cpp app/src/http_client.cpp app/src/content_view.cpp
TARGET = bin/main

# Default rule to build executable
all: $(TARGET)

$(TARGET): $(APP)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) $(APP) -o $(TARGET) $(LDFLAGS)

# Clean rule to remove output binary
clean:
	rm -f $(TARGET)