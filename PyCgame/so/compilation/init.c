#define JEU_BUILD_DLL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>


#include <time.h>

#include "image.h"

#define TAILLE_CANAL 32



JEU_API Gestionnaire* initialisation(
    int hauteur,
    int largeur,
    float fps,
    int coeff,
    char *lien_image, char *lien_son,
    bool dessiner, bool bande_noir, int r, int g, int b, const char *nom_fenetre,bool debug
) {
    mettre_debug(debug);
    // Rediriger stderr vers un fichier
    FILE *f = freopen("erreurs.log", "w", stderr);
    (void)f; // éviter warning si inutilisé



    srand((unsigned int)time(NULL));
    if(debug)fprintf(stderr, "[DEBUG] Début initialisation\n");


    // Allouer canaux sons
    Mix_AllocateChannels(TAILLE_CANAL); 

    // malloc gestionnaire
    Gestionnaire *jeu = (Gestionnaire*)malloc(sizeof(Gestionnaire));
    if (!jeu) {
        if(debug)fprintf(stderr, "ERREUR: allocation Gestionnaire échouée\n");
        return NULL;
    }
    memset(jeu, 0, sizeof(Gestionnaire));

    // Champs principaux
    jeu->run = true;
    jeu->fps = fps;
    jeu->hauteur = hauteur;
    jeu->largeur = largeur;
    jeu->coeff_minimise = coeff;
    jeu->controller = NULL;
    // malloc sous-structures
    jeu->fond     = (fond_actualiser*)malloc(sizeof(fond_actualiser));
    jeu->entrees  = (GestionnaireEntrees*)malloc(sizeof(GestionnaireEntrees));
    jeu->image    = (Tableau_image*)malloc(sizeof(Tableau_image));
    jeu->textures = (GestionnaireTextures*)malloc(sizeof(GestionnaireTextures));
    jeu->sons     = (GestionnaireSon*)malloc(sizeof(GestionnaireSon));

    if (!jeu->fond || !jeu->entrees || !jeu->image || !jeu->textures || !jeu->sons) {
        if(debug)fprintf(stderr, "ERREUR: allocation sous-structures échouée\n");
        free_gestionnaire(jeu);
        return NULL;
    }

    memset(jeu->fond, 0, sizeof(fond_actualiser));
    memset(jeu->entrees, 0, sizeof(GestionnaireEntrees));
    memset(jeu->image, 0, sizeof(Tableau_image));
    memset(jeu->textures, 0, sizeof(GestionnaireTextures));
    memset(jeu->sons, 0, sizeof(GestionnaireSon));

    // Fond
    jeu->fond->dessiner = dessiner;
    jeu->fond->bande_noir = bande_noir;
    jeu->fond->r = r;
    jeu->fond->g = g;
    jeu->fond->b = b;

    // Images
    jeu->image->capacite_images = 10;
    jeu->image->nb_images = 0;
    jeu->image->tab = (image*)malloc(sizeof(image) * jeu->image->capacite_images);
    if (!jeu->image->tab) {
        if(debug)fprintf(stderr, "ERREUR: allocation tableau images échouée\n");
        free_gestionnaire(jeu);
        return NULL;
    }
    memset(jeu->image->tab, 0, sizeof(image) * jeu->image->capacite_images);

    // Init fenêtre
    if (fenetre_init(jeu,nom_fenetre) != 0) {
        if(debug)fprintf(stderr, "ERREUR: fenetre_init a échoué\n");
        free_gestionnaire(jeu);
        return NULL;
    }
    if(debug)fprintf(stderr, "[DEBUG] fenetre=%p rendu=%p\n", (void*)jeu->fenetre, (void*)jeu->rendu);

    // Init SDL_image
    int img_flags = IMG_INIT_PNG;
    if ((IMG_Init(img_flags) & img_flags) != img_flags) {
        if(debug)fprintf(stderr, "ERREUR: IMG_Init a échoué: %s\n", IMG_GetError());
        free_gestionnaire(jeu);
        return NULL;
    }
    // init controller
    if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) < 0) {
        if(debug)fprintf(stderr, "ERREUR: SDL_INIT_GAMECONTROLLER a échoué: %s\n", SDL_GetError());
    }
    if (SDL_InitSubSystem(SDL_INIT_JOYSTICK) < 0) {
        fprintf(stderr, "Erreur SDL_INIT_JOYSTICK: %s\n", SDL_GetError());
    }

    // Init SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        if(debug)fprintf(stderr, "ERREUR: Mix_OpenAudio a échoué: %s\n", Mix_GetError());
        free_gestionnaire(jeu);
        return NULL;
    }

    // Init gestionnaires
    if (jeu->rendu) {
        init_gestionnaire_textures(jeu->textures, jeu->rendu);
    } else {
        if(debug)fprintf(stderr, "ERREUR: rendu NULL, impossible d'initialiser les textures\n");
        free_gestionnaire(jeu);
        return NULL;
    }

    init_gestionnaire_son(jeu->sons);

    // Charger ressources
    charger_toutes_les_textures(jeu->textures, lien_image);
    charger_tous_les_sons(jeu->sons, lien_son);

    if(debug)fprintf(stderr, "[DEBUG] Initialisation OK (l=%d h=%d coeff=%d)\n", largeur, hauteur, coeff);

    return jeu;
}


