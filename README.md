# 🎮 PyCgame

**PyCgame** est un module Python pour créer facilement des jeux 2D avec :

* 🖼️ Gestion des **images**
* 🔊 Gestion des **sons**
* ⌨️ Gestion du **clavier/souris**
* 🎮 Support des **manettes**
* 🧮 Fonctions **mathématiques intégrées**

👉 Compatible avec **Windows 64 bits** et **Linux**.
👉 Bientôt : compilation automatique pour Linux avec `make`.

---

## 🚀 Nouveautés

* 🎮 **Support des manettes (controllers)**
* ⏸️ Pause / Reprendre des sons (par fichier ou par canal)

---

## ⚡ Installation

Depuis PyPI :

```bash
pip install PyCgame
```

Import obligatoire dans votre projet :

```python
from PyCgame import PyCgame
```

⚠️ Attention : l’import exact `from PyCgame import PyCgame` doit être utilisé.

---

## 🐧 Utilisation sous Linux

Si les `.so` fournis ne sont pas compatibles avec votre distribution, vous devez les recompiler :

1. Allez dans le dossier `/compilation`
2. Exécutez :

   ```bash
   make
   ```
3. Un dossier `/so` sera créé avec `libjeu.so`
4. Déplacez ce dossier `so` dans le **dossier parent** de `compilation`
5. Placez au même endroit :

   * `sdl2.so`
   * `sdl2mixer.so`
   * `sdl2image.so`
6. Pour tester, placez `exemple.py` dans `PyCgame/exemple/`

---

## 🚀 Initialisation d’un jeu

```python
PyCgame.init(
    largeur=160,           # largeur virtuelle
    hauteur=90,            # hauteur virtuelle
    fps=60,                # nombre d’images par seconde
    coeff=3,               # facteur de mise à l’échelle
    chemin_image="./assets", # dossier images
    chemin_son="./assets",   # dossier sons
    dessiner=True,         # dessiner le fond ?
    bande_noir=True,       # bandes noires si ratio différent ?
    r=0, g=0, b=0,         # couleur de fond
    update_func=Update,    # fonction d’update
    nom_fenetre="MonJeu"  # nom de la fenêtre
)

PyCgame.stopper_jeu()
```

---

## 🔄 Boucle de mise à jour

```python
def Update():
    if PyCgame.touche_presser("Espace"):
        print("Espace pressée !")
```

---

## 📊 Propriétés globales

| Propriété            | Description                         |
| -------------------- | ----------------------------------- |
| `PyCgame.largeur`    | largeur virtuelle                   |
| `PyCgame.hauteur`    | hauteur virtuelle                   |
| `PyCgame.dt`         | delta time entre frames             |
| `PyCgame.fps`        | FPS actuel                          |
| `PyCgame.time`       | temps écoulé                        |
| `PyCgame.run`        | bool : le jeu tourne ?              |
| `PyCgame.decalage_x` | décalage en x du jeu en plein écran |
| `PyCgame.decalage_y` | décalage en y du jeu en plein écran |

---

## 🖱️ Gestion de la souris

```python
PyCgame.mouse_x
PyCgame.mouse_y
PyCgame.mouse_presse
PyCgame.mouse_juste_presse
PyCgame.mouse_droit_presse
PyCgame.mouse_droit_juste_presse
```

---

## ⌨️ Gestion du clavier

### Vérification des touches

```python
PyCgame.touche_presser("A")
PyCgame.touche_enfoncee("A")
```

### Liste complète des touches supportées

#### Lettres :

`A` … `Z` (majuscules ou minuscules acceptées)

#### Chiffres :

`0` … `9`

#### Touches spéciales :

* `espace`
* `entrer` / `return`
* `echap` / `escape`
* `tab`
* `maj` / `shift`
* `ctrl` / `control`
* `alt`
* `altgr`
* `capslock` / `verrmaj`
* `verrnum` / `numlock`

#### Navigation :

