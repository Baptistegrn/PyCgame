cc = gcc -Wall

src = main.c boucle.c caractere.c chargement.c fenetre.c image.c init.c input.c maths.c son.c

obj = $(src:.c=.o)
dep = $(src:.c=.d)

lib_dir = ./so
exe_dir = ./exe

lib = $(lib_dir)/libjeu.so
exe = $(exe_dir)/jeu

cflags = -fPIC $(shell sdl2-config --cflags)
cflags += $(shell pkg-config --cflags SDL2_image SDL2_mixer)

ldflags = $(shell sdl2-config --libs)
ldflags += $(shell pkg-config --libs SDL2_image SDL2_mixer) -lm

all: $(lib) $(exe)

$(lib): $(obj) | $(lib_dir)
	$(cc) -shared -o $@ $^ $(ldflags)

$(exe): $(obj) | $(exe_dir)
	$(cc) $^ -o $@ $(ldflags)

$(lib_dir):
	mkdir -p $@

$(exe_dir):
	mkdir -p $@

%.o: %.c
	$(cc) -MMD -c $< -o $@ $(cflags)

-include $(dep)

clean:
	rm -f *.o *.d
	rm -f $(lib) $(exe)

.PHONY: deps
deps:
	sudo apt update
	sudo apt install -y libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev pkg-config
