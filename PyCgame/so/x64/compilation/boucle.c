#define JEU_BUILD_DLL
#include "image.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static UpdateCallback g_update_callback = NULL;

JEU_API void set_update_callback(UpdateCallback cb) {
    g_update_callback = cb;
}
JEU_API void boucle_principale(Gestionnaire *jeu) {
    if (!jeu) {
        fprintf(stderr, "Erreur: Gestionnaire NULL dans boucle_principale\n");
        return;
    }


    Uint32 last_ticks = SDL_GetTicks();

    while (jeu->run) {
        Uint32 frame_start = SDL_GetTicks();

        jeu->temps_frame++;
        // entrees
        input_update(jeu, jeu->entrees);

        if (g_update_callback) {
            g_update_callback(jeu);
        } else {
            //python
            update(jeu);
        }


        if (!jeu->fond) {
            fprintf(stderr, "Erreur: fond NULL\n");
        } else {
            actualiser(jeu, jeu->fond->dessiner, jeu->fond->bande_noir,
                       jeu->fond->r, jeu->fond->g, jeu->fond->b);
        }

        if (jeu->fps <= 0) {
            fprintf(stderr, "Erreur: FPS invalide (<=0), correction à 60\n");
            jeu->fps = 60;
        }

        float dt_theorique = 1.0f / (float)jeu->fps; 
        Uint32 frame_time_ms = SDL_GetTicks() - frame_start;
        float frame_time_s = frame_time_ms / 1000.0f;

        if (frame_time_s < dt_theorique) {
            SDL_Delay((Uint32)((dt_theorique - frame_time_s) * 1000.0f));
        }

        Uint32 current_ticks = SDL_GetTicks();
        float dt_reel = (current_ticks - last_ticks) / 1000.0f;
        last_ticks = current_ticks;
        if (dt_reel > dt_theorique) {
            jeu->dt = dt_reel;
        } else {
            jeu->dt = dt_theorique;
        }
    }

    liberer_jeu(jeu);
    return;
}





JEU_API void update(Gestionnaire *jeu) {


    if (g_update_callback) {
        g_update_callback(jeu);
        return;
    }



    (void)jeu;
}











 

