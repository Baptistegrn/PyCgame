
#define JEU_BUILD_DLL

#include "image.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


float ajouter_char_dans_tableau(Gestionnaire *jeu, int id, const char *lien_image, 
                               char lettre, float posx, float posy, float coeff, int sens,int rotation) {

    char lien_image_lettre[TAILLE_LIEN_GT];
    snprintf(lien_image_lettre, sizeof(lien_image_lettre), "%s/%d.png", lien_image, (unsigned char)lettre);

    SDL_Texture* texture = recuperer_texture_par_lien(jeu->textures, lien_image_lettre);
    if (!texture) {
        fprintf(stderr," erreur ouverture texture lettre : %s\n",lien_image_lettre);
        return 0.0f;
    }

    int texW = 0, texH = 0;
    if (SDL_QueryTexture(texture, NULL, NULL, &texW, &texH) != 0) {
        if(debug)fprintf(stderr,"erreur pour recuperer coordonnees  texture \n");
        return 0.0f;
    }

    float largeur_finale = (float)texW * coeff;
    float hauteur_finale = (float)texH * coeff;

    ajouter_image_au_tableau(jeu, lien_image_lettre, posx, posy, largeur_finale, hauteur_finale, sens, id,rotation);

    return largeur_finale; 
}


JEU_API void ajouter_mot_dans_tableau(Gestionnaire *jeu, int id, const char *chemin, 
                              const char *mot, float posx, float posy, float coeff, 
                              int sens, float ecart, int rotation) {
    int taillechaine = (int)strlen(mot);
    float sum = 0.0f;

    for (int i = 0; i < taillechaine; i++) {
        float largeur = ajouter_char_dans_tableau(jeu, id, chemin, mot[i], 
                                                  posx + sum, posy, coeff, sens,rotation);
        sum += largeur + ecart; 
    }
    return;
}


JEU_API void ecrire_dans_console(const char *mot){
    if(debug)fprintf(stderr,"%s",mot);
    return;


}