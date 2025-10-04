import ctypes
from ctypes import c_int, c_float, c_bool, c_void_p, POINTER, c_double, c_char_p # bibli entre python et c
import os
import platform
TAILLE_LIEN_GT = 256 # taille des liens maximum des dossiers vers les images et son
TAILLE_CANAL =32 # nb canaux de sons

import subprocess


# structure c : il yen a bcp plus que utlise au cas ou il faudrait les utiliser plus tard 
class Image(ctypes.Structure):
    _fields_ = [
        ("id", c_int),
        ("posx", c_float), ("posy", c_float),
        ("taillex", c_float), ("tailley", c_float),
        ("sens", c_int), ("rotation", c_int),
        ("texture", c_void_p)
    ]

class TableauImage(ctypes.Structure):
    _fields_ = [
        ("tab", POINTER(Image)),
        ("nb_images", c_int),
        ("capacite_images", c_int)
    ]

class FondActualiser(ctypes.Structure):
    _fields_ = [
        ("r", c_int), ("g", c_int), ("b", c_int),
        ("dessiner", c_bool),
        ("bande_noir", c_bool)
    ]

class GestionnaireEntrees(ctypes.Structure):
    _fields_ = [
        ("mouse_x", c_int), ("mouse_y", c_int),
        ("mouse_right_pressed", c_bool),
        ("mouse_right_just_pressed", c_bool),      
        ("mouse_pressed", c_bool),
        ("mouse_just_pressed", c_bool),
        ("keys", c_bool * 512),
        ("keys_pressed", c_bool * 512),
        ("quit", c_bool)
    ]

class Gestionnaire(ctypes.Structure):
    _fields_ = [
        ("run", c_bool),
        ("dt", c_float), ("fps", c_float),
        ("largeur", c_int), ("hauteur", c_int),
        ("coeff_minimise", c_int),
        ("largeur_actuel", c_int), ("hauteur_actuel", c_int),
        ("decalage_x", c_float), ("decalage_y", c_float),
        ("plein_ecran", c_bool),
        ("temps_frame", c_int), 
        ("fenetre", c_void_p), ("rendu", c_void_p),
        ("fond", POINTER(FondActualiser)),
        ("image", POINTER(TableauImage)),
        ("entrees", POINTER(GestionnaireEntrees)),
        ("textures", c_void_p),
        ("sons", c_void_p)
    ]


_pkg_dir = os.path.dirname(__file__)
is_64bits = platform.architecture()[0] == "64bit"
subfolder = "x64" if is_64bits else "x32"

system = platform.system().lower()

if system == "windows":
    dll_path = os.path.join(_pkg_dir, "dll", subfolder, "libjeu.dll")
    jeu = ctypes.CDLL(dll_path)

elif system == "linux":
    so_path = os.path.join(_pkg_dir, "so", "libjeu.so")

    if not os.path.exists(so_path):
        compilation_path = os.path.join(_pkg_dir, "so", "compilation")
        subprocess.run(["make"], cwd=compilation_path, check=True)

    jeu = ctypes.CDLL(so_path)

else:
    raise RuntimeError(f"Système non supporté : {system}")



""" ca cest pour ctypes je donne les types de mes fonctions c """
# Initialisation
jeu.initialisation.argtypes = [c_int, c_int, c_float, c_int, c_char_p, c_char_p, c_bool, c_bool, c_int, c_int, c_int,c_char_p,c_bool]
jeu.initialisation.restype = POINTER(Gestionnaire)
#colorier
jeu.colorier.argtypes = [POINTER(Gestionnaire),c_int,c_int,c_int]
jeu.colorier.restype = None

# Boucle et update
jeu.update.argtypes = [POINTER(Gestionnaire)]
jeu.update.restype = None

jeu.boucle_principale.argtypes = [POINTER(Gestionnaire)]
jeu.boucle_principale.restype = None

jeu.liberer_jeu.argtypes = [POINTER(Gestionnaire)]
jeu.liberer_jeu.restype = None

# Images
jeu.ajouter_image_au_tableau.argtypes = [POINTER(Gestionnaire), c_char_p,c_float, c_float, c_float, c_float,c_int, c_int, c_int]
jeu.ajouter_image_au_tableau.restype = c_int

