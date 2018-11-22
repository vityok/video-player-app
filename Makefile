CXX=g++
CXX_FLAGS=-std=c++14 -Wall -Wextra -g
CPP_FLAGS=-Iinc/ $(shell pkg-config gtkmm-3.0 --cflags)

LD_FLAGS=$(shell pkg-config gtkmm-3.0 --libs)

BINARY=video-player-app

SRC_DIR=src

SRCS=$(wildcard $(SRC_DIR)/*.cpp)
OBJS=$(subst .cpp,.o,$(SRCS))


.PHONY: all clean

all: $(BINARY)

# Creation of the executable
$(BINARY): $(OBJS)
	@echo "Linking: $@"
	$(CXX) $(OBJS) -o $@ $(LD_FLAGS)


$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp inc/video-player-app.hpp
	$(CXX) $(CXX_FLAGS) $(CPP_FLAGS) -c $< -o $@

clean:
	rm -f $(BINARY)
	rm -f $(SRC_DIR)/*.o
	rm -f $(SRC_DIR)/*~
