from PyCgame import PyCgame
import random
import colorsys

# Flags et variables globales
math_demo_done = False
init_mannette = False
t = 0
images_a_afficher = []  # Liste pour gérer les images (tu peux stocker tuples x,y,w,h,lien)

def update_jeu():
    """
    Fonction appelée à chaque frame par le moteur.
    Gère entrées clavier, images, sons et calculs.
    """
    global math_demo_done, init_mannette, t, images_a_afficher

    # Initialisation de la manette
    if not init_mannette:
        PyCgame.init_mannette()
        init_mannette = True

    # Arc-en-ciel RGB
    t += 1
    hue = (t * 0.005) % 1.0
    r_f, g_f, b_f = colorsys.hsv_to_rgb(hue, 1.0, 1.0)
    PyCgame.colorier(int(r_f * 255), int(g_f * 255), int(b_f * 255))

    # Gestion des sons
    if PyCgame.touche_mannette_juste_presse("X"):
        print(f"{PyCgame.random(0,1000)}")
        print("Infos joysticks :", PyCgame.renvoie_joysticks())
        PyCgame.pause_son("./assets/test.wav")

    if PyCgame.touche_mannette_juste_presse("Y"):
        PyCgame.reprendre_son("./assets/test.wav")

    if PyCgame.touche_mannette_enfoncee("A"):
        print("Touche mannette A enfoncée")

    if PyCgame.touche_presser("X"):
        PyCgame.jouer_son("./assets/test.wav", boucle=2, canal=3)
    if PyCgame.touche_presser("Z"):
        PyCgame.arreter_canal(3)

    # Gestion des images
    if PyCgame.touche_presser("S"):
        # Ajouter une image aléatoire dans la liste
        for _ in range(2):
            x, y = PyCgame.random(0, 200), PyCgame.random(0, 100)

            w, h = PyCgame.random(50, 120), PyCgame.random(50, 120)
            lien = "./assets/test.png"
            images_a_afficher.append((lien, x, y, w, h))

    if PyCgame.touche_presser("gauche"):
        # Vider la liste pour "supprimer" les images
        images_a_afficher = []

    if PyCgame.touche_presser("I"):
        # Déplacer/redimensionner la première image si elle existe
        if images_a_afficher:
            lien, _, _, _, _ = images_a_afficher[0]
            images_a_afficher[0] = (lien, 100, 100, 120, 120)

    # Dessiner toutes les images stockées
    for lien, x, y, w, h in images_a_afficher:
        PyCgame.dessiner_image(lien, x, y, w, h)


    PyCgame.dessiner_image_batch(['./assets/test.png','./assets/test.png'],[30,40],[30,40],[100,100],[100,100])

    # Fonctions système
    if PyCgame.touche_presser("F3"):
        PyCgame.redimensionner_fenetre()
    if PyCgame.touche_presser("F4"):
        PyCgame.stopper_jeu()
    if PyCgame.touche_enfoncee("F5"):
        PyCgame.dessiner_mot(
            "./assets/police",
            "Hello PyCgame ! 123 ;:[]$",
            x=10,
            y=90,
            coeff=1,
            ecart=2
        )
    if PyCgame.touche_presser("F6"):
        PyCgame.ecrire_console("[LOG] Hello depuis Python !\n")

    # Démo math (une seule fois)
    if not math_demo_done:
        print("\n=== Démonstration des fonctions math ===")
        print("abs(-5)       =", PyCgame.abs_val(-5))
        print("clamp(10,0,5) =", PyCgame.clamp(10, 0, 5))
        print("pow(2,3)      =", PyCgame.pow(2, 3))
        print("sqrt(16)      =", PyCgame.sqrt(16))
        print("cbrt(27)      =", PyCgame.cbrt(27))
        print("exp(1)        =", PyCgame.exp(1))
        print("log(10)       =", PyCgame.log(10))
        print("log10(100)    =", PyCgame.log10(100))
        print("log2(8)       =", PyCgame.log2(8))
        print("sin(pi)       =", PyCgame.sin(3.14159))
        print("cos(0)        =", PyCgame.cos(0))
        print("tan(1)        =", PyCgame.tan(1))
        print("asin(0.5)     =", PyCgame.asin(0.5))
        print("acos(0.5)     =", PyCgame.acos(0.5))
        print("atan(1)       =", PyCgame.atan(1))
        print("atan2(1,1)    =", PyCgame.atan2(1, 1))
        print("sinh(1)       =", PyCgame.sinh(1))
        print("cosh(1)       =", PyCgame.cosh(1))
        print("tanh(1)       =", PyCgame.tanh(1))
        print("asinh(1)      =", PyCgame.asinh(1))
        print("acosh(2)      =", PyCgame.acoshm(2))
        print("atanh(0.5)    =", PyCgame.atanh(0.5))
        print("floor(2.7)    =", PyCgame.floor(2.7))
        print("ceil(2.3)     =", PyCgame.ceil(2.3))
        print("round(2.5)    =", PyCgame.round(2.5))
        print("trunc(2.9)    =", PyCgame.trunc(2.9))
        print("fmod(5.5,2)   =", PyCgame.fmod(5.5, 2))
        print("hypot(3,4)    =", PyCgame.hypot(3, 4))
        print("=== Fin démo math ===\n")
        math_demo_done = True

# 🚀 Initialisation du moteur
PyCgame.init(
    largeur=320,
    hauteur=160,
    fps=60,
    coeff=4,
    chemin_image="./assets",
    chemin_son="./assets",
    dessiner=True,
    bande_noir=False,
    r=50, g=3, b=70,
    update_func=update_jeu,
    nom_fenetre="coucou PyCgame",
    debug=True
)
