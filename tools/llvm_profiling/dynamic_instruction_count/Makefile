PROJECT=cdi


COMMON_FLAGS=-g -O3 -std=c++1y -I.. -Wall -Wextra -Wno-unused-parameter \
	`llvm-config --cxxflags --ldflags --system-libs --libs`
SRC_DIR?=$(PWD)
SRC=CountDynamicInstructions.cpp

.PHONY:
	clean $(PROJECT)

cdi:
	@echo Compiling $(SRC)
	clang++ $(SRC) $(COMMON_FLAGS) -o $(PROJECT)

resnet50: $(PROJECT)
	# cp /home/zkyd/ISCA2021/results/resnet50/llvm/module_0000.ir-with-opt-noconst.ll $@.ll
	./$(PROJECT) $@.ll $@-instrumented.bc | sudo tee /home/zkyd/ISCA2021/results/resnet50/llvm/static_insmix.log

clean:
	rm -rf $(PROJECT)
