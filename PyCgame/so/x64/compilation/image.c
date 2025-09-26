 #define JEU_BUILD_DLL
#include "image.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h> 



int ajouter_image_au_jeu(Gestionnaire *gestionnaire, image nouvelle) {
    if (!gestionnaire) {
        fprintf(stderr, "ajouter_image_au_jeu: gestionnaire NULL\n");
        return -1;
    }

    Tableau_image *jeu = gestionnaire->image;
    if (!jeu) {
        fprintf(stderr, "ajouter_image_au_jeu: gestionnaire->image NULL\n");
        return -1;
    }

    if (jeu->nb_images >= jeu->capacite_images) {
        int new_cap = (jeu->capacite_images == 0) ? 50 : jeu->capacite_images +50;
        image *tmp = realloc(jeu->tab, sizeof(image) * new_cap);
        if (!tmp) {
            fprintf(stderr, "ajouter_image_au_jeu: erreur realloc images\n");
            return -1;
        }
        jeu->tab = tmp;
        jeu->capacite_images = new_cap;
    }

    jeu->tab[jeu->nb_images] = nouvelle;
    return jeu->nb_images++;
}


JEU_API int ajouter_image_au_tableau(Gestionnaire *gestionnaire, const char *id,
                                     float x, float y, float w, float h,
                                     int sens, int id_num,int rotation) {
    if (!gestionnaire) {
        fprintf(stderr, "ajouter_image_au_tableau: gestionnaire NULL\n");
        return -1;
    }
    if (!gestionnaire->textures) {
        fprintf(stderr, "ajouter_image_au_tableau: textures NULL\n");
        return -1;
    }

    image img;
    memset(&img, 0, sizeof(image));
    img.posx = x;
    img.posy = y;
    img.taillex = w;
    img.tailley = h;
    img.sens = sens;
    img.id = id_num;
    img.rotation = rotation;

    SDL_Texture *tex = recuperer_texture_par_lien(gestionnaire->textures, id);
    if (!tex) {
        fprintf(stderr, "ajouter_image_au_tableau: erreur texture introuvable %s\n", id);
    }
    img.texture = tex;

    return ajouter_image_au_jeu(gestionnaire, img);
}


void afficher_images(Gestionnaire *gestionnaire) {
    if (!gestionnaire) {
        fprintf(stderr, "afficher_images: gestionnaire NULL\n");
        return;
    }
    if (!gestionnaire->rendu) {
        fprintf(stderr, "afficher_images: rendu NULL\n");
        return;
    }

    Tableau_image *jeu = gestionnaire->image;
    if (!jeu) {
        fprintf(stderr, "afficher_images: jeu NULL\n");
        return;
    }

    float coeff_largeur = (float)gestionnaire->largeur_actuel / (float)gestionnaire->largeur;
    float coeff_hauteur = (float)gestionnaire->hauteur_actuel / (float)gestionnaire->hauteur;

    for (int i = 0; i < jeu->nb_images; i++) {
        image *img = &jeu->tab[i];
        if (!img->texture) {
            fprintf(stderr, "afficher_images: image %d sans texture\n", i);
            continue;
        }

        if (img->posx > gestionnaire->largeur || img->posx < -img->taillex ||
            img->posy > gestionnaire->hauteur || img->posy < -img->tailley) {
            continue; 
        }

        SDL_Rect dst = {
            (int)lroundf(img->posx * coeff_largeur + gestionnaire->decalage_x),
            (int)lroundf(img->posy * coeff_hauteur + gestionnaire->decalage_y),
            (int)lroundf(img->taillex * coeff_largeur),
            (int)lroundf(img->tailley * coeff_hauteur)
        };

        SDL_Point centre = { dst.w / 2, dst.h / 2 };

        if (SDL_RenderCopyEx(
                gestionnaire->rendu,
                img->texture,
                NULL,             
                &dst,             
                img->rotation,    
                &centre,          
                (img->sens == 1) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE
            ) != 0) {
            fprintf(stderr, "afficher_images: SDL_RenderCopyEx erreur: %s\n", SDL_GetError());
        }
    }
    return;
}



void free_tab_images(Gestionnaire *gestionnaire) {
    if (!gestionnaire) return;
    if (!gestionnaire->image) return;


    free(gestionnaire->image->tab);
    gestionnaire->image->tab = NULL;
    gestionnaire->image->nb_images = 0;
    gestionnaire->image->capacite_images = 0;
    return;
}




//  dessine des bandes noir si true quand on redimensionne si ce nest pas a lechelle

