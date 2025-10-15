
#define JEU_BUILD_DLL

#include "image.h"
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


JEU_API void redimensionner_fenetre(Gestionnaire *gestionnaire) {
    if (!gestionnaire) {
        if(debug) fprintf(stderr, "DEBUG: redimensionner_fenetre_decalage gestionnaire nul\n");
        return;
    }

    SDL_Window   *fenetre   = gestionnaire->fenetre;
    SDL_Renderer *rendu     = gestionnaire->rendu;
    int largeur_base        = gestionnaire->largeur;
    int hauteur_base        = gestionnaire->hauteur;
    int largeur_actuelle    = gestionnaire->largeur_actuel;
    int hauteur_actuelle    = gestionnaire->hauteur_actuel;
    int dec_x               = gestionnaire->decalage_x; // int maintenant
    int dec_y               = gestionnaire->decalage_y; // int maintenant
    int plein_ecran         = gestionnaire->plein_ecran;
    int coeff_minimise      = gestionnaire->coeff_minimise; // int maintenant

    if (!fenetre || !rendu) {
        if(debug) fprintf(stderr, "DEBUG: redimensionner_fenetre_decalage fenetre/rendu nul\n");
        return;
    }

    int displayIndex = SDL_GetWindowDisplayIndex(fenetre);
    SDL_Rect displayBounds;
    SDL_GetDisplayBounds(displayIndex, &displayBounds);
    SDL_DisplayMode mode;
    SDL_GetCurrentDisplayMode(displayIndex, &mode);

    // Souris
    float mouse_x_univers = gestionnaire->entrees->mouse_x; 
    float mouse_y_univers = gestionnaire->entrees->mouse_y;

    if (plein_ecran) {
        dec_x = 0;
        dec_y = 0;
        largeur_actuelle = largeur_base * coeff_minimise;
        hauteur_actuelle = hauteur_base * coeff_minimise;

        SDL_SetWindowFullscreen(fenetre, 0);
        SDL_SetWindowSize(fenetre, largeur_actuelle, hauteur_actuelle);
        SDL_SetWindowPosition(fenetre,
            displayBounds.x + (mode.w - largeur_actuelle) / 2,
            displayBounds.y + (mode.h - hauteur_actuelle) / 2
        );
        SDL_SetWindowBordered(fenetre, SDL_TRUE);

    } else {
        int coeff_l = mode.w / largeur_base;
        int coeff_h = mode.h / hauteur_base;
        int coeff = (coeff_l < coeff_h) ? coeff_l : coeff_h; // coefficient entier

        largeur_actuelle = largeur_base * coeff;
        hauteur_actuelle = hauteur_base * coeff;

        // Décalage centré (int)
        dec_x = (mode.w - largeur_actuelle) / 2;
        dec_y = (mode.h - hauteur_actuelle) / 2;

        SDL_SetWindowFullscreen(fenetre, SDL_WINDOW_FULLSCREEN_DESKTOP);
    }

    plein_ecran = !plein_ecran;

    // Recalcul souris dans l'univers (inchangé)
    float coeff_apres_l = (float)largeur_actuelle / (float)largeur_base;
    float coeff_apres_h = (float)hauteur_actuelle / (float)hauteur_base;

    int mouse_x_screen = (int)lround(mouse_x_univers * coeff_apres_l + dec_x);
    int mouse_y_screen = (int)lround(mouse_y_univers * coeff_apres_h + dec_y);

    SDL_WarpMouseInWindow(fenetre, mouse_x_screen, mouse_y_screen);
    if (gestionnaire->entrees) {
        gestionnaire->entrees->mouse_x = mouse_x_univers;
        gestionnaire->entrees->mouse_y = mouse_y_univers;
    }
    
    // Sauvegarde
    gestionnaire->largeur_actuel = largeur_actuelle;
    gestionnaire->hauteur_actuel = hauteur_actuelle;
    gestionnaire->decalage_x     = dec_x;
    gestionnaire->decalage_y     = dec_y;
    gestionnaire->plein_ecran    = plein_ecran;
}

