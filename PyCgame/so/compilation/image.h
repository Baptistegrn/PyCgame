#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <SDL.h>
#include <SDL_image.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL_mixer.h>



#define TAILLE_LIEN_GT 256



extern bool debug;



#ifdef _WIN32
  #ifdef JEU_BUILD_DLL
    #define JEU_API __declspec(dllexport)
  #else
    #define JEU_API __declspec(dllimport)
  #endif
#else
  #define JEU_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

//textures
typedef struct TextureEntry {
    char id[TAILLE_LIEN_GT];
    SDL_Texture *texture;
} TextureEntry;

typedef struct GestionnaireTextures {
    TextureEntry *entrees;
    int taille;
    int capacite;
    SDL_Renderer *rendu;
} GestionnaireTextures;


//son
typedef struct SonEntry {
    char id[TAILLE_LIEN_GT];
    Mix_Chunk *son;
} SonEntry;

typedef struct GestionnaireSon {
    SonEntry *entrees;
    int taille;
    int capacite;
} GestionnaireSon;

typedef struct {
    int x;
    int y;
} StickPos;

typedef struct {
    StickPos left;   
    StickPos right;  
} ControllerEtatJoy;
typedef struct{
    int triggerleft;
    int triggerright;
} Trigger;

typedef struct GestionnaireEntrees {
    int  mouse_x, mouse_y;
    bool mouse_pressed;
    bool mouse_just_pressed;
    bool mouse_right_pressed;
    bool mouse_right_just_pressed;


    bool keys[SDL_NUM_SCANCODES];
    bool keys_pressed[SDL_NUM_SCANCODES];


    bool controller[SDL_CONTROLLER_BUTTON_MAX];
    bool controller_pressed[SDL_CONTROLLER_BUTTON_MAX];
    ControllerEtatJoy Joy;
    Trigger trigger;

} GestionnaireEntrees;

typedef struct {
    int id;
    float posx, posy;
    float taillex, tailley;
    int   sens;int rotation;
    SDL_Texture *texture;
} image;

typedef struct Tableau_image {
    image *tab;
    int nb_images;
    int capacite_images;
} Tableau_image;

typedef struct fond_actualiser {
    int r, g, b;
    bool dessiner;    bool bande_noir;
} fond_actualiser;

typedef struct Gestionnaire {
    // a mettre dans une struct
    bool run;
    float dt;
    float fps;
    int   largeur, hauteur;
    int   coeff_minimise;
    int   largeur_actuel, hauteur_actuel;
    float decalage_x,decalage_y;
    bool  plein_ecran;
    Uint32       temps_frame;
    // la aussi
    SDL_Window  *fenetre;
    SDL_Renderer*rendu;

    fond_actualiser       *fond;
    Tableau_image         *image;
    GestionnaireEntrees   *entrees;
    GestionnaireTextures  *textures;
    GestionnaireSon *sons;
    SDL_GameController* controller;
    SDL_Joystick *joystick;
} Gestionnaire;
//callback
typedef void (*UpdateCallback)(Gestionnaire *jeu);


JEU_API void set_update_callback(UpdateCallback cb);
JEU_API Gestionnaire* initialisation(int hauteur, int largeur, float fps, int coeff, char *lien_image,char *lien_son,
                                     bool dessiner,bool bande_noir, int r, int g, int b,const char *nom_fenetre,bool debug);
JEU_API void update(Gestionnaire *jeu);

JEU_API void mettre_debug(bool d);
JEU_API void colorier(Gestionnaire *gestionnaire ,int r, int g,int b);


JEU_API int ajouter_image_au_tableau(Gestionnaire *gestionnaire, const char *id,
                                     float x, float y, float w, float h, int sens, int id_num,int rotation);

JEU_API void supprimer_images_par_id(Gestionnaire *gestionnaire, int id_supprimer);
JEU_API void modifier_images(Gestionnaire *jeu, float x, float y, float w, float h, int sens, int id_num,int rotation);

JEU_API void modifier_images_batch(Gestionnaire *jeu, float* x, float* y,
                             float* w, float *h, int *sens, int *id_num,int *rotate,int taille);
JEU_API void supprimer_images_par_id_batch(Gestionnaire *jeu, int *id_supprimer,int taille); 
JEU_API void modifier_texture_image_batch(Gestionnaire *jeu, const char **lien, int *id,int taille);
JEU_API void modifier_texture_image(Gestionnaire *jeu, const char *lien, int id);