jeu.supprimer_images_par_id.argtypes = [POINTER(Gestionnaire), c_int]
jeu.supprimer_images_par_id.restype = None
jeu.supprimer_images_par_id_batch.argtypes = [POINTER(Gestionnaire),POINTER(c_int), c_int]
jeu.supprimer_images_par_id_batch.restype = None


jeu.modifier_images.argtypes = [POINTER(Gestionnaire),c_float, c_float, c_float, c_float,c_int, c_int, c_int]
jeu.modifier_images.restype = None
jeu.modifier_images_batch.argtypes = [POINTER(Gestionnaire),POINTER(c_float),  POINTER(c_float),  POINTER(c_float),POINTER(c_float),POINTER(c_int),POINTER(c_int),POINTER(c_int),c_int]
jeu.modifier_images_batch.restype = None


jeu.modifier_texture_image.argtypes = [POINTER(Gestionnaire), c_char_p, c_int]
jeu.modifier_texture_image.restype = None
jeu.modifier_texture_image_batch.argtypes = [POINTER(Gestionnaire),POINTER(c_char_p), POINTER(c_int),c_int]
jeu.modifier_texture_image_batch.restype = None
# Sons
jeu.jouer_son.argtypes = [POINTER(Gestionnaire), c_char_p, c_int, c_int]
jeu.jouer_son.restype = None

jeu.arreter_son.argtypes = [POINTER(Gestionnaire), c_char_p]
jeu.arreter_son.restype = None

jeu.arreter_canal.argtypes = [c_int]
jeu.arreter_canal.restype = None

# touche 

jeu.touche_juste_presse.argtypes = [POINTER(Gestionnaire), c_char_p]
jeu.touche_juste_presse.restype = c_bool

# maths
jeu.random_jeu.argtypes = [c_int, c_int]
jeu.random_jeu.restype = c_int

jeu.abs_val.argtypes = [c_double]
jeu.abs_val.restype = c_double

jeu.clamp.argtypes = [c_double, c_double, c_double]
jeu.clamp.restype = c_double

jeu.pow_custom.argtypes = [c_double, c_double]
jeu.pow_custom.restype = c_double

jeu.sqrt_custom.argtypes = [c_double]
jeu.sqrt_custom.restype = c_double

jeu.cbrt_custom.argtypes = [c_double]
jeu.cbrt_custom.restype = c_double

jeu.exp_custom.argtypes = [c_double]
jeu.exp_custom.restype = c_double

jeu.log_custom.argtypes = [c_double]
jeu.log_custom.restype = c_double

jeu.log10_custom.argtypes = [c_double]
jeu.log10_custom.restype = c_double

jeu.log2_custom.argtypes = [c_double]
jeu.log2_custom.restype = c_double

jeu.sin_custom.argtypes = [c_double]
jeu.sin_custom.restype = c_double

jeu.cos_custom.argtypes = [c_double]
jeu.cos_custom.restype = c_double

jeu.tan_custom.argtypes = [c_double]
jeu.tan_custom.restype = c_double

jeu.asin_custom.argtypes = [c_double]
jeu.asin_custom.restype = c_double

jeu.acos_custom.argtypes = [c_double]
jeu.acos_custom.restype = c_double

jeu.atan_custom.argtypes = [c_double]
jeu.atan_custom.restype = c_double

jeu.atan2_custom.argtypes = [c_double, c_double]
jeu.atan2_custom.restype = c_double

jeu.sinh_custom.argtypes = [c_double]
jeu.sinh_custom.restype = c_double

jeu.cosh_custom.argtypes = [c_double]
jeu.cosh_custom.restype = c_double

jeu.tanh_custom.argtypes = [c_double]
jeu.tanh_custom.restype = c_double

jeu.asinh_custom.argtypes = [c_double]
jeu.asinh_custom.restype = c_double

jeu.acosh_custom.argtypes = [c_double]
jeu.acosh_custom.restype = c_double

jeu.atanh_custom.argtypes = [c_double]
jeu.atanh_custom.restype = c_double

jeu.floor_custom.argtypes = [c_double]
jeu.floor_custom.restype = c_double

jeu.ceil_custom.argtypes = [c_double]
jeu.ceil_custom.restype = c_double

jeu.round_custom.argtypes = [c_double]
jeu.round_custom.restype = c_double

jeu.trunc_custom.argtypes = [c_double]
jeu.trunc_custom.restype = c_double

