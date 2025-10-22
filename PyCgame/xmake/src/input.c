#define JEU_BUILD_DLL

#include "image.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <SDL_image.h>
#include <ctype.h>

#include <maths.h>
void input_update(Gestionnaire *jeu, GestionnaireEntrees *entrees) {
    if (!jeu || !entrees) return;

    SDL_Event event;

    entrees->mouse_just_pressed = false;
    entrees->mouse_right_just_pressed = false; 
    memset(entrees->keys_pressed, false, sizeof(entrees->keys_pressed));
    memset(entrees->controller_pressed, false, sizeof(entrees->controller_pressed));

    while (SDL_PollEvent(&event)) {
        // Souris
        int raw_x, raw_y;
        SDL_GetMouseState(&raw_x, &raw_y);

        float coeff_largeur = (jeu->largeur != 0) ? (float)jeu->largeur_actuel / jeu->largeur : 1.0f;
        float coeff_hauteur = (jeu->hauteur != 0) ? (float)jeu->hauteur_actuel / jeu->hauteur : 1.0f;

        entrees->mouse_x = (int)lroundf((raw_x - jeu->decalage_x) / coeff_largeur);
        entrees->mouse_y = (int)lroundf((raw_y - jeu->decalage_y) / coeff_hauteur);

        switch (event.type) {
            case SDL_QUIT:
                jeu->run = false;
                break;

            case SDL_WINDOWEVENT:
                break;

            case SDL_CONTROLLERBUTTONDOWN:
                if (event.cbutton.button < SDL_CONTROLLER_BUTTON_MAX) {
                    entrees->controller[event.cbutton.button] = true;
                    entrees->controller_pressed[event.cbutton.button] = true;
                }
                break;

            case SDL_CONTROLLERBUTTONUP:
                if (event.cbutton.button < SDL_CONTROLLER_BUTTON_MAX) {
                    entrees->controller[event.cbutton.button] = false;
                }
                break;

            case SDL_CONTROLLERAXISMOTION:
                switch (event.caxis.axis) {
                    case SDL_CONTROLLER_AXIS_LEFTX:
                        entrees->Joy.left.x = event.caxis.value;
                        break;
                    case SDL_CONTROLLER_AXIS_LEFTY:
                        entrees->Joy.left.y = event.caxis.value;
                        break;
                    case SDL_CONTROLLER_AXIS_RIGHTX:
                        entrees->Joy.right.x = event.caxis.value;
                        break;
                    case SDL_CONTROLLER_AXIS_RIGHTY:
                        entrees->Joy.right.y = event.caxis.value;
                        break;
                    case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
                        entrees->trigger.triggerleft = event.caxis.value;
                        break;
                    case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
                        entrees->trigger.triggerright = event.caxis.value;
                        break;
                }
                break;

            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    entrees->mouse_pressed = true;
                    entrees->mouse_just_pressed = true;
                } else if (event.button.button == SDL_BUTTON_RIGHT) {
                    entrees->mouse_right_pressed = true;
                    entrees->mouse_right_just_pressed = true;
                }
                break;

            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    entrees->mouse_pressed = false;
                } else if (event.button.button == SDL_BUTTON_RIGHT) {
                    entrees->mouse_right_pressed = false;
                }
                break;

            case SDL_KEYDOWN:
                if (event.key.keysym.scancode < SDL_NUM_SCANCODES) {
                    entrees->keys[event.key.keysym.scancode] = true;
                    entrees->keys_pressed[event.key.keysym.scancode] = true;
                }
                break;

            case SDL_KEYUP:
                if (event.key.keysym.scancode < SDL_NUM_SCANCODES) {
                    entrees->keys[event.key.keysym.scancode] = false;
                }
                break;

            default:
                break;
        }
    }
}




