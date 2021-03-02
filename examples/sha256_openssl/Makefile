SRC=sha256.cpp
OBJ=sha256
CPPFLAGS=-O3 -g
LDFLAGS=-lssl -lcrypto
WPC_ROOT_DIR=$(shell readlink -f $(PWD)/../..)
LLVM_PROFILING_DIR=$(WPC_ROOT_DIR)/tools/llvm_profiling

all: sha256 sha256_instr_insmix

common:
	mkdir -p build

clean:
	rm -f sha256 sha256_instr_insmix
	rm -rf build

sha256: $(SRC) common
	clang++ $(SRC) $(CPPFLAGS) $(LDFLAGS) -o build/$@
	ln -sv build/$@ $@

sha256_instr_insmix: $(SRC) common
	clang++ -S -emit-llvm $(SRC) $(CPPFLAGS) -o build/$(OBJ).ll
	$(LLVM_PROFILING_DIR)/dynamic_instruction_count/cdi build/$(OBJ).ll build/$@.bc \
		> build/static_insmix.log \
		|| (echo "instrumentation failed"; exit 1)
	llc -filetype=obj build/$@.bc -o build/$@.o \
		|| (echo "llc failed"; exit 1)
	clang++ build/$@.o /opt/benchcpu/llvm-profiling/runtime.cpp \
		$(CPPFLAGS) $(LDFLAGS) \
		`llvm-config --cxxflags --ldflags --system-libs --libs` -o build/$@ \
		|| (echo "clang++ failed"; exit 1)
	ln -sv build/$@ $@
	tree .