void dessiner_bandes_noires(SDL_Renderer *rendu, double decalage_x, double decalage_y,
                            int largeur, int hauteur) {
    if (!rendu) {
        fprintf(stderr, "dessiner_bandes_noires: rendu NULL\n");
        return;
    }

    SDL_SetRenderDrawColor(rendu, 10, 10, 10, 255);

    SDL_Rect rect;
    int dx = (int)lround(decalage_x);
    int dy = (int)lround(decalage_y);

    rect = (SDL_Rect){0, 0, dx, hauteur};
    SDL_RenderFillRect(rendu, &rect);

    rect = (SDL_Rect){largeur - dx, 0, dx, hauteur};
    SDL_RenderFillRect(rendu, &rect);

    rect = (SDL_Rect){0, 0, largeur, dy};
    SDL_RenderFillRect(rendu, &rect);

    rect = (SDL_Rect){0, hauteur - dy, largeur, dy};
    SDL_RenderFillRect(rendu, &rect);
    return;
}



void actualiser(Gestionnaire *jeu, bool colorier,bool bande_noir, int r, int g, int b) {
    if (!jeu) {
        fprintf(stderr, "actualiser: jeu NULL\n");
        return;
    }

    SDL_Renderer *rendu = jeu->rendu;
    if (!rendu) {
        fprintf(stderr, "actualiser: rendu NULL\n");
        return;
    }

    if (colorier) {
        SDL_SetRenderDrawColor(rendu, r, g, b, 255);
    }
    SDL_RenderClear(rendu);

    afficher_images(jeu);
    if(bande_noir){
    int largeur, hauteur;
    SDL_GetWindowSize(jeu->fenetre, &largeur, &hauteur);
    dessiner_bandes_noires(rendu, jeu->decalage_x, jeu->decalage_y, largeur, hauteur);}

    SDL_RenderPresent(rendu);
    return;
}


JEU_API void supprimer_images_par_id(Gestionnaire *jeu, int id_supprimer) {
    if (!jeu || !jeu->image) {
        fprintf(stderr, "supprimer_images_par_id: pointeur NULL\n");
        return;
    }

    Tableau_image *tab_img = jeu->image;
    int count = 0;

    for (int i = 0; i < tab_img->nb_images; i++) {
        if (tab_img->tab[i].id != id_supprimer) {
            count++;
        }
    }

    if (count == tab_img->nb_images) {
        return;
    }

    image *nouveau_tab = (count > 0) ? malloc(sizeof(image) * count) : NULL;
    if (count > 0 && !nouveau_tab) {
        fprintf(stderr, "supprimer_images_par_id: malloc echoue\n");
        return;
    }

    int j = 0;
    for (int i = 0; i < tab_img->nb_images; i++) {
        if (tab_img->tab[i].id != id_supprimer) {
            nouveau_tab[j++] = tab_img->tab[i];
        } else {
            tab_img->tab[i].texture = NULL;

        }
    }

    free(tab_img->tab);
    tab_img->tab = nouveau_tab;
    tab_img->nb_images = count;
    tab_img->capacite_images = count;
    return;
}

// renvoie tout les indices pour supp u modifier des textures dans le tab 
int *renvoie_id_indices(Gestionnaire *jeu, int id, int *ptr) {
    if (!jeu || !jeu->image) {
        fprintf(stderr, "renvoie_id_indices: pointeur NULL\n");
        return NULL;
    }

    Tableau_image *tab_img = jeu->image;
    int taille = 0;
    for (int i = 0; i < tab_img->nb_images; i++) {
        if (id == tab_img->tab[i].id) taille++;
    }

    int *tab = malloc(sizeof(int) * taille);
    if (!tab) {
        fprintf(stderr, "renvoie_id_indices: malloc echoue\n");
        return NULL;
    }

    int x = 0;
    for (int i = 0; i < tab_img->nb_images; i++) {
        if (id == tab_img->tab[i].id) {
            tab[x++] = i;
        }
    }

    *ptr = taille;
    return tab;
}


JEU_API void modifier_images(Gestionnaire *jeu, float x, float y,
                             float w, float h, int sens, int id_num,int rotate) {
    if (!jeu || !jeu->image) return;

    int nb_indices = 0;
    int *indices = renvoie_id_indices(jeu, id_num, &nb_indices);
    if (!indices) return;

    for (int i = 0; i < nb_indices; i++) {
        int idx = indices[i];
        jeu->image->tab[idx].posx = x;
        jeu->image->tab[idx].posy = y;
        jeu->image->tab[idx].taillex = w;
        jeu->image->tab[idx].tailley = h;
        jeu->image->tab[idx].sens = sens;
        jeu->image->tab[idx].rotation = rotate;
    }

    free(indices);
    return;
}


JEU_API void modifier_texture_image(Gestionnaire *jeu, const char *lien, int id) {
    if (!jeu || !jeu->image) return;

    int nb_indices = 0;
    int *indices = renvoie_id_indices(jeu, id, &nb_indices);
    if (!indices) return;

    SDL_Texture *tex = recuperer_texture_par_lien(jeu->textures, lien);
    if (!tex) {
        fprintf(stderr, "modifier_texture_image: texture introuvable %s\n", lien);
    }

    for (int i = 0; i < nb_indices; i++) {
        int idx = indices[i];
        jeu->image->tab[idx].texture = tex;
    }

    free(indices);
    return;
}
