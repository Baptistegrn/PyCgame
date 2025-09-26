#define JEU_BUILD_DLL
#include "image.h"
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#pragma comment(lib, "user32.lib")


int fenetre_init(Gestionnaire *gestionnaire,const char *nom_fenetre) {
    if (!gestionnaire) {
        fprintf(stderr, "DEBUG: fenetre_init gestionnaire nul\n");
        return 99;
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "DEBUG: SDL_Init failed -> %s\n", SDL_GetError());
        return 1;
    }
    fprintf(stderr, "DEBUG: SDL_Init ok\n");

    SDL_ShowCursor(SDL_DISABLE);

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        fprintf(stderr, "DEBUG: IMG_Init PNG failed -> %s\n", IMG_GetError());
        SDL_Quit();
        return 2;
    }
    fprintf(stderr, "DEBUG: IMG_Init PNG ok\n");


    gestionnaire->largeur_actuel = gestionnaire->largeur * gestionnaire->coeff_minimise;
    gestionnaire->hauteur_actuel = gestionnaire->hauteur * gestionnaire->coeff_minimise;
    gestionnaire->plein_ecran = false;

    fprintf(stderr, "DEBUG: tailles init l=%d h=%d l_act=%d h_act=%d coeff=%d\n",
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
        fprintf(stderr, "DEBUG: SDL_CreateWindow failed -> %s\n", SDL_GetError());
        IMG_Quit();
        SDL_Quit();
        return 3;
    }
    fprintf(stderr, "DEBUG: fenetre creee\n");

    //render accelere
    Uint32 flags = SDL_RENDERER_ACCELERATED ;

    gestionnaire->rendu = SDL_CreateRenderer(gestionnaire->fenetre, -1, flags);
    if (!gestionnaire->rendu) {
        fprintf(stderr, "DEBUG: SDL_CreateRenderer failed -> %s\n", SDL_GetError());
        SDL_DestroyWindow(gestionnaire->fenetre);
        IMG_Quit();
        SDL_Quit();
        return 4;
    }
    fprintf(stderr, "DEBUG: renderer cree\n");

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    SDL_RenderSetIntegerScale(gestionnaire->rendu, SDL_TRUE);

    if (gestionnaire->textures) {
        gestionnaire->textures->rendu = gestionnaire->rendu;
    }

    fprintf(stderr, "DEBUG: fenetre_init ok\n");
    return 0;
}




void liberer_jeu(Gestionnaire *jeu){
    if(!jeu) { 
        fprintf(stderr, "DEBUG: liberer_jeu jeu nul\n"); 
        return; 
    }

    fprintf(stderr, "DEBUG: liberer_jeu debut\n");
    free_tab_images(jeu);
    if(jeu->textures) {
        for(int i=0;i<jeu->textures->taille;i++){
            if (jeu->textures->entrees[i].texture) {
                SDL_DestroyTexture(jeu->textures->entrees[i].texture);
            }
        }
        free(jeu->textures->entrees);
        free(jeu->textures);
    }
    if(jeu->sons) {
        for(int i=0;i<jeu->sons->taille;i++){
            if(jeu->sons->entrees[i].son) {
                Mix_FreeChunk(jeu->sons->entrees[i].son);
            }
        }
        free(jeu->sons->entrees);
        free(jeu->sons);
    }
    if(jeu->entrees) free(jeu->entrees);
    if(jeu->controller)    SDL_GameControllerClose(jeu->controller);
    if(jeu->rendu) SDL_DestroyRenderer(jeu->rendu);
    if(jeu->fenetre) SDL_DestroyWindow(jeu->fenetre);
    Mix_CloseAudio();
    IMG_Quit();
    SDL_Quit();
    free(jeu);
    fprintf(stderr, "DEBUG: liberer_jeu fin\n");
}