char* normaliser_nom(const char *src) {
    int taille = strlen(src);
    char *dst = malloc(sizeof(char)*(taille+1));
    for (int i = 0 ; i<taille; i++) { 
        char c = (char)src[i];
        if (c >= 'A' && c <= 'Z') {
            dst[i] = c + 32;
        }
        else dst[i] = src[i];
    }
    dst[taille] = '\0';
    return dst;
}
SDL_GameControllerButton bouton_manette_depuis_nom(const char *nom_non_normalise) {
    char *nom = normaliser_nom(nom_non_normalise);
    if (!nom || strlen(nom) == 0) {
        if(debug)fprintf(stderr, "Erreur: nom de bouton manette NULL ou vide\n");
        return SDL_CONTROLLER_BUTTON_INVALID;
    }

    // boutons principaux
    if (strcmp(nom, "a") == 0) return SDL_CONTROLLER_BUTTON_A;
    if (strcmp(nom, "b") == 0) return SDL_CONTROLLER_BUTTON_B;
    if (strcmp(nom, "x") == 0) return SDL_CONTROLLER_BUTTON_X;
    if (strcmp(nom, "y") == 0) return SDL_CONTROLLER_BUTTON_Y;

    // système
    if (strcmp(nom, "start") == 0) return SDL_CONTROLLER_BUTTON_START;
    if (strcmp(nom, "back") == 0 || strcmp(nom, "select") == 0) return SDL_CONTROLLER_BUTTON_BACK;
    if (strcmp(nom, "guide") == 0 || strcmp(nom, "home") == 0) return SDL_CONTROLLER_BUTTON_GUIDE;

    // sticks cliquables
    if (strcmp(nom, "leftstick") == 0 || strcmp(nom, "l3") == 0) return SDL_CONTROLLER_BUTTON_LEFTSTICK;
    if (strcmp(nom, "rightstick") == 0 || strcmp(nom, "r3") == 0) return SDL_CONTROLLER_BUTTON_RIGHTSTICK;

    // bumpers
    if (strcmp(nom, "lb") == 0 || strcmp(nom, "l1") == 0 || strcmp(nom, "leftshoulder") == 0) return SDL_CONTROLLER_BUTTON_LEFTSHOULDER;
    if (strcmp(nom, "rb") == 0 || strcmp(nom, "r1") == 0 || strcmp(nom, "rightshoulder") == 0) return SDL_CONTROLLER_BUTTON_RIGHTSHOULDER;

    // croix directionnelle
    if (strcmp(nom, "haut") == 0 || strcmp(nom, "up") == 0) return SDL_CONTROLLER_BUTTON_DPAD_UP;
    if (strcmp(nom, "bas") == 0 || strcmp(nom, "down") == 0) return SDL_CONTROLLER_BUTTON_DPAD_DOWN;
    if (strcmp(nom, "gauche") == 0 || strcmp(nom, "left") == 0) return SDL_CONTROLLER_BUTTON_DPAD_LEFT;
    if (strcmp(nom, "droite") == 0 || strcmp(nom, "right") == 0) return SDL_CONTROLLER_BUTTON_DPAD_RIGHT;

    // modernes (PS4/PS5/Xbox Elite)
    if (strcmp(nom, "share") == 0 || strcmp(nom, "capture") == 0) return SDL_CONTROLLER_BUTTON_MISC1;
    if (strcmp(nom, "paddle1") == 0) return SDL_CONTROLLER_BUTTON_PADDLE1;
    if (strcmp(nom, "paddle2") == 0) return SDL_CONTROLLER_BUTTON_PADDLE2;
    if (strcmp(nom, "paddle3") == 0) return SDL_CONTROLLER_BUTTON_PADDLE3;
    if (strcmp(nom, "paddle4") == 0) return SDL_CONTROLLER_BUTTON_PADDLE4;
    if (strcmp(nom, "touchpad") == 0) return SDL_CONTROLLER_BUTTON_TOUCHPAD;

    if(debug)fprintf(stderr, "Erreur: nom de bouton manette inconnu (%s)\n", nom);
    return SDL_CONTROLLER_BUTTON_INVALID;
}

