cc = gcc -Wall

# Fichiers source
src = main.c boucle.c caractere.c chargement_image.c fenetre.c image.c init.c input.c maths.c chargement_son.c debug.c trouver_png.c trouver_wav.c liberer.c redimensionnement.c controller.c normaliser.c random.c son.c

# Fichiers objets et dépendances (dans src/)
obj = $(src:.c=.o)
dep = $(src:.c=.d)

# Dossiers de sortie
lib_dir = ..
exe_dir = ./exe

# Cibles
lib = $(lib_dir)/libjeu.so
exe = $(exe_dir)/jeu

# Flags de compilation et de liaison
cflags = -fPIC $(shell sdl2-config --cflags)
cflags += $(shell pkg-config --cflags SDL2_image SDL2_mixer)

ldflags = $(shell sdl2-config --libs)
ldflags += $(shell pkg-config --libs SDL2_image SDL2_mixer) -lm

# Cible par défaut : build lib + exe
all: $(lib) $(exe)

# Librairie partagée
$(lib): $(obj) | $(lib_dir)
	$(cc) -shared -o $@ $^ $(ldflags)

# Exécutable
$(exe): $(obj) | $(exe_dir)
	$(cc) $^ -o $@ $(ldflags)

# Création des dossiers si nécessaires
$(lib_dir):
	mkdir -p $@

$(exe_dir):
	mkdir -p $@

# Compilation avec dépendances
%.o: %.c
	$(cc) -MMD -c $< -o $@ $(cflags)

# Inclusion des fichiers de dépendance
-include $(dep)

# Nettoyage
clean:
	rm -f *.o *.d
	rm -f $(lib) $(exe)
