# Makefile for compiling emit_function.cpp

CXX = clang++
CXXFLAGS = -O3 $(shell /Users/liushanlin/cpp_directory/llvm-project/build/bin/llvm-config --cxxflags)
LDFLAGS = $(shell /Users/liushanlin/cpp_directory/llvm-project/build/bin/llvm-config --ldflags)
LIBS = $(shell /Users/liushanlin/cpp_directory/llvm-project/build/bin/llvm-config --system-libs --libs core)

TARGET = emit_module \
		emit_function \
		emit_basicblock \
		emit_globalvar \
		emit_return \
		emit_arguments \
		emit_arithmetic_instruction \
		emit_condition \
		emit_loop
# SRC = emit_module.cpp


all: $(TARGET)

%: %.cpp
	$(CXX) $(CXXFLAGS) $< $(LDFLAGS) $(LIBS)  -o $@

clean:
	rm -f $(TARGET)

.PHONY: all clean