jeu.fmod_custom.argtypes = [c_double, c_double]
jeu.fmod_custom.restype = c_double

jeu.hypot_custom.argtypes = [c_double, c_double]
jeu.hypot_custom.restype = c_double


jeu.redimensionner_fenetre.argtypes = [POINTER(Gestionnaire)] 
jeu.redimensionner_fenetre.restype = None     


jeu.ecrire_dans_console.argtypes = [c_char_p] 
jeu.ecrire_dans_console.restype = None  


jeu.ajouter_mot_dans_tableau.argtypes = [
    POINTER(Gestionnaire),  c_int, c_char_p, c_char_p, c_float,c_float, c_float,c_int,  c_float,c_int                   
]
jeu.ajouter_mot_dans_tableau.restype = None


jeu.pause_canal.argtypes = [c_int]
jeu.pause_canal.restype = None

jeu.pause_son.argtypes = [POINTER(Gestionnaire), c_char_p]
jeu.pause_son.restype = None

jeu.reprendre_canal.argtypes = [c_int]
jeu.reprendre_canal.restype = None

jeu.reprendre_son.argtypes = [POINTER(Gestionnaire), c_char_p]
jeu.reprendre_son.restype = None

jeu.touche_enfoncee.argtypes = [POINTER(Gestionnaire), c_char_p]
jeu.touche_enfoncee.restype = c_bool

jeu.touche_mannette_enfoncee.argtypes = [POINTER(Gestionnaire), c_char_p]
jeu.touche_mannette_enfoncee.restype = c_bool

jeu.touche_mannette_juste_presse.argtypes = [POINTER(Gestionnaire), c_char_p]
jeu.touche_mannette_juste_presse.restype = c_bool

jeu.init_controller.argtypes = [POINTER(Gestionnaire), c_int]
jeu.init_controller.restype = None

jeu.fermer_controller.argtypes = [POINTER(Gestionnaire)]
jeu.fermer_controller.restype = None


jeu.renvoie_joysticks.argtypes = [POINTER(GestionnaireEntrees), c_float]
jeu.renvoie_joysticks.restype = POINTER(c_float)

jeu.fermer_joystick.argtypes = [POINTER(Gestionnaire)]
jeu.fermer_joystick.restype = None

UpdateCallbackType = ctypes.CFUNCTYPE(None, POINTER(Gestionnaire))
jeu.set_update_callback.argtypes = [UpdateCallbackType]
jeu.set_update_callback.restype = None


