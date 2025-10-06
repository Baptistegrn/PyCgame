#define JEU_BUILD_DLL
#include "image.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h> 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
void free_tab_images(Gestionnaire *gestionnaire) {
    if (!gestionnaire || !gestionnaire->image) return;

    free(gestionnaire->image->tab);
    gestionnaire->image->tab = NULL;
    gestionnaire->image->nb_images = 0;
    gestionnaire->image->capacite_images = 0;
}


void liberer_gestionnaire_son(GestionnaireSon *gs) {
    if (!gs) return;

    for (int i = 0; i < gs->taille; i++) {
        if (gs->entrees[i].son) {
            Mix_FreeChunk(gs->entrees[i].son);
        }
    }

    free(gs->entrees);
    gs->entrees = NULL;
    gs->taille = 0;
    gs->capacite = 0;

    if (debug) fprintf(stderr, "DEBUG: gestionnaire son libéré\n");
}

void liberer_gestionnaire_image(GestionnaireTextures *gs) {
    if (!gs) return;

    for (int i = 0; i < gs->taille; i++) {
        if (gs->entrees[i].texture) {
            SDL_DestroyTexture(gs->entrees[i].texture); 
        }
    }

    free(gs->entrees);
    gs->entrees = NULL;
    gs->taille = 0;
    gs->capacite = 0;

    if (debug) fprintf(stderr, "DEBUG: gestionnaire image libéré\n");
}
 void free_gestionnaire(Gestionnaire *jeu) {
    if (!jeu) return;

    if (jeu->image) {
        free(jeu->image->tab);
        free(jeu->image);
    }
    if (jeu->fond) free(jeu->fond);
    if (jeu->entrees) free(jeu->entrees);
    if (jeu->textures) free(jeu->textures);
    if (jeu->sons) free(jeu->sons);

    free(jeu);
}


void liberer_jeu(Gestionnaire *jeu) {
    if (!jeu) {
        if (debug) fprintf(stderr, "DEBUG: liberer_jeu jeu nul\n");
        return;
    }

    if (debug) fprintf(stderr, "DEBUG: liberer_jeu debut\n");

    // Libération spécifique des images tabulaires
    free_tab_images(jeu);

    // Libération des textures SDL
    // free les textures puis le reste 
    liberer_gestionnaire_image(jeu->textures);
    free(jeu->textures);

    // Libération des sons
    liberer_gestionnaire_son(jeu->sons);
    free(jeu->sons);

    if (jeu->entrees) free(jeu->entrees);

    if (jeu->controller) fermer_controller(jeu);
    if (jeu->joystick) fermer_joystick(jeu);

    if (jeu->rendu) SDL_DestroyRenderer(jeu->rendu);
    if (jeu->fenetre) SDL_DestroyWindow(jeu->fenetre);

    Mix_CloseAudio();
    IMG_Quit();
    SDL_Quit();

    free(jeu);

    if (debug) fprintf(stderr, "DEBUG: liberer_jeu fin\n");
}



