#define JEU_BUILD_DLL
#include "image.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL_mixer.h>

void init_gestionnaire_son(GestionnaireSon *gs) {
    if (!gs) {
        if (debug) fprintf(stderr, "DEBUG: init_gestionnaire_son argument invalide\n");
        return;
    }
    gs->capacite = 50;
    gs->taille = 0;
    gs->entrees = malloc(sizeof(SonEntry) * gs->capacite);
    if (!gs->entrees) {
        if (debug) fprintf(stderr, "DEBUG: malloc gestionnaire son failed\n");
    } else if (debug) fprintf(stderr, "DEBUG: gestionnaire son initialise capacite=%d\n", gs->capacite);
}

static void agrandir_si_plein_son(GestionnaireSon *gs) {
    if (!gs) {
        if (debug) fprintf(stderr, "DEBUG: agrandir_si_plein_son argument invalide\n");
        return;
    }
    if (gs->taille >= gs->capacite) {
        int old_cap = gs->capacite;
        gs->capacite += 50;
        SonEntry *tmp = realloc(gs->entrees, sizeof(SonEntry) * gs->capacite);
        if (tmp) {
            gs->entrees = tmp;
            if (debug) fprintf(stderr, "DEBUG: realloc gestionnaire son reussi ancien_cap=%d nouveau_cap=%d\n", old_cap, gs->capacite);
        } else if (debug) fprintf(stderr, "DEBUG: realloc gestionnaire son failed\n");
    }
}

Mix_Chunk* charger_un_son(GestionnaireSon *gs, const char *lien_complet) {
    if (!gs || !lien_complet) {
        if (debug) fprintf(stderr, "DEBUG: charger_un_son argument invalide\n");
        return NULL;
    }

    Mix_Chunk *son = Mix_LoadWAV(lien_complet);
    if (!son) {
        if (debug) fprintf(stderr, "DEBUG: Mix_LoadWAV failed lien=%s erreur=%s\n", lien_complet, Mix_GetError());
        return NULL;
    }

    agrandir_si_plein_son(gs);
    int index = gs->taille++;
    SonEntry *entree = &gs->entrees[index];
    strncpy(entree->id, lien_complet, TAILLE_LIEN_GT - 1);
    entree->id[TAILLE_LIEN_GT - 1] = '\0';
    entree->son = son;

    if (debug) fprintf(stderr, "DEBUG: son charge id=%s index=%d\n", entree->id, index);
    return son;
}

Mix_Chunk* recuperer_son_par_lien(GestionnaireSon *gs, const char *lien) {
    if (!gs || !lien) {
        if (debug) fprintf(stderr, "DEBUG: recuperer_son_par_lien argument invalide\n");
        return NULL;
    }

    for (int i = 0; i < gs->taille; i++) {
        SonEntry *entree = &gs->entrees[i];
        if (strcmp(entree->id, lien) == 0) {
            if (debug) fprintf(stderr, "DEBUG: son trouve lien=%s index=%d\n", lien, i);
            return entree->son;
        }
    }

    if (debug) fprintf(stderr, "DEBUG: son non trouve lien=%s\n", lien);
    return NULL;
}

void charger_tous_les_sons(GestionnaireSon *gs, const char *dossier) {
    if (!gs || !dossier) {
        if (debug) fprintf(stderr, "DEBUG: charger_tous_les_sons argument invalide\n");
        return;
    }

    char **liste_sons = NULL;
    int nb = 0;

    if (collect_wavs(dossier, &liste_sons, &nb) != 0) {
        if (debug) fprintf(stderr, "DEBUG: collecte fichiers WAV echouee dossier=%s\n", dossier);
        return;
    }

    if (debug) fprintf(stderr, "DEBUG: collecte terminee nb=%d dossier=%s\n", nb, dossier);

    for (int i = 0; i < nb; i++) {
        Mix_Chunk *son = charger_un_son(gs, liste_sons[i]);
        if (!son && debug) fprintf(stderr, "DEBUG: erreur chargement son %s\n", liste_sons[i]);
        free(liste_sons[i]);
    }

    free(liste_sons);
}