class _PyCgame:
    def __init__(self):
        self._g = None
        self._callback_ref = None
        self._user_update = None

    def init(self, largeur=160, hauteur=90, fps=60, coeff=3,
             chemin_image=".", chemin_son=".",
             dessiner=True, bande_noir=True, r=0, g=0, b=0,
             update_func=None,nom_fenetre="fenetre",debug=False):
        #init
        self._g = jeu.initialisation(
            hauteur, largeur, fps, coeff,
            chemin_image.encode("utf-8"), chemin_son.encode("utf-8"),
            dessiner, bande_noir, r, g, b,nom_fenetre.encode("utf-8"),debug
        )
        if not self._g:
            raise RuntimeError("Initialisation échouée")

        #callback
        if update_func:
            if not callable(update_func):
                raise ValueError("update_func doit être callable")
            self._user_update = update_func

            UpdateCallbackType = ctypes.CFUNCTYPE(None, POINTER(Gestionnaire))
            def wrapper(g):
                if self._user_update:
                    self._user_update()
            self._callback_ref = UpdateCallbackType(wrapper)
            jeu.set_update_callback(self._callback_ref)

        jeu.boucle_principale(self._g)

 

    # variable global
    @property
    def largeur(self): return self._g.contents.largeur if self._g else 0
    @property
    def hauteur(self): return self._g.contents.hauteur if self._g else 0
    @property
    def dt(self): return self._g.contents.dt if self._g else 0.0
    @property
    def fps(self): return self._g.contents.fps if self._g else 0.0
    @property
    def time(self): return self._g.contents.temps_frame if self._g else 0
    @property
    def mouse_x(self): return self._g.contents.entrees.contents.mouse_x if self._g else 0
    @property
    def mouse_y(self): return self._g.contents.entrees.contents.mouse_y if self._g else 0
    @property
    def mouse_presse(self): return self._g.contents.entrees.contents.mouse_pressed if self._g else False
    @property
    def mouse_juste_presse(self): return self._g.contents.entrees.contents.mouse_just_pressed if self._g else False
    @property
    def mouse_droit_presse(self): return self._g.contents.entrees.contents.mouse_right_pressed if self._g else False
    @property
    def mouse_droit_juste_presse(self): return self._g.contents.entrees.contents.mouse_right_just_pressed if self._g else False
    @property
    def decalage_x(self): return self._g.contents.decalage_x/(self._g.contents.largeur_actuel/self._g.contents.largeur) if self._g else 0
    @property
    def decalage_y(self): return self._g.contents.decalage_y/(self._g.contents.hauteur_actuel/self._g.contents.hauteur) if self._g else 0
    @property
    def run(self):
        return self._g.contents.run if self._g else False

    #touches
    def touche_presser(self, key_name):
        if not self._g:
            return False
        return jeu.touche_juste_presse(self._g, key_name.encode("utf-8"))

    def touche_enfoncee(self, key_name):
        if not self._g:
            return False
        return jeu.touche_enfoncee(self._g, key_name.encode("utf-8"))

    # iamges
    def ajouter_image(self, lien, x, y, w, h, id_num,sens=0, rotation=0):
        return jeu.ajouter_image_au_tableau(self._g, lien.encode("utf-8"),
                                            x, y, w, h, sens, id_num, rotation)
    def ajouter_mot(self, lien, mot, x, y, coeff, ecart, id_num, sens=0, rotation=0):
        return jeu.ajouter_mot_dans_tableau(
            self._g,
            id_num,
            lien.encode("utf-8"),
            mot.encode("utf-8"),
            x, y, coeff,
            sens,
            ecart,
            rotation  
        )

    def ecrire_console(self,mot):
        return jeu.ecrire_dans_console(mot.encode("utf-8"))
    def supprimer_image(self, id_num):
        jeu.supprimer_images_par_id(self._g, id_num)

    def supprimer_images_par_id_batch(self, id_supprimer_list):
        if not self._g:
            raise RuntimeError("Jeu non initialisé")
        # conversion
        taille = len(id_supprimer_list)
        Ids = (c_int * taille)(*id_supprimer_list)
        jeu.supprimer_images_par_id_batch(self._g, Ids,taille)


    def modifier_image(self, x, y, w, h, id_num,sens=0, rotation=0):
        jeu.modifier_images(self._g, x, y, w, h, sens, id_num, rotation)

    def modifier_images_batch(self, x_list, y_list, w_list, h_list, sens_list, id_list, rotate_list):
            if not self._g:
                raise RuntimeError("Jeu non initialisé")
            taille = len(x_list)
            # ya une conversion de tab a faire
            X = (c_float * taille)(*x_list)
            Y = (c_float * taille)(*y_list)
            W = (c_float * taille)(*w_list)
            H = (c_float * taille)(*h_list)
            S = (c_int * taille)(*sens_list)
            I = (c_int * taille)(*id_list)
            R = (c_int * taille)(*rotate_list)
            jeu.modifier_images_batch(self._g, X, Y, W, H, S, I, R,taille)


    def modifier_texture(self, lien, id_num):
        jeu.modifier_texture_image(self._g, lien.encode("utf-8"), id_num)

    def modifier_texture_image_batch(self, liens_list, id_list):
        if not self._g:
            raise RuntimeError("Jeu non initialisé")
        taille = len(liens_list)
        Liens = (c_char_p * taille)(*[
            s.encode("utf-8") if isinstance(s, str) else s for s in liens_list
        ])
        Ids = (c_int * taille)(*id_list)
        jeu.modifier_texture_image_batch(self._g, Liens, Ids, taille)



    # sons
    def jouer_son(self, lien, boucle=0, canal=-1):
        jeu.jouer_son(self._g, lien.encode("utf-8"), boucle, canal)

    def arreter_son(self, lien):
        jeu.arreter_son(self._g, lien.encode("utf-8"))

    def arreter_canal(self, canal):
        jeu.arreter_canal(canal)
    def stopper_jeu(self):
        self._g.contents.run=False
    # maths jen ai mis un packet
    def abs_val(self, x): return jeu.abs_val(c_double(x))
    def clamp(self, x, min_, max_): return jeu.clamp(c_double(x), c_double(min_), c_double(max_))
    def pow(self, base, exp): return jeu.pow_custom(c_double(base), c_double(exp))
    def sqrt(self, x): return jeu.sqrt_custom(c_double(x))
    def cbrt(self, x): return jeu.cbrt_custom(c_double(x))
    def exp(self, x): return jeu.exp_custom(c_double(x))
    def log(self, x): return jeu.log_custom(c_double(x))
    def log10(self, x): return jeu.log10_custom(c_double(x))
    def log2(self, x): return jeu.log2_custom(c_double(x))
    def sin(self, x): return jeu.sin_custom(c_double(x))
    def cos(self, x): return jeu.cos_custom(c_double(x))
    def tan(self, x): return jeu.tan_custom(c_double(x))
    def asin(self, x): return jeu.asin_custom(c_double(x))
    def acos(self, x): return jeu.acos_custom(c_double(x))
    def atan(self, x): return jeu.atan_custom(c_double(x))
    def atan2(self, y, x): return jeu.atan2_custom(c_double(y), c_double(x))
    def sinh(self, x): return jeu.sinh_custom(c_double(x))
    def cosh(self, x): return jeu.cosh_custom(c_double(x))
    def tanh(self, x): return jeu.tanh_custom(c_double(x))
    def asinh(self, x): return jeu.asinh_custom(c_double(x))
    def acoshm(self, x): return jeu.acosh_custom(c_double(x))
    def atanh(self, x): return jeu.atanh_custom(c_double(x))
    def floor(self, x): return jeu.floor_custom(c_double(x))
    def ceil(self, x): return jeu.ceil_custom(c_double(x))
    def round(self, x): return jeu.round_custom(c_double(x))
    def trunc(self, x): return jeu.trunc_custom(c_double(x))
    def fmod(self, x, y): return jeu.fmod_custom(c_double(x), c_double(y))
    def hypot(self, x, y): return jeu.hypot_custom(c_double(x), c_double(y))
    def redimensionner_fenetre(self):
        if not self._g:
            raise RuntimeError("Jeu non initialisé")
        jeu.redimensionner_fenetre(self._g)
    def pause_canal(self, canal):
        jeu.pause_canal(canal)

    def pause_son(self, lien):
        if not self._g: return
        jeu.pause_son(self._g, lien.encode("utf-8"))

    def reprendre_canal(self, canal):
        jeu.reprendre_canal(canal)

    def reprendre_son(self, lien):
        if not self._g: return
        jeu.reprendre_son(self._g, lien.encode("utf-8"))

    def touche_mannette_enfoncee(self, key_name):
        if not self._g:
            return False
        return jeu.touche_mannette_enfoncee(self._g, key_name.encode("utf-8"))

    def touche_mannette_juste_presse(self, key_name):
        if not self._g:
            return False
        return jeu.touche_mannette_juste_presse(self._g, key_name.encode("utf-8"))
    
    def renvoie_joysticks(self, dead_zone=0.1):
        if not self._g:
            raise RuntimeError("Jeu non initialisé")
        entrees_ptr = self._g.contents.entrees
        if not entrees_ptr:
            return None

        ptr = jeu.renvoie_joysticks(entrees_ptr, dead_zone)
        if not ptr:
            return None
        return [ptr[i] for i in range(6)]

    def init_mannette(self, index=0):
        if not self._g:
            raise RuntimeError("Jeu non initialisé")
        jeu.init_controller(self._g, index)

    def fermer_controller(self):
        if not self._g:
            return
        jeu.fermer_controller(self._g)

    def fermer_joystick(self):
        if not self._g:
            return
        jeu.fermer_joystick(self._g)

    def random(self, min, max):
        return jeu.random_jeu(min, max)
    def colorier(self,r,g,b):
        if not self._g:
            return
        return jeu.colorier(self._g,r,g,b)
    #update
    def set_update_callback(self, py_func):
        if not callable(py_func):
            raise ValueError("update doit être une fonction")
        self._user_update = py_func

    def update(self):
        if not self._g:
            raise RuntimeError("Jeu non initialisé")
        jeu.update(self._g)
        if self._user_update:
            self._user_update()




PyCgame = _PyCgame()