# Makefile for compiling emit_function.cpp

CXX = clang++
CXXFLAGS = -O3 $(shell /home/shanlin/llvm-project/build/bin/llvm-config --cxxflags)
LDFLAGS = $(shell /home/shanlin/llvm-project/build/bin/llvm-config --ldflags)
LIBS = $(shell /home/shanlin/llvm-project/build/bin/llvm-config --system-libs --libs core)

TARGET = emit_module \
		emit_function 
# SRC = emit_module.cpp


all: $(TARGET)

%: %.cpp
	$(CXX) $(CXXFLAGS) $< $(LDFLAGS) $(LIBS) -o $@

clean:
	rm -f $(TARGET)

.PHONY: all clean
