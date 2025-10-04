
from PyCgame import PyCgame
import random
import  colorsys
# Flag pour afficher les fonctions mathématiques une seule fois
math_demo_done = False
init_mannette = False
r,g,b,t= 0,0,0,0
def update_jeu():
    """
    Cette fonction est appelée à chaque frame par le moteur.
    C'est ici quon gères les entrées clavier, les images,
    le son et les calculs.
    """
    global math_demo_done,init_mannette,r,g,b,t
    if not(init_mannette):
        PyCgame.init_controller()
        init_mannette=True
    # Affichage d'infos de debug sur le jeu en cours
    #print(f"[INFO] dt={PyCgame.dt}, time={PyCgame.time},decalage x,y:{PyCgame.decalage_x},{PyCgame.decalage_y} fps={PyCgame.fps}")
    #print(f"mouse : {PyCgame.mouse_x} {PyCgame.mouse_y} {PyCgame.mouse_presse} {PyCgame.mouse_juste_presse}")
    # 🎵 Contrôle du son
    # 🌈 Arc-en-ciel RGB via HSV
    t+=1
    hue = (t * 0.005) % 1.0   # tourne lentement (0.2 = vitesse de rotation)
    sat = 1.0
    val = 1.0

        # HSV -> RGB
    r_f, g_f, b_f = colorsys.hsv_to_rgb(hue, sat, val)
    r, g, b = int(r_f * 255), int(g_f * 255), int(b_f * 255)

    PyCgame.colorier(r, g, b)
    if PyCgame.touche_mannette_juste_presse("X"):
        print(f"{PyCgame.random(0,1000)}")
        for i in PyCgame.renvoie_joysticks():
            print(f"info : {i}")
        PyCgame.pause_son("./assets/test.wav")
    if PyCgame.touche_mannette_juste_presse("Y"):
        PyCgame.reprendre_son("./assets/test.wav")
    if PyCgame.touche_mannette_enfoncee("A"):
        print("touche mannette enfoncee")
        
    if PyCgame.touche_presser("X"):  # Jouer un son
        PyCgame.jouer_son("./assets/test.wav", boucle=2, canal=3)
    if PyCgame.touche_presser("Z"):  # Stopper un canal
        PyCgame.arreter_canal(3)

    # 🎨 Gestion des images
    if PyCgame.touche_presser("S"):  # Ajouter une image aléatoire
        PyCgame.ajouter_image(
            "./assets/test.png",
            random.randint(0, 50),
            random.randint(0, 50),
            100, 100,
            id_num=30
        )
        PyCgame.ajouter_image(
            "./assets/test.png",
            random.randint(0, 50),
            random.randint(0, 50),
            100, 100,
            id_num=20
        )
    if PyCgame.touche_presser("gauche"):  # Supprimer l’image avec id=30
        PyCgame.supprimer_images_par_id_batch([30,20,10])
    if PyCgame.touche_presser("I"):  # Modifier l’image (déplacer/redimensionner)
        PyCgame.modifier_image(0, 0, 120, 120, id_num=30)

    # ⚙️ Fonctions système
    if PyCgame.touche_presser("F3"):  # Redimensionner la fenêtre
        PyCgame.redimensionner_fenetre()
    if PyCgame.touche_presser("F4"):  # Quitter le jeu proprement
        PyCgame.stopper_jeu()
    if PyCgame.touche_presser("F5"):
        PyCgame.ajouter_mot(
            "./assets/police",
            "Hello PyCgame ! 123 ;:[]$",
            x=10,
            y=90,
            coeff=1,
            ecart=2,
            id_num=10,
            sens=0,
            rotation=0
        )

    # 🔤 Affichage de texte avec police custom

    # 📝 Écriture dans le log
    if PyCgame.touche_presser("F6"):
        PyCgame.ecrire_console("[LOG] Hello depuis Python !\n")

    # ➗ Démo math (affichée une seule fois)
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
    update_func=update_jeu,nom_fenetre="coucou PyCgame",debug=True
)
