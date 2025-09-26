#define JEU_BUILD_DLL


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
void init_gestionnaire_son(GestionnaireSon *gs){
    if(!gs){
        fprintf(stderr,"DEBUG: init_gestionnaire_son argument invalide\n");
        return;
    }
    gs->capacite = 50;
    gs->taille = 0;
    gs->entrees = malloc(sizeof(SonEntry)*gs->capacite);
    if(!gs->entrees){
        fprintf(stderr,"DEBUG: malloc gestionnaire son failed\n");
    } else {
        fprintf(stderr,"DEBUG: gestionnaire son initialise capacite=%d\n", gs->capacite);
    }
}
static void agrandir_si_plein_son(GestionnaireSon *gs){
    if(!gs){
        fprintf(stderr,"DEBUG: agrandir_si_plein_son argument invalide\n");
        return;
    }
    if(gs->taille >= gs->capacite){
        int old_cap = gs->capacite;
        gs->capacite += 50;
        SonEntry *tmp = realloc(gs->entrees,sizeof(SonEntry)*gs->capacite);
        if(tmp){
            gs->entrees = tmp;
            fprintf(stderr,"DEBUG: realloc gestionnaire son reussi ancien_cap=%d nouveau_cap=%d\n", old_cap, gs->capacite);
        } else {
            fprintf(stderr,"DEBUG: realloc gestionnaire son failed\n");
        }
    }
}

Mix_Chunk* charger_un_son(GestionnaireSon *gs, const char *lien_complet){
    if(!gs || !lien_complet){
        fprintf(stderr,"DEBUG: charger_un_son argument invalide\n");
        return NULL;
    }

    Mix_Chunk *son = Mix_LoadWAV(lien_complet);
    if(!son){
        fprintf(stderr,"DEBUG: Mix_LoadWAV failed lien=%s erreur=%s\n", lien_complet, Mix_GetError());
        return NULL;
    }

    agrandir_si_plein_son(gs);
    int index = gs->taille++;
    SonEntry *entree = &gs->entrees[index];
    strncpy(entree->id, lien_complet, TAILLE_LIEN_GT-1);
    entree->id[TAILLE_LIEN_GT-1] = '\0';
    entree->son = son;

    fprintf(stderr,"DEBUG: son charge id=%s index=%d\n", entree->id, index);
    return son;
}

Mix_Chunk* recuperer_son_par_lien(GestionnaireSon *gs, const char *lien){
    if(!gs || !lien){
        fprintf(stderr,"DEBUG: recuperer_son_par_lien argument invalide\n");
        return NULL;
    }

    int taille = gs->taille;
    for(int i=0;i<taille;i++){
        SonEntry *entree = &gs->entrees[i];
        if(strcmp(entree->id,lien)==0){
            fprintf(stderr,"DEBUG: son trouve lien=%s index=%d\n", lien, i);
            return entree->son;
        }
    }
    fprintf(stderr,"DEBUG: son non trouve lien=%s\n", lien);
    return NULL;
}



void liberer_gestionnaire_son(GestionnaireSon *gs){
    if(!gs) return;
    for(int i=0;i<gs->taille;i++){
        if(gs->entrees[i].son){
            Mix_FreeChunk(gs->entrees[i].son);
        }
    }
    free(gs->entrees);
    gs->entrees=NULL;
    gs->taille=0;
    gs->capacite=0;
    fprintf(stderr,"DEBUG: gestionnaire son libere\n");
}

static int ends_with_wav(const char *name){
    if(!name) return 0;
    size_t len = strlen(name);
    if(len <4) return 0;
    const char *ext = name+len-4;
    return (tolower(ext[0])=='.' && tolower(ext[1])=='w' && tolower(ext[2])=='a' && tolower(ext[3])=='v');
}





static int collect_wavs(const char *dir, char ***out_list, int *out_count) {
    if (!dir || !out_list || !out_count) {
        fprintf(stderr, "DEBUG: collect_wavs argument invalide\n");
        return -1;
    }

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
            collect_wavs(fullpath, out_list, out_count);
        } else if (ends_with_wav(entry->d_name)) {
            char **tmp = realloc(*out_list, sizeof(char*) * (*out_count + 1));
            if (!tmp) {
                closedir(dp);
                fprintf(stderr, "DEBUG: realloc collect_wavs failed\n");
                return -1;
            }
            *out_list = tmp;
            (*out_list)[*out_count] = strdup(fullpath);
            if(!(*out_list)[*out_count]){
                closedir(dp);
                fprintf(stderr,"DEBUG: strdup failed\n");
                return -1;
            }
            fprintf(stderr,"DEBUG: wav trouve %s\n", fullpath);
            (*out_count)++;
        }
    }

    closedir(dp);
    return 0;
}


