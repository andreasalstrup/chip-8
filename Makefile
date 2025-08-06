.PHONY: run
run:
	./build/chip-8

.PHONY: build
build:
	cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_BUILD_TYPE=Debug -B build
	cmake --build build

.PHONY: build-wasm
build-wasm:
	make -f Makefile.emscripten 

# .PHONY: build-wasm
# build-wasm:
# 	emcmake cmake -B build/wasm -DCMAKE_BUILD_TYPE=MinSizeRel
# 	cmake --build build/wasm

.PHONY: serve-wasm
serve-wasm:
	python3 -m http.server -d build/wasm

.PHONY: clean
clean:
	rm -rf ./build
	make -f Makefile.emscripten clean
