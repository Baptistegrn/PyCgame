#define JEU_BUILD_DLL
#include "image.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h> 



JEU_API void init_controller(Gestionnaire *jeu , int index) {
    if (SDL_NumJoysticks() <= index) {
        if(debug)fprintf(stderr, "Erreur: aucune manette disponible à l'index %d\n", index);
        return;
    }

    if (!SDL_IsGameController(index)) {
        if(debug)fprintf(stderr, "Erreur: l'appareil %d n'est pas une manette reconnue\n", index);
        return;
    }

    SDL_GameController *controller = SDL_GameControllerOpen(index);
    //controller
    if (!controller) {
        if(debug)fprintf(stderr, "Erreur: impossible d'ouvrir la manette %d : %s\n", index, SDL_GetError());
        return;
    }
    //joy
    if (SDL_NumJoysticks() > 0) {
        SDL_Joystick *joy = SDL_JoystickOpen(0);

    if(debug)fprintf(stderr,"Manette %d ouverte: %s\n", index, SDL_GameControllerName(controller));
    jeu->controller = controller;
    jeu->joystick = joy;
}
}


JEU_API void fermer_controller(Gestionnaire *jeu){
    if(jeu->controller)    SDL_GameControllerClose(jeu->controller);

    return;
}

JEU_API void fermer_joystick(Gestionnaire *jeu) {

    if (jeu->joystick) SDL_JoystickClose(jeu->joystick);

    return;
    
}



JEU_API float* renvoie_joysticks(GestionnaireEntrees *entrees,float dead_zone) {
    if (!entrees) {if(debug)fprintf(stderr,"erreur gestionnaire entrees non initialise\n");return NULL;}

    float *tab = malloc(sizeof(int) * 6);
    if (!tab) {if(debug)fprintf(stderr,"erreur allocation tableau de 6 cases\n");return NULL;}

    // Stick gauche
    tab[0] = (float)((float)entrees->Joy.left.x/32766.0f);
    tab[1] = (float)((float)entrees->Joy.left.y/32766.0f);

    // Stick droit
    tab[2] =(float) ((float)entrees->Joy.right.x/32766.0f);
    tab[3] =(float) ((float)entrees->Joy.right.y/32766.0f);

    // Triggers
    tab[4] =(float)  ((float)entrees->trigger.triggerleft/32766.0f);
    tab[5] =(float) ((float) entrees->trigger.triggerright/32766.0f);

    for(int i =0 ; i<6 ; i++ )if(fabsf(tab[i])<dead_zone)tab[i]=0;



    return tab;
}