void charger_tous_les_sons(GestionnaireSon *gs, const char *dossier){
    if(!gs || !dossier){
        fprintf(stderr,"DEBUG: charger_tous_les_sons argument invalide\n");
        return;
    }

    char **liste_sons = NULL;
    int nb = 0;

    if(collect_wavs(dossier,&liste_sons,&nb)!=0){
        fprintf(stderr,"DEBUG: collecte fichiers WAV echouee dossier=%s\n",dossier);
        return;
    }

    fprintf(stderr,"DEBUG: collecte terminee nb=%d dossier=%s\n", nb, dossier);

    for(int i=0;i<nb;i++){
        Mix_Chunk *son = charger_un_son(gs,liste_sons[i]);
        if(!son) fprintf(stderr,"DEBUG: erreur chargement son %s\n", liste_sons[i]);
        free(liste_sons[i]);
    }

    free(liste_sons);
}


JEU_API void jouer_son(Gestionnaire *gestionnaire, const char *lien, int boucle, int canal) {
    if (!gestionnaire || !gestionnaire->sons) {
        fprintf(stderr, "DEBUG: jouer_son gestionnaire ou sons NULL\n");
        return;
    }

    GestionnaireSon *gs = gestionnaire->sons;
    Mix_Chunk *son = recuperer_son_par_lien(gs, lien);

    if (!son) {
        fprintf(stderr, "DEBUG: son introuvable (%s)\n", lien);
        return;
    }

    if (Mix_PlayChannel(canal, son, boucle-1) == -1) {
        fprintf(stderr, "DEBUG: Mix_PlayChannel failed (%s)\n", Mix_GetError());
    }
}


JEU_API void arreter_son(Gestionnaire *gestionnaire, const char *lien) {
    if (!gestionnaire || !gestionnaire->sons) return;

    GestionnaireSon *gs = gestionnaire->sons;
    Mix_Chunk *son = recuperer_son_par_lien(gs, lien);
    if (!son) {
        fprintf(stderr, "DEBUG: arreter_son introuvable (%s)\n", lien);
        return;
    }

    int nb_canaux = Mix_AllocateChannels(-1);
    for (int i = 0; i < nb_canaux; i++) {
        if (Mix_GetChunk(i) == son) {
            Mix_HaltChannel(i);
        }
    }

    fprintf(stderr, "DEBUG: arreter_son fini (%s)\n", lien);
}


JEU_API void arreter_canal(int canal) {
    if (canal < 0) {
        fprintf(stderr, "DEBUG: arreter_canal canal invalide (%d)\n", canal);
        return;
    }

    if (Mix_Playing(canal)) {
        Mix_HaltChannel(canal);
        fprintf(stderr, "DEBUG: canal %d arrete\n", canal);
    } else {
        fprintf(stderr, "DEBUG: canal %d n'est pas en lecture\n", canal);
    }
}


JEU_API void pause_canal(int canal) {
    if (canal < 0) {
        fprintf(stderr, "DEBUG: pause_canal canal invalide (%d)\n", canal);
        return;
    }

    if (Mix_Playing(canal)) {
        Mix_Pause(canal);

        fprintf(stderr, "DEBUG: canal %d pause\n", canal);
    } else {
        fprintf(stderr, "DEBUG: canal %d n'est pas en lecture\n", canal);
    }
}


JEU_API void reprendre_canal(int canal) {
    if (canal < 0) {
        fprintf(stderr, "DEBUG: reprendre_canal canal invalide (%d)\n", canal);
        return;
    }

    Mix_Resume(canal);
}


JEU_API void pause_son(Gestionnaire *gestionnaire, const char *lien) {
    if (!gestionnaire || !gestionnaire->sons) return;

    GestionnaireSon *gs = gestionnaire->sons;
    Mix_Chunk *son = recuperer_son_par_lien(gs, lien);
    if (!son) {
        fprintf(stderr, "DEBUG: pause_son introuvable (%s)\n", lien);
        return;
    }

    int nb_canaux = Mix_AllocateChannels(-1); 
    for (int i = 0; i < nb_canaux; i++) {
        if (Mix_GetChunk(i) == son) {
            Mix_Pause(i);
        }
    }

    fprintf(stderr, "DEBUG: pause_son fini (%s)\n", lien);
}


JEU_API void reprendre_son(Gestionnaire *gestionnaire, const char *lien) {
    if (!gestionnaire || !gestionnaire->sons) return;

    GestionnaireSon *gs = gestionnaire->sons;
    Mix_Chunk *son = recuperer_son_par_lien(gs, lien);
    if (!son) {
        fprintf(stderr, "DEBUG: rependre_son introuvable (%s)\n", lien);
        return;
    }

    int nb_canaux = Mix_AllocateChannels(-1); 
    for (int i = 0; i < nb_canaux; i++) {
        if (Mix_GetChunk(i) == son) {
            Mix_Resume(i);
        }
    }

    fprintf(stderr, "DEBUG: reprendre_son fini (%s)\n", lien);
}