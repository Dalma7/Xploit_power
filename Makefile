CXX = ccache g++
CXXFLAGS = -O0 -g -Wall -std=c++17

SOURCES = main.cpp \
          aegis/aegis.cpp \
          hexer/hexer.cpp \
          stry/stry.cpp \
          dread/dread.cpp \
          Bony_Stony/bony_stony.cpp

OBJECTS = $(SOURCES:.cpp=.o)

TARGET = xploit

GREEN = \033[0;32m
RED = \033[0;31m
NC = \033[0m

all: $(TARGET)
	@echo -e "$(GREEN)[+] Build complete!$(NC)"

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ -lcurl


%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: all
	@echo -e "$(GREEN)[*] Running $(TARGET) ...$(NC)"
	./$(TARGET)

clean:
	@rm -f $(OBJECTS) $(TARGET)
	@echo -e "$(RED)[*] Cleaned up build files.$(NC)"
