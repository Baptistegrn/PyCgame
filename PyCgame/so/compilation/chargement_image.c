#include "image.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include <SDL_image.h>

void init_gestionnaire_textures(GestionnaireTextures *gt, SDL_Renderer *rendu) {
    if (!gt || !rendu) {
        if (debug) fprintf(stderr,"DEBUG: init_gestionnaire_textures argument invalide\n");
        return;
    }
    gt->capacite = 50;
    gt->taille = 0;
    gt->rendu = rendu;
    gt->entrees = malloc(sizeof(TextureEntry) * gt->capacite);
    if (!gt->entrees) {
        if (debug) fprintf(stderr,"DEBUG: malloc gestionnaire textures failed\n");
    } else if (debug) fprintf(stderr,"DEBUG: gestionnaire textures initialise capacite=%d\n", gt->capacite);
}

static void agrandir_si_plein(GestionnaireTextures *gt) {
    if (!gt) {
        if (debug) fprintf(stderr,"DEBUG: agrandir_si_plein argument invalide\n");
        return;
    }
    if (gt->taille >= gt->capacite) {
        int old_cap = gt->capacite;
        gt->capacite += 50;
        TextureEntry *tmp = realloc(gt->entrees, sizeof(TextureEntry) * gt->capacite);
        if (tmp) {
            gt->entrees = tmp;
            if (debug) fprintf(stderr,"DEBUG: realloc reussi ancien_cap=%d nouveau_cap=%d\n", old_cap, gt->capacite);
        } else if (debug) fprintf(stderr,"DEBUG: realloc gestionnaire textures failed\n");
    }
}

SDL_Texture *charger_une_texture(GestionnaireTextures *gt, const char *lien_complet) {
    if (!gt || !lien_complet) {
        if (debug) fprintf(stderr,"DEBUG: charger_une_texture argument invalide\n");
        return NULL;
    }

    SDL_Surface *surface = IMG_Load(lien_complet);
    if (!surface) {
        if (debug) fprintf(stderr,"DEBUG: IMG_Load failed lien=%s erreur=%s\n", lien_complet, IMG_GetError());
        return NULL;
    }

    SDL_Texture *tex = SDL_CreateTextureFromSurface(gt->rendu, surface);
    SDL_FreeSurface(surface);
    if (!tex) {
        if (debug) fprintf(stderr,"DEBUG: SDL_CreateTextureFromSurface failed lien=%s erreur=%s\n", lien_complet, SDL_GetError());
        return NULL;
    }

    agrandir_si_plein(gt);
    int index = gt->taille++;
    TextureEntry *entree = &gt->entrees[index];
    strncpy(entree->id, lien_complet, TAILLE_LIEN_GT - 1);
    entree->id[TAILLE_LIEN_GT - 1] = '\0';
    entree->texture = tex;
    if (debug) fprintf(stderr,"DEBUG: texture chargee id=%s index=%d\n", entree->id, index);

    return tex;
}

void charger_toutes_les_textures(GestionnaireTextures *gt, const char *dossier) {
    if (!gt || !dossier) {
        if (debug) fprintf(stderr,"DEBUG: charger_toutes_les_textures argument invalide\n");
        return;
    }

    char **liste_textures = NULL;
    int nb = 0;

    if (collect_pngs(dossier, &liste_textures, &nb) != 0) {
        if (debug) fprintf(stderr,"DEBUG: collecte fichiers PNG echouee dossier=%s\n", dossier);
        return;
    }

    if (debug) fprintf(stderr,"DEBUG: collecte terminee nb=%d dossier=%s\n", nb, dossier);

    for (int i = 0; i < nb; i++) {
        SDL_Texture *tex = charger_une_texture(gt, liste_textures[i]);
        if (!tex && debug) fprintf(stderr,"DEBUG: erreur chargement texture %s\n", liste_textures[i]);
        free(liste_textures[i]);
    }

    free(liste_textures);
}

SDL_Texture* recuperer_texture_par_lien(GestionnaireTextures *gt, const char *lien) {
    if (!gt || !lien) {
        if (debug) fprintf(stderr,"DEBUG: recuperer_texture_par_lien argument invalide\n");
        return NULL;
    }

    for (int i = 0; i < gt->taille; i++) {
        TextureEntry *entree = &gt->entrees[i];
        if (strcmp(entree->id, lien) == 0) {
            if (debug) fprintf(stderr,"DEBUG: texture trouvee lien=%s index=%d\n", lien, i);
            return entree->texture;
        }
    }

    if (debug) fprintf(stderr,"DEBUG: texture non trouvee lien=%s\n", lien);
    return NULL;
}
