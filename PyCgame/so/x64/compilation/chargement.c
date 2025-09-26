#include "image.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include <dirent.h>     
#include <sys/stat.h>   
#include <unistd.h>    
#include <limits.h>     
#include <errno.h>     



void init_gestionnaire_textures(GestionnaireTextures *gt, SDL_Renderer *rendu){
    if(!gt || !rendu){
        fprintf(stderr,"DEBUG: init_gestionnaire_textures argument invalide\n");
        return;
    }
    gt->capacite = 50;
    gt->taille = 0;
    gt->rendu = rendu;
    gt->entrees = malloc(sizeof(TextureEntry)*gt->capacite);
    if(!gt->entrees){
        fprintf(stderr,"DEBUG: malloc gestionnaire textures failed\n");
    } else {
        fprintf(stderr,"DEBUG: gestionnaire textures initialise capacite=%d\n", gt->capacite);
    }
}

static void agrandir_si_plein(GestionnaireTextures *gt){
    if(!gt){
        fprintf(stderr,"DEBUG: agrandir_si_plein argument invalide\n");
        return;
    }
    if(gt->taille >= gt->capacite){
        int old_cap = gt->capacite;
        gt->capacite += 50;
        TextureEntry *tmp = realloc(gt->entrees,sizeof(TextureEntry)*gt->capacite);
        if(tmp){
            gt->entrees = tmp;
            fprintf(stderr,"DEBUG: realloc reussi ancien_cap=%d nouveau_cap=%d\n", old_cap, gt->capacite);
        } else {
            fprintf(stderr,"DEBUG: realloc gestionnaire textures failed\n");
        }
    }
}

SDL_Texture *charger_une_texture(GestionnaireTextures *gt, const char *lien_complet){
    if(!gt || !lien_complet){
        fprintf(stderr,"DEBUG: charger_une_texture argument invalide\n");
        return NULL;
    }

    SDL_Surface *surface = IMG_Load(lien_complet);
    if(!surface){
        fprintf(stderr,"DEBUG: IMG_Load failed lien=%s erreur=%s\n", lien_complet, IMG_GetError());
        return NULL;
    }

    SDL_Renderer *rendu = gt->rendu;
    SDL_Texture *tex = SDL_CreateTextureFromSurface(rendu, surface);
    SDL_FreeSurface(surface);

    if(!tex){
        fprintf(stderr,"DEBUG: SDL_CreateTextureFromSurface failed lien=%s erreur=%s\n", lien_complet, SDL_GetError());
        return NULL;
    }

    agrandir_si_plein(gt);
    int index = gt->taille++;
    TextureEntry *entree = &gt->entrees[index];
    strncpy(entree->id, lien_complet, TAILLE_LIEN_GT-1);
    entree->id[TAILLE_LIEN_GT-1] = '\0';
    entree->texture = tex;

    fprintf(stderr,"DEBUG: texture chargee id=%s index=%d\n", entree->id, index);
    return tex;
}










static int ends_with_png(const char *name){
    if(!name) return 0;
    size_t len = strlen(name);
    if(len <4) return 0;
    const char *ext = name+len-4;
    return (tolower(ext[0])=='.' && tolower(ext[1])=='p' && tolower(ext[2])=='n' && tolower(ext[3])=='g');
}





static int collect_pngs(const char *dir, char ***out_list, int *out_count) {
    if (!dir || !out_list || !out_count) {
        fprintf(stderr, "DEBUG: collect_pngs argument invalide\n");
        return -1;
    }
    //ouverture de dossier
    DIR *dp = opendir(dir);
    if (!dp) {
        fprintf(stderr, "DEBUG: opendir failed dir=%s: %s\n", dir, strerror(errno));
        return -1;
    }
    
    struct dirent *entry;
    while ((entry = readdir(dp)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        char fullpath[PATH_MAX];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", dir, entry->d_name);

        struct stat st;
        if (stat(fullpath, &st) == -1) {
            fprintf(stderr, "DEBUG: stat failed %s: %s\n", fullpath, strerror(errno));
            continue;
        }

        if (S_ISDIR(st.st_mode)) {
            // Appel récursif
            collect_pngs(fullpath, out_list, out_count);
        } else {
            if (ends_with_png(entry->d_name)) {
                char **tmp = realloc(*out_list, sizeof(char *) * (*out_count + 1));
                if (!tmp) {
                    closedir(dp);
                    fprintf(stderr, "DEBUG: realloc collect_pngs failed\n");
                    return -1;
                }
                *out_list = tmp;
                (*out_list)[*out_count] = strdup(fullpath);
                if (!(*out_list)[*out_count]) {
                    closedir(dp);
                    fprintf(stderr, "DEBUG: strdup failed\n");
                    return -1;
                }
                fprintf(stderr, "DEBUG: png trouve %s\n", fullpath);
                (*out_count)++;
            }
        }
    }

    closedir(dp);
    return 0;
}








void charger_toutes_les_textures(GestionnaireTextures *gt, const char *dossier){
    if(!gt || !dossier){
        fprintf(stderr,"DEBUG: charger_toutes_les_textures argument invalide\n");
        return;
    }
    //  tableau des liens des textures
    char **liste_textures = NULL;
    int nb = 0;
    // on charge la 
    if(collect_pngs(dossier,&liste_textures,&nb)!=0){
        fprintf(stderr,"DEBUG: collecte fichiers PNG echouee dossier=%s\n",dossier);
        return;
    }
    // debug pour verifier si on a bien charge la ou il faut
    fprintf(stderr,"DEBUG: collecte terminee nb=%d dossier=%s\n", nb, dossier);

    for(int i=0;i<nb;i++){
        // on charge
        SDL_Texture *tex = charger_une_texture(gt,liste_textures[i]);
        if(!tex) fprintf(stderr,"DEBUG: erreur chargement texture %s\n", liste_textures[i]);
        free(liste_textures[i]);
    }

    free(liste_textures);
}

SDL_Texture* recuperer_texture_par_lien(GestionnaireTextures *gt, const char *lien){
    if(!gt || !lien){
        fprintf(stderr,"DEBUG: recuperer_texture_par_lien argument invalide\n");
        return NULL;
    }

    int taille = gt->taille;
    for(int i=0;i<taille;i++){
        TextureEntry *entree = &gt->entrees[i];
        if(strcmp(entree->id,lien)==0){
            fprintf(stderr,"DEBUG: texture trouvee lien=%s index=%d\n", lien, i);
            return entree->texture;
        }
    }
    fprintf(stderr,"DEBUG: texture non trouvee lien=%s\n", lien);
    return NULL;
}


