.PHONY: run
run:
	(cd ./build/ && ./chip-8)

.PHONY: build
build:
	cmake -B build
	cmake --build build

.PHONY: build-wasm
build-wasm:
	emcmake cmake -B build/wasm -DCMAKE_BUILD_TYPE=MinSizeRel
	cmake --build build/wasm

.PHONY: serve-wasm
serve-wasm:
	python3 -m http.server -d build/wasm

.PHONY: clean
clean:
	rm -rf ./build
