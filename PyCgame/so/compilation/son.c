#define JEU_BUILD_DLL

#include "image.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>


JEU_API void jouer_son(Gestionnaire *gestionnaire, const char *lien, int boucle, int canal) {
    if (!gestionnaire || !gestionnaire->sons) {
        if(debug)fprintf(stderr, "DEBUG: jouer_son gestionnaire ou sons NULL\n");
        return;
    }

    GestionnaireSon *gs = gestionnaire->sons;
    Mix_Chunk *son = recuperer_son_par_lien(gs, lien);

    if (!son) {
        if(debug)fprintf(stderr, "DEBUG: son introuvable (%s)\n", lien);
        return;
    }

    if (Mix_PlayChannel(canal, son, boucle-1) == -1) {
        if(debug)fprintf(stderr, "DEBUG: Mix_PlayChannel failed (%s)\n", Mix_GetError());
    }
}


JEU_API void arreter_son(Gestionnaire *gestionnaire, const char *lien) {
    if (!gestionnaire || !gestionnaire->sons) return;

    GestionnaireSon *gs = gestionnaire->sons;
    Mix_Chunk *son = recuperer_son_par_lien(gs, lien);
    if (!son) {
        if(debug)fprintf(stderr, "DEBUG: arreter_son introuvable (%s)\n", lien);
        return;
    }

    int nb_canaux = Mix_AllocateChannels(-1); // nombre de canaux existants
    for (int i = 0; i < nb_canaux; i++) {
        if (Mix_GetChunk(i) == son) {
            Mix_HaltChannel(i);
        }
    }

    if(debug)fprintf(stderr, "DEBUG: arreter_son fini (%s)\n", lien);
}


JEU_API void arreter_canal(int canal) {
    if (canal < 0) {
        if(debug)fprintf(stderr, "DEBUG: arreter_canal canal invalide (%d)\n", canal);
        return;
    }

    if (Mix_Playing(canal)) {
        Mix_HaltChannel(canal);
       if(debug) fprintf(stderr, "DEBUG: canal %d arrete\n", canal);
    } else {
        if(debug)fprintf(stderr, "DEBUG: canal %d n'est pas en lecture\n", canal);
    }
}


JEU_API void pause_canal(int canal) {
    if (canal < 0) {
        if(debug)fprintf(stderr, "DEBUG: pause_canal canal invalide (%d)\n", canal);
        return;
    }

    if (Mix_Playing(canal)) {
        Mix_Pause(canal);

        if(debug)fprintf(stderr, "DEBUG: canal %d pause\n", canal);
    } else {
        if(debug)fprintf(stderr, "DEBUG: canal %d n'est pas en lecture\n", canal);
    }
}


JEU_API void reprendre_canal(int canal) {
    if (canal < 0) {
        if(debug)fprintf(stderr, "DEBUG: reprendre_canal canal invalide (%d)\n", canal);
        return;
    }

    Mix_Resume(canal);
}


JEU_API void pause_son(Gestionnaire *gestionnaire, const char *lien) {
    if (!gestionnaire || !gestionnaire->sons) return;

    GestionnaireSon *gs = gestionnaire->sons;
    Mix_Chunk *son = recuperer_son_par_lien(gs, lien);
    if (!son) {
        if(debug)fprintf(stderr, "DEBUG: pause_son introuvable (%s)\n", lien);
        return;
    }

    int nb_canaux = Mix_AllocateChannels(-1); // nombre de canaux existants
    for (int i = 0; i < nb_canaux; i++) {
        if (Mix_GetChunk(i) == son) {
            Mix_Pause(i);
        }
    }

    if(debug)fprintf(stderr, "DEBUG: pause_son fini (%s)\n", lien);
}


JEU_API void reprendre_son(Gestionnaire *gestionnaire, const char *lien) {
    if (!gestionnaire || !gestionnaire->sons) return;

    GestionnaireSon *gs = gestionnaire->sons;
    Mix_Chunk *son = recuperer_son_par_lien(gs, lien);
    if (!son) {
       if(debug) fprintf(stderr, "DEBUG: rependre_son introuvable (%s)\n", lien);
        return;
    }

    int nb_canaux = Mix_AllocateChannels(-1); // nombre de canaux existants
    for (int i = 0; i < nb_canaux; i++) {
        if (Mix_GetChunk(i) == son) {
            Mix_Resume(i);
        }
    }

    if(debug)fprintf(stderr, "DEBUG: reprendre_son fini (%s)\n", lien);}
