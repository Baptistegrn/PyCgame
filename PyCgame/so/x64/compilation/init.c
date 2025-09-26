#define JEU_BUILD_DLL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>




#include "image.h"

#define TAILLE_CANAL 32

static void free_gestionnaire(Gestionnaire *jeu) {
    if (!jeu) return;

    if (jeu->image) {
        if (jeu->image->tab) free(jeu->image->tab);
        free(jeu->image);
    }
    if (jeu->fond) free(jeu->fond);
    if (jeu->entrees) free(jeu->entrees);
    if (jeu->textures) free(jeu->textures);
    if (jeu->sons) free(jeu->sons);

    free(jeu);
}

JEU_API Gestionnaire* initialisation(
    int hauteur,
    int largeur,
    float fps,
    int coeff,
    char *lien_image, char *lien_son,
    bool dessiner, bool bande_noir, int r, int g, int b, const char *nom_fenetre
) {

    FILE *f = freopen("erreurs.log", "w", stderr);
    (void)f; 

    fprintf(stderr, "[DEBUG] Début initialisation\n");


    //son
    Mix_AllocateChannels(TAILLE_CANAL); 


    Gestionnaire *jeu = (Gestionnaire*)malloc(sizeof(Gestionnaire));
    if (!jeu) {
        fprintf(stderr, "ERREUR: allocation Gestionnaire échouée\n");
        return NULL;
    }
    memset(jeu, 0, sizeof(Gestionnaire));


    jeu->run = true;
    jeu->fps = fps;
    jeu->hauteur = hauteur;
    jeu->largeur = largeur;
    jeu->coeff_minimise = coeff;
    jeu->controller = NULL;
    jeu->fond     = (fond_actualiser*)malloc(sizeof(fond_actualiser));
    jeu->entrees  = (GestionnaireEntrees*)malloc(sizeof(GestionnaireEntrees));
    jeu->image    = (Tableau_image*)malloc(sizeof(Tableau_image));
    jeu->textures = (GestionnaireTextures*)malloc(sizeof(GestionnaireTextures));
    jeu->sons     = (GestionnaireSon*)malloc(sizeof(GestionnaireSon));

    if (!jeu->fond || !jeu->entrees || !jeu->image || !jeu->textures || !jeu->sons) {
        fprintf(stderr, "ERREUR: allocation sous-structures échouée\n");
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
        fprintf(stderr, "ERREUR: allocation tableau images échouée\n");
        free_gestionnaire(jeu);
        return NULL;
    }
    memset(jeu->image->tab, 0, sizeof(image) * jeu->image->capacite_images);

    // Init fenêtre
    if (fenetre_init(jeu,nom_fenetre) != 0) {
        fprintf(stderr, "ERREUR: fenetre_init a échoué\n");
        free_gestionnaire(jeu);
        return NULL;
    }
    fprintf(stderr, "[DEBUG] fenetre=%p rendu=%p\n", (void*)jeu->fenetre, (void*)jeu->rendu);

    // Init SDL_image
    int img_flags = IMG_INIT_PNG;
    if ((IMG_Init(img_flags) & img_flags) != img_flags) {
        fprintf(stderr, "ERREUR: IMG_Init a échoué: %s\n", IMG_GetError());
        free_gestionnaire(jeu);
        return NULL;
    }
    if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) < 0) {
        fprintf(stderr, "ERREUR: SDL_INIT_GAMECONTROLLER a échoué: %s\n", SDL_GetError());
    }

    // Init SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        fprintf(stderr, "ERREUR: Mix_OpenAudio a échoué: %s\n", Mix_GetError());
        free_gestionnaire(jeu);
        return NULL;
    }

    // Init gestionnaires
    if (jeu->rendu) {
        init_gestionnaire_textures(jeu->textures, jeu->rendu);
    } else {
        fprintf(stderr, "ERREUR: rendu NULL, impossible d'initialiser les textures\n");
        free_gestionnaire(jeu);
        return NULL;
    }

    init_gestionnaire_son(jeu->sons);

    // Charger ressources
    charger_toutes_les_textures(jeu->textures, lien_image);
    charger_tous_les_sons(jeu->sons, lien_son);

    fprintf(stderr, "[DEBUG] Initialisation OK (l=%d h=%d coeff=%d)\n", largeur, hauteur, coeff);

    return jeu;
}



// Fonction d'init d'une manette
void init_controller(Gestionnaire *jeu , int index) {
    if (SDL_NumJoysticks() <= index) {
        fprintf(stderr, "Erreur: aucune manette disponible à l'index %d\n", index);
        return;
    }

    if (!SDL_IsGameController(index)) {
        fprintf(stderr, "Erreur: l'appareil %d n'est pas une manette reconnue\n", index);
        return;
    }

    SDL_GameController *controller = SDL_GameControllerOpen(index);
    if (!controller) {
        fprintf(stderr, "Erreur: impossible d'ouvrir la manette %d : %s\n", index, SDL_GetError());
        return;
    }

    fprintf(stderr,"Manette %d ouverte: %s\n", index, SDL_GameControllerName(controller));
    jeu->controller = controller;
}


JEU_API void fermer_controller(Gestionnaire *jeu){
    if(jeu->controller)    SDL_GameControllerClose(jeu->controller);

    return;
}