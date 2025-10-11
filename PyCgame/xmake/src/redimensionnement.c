
#define JEU_BUILD_DLL

#include "image.h"
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>



JEU_API void redimensionner_fenetre(Gestionnaire *gestionnaire) {
    if (!gestionnaire) {
        if(debug)fprintf(stderr, "DEBUG: redimensionner_fenetre_decalage gestionnaire nul\n");
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
        if(debug)fprintf(stderr, "DEBUG: redimensionner_fenetre_decalage fenetre/rendu nul\n");
        return;
    }




    int displayIndex = SDL_GetWindowDisplayIndex(fenetre);
    SDL_Rect displayBounds;
    SDL_GetDisplayBounds(displayIndex, &displayBounds);
    SDL_DisplayMode mode;
    SDL_GetCurrentDisplayMode(displayIndex, &mode);


    //souris
    int raw_x = 0, raw_y = 0;
    SDL_GetMouseState(&raw_x, &raw_y);

    float coeff_avant_l = (float)largeur_actuelle / (float)largeur_base;
    float coeff_avant_h = (float)hauteur_actuelle / (float)hauteur_base;
    float mouse_x_univers = (raw_x - dec_x) / coeff_avant_l;
    float mouse_y_univers = (raw_y - dec_y) / coeff_avant_h;







    if(debug)fprintf(stderr,
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
        if(debug)fprintf(stderr, "DEBUG: mode fenetre conserve echelle l_act=%d h_act=%d dec=(%.2f,%.2f)\n",
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
        if(debug)fprintf(stderr,
            "DEBUG: mode plein ecran conserve echelle mode=(%dx%d) l_act=%d h_act=%d dec=(%.2f,%.2f)\n",
            mode.w, mode.h, largeur_actuelle, hauteur_actuelle, dec_x, dec_y
        );
    }

    plein_ecran = !plein_ecran;


    //souris
    float coeff_apres_l = (float)largeur_actuelle / (float)largeur_base;
    float coeff_apres_h = (float)hauteur_actuelle / (float)hauteur_base;
    int mouse_x_screen = (int)(mouse_x_univers * coeff_apres_l + dec_x);
    int mouse_y_screen = (int)(mouse_y_univers * coeff_apres_h + dec_y);

    SDL_WarpMouseInWindow(fenetre, mouse_x_screen, mouse_y_screen);
    if (gestionnaire->entrees) {
        gestionnaire->entrees->mouse_x = mouse_x_screen;
        gestionnaire->entrees->mouse_y = mouse_y_screen;
    }

    if(debug)fprintf(stderr,
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