// renvoie a partir de *char la touche sdl
SDL_Scancode scancode_depuis_nom(const char *nom_non_normalise) {
    char *nom = normaliser_nom(nom_non_normalise);
    if (!nom || strlen(nom) == 0) {
        if(debug)fprintf(stderr, "Erreur: nom de touche NULL ou vide\n");
        return SDL_SCANCODE_UNKNOWN;
    }

if (strcmp(nom, "backspace") == 0 || strcmp(nom, "retour") == 0 || strcmp(nom, "effacer") == 0) 
    return SDL_SCANCODE_BACKSPACE;

// Modificateurs droits
if (strcmp(nom, "rshift") == 0 || strcmp(nom, "majdroit") == 0) return SDL_SCANCODE_RSHIFT;
if (strcmp(nom, "rctrl") == 0 || strcmp(nom, "ctrldroit") == 0) return SDL_SCANCODE_RCTRL;

// Touches Windows/Command
if (strcmp(nom, "windows") == 0 || strcmp(nom, "win") == 0 || strcmp(nom, "cmd") == 0 || strcmp(nom, "command") == 0) 
    return SDL_SCANCODE_LGUI;
if (strcmp(nom, "rwindows") == 0 || strcmp(nom, "rwin") == 0 || strcmp(nom, "rcmd") == 0) 
    return SDL_SCANCODE_RGUI;

// Ponctuation et symboles
if (strcmp(nom, "-") == 0 || strcmp(nom, "moins") == 0) return SDL_SCANCODE_MINUS;
if (strcmp(nom, "=") == 0 || strcmp(nom, "egal") == 0) return SDL_SCANCODE_EQUALS;
if (strcmp(nom, "[") == 0) return SDL_SCANCODE_LEFTBRACKET;
if (strcmp(nom, "]") == 0) return SDL_SCANCODE_RIGHTBRACKET;
if (strcmp(nom, "\\") == 0) return SDL_SCANCODE_BACKSLASH;
if (strcmp(nom, ";") == 0 || strcmp(nom, "pointvirgule") == 0) return SDL_SCANCODE_SEMICOLON;
if (strcmp(nom, "'") == 0 || strcmp(nom, "apostrophe") == 0) return SDL_SCANCODE_APOSTROPHE;
if (strcmp(nom, "`") == 0) return SDL_SCANCODE_GRAVE;
if (strcmp(nom, ",") == 0 || strcmp(nom, "virgule") == 0) return SDL_SCANCODE_COMMA;
if (strcmp(nom, ".") == 0 || strcmp(nom, "point") == 0) return SDL_SCANCODE_PERIOD;
if (strcmp(nom, "/") == 0 || strcmp(nom, "slash") == 0) return SDL_SCANCODE_SLASH;

// Pavé numérique (complément)
if (strcmp(nom, "kp=") == 0) return SDL_SCANCODE_KP_EQUALS;
        // Touches spéciales
    if (strcmp(nom, "espace") == 0 || strcmp(nom, "space") == 0) return SDL_SCANCODE_SPACE;
    if (strcmp(nom, "entrer") == 0 || strcmp(nom, "return") == 0) return SDL_SCANCODE_RETURN;

    if (strcmp(nom, "echap") == 0 || strcmp(nom, "escape") == 0) return SDL_SCANCODE_ESCAPE;
    if (strcmp(nom, "tab") == 0) return SDL_SCANCODE_TAB;
    if (strcmp(nom, "maj") == 0 || strcmp(nom, "shift") == 0) return SDL_SCANCODE_LSHIFT;
    if (strcmp(nom, "ctrl") == 0 || strcmp(nom, "control") == 0) return SDL_SCANCODE_LCTRL;
    if (strcmp(nom, "alt") == 0) return SDL_SCANCODE_LALT;
    if (strcmp(nom, "altgr") == 0) return SDL_SCANCODE_RALT;
    if (strcmp(nom, "capslock") == 0) return SDL_SCANCODE_CAPSLOCK;
    if (strcmp(nom, "verrnum") == 0 || strcmp(nom, "numlock") == 0) return SDL_SCANCODE_NUMLOCKCLEAR;
    if (strcmp(nom, "verrmaj") == 0) return SDL_SCANCODE_CAPSLOCK;


    // Touches de navigation
    if (strcmp(nom, "haut") == 0 || strcmp(nom, "up") == 0) return SDL_SCANCODE_UP;
    if (strcmp(nom, "bas") == 0 || strcmp(nom, "down") == 0) return SDL_SCANCODE_DOWN;
    if (strcmp(nom, "gauche") == 0 || strcmp(nom, "left") == 0) return SDL_SCANCODE_LEFT;
    if (strcmp(nom, "droite") == 0 || strcmp(nom, "right") == 0) return SDL_SCANCODE_RIGHT;

    if (strcmp(nom, "insert") == 0) return SDL_SCANCODE_INSERT;
    if (strcmp(nom, "suppr") == 0 || strcmp(nom, "delete") == 0) return SDL_SCANCODE_DELETE;
    if (strcmp(nom, "home") == 0) return SDL_SCANCODE_HOME;
    if (strcmp(nom, "end") == 0) return SDL_SCANCODE_END;
    if (strcmp(nom, "pageup") == 0 || strcmp(nom, "precedent") == 0) return SDL_SCANCODE_PAGEUP;
    if (strcmp(nom, "pagedown") == 0 || strcmp(nom, "suivant") == 0) return SDL_SCANCODE_PAGEDOWN;

    // Autres touches spéciales utiles
    if (strcmp(nom, "menu") == 0 || strcmp(nom, "context") == 0) return SDL_SCANCODE_APPLICATION;
    if (strcmp(nom, "printscreen") == 0 || strcmp(nom, "impr") == 0) return SDL_SCANCODE_PRINTSCREEN;
    if (strcmp(nom, "scrolllock") == 0) return SDL_SCANCODE_SCROLLLOCK;
    if (strcmp(nom, "pause") == 0 || strcmp(nom, "break") == 0) return SDL_SCANCODE_PAUSE;

    // Pavé numérique
    if (strcmp(nom, "kp0") == 0) return SDL_SCANCODE_KP_0;
    if (strcmp(nom, "kp1") == 0) return SDL_SCANCODE_KP_1;
    if (strcmp(nom, "kp2") == 0) return SDL_SCANCODE_KP_2;
    if (strcmp(nom, "kp3") == 0) return SDL_SCANCODE_KP_3;
    if (strcmp(nom, "kp4") == 0) return SDL_SCANCODE_KP_4;
    if (strcmp(nom, "kp5") == 0) return SDL_SCANCODE_KP_5;
    if (strcmp(nom, "kp6") == 0) return SDL_SCANCODE_KP_6;
    if (strcmp(nom, "kp7") == 0) return SDL_SCANCODE_KP_7;
    if (strcmp(nom, "kp8") == 0) return SDL_SCANCODE_KP_8;
    if (strcmp(nom, "kp9") == 0) return SDL_SCANCODE_KP_9;
    if (strcmp(nom, "kp+") == 0) return SDL_SCANCODE_KP_PLUS;
    if (strcmp(nom, "kp-") == 0) return SDL_SCANCODE_KP_MINUS;
    if (strcmp(nom, "kp*") == 0) return SDL_SCANCODE_KP_MULTIPLY;
    if (strcmp(nom, "kp/") == 0) return SDL_SCANCODE_KP_DIVIDE;
    if (strcmp(nom, "kp.") == 0) return SDL_SCANCODE_KP_PERIOD;
    if (strcmp(nom, "kpentrer") == 0 || strcmp(nom, "kpreturn") == 0) return SDL_SCANCODE_KP_ENTER;


    //f
    if (nom[0] == 'f' ) {
        if (strlen(nom) == 2) {
            int num = nom[1] - '0';
            if (num >= 1 && num <= 9) {
                return SDL_SCANCODE_F1 + (num - 1);
            } else {
                if(debug)fprintf(stderr, "Erreur: nom de touche F invalide (%s)\n", nom);
            }
        } else if (strlen(nom) == 3 && nom[1] == '1') {
            if (nom[2] == '0') return SDL_SCANCODE_F10;
            else if (nom[2] == '1') return SDL_SCANCODE_F11;
            else if (nom[2] == '2') return SDL_SCANCODE_F12;
            else if(debug)fprintf(stderr, "Erreur: nom de touche F invalide (%s)\n", nom);
        } else {
            if(debug)fprintf(stderr, "Erreur: nom de touche F invalide (%s)\n", nom);
        }
    }

    //lettre
    if (strlen(nom) == 1) {
        char c = nom[0];
        if (c >= 'a' && c <= 'z'){ c -= 32; 
            return SDL_SCANCODE_A + (c - 'A');
        }

        // Chiffres 
        if (c >= '0' && c <= '9') {
            if (c == '0') return SDL_SCANCODE_0;
            return SDL_SCANCODE_1 + (c - '1');
        }
    }

    if(debug)fprintf(stderr, "Erreur: nom de touche inconnu (%s)\n", nom);
    return SDL_SCANCODE_UNKNOWN;
}



