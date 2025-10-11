#define JEU_BUILD_DLL
#include "image.h"
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int fenetre_init(Gestionnaire *gestionnaire,const char *nom_fenetre) {
    if (!gestionnaire) {
        if(debug)fprintf(stderr, "DEBUG: fenetre_init gestionnaire nul\n");
        return 99;
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        if(debug)fprintf(stderr, "DEBUG: SDL_Init failed -> %s\n", SDL_GetError());
        return 1;
    }
    if(debug)fprintf(stderr, "DEBUG: SDL_Init ok\n");

    SDL_ShowCursor(SDL_DISABLE);

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        if(debug)fprintf(stderr, "DEBUG: IMG_Init PNG failed -> %s\n", IMG_GetError());
        SDL_Quit();
        return 2;
    }
    if(debug)fprintf(stderr, "DEBUG: IMG_Init PNG ok\n");


    gestionnaire->largeur_actuel = gestionnaire->largeur * gestionnaire->coeff_minimise;
    gestionnaire->hauteur_actuel = gestionnaire->hauteur * gestionnaire->coeff_minimise;
    gestionnaire->plein_ecran = false;

    if(debug)fprintf(stderr, "DEBUG: tailles init l=%d h=%d l_act=%d h_act=%d coeff=%d\n",
            gestionnaire->largeur, gestionnaire->hauteur,
            gestionnaire->largeur_actuel, gestionnaire->hauteur_actuel,
            gestionnaire->coeff_minimise);

    gestionnaire->fenetre = SDL_CreateWindow(
        nom_fenetre,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        gestionnaire->largeur_actuel,
        gestionnaire->hauteur_actuel,
        SDL_WINDOW_SHOWN
    );

    if (!gestionnaire->fenetre) {
        if(debug)fprintf(stderr, "DEBUG: SDL_CreateWindow failed -> %s\n", SDL_GetError());
        IMG_Quit();
        SDL_Quit();
        return 3;
    }
    if(debug)fprintf(stderr, "DEBUG: fenetre creee\n");

    //render accelere
    Uint32 flags = SDL_RENDERER_ACCELERATED ;

    gestionnaire->rendu = SDL_CreateRenderer(gestionnaire->fenetre, -1, flags);
    if (!gestionnaire->rendu) {
        if(debug)fprintf(stderr, "DEBUG: SDL_CreateRenderer failed -> %s\n", SDL_GetError());
        SDL_DestroyWindow(gestionnaire->fenetre);
        IMG_Quit();
        SDL_Quit();
        return 4;
    }
    if(debug)fprintf(stderr, "DEBUG: renderer cree\n");

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    SDL_RenderSetIntegerScale(gestionnaire->rendu, SDL_TRUE);

    if (gestionnaire->textures) {
        gestionnaire->textures->rendu = gestionnaire->rendu;
    }

    if(debug)fprintf(stderr, "DEBUG: fenetre_init ok\n");
    return 0;
}






JEU_API void colorier(Gestionnaire *jeu ,int r, int g,int b){
    if(!jeu){
        if(debug)fprintf(stderr,"erreur gestionnaire vide dans colorier\n");
        return; 
    }
    jeu->fond->r=r;
    jeu->fond->g=g;
    jeu->fond->b=b;
    return;
}

