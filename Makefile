CXX = g++
CXXFLAGS = -Wall -Wextra -Wpedantic -O2 -march=native -std=c++17 $(shell pkgconf --cflags glfw3)
CPPFLAGS = -Iinclude/ -Iinclude/glad/
LDFLAGS  = $(shell pkgconf --libs glfw3)

SRCDIR = src
SRC = $(wildcard $(SRCDIR)/*.cpp)

OBJDIR = build
OBJ = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRC))

DEP = $(OBJ:.o=.d)

TARGET = a.out

all: $(TARGET)

debug: CXXFLAGS += -MMD -g3 -ggdb -fsanitize=address -fsanitize=undefined
debug: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

-include $(DEP)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

.PHONY: clean
clean:
	rm -r $(OBJDIR)
