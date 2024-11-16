all: build run

build: src/main.c
	cc src/main.c -o3 -Wall -I./src/raylib-5.0_linux_amd64/include/ -L./src/raylib-5.0_linux_amd64/lib/ -l:libraylib.a -lm -o tuned-gui 

run: ./tuned-gui
	./tuned-gui