* `haut` / `up`
* `bas` / `down`
* `gauche` / `left`
* `droite` / `right`
* `insert`
* `suppr` / `delete`
* `home`
* `end`
* `pageup` / `precedent`
* `pagedown` / `suivant`

#### Système :

* `menu` / `context`
* `printscreen` / `impr`
* `scrolllock`
* `pause` / `break`

#### Pavé numérique :

* `kp0` … `kp9`
* `kp+`
* `kp-`
* `kp*`
* `kp/`
* `kp.`
* `kpentrer` / `kpreturn`

#### Fonctions :

* `F1` … `F12`

---

## 🎮 Gestion des manettes

```python
PyCgame.init_controller(0)

if PyCgame.touche_mannette_juste_presse("X"):
    PyCgame.pause_son("./assets/test.wav")

if PyCgame.touche_mannette_juste_presse("Y"):
    PyCgame.reprendre_son("./assets/test.wav")

if PyCgame.touche_mannette_enfoncee("A"):
    print("A maintenu")

PyCgame.fermer_controller()
```

### Boutons supportés

#### Boutons principaux :

* `a`, `b`, `x`, `y`

#### Système :

* `start`, `back`, `select`, `guide`, `home`, `share`, `capture`

#### Sticks cliquables :

* `leftstick`, `l3`
* `rightstick`, `r3`

#### Bumpers :

* `lb`, `l1`, `leftshoulder`
* `rb`, `r1`, `rightshoulder`

#### Triggers :

* `lt`, `l2`
* `rt`, `r2`

#### Croix directionnelle (D-Pad) :

* `haut` / `up`
* `bas` / `down`
* `gauche` / `left`
* `droite` / `right`

#### Additionnels :

* `paddle1`, `paddle2`, `paddle3`, `paddle4`
* `touchpad`

---

## 🖼️ Images et texte

```python
PyCgame.ajouter_image("./assets/perso.png", 10, 20, 32, 32, id_num=2)
PyCgame.ajouter_mot("./assets/police.png", "Hello", 50, 50, 1, 1, id_num=1)
PyCgame.supprimer_image(1)
PyCgame.modifier_image(20, 30, 32, 32, id_num=1)
PyCgame.modifier_texture("./assets/nouvelle_image.png", id_num=2)
PyCgame.ecrire_console("Hello World !")
```

---

## 🔊 Sons

```python
PyCgame.jouer_son("./assets/son.wav", boucle=1, canal=3)
PyCgame.arreter_son("./assets/son.wav")
PyCgame.arreter_canal(3)

# Pause/Reprendre
PyCgame.pause_canal(3)
PyCgame.pause_son("./assets/son.wav")
PyCgame.reprendre_canal(3)
PyCgame.reprendre_son("./assets/son.wav")
```

---

## 🧮 Fonctions mathématiques

```python
PyCgame.abs_val(-5)
PyCgame.clamp(10, 0, 5)
PyCgame.pow(2, 3)
PyCgame.sqrt(16)
PyCgame.sin(3.14)
PyCgame.atan2(1, 1)
```

Inclus aussi : `cos`, `tan`, `log`, `exp`, `floor`, `ceil`, `round`, `trunc`, `fmod`, `hypot`, etc.

---

## 🖥️ Redimensionnement

```python
PyCgame.redimensionner_fenetre()
```

---

## 📂 Exemple minimal

```python
from PyCgame import PyCgame

def update():
    if PyCgame.touche_presser("Espace"):
        print("Espace pressée !")

PyCgame.init(largeur=160, hauteur=90, fps=60, update_func=update)
```

---

## ✅ Notes importantes

* Les chemins des fichiers sont relatifs au projet.
* Les `id_num` doivent être **uniques** pour chaque image/texte.
* `update_func` doit être une **fonction callable**.
* Pour les manettes : toujours appeler `PyCgame.init_controller()` après `PyCgame.init()` et fermer avec `PyCgame.fermer_controller()` avant de quitter.

---

## 📬 Support

Pour signaler un bug ou proposer une amélioration :
📧 **[Baptiste.guerin34@gmail.com](mailto:Baptiste.guerin34@gmail.com)**
