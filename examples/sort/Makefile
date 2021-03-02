SRC=sort.cpp
OBJ=sort
CPPFLAGS=-O3 -g
WPC_ROOT_DIR=$(shell readlink -f $(PWD)/../..)
LLVM_PROFILING_DIR=$(WPC_ROOT_DIR)/tools/llvm_profiling

all: sort sort_instr_insmix

common:
	mkdir -p build

clean:
	rm -rf build

sort: $(SRC) common
	clang++ $(SRC) $(CPPFLAGS) -o build/$@

sort_instr_insmix: $(SRC) common
	clang++ -S -emit-llvm $(SRC) $(CPPFLAGS) -o build/$(OBJ).ll
	$(LLVM_PROFILING_DIR)/dynamic_instruction_count/cdi build/$(OBJ).ll build/$@.bc \
		> build/static_insmix.log \
		|| (echo "instrumentation failed"; exit 1)
	llc -filetype=obj build/$@.bc -o build/$@.o \
		|| (echo "llc failed"; exit 1)
	clang++ build/$@.o /opt/benchcpu/llvm-profiling/runtime.cpp $(CPPFLAGS) \
		`llvm-config --cxxflags --ldflags --system-libs --libs` -o build/$@ \
		|| (echo "clang++ failed"; exit 1)
	ln -sv build/$@ $@
	tree .

sort_mr: sort_mr.java common
	# please run `source ./env.sh` first.
	javac sort_mr.java -d build/
	cd build && jar -cvf sort_mr.jar *.class && cd -
