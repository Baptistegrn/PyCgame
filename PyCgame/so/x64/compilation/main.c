#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "image.h"

// main de test pour compilation normal
int main(int argc, char *argv[]) {
    (void)argc; (void)argv;
    
    int largeur =320;
    int hauteur =180;
    char *lien = ".";
    Gestionnaire *jeu = initialisation(hauteur,largeur,60.0f,3,lien,lien,true,true,20,30,100,"coucou");

    boucle_principale(jeu);
    return EXIT_SUCCESS;
}