JEU_API bool touche_juste_presse(Gestionnaire *jeu, const char *touche) {
    if (!jeu || !jeu->entrees) {
        if(debug)fprintf(stderr, "Erreur: jeu ou entrees NULL dans touche_juste_presse\n");
        return false;
    }
    SDL_Scancode sc = scancode_depuis_nom(touche);
    if (sc == SDL_SCANCODE_UNKNOWN) return false;
    return jeu->entrees->keys_pressed[sc];
}

JEU_API bool touche_enfoncee(Gestionnaire *jeu, const char *touche) {
    if (!jeu || !jeu->entrees) {
        if(debug)fprintf(stderr, "Erreur: jeu ou entrees NULL dans touche_enfoncee\n");
        return false;
    }
    SDL_Scancode sc = scancode_depuis_nom(touche);
    if (sc == SDL_SCANCODE_UNKNOWN) return false;
    return jeu->entrees->keys[sc];
}


JEU_API bool touche_mannette_juste_presse(Gestionnaire *jeu , const char *touche){
    if (!jeu || !jeu->entrees) {
        if(debug)fprintf(stderr, "Erreur: jeu ou entrees NULL dans touche_manette_juste_presse\n");
        return false;
    }
    SDL_GameControllerButton sc = bouton_manette_depuis_nom(touche);
    if (sc ==SDL_CONTROLLER_BUTTON_INVALID ) return false;
    return jeu->entrees->controller_pressed[sc];
}


JEU_API bool touche_mannette_enfoncee(Gestionnaire *jeu , const char *touche){
    if (!jeu || !jeu->entrees) {
        if(debug)fprintf(stderr, "Erreur: jeu ou entrees NULL dans touche_manette_enfoncee\n");
        return false;
    }
    SDL_GameControllerButton sc = bouton_manette_depuis_nom(touche);
    if (sc ==SDL_CONTROLLER_BUTTON_INVALID ) return false;
    return jeu->entrees->controller[sc];
}