JEU_API void redimensionner_fenetre(Gestionnaire *gestionnaire) {
    if (!gestionnaire) {
        fprintf(stderr, "DEBUG: redimensionner_fenetre_decalage gestionnaire nul\n");
        return;
    }

    SDL_Window   *fenetre   = gestionnaire->fenetre;
    SDL_Renderer *rendu     = gestionnaire->rendu;
    int largeur_base        = gestionnaire->largeur;
    int hauteur_base        = gestionnaire->hauteur;
    int largeur_actuelle    = gestionnaire->largeur_actuel;
    int hauteur_actuelle    = gestionnaire->hauteur_actuel;
    float dec_x             = gestionnaire->decalage_x;
    float dec_y             = gestionnaire->decalage_y;
    int plein_ecran         = gestionnaire->plein_ecran;
    float coeff_minimise    = gestionnaire->coeff_minimise;

    if (!fenetre || !rendu) {
        fprintf(stderr, "DEBUG: redimensionner_fenetre_decalage fenetre/rendu nul\n");
        return;
    }

    int displayIndex = SDL_GetWindowDisplayIndex(fenetre);
    SDL_Rect displayBounds;
    SDL_GetDisplayBounds(displayIndex, &displayBounds);
    SDL_DisplayMode mode;
    SDL_GetCurrentDisplayMode(displayIndex, &mode);

    // --- 1) Sauvegarder la position souris (écran -> univers)
    int raw_x = 0, raw_y = 0;
    SDL_GetMouseState(&raw_x, &raw_y);

    float coeff_avant_l = (float)largeur_actuelle / (float)largeur_base;
    float coeff_avant_h = (float)hauteur_actuelle / (float)hauteur_base;
    float mouse_x_univers = (raw_x - dec_x) / coeff_avant_l;
    float mouse_y_univers = (raw_y - dec_y) / coeff_avant_h;

    fprintf(stderr,
        "DEBUG: AVANT (decalage) raw=(%d,%d) dec=(%.2f,%.2f) coeff=(%.3f,%.3f) -> univers=(%.2f,%.2f)\n",
        raw_x, raw_y, dec_x, dec_y,
        coeff_avant_l, coeff_avant_h, mouse_x_univers, mouse_y_univers
    );

    if (plein_ecran) {

        dec_x = 0.0f;
        dec_y = 0.0f;
        largeur_actuelle = (int)(largeur_base * coeff_minimise);
        hauteur_actuelle = (int)(hauteur_base * coeff_minimise);

        SDL_SetWindowSize(fenetre, largeur_actuelle, hauteur_actuelle);
        SDL_SetWindowPosition(
            fenetre,
            displayBounds.x + (mode.w - largeur_actuelle) / 2,
            displayBounds.y + (mode.h - hauteur_actuelle) / 2
        );
        SDL_SetWindowBordered(gestionnaire->fenetre, SDL_TRUE);
        fprintf(stderr, "DEBUG: mode fenetre conserve echelle l_act=%d h_act=%d dec=(%.2f,%.2f)\n",
                largeur_actuelle, hauteur_actuelle, dec_x, dec_y);
    } if(!plein_ecran) {

        float coeff_l = (float)mode.w / (float)largeur_base;
        float coeff_h = (float)mode.h / (float)hauteur_base;

        if (coeff_l > coeff_h) {
            float reste = coeff_l - coeff_h;
            dec_x = reste * largeur_base / 2.0f;
            dec_y = 0.0f;
            largeur_actuelle = (int)(largeur_base * coeff_h);
            hauteur_actuelle = (int)(hauteur_base * coeff_h);
        } else {
            float reste = coeff_h - coeff_l;
            dec_y = reste * hauteur_base / 2.0f;
            dec_x = 0.0f;
            largeur_actuelle = (int)(largeur_base * coeff_l);
            hauteur_actuelle = (int)(hauteur_base * coeff_l);
        }


        SDL_SetWindowSize(fenetre, mode.w, mode.h);
        SDL_SetWindowPosition(fenetre, displayBounds.x, displayBounds.y);
        SDL_SetWindowBordered(gestionnaire->fenetre, SDL_FALSE);
        fprintf(stderr,
            "DEBUG: mode plein ecran conserve echelle mode=(%dx%d) l_act=%d h_act=%d dec=(%.2f,%.2f)\n",
            mode.w, mode.h, largeur_actuelle, hauteur_actuelle, dec_x, dec_y
        );
    }

    plein_ecran = !plein_ecran;

    float coeff_apres_l = (float)largeur_actuelle / (float)largeur_base;
    float coeff_apres_h = (float)hauteur_actuelle / (float)hauteur_base;
    int mouse_x_screen = (int)(mouse_x_univers * coeff_apres_l + dec_x);
    int mouse_y_screen = (int)(mouse_y_univers * coeff_apres_h + dec_y);

    SDL_WarpMouseInWindow(fenetre, mouse_x_screen, mouse_y_screen);
    if (gestionnaire->entrees) {
        gestionnaire->entrees->mouse_x = mouse_x_screen;
        gestionnaire->entrees->mouse_y = mouse_y_screen;
    }

    fprintf(stderr,
        "DEBUG: APRES (decalage) univers=(%.2f,%.2f) coeff=(%.3f,%.3f) dec=(%.2f,%.2f) -> screen=(%d,%d)\n",
        mouse_x_univers, mouse_y_univers,
        coeff_apres_l, coeff_apres_h, dec_x, dec_y,
        mouse_x_screen, mouse_y_screen
    );


    gestionnaire->largeur_actuel = largeur_actuelle;
    gestionnaire->hauteur_actuel = hauteur_actuelle;
    gestionnaire->decalage_x     = dec_x;
    gestionnaire->decalage_y     = dec_y;
    gestionnaire->plein_ecran    = plein_ecran;
}