JEU_API bool touche_juste_presse(Gestionnaire *jeu, const char *touche);
JEU_API bool touche_enfoncee(Gestionnaire *jeu, const char *touche);

JEU_API bool touche_mannette_enfoncee(Gestionnaire *jeu , const char *touche);
JEU_API bool touche_mannette_juste_presse(Gestionnaire *jeu , const char *touche);
JEU_API void init_controller(Gestionnaire *jeu,int index);
JEU_API void fermer_controller(Gestionnaire *jeu);

JEU_API float* renvoie_joysticks(GestionnaireEntrees *entrees,float dead_zone);
JEU_API void fermer_joystick(Gestionnaire *jeu);


JEU_API void redimensionner_fenetre(Gestionnaire *gestionnaire);
JEU_API void boucle_principale(Gestionnaire *jeu);
JEU_API void ajouter_mot_dans_tableau(Gestionnaire *jeu, int id, const char *chemin, 
                              const char *mot, float posx, float posy, float coeff, 
                              int sens, float ecart,int decalage);

JEU_API int random_jeu(int min, int max);

JEU_API void jouer_son(Gestionnaire *gestionnaire, const char *lien, int boucle, int canal);
JEU_API void arreter_canal(int canal);
JEU_API void arreter_son(Gestionnaire *gestionnaire, const char *lien);
JEU_API void pause_canal(int canal);
JEU_API void pause_son(Gestionnaire *gestionnaire, const char *lien);
JEU_API void reprendre_canal(int canal);
JEU_API void reprendre_son(Gestionnaire *gestionnaire, const char *lien);


JEU_API void liberer_jeu(Gestionnaire *jeu);

JEU_API void ecrire_dans_console(const char *mot);
// Fonctions de base
JEU_API double abs_val(double x);
JEU_API double clamp(double x, double min, double max);

// Puissances et racines
JEU_API double pow_custom(double base, double exp);
JEU_API double sqrt_custom(double x);
JEU_API double cbrt_custom(double x);

// Logarithmes et exponentielles
JEU_API double exp_custom(double x);
JEU_API double log_custom(double x);
JEU_API double log10_custom(double x);
JEU_API double log2_custom(double x);

// Trigonométrie
JEU_API double sin_custom(double x);
JEU_API double cos_custom(double x);
JEU_API double tan_custom(double x);
JEU_API double asin_custom(double x);
JEU_API double acos_custom(double x);
JEU_API double atan_custom(double x);
JEU_API double atan2_custom(double y, double x);

// Hyperboliques
JEU_API double sinh_custom(double x);
JEU_API double cosh_custom(double x);
JEU_API double tanh_custom(double x);
JEU_API double asinh_custom(double x);
JEU_API double acosh_custom(double x);
JEU_API double atanh_custom(double x);

// Arrondis et manipulation
JEU_API double floor_custom(double x);
JEU_API double ceil_custom(double x);
JEU_API double round_custom(double x);
JEU_API double trunc_custom(double x);

// Divers
JEU_API double fmod_custom(double x, double y);
JEU_API double hypot_custom(double x, double y);








int fenetre_init(Gestionnaire *gestionnaire,const char *nom_fenetre);


void actualiser(Gestionnaire *jeu, bool colorier,bool bande_noir, int r, int g, int b);



void afficher_images(Gestionnaire *gestionnaire);

SDL_Texture* recuperer_texture_par_lien(GestionnaireTextures *gt, const char *lien);
Mix_Chunk* recuperer_son_par_lien(GestionnaireSon *gs, const char *lien);
void input_update(Gestionnaire *jeu, GestionnaireEntrees *entrees);
SDL_Scancode scancode_depuis_nom(const char *nom);

void init_gestionnaire_son(GestionnaireSon *gs);
void charger_tous_les_sons(GestionnaireSon *gs, const char *dossier);
void init_gestionnaire_textures(GestionnaireTextures *gt, SDL_Renderer *rendu);
void charger_toutes_les_textures(GestionnaireTextures *gt, const char *dossier);


int ends_with_png(const char *name);
void normaliser_chemin(char *chemin);
int collect_pngs(const char *dir, char ***out_list, int *out_count) ;

int collect_wavs(const char *dir, char ***out_list, int *out_count) ;
int ends_with_wav(const char *name);

void free_tab_images(Gestionnaire *gestionnaire);
void liberer_gestionnaire_son(GestionnaireSon *gs);
void liberer_gestionnaire_image(GestionnaireTextures *gs);
void free_gestionnaire(Gestionnaire *jeu);
#ifdef __cplusplus
}
#endif

#endif 
