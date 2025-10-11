#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "image.h"



int main(int argc, char *argv[]) {
    (void)argc; (void)argv;
    
    // Initialisation du gestionnaire principal
    int largeur =320;
    int hauteur =180;
    char *lien = "../assets";
    Gestionnaire *jeu = initialisation(hauteur,largeur,200.0f,3,lien,lien,true,true,20,30,100,"coucou",true);
    boucle_principale(jeu);
    return EXIT_SUCCESS;
}





















































