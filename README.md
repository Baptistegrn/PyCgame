# 🎮 PyCgame

**PyCgame** est un module Python pour créer facilement des jeux 2D avec images, sons, clavier/souris et fonctions mathématiques intégrées.

👉 Actuellement disponible pour **Windows 64 bits** (et 32 bits).
👉 Une version **Linux** vient d'arriver.

------------------------------------------------------

> **Nouveautés :** prise en charge des manettes (controller) + fonctions pause/reprendre pour les sons (par canal ou par fichier).

## ⚡ Installation

Installez **PyCgame** directement depuis **PyPI** :

```bash
pip install PyCgame
```

Assurez-vous que le module est accessible depuis votre projet :

```python
from PyCgame import PyCgame
```

> ⚠️ **Important** : l’import `from PyCgame import PyCgame` est **obligatoire**.

---

## 🚀 Initialisation d’un jeu et stopper

```python
PyCgame.init(
    largeur=160, #largeur
    hauteur=90, #hauteur
    fps=60, #actualisation
    coeff=3, #coeff de l'écran sans redimensionner ici 3x160,3x90
    chemin_image="./assets", # dossier contenant les images
    chemin_son="./assets",   # dossier contenant les sons
    dessiner=True,           # est-ce que je dessine un fond quand j'actualise ?
    bande_noir=True,         # est-ce que je dessine des bandes noires si ma fenêtre plein écran n'est pas proportionnelle ?
    r=0, g=0, b=0,           # couleur de l'actualisation
    update_func=Update       # nom de la fonction à actualiser
    nom_fenetre= "test" # nom de la fenetre
)

PyCgame.stopper_jeu()
```

---

## mise à jour

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
PyCgame.mouse_x              # position X de la souris (dans le repère virtuel du jeu)
PyCgame.mouse_y              # position Y de la souris (dans le repère virtuel du jeu)
PyCgame.mouse_presse         # bool : clic gauche maintenu ?
PyCgame.mouse_juste_presse   # bool : clic gauche pressé uniquement cette frame
PyCgame.mouse_droit_presse   # bool : clic droit maintenu ?
PyCgame.mouse_droit_juste_presse # bool : clic droit pressé uniquement cette frame
```

Exemple :

```python
def Update():
    if PyCgame.mouse_juste_presse:
        print("Clic gauche détecté à", PyCgame.mouse_x, PyCgame.mouse_y)
```

---

## ⌨️ Gestion du clavier

### Vérification des touches

```python
PyCgame.touche_presser("A")      # Pressée uniquement cette frame
PyCgame.touche_enfoncee("A")     # Maintenue enfoncée
```

### Touches supportées

* **Touches spéciales :**

  * `espace`, `entrer` (ou `return`), `echap` (ou `escape`), `tab`
  * `maj` / `shift`, `ctrl` / `control`, `alt`, `altgr`
  * `capslock` / `verrmaj`, `verrnum` / `numlock`

* **Touches de navigation :**

  * `haut` / `up`, `bas` / `down`, `gauche` / `left`, `droite` / `right`
  * `insert`, `suppr` / `delete`, `home`, `end`
  * `pageup` / `precedent`, `pagedown` / `suivant`

* **Touches système :**

  * `menu` / `context`, `printscreen` / `impr`
  * `scrolllock`, `pause` / `break`

* **Pavé numérique :**

  * `kp0` … `kp9`
  * `kp+`, `kp-`, `kp*`, `kp/`, `kp.`, `kpentrer` / `kpreturn`

* **Touches fonction :**

  * `F1` … `F12`

* **Lettres :**

  * `A` … `Z` (majuscules ou minuscules acceptées)

* **Chiffres :**

  * `0` … `9`

---

## 🎮 Gestion des manettes (controller)

> **Nouveaux API** ajoutés pour gérer les manettes :

```python
# lecture des boutons
PyCgame.touche_mannette_enfoncee("A")         # bouton maintenu
PyCgame.touche_mannette_juste_presse("A")     # bouton pressé uniquement cette frame

# initialisation / fermeture
PyCgame.init_controller(index=0)    # ouvre la manette à l'index donné (0 par défaut)
PyCgame.fermer_controller()         # ferme la manette
```

**Exemple d'utilisation** :

```python
# à appeler après PyCgame.init(...)
PyCgame.init_controller(0)

if PyCgame.touche_mannette_juste_presse("X"):
    PyCgame.pause_son("./assets/test.wav")

if PyCgame.touche_mannette_juste_presse("Y"):
    PyCgame.reprendre_son("./assets/test.wav")

if PyCgame.touche_mannette_enfoncee("A"):
    print("touche manette enfoncee")

# quand tu fermes le jeu
PyCgame.fermer_controller()
```

**Remarques :**

* Les noms de boutons sont **normalisés** et insensibles à la casse (`"A"`, `"a"` fonctionnent).
* Assure-toi d'appeler `PyCgame.init_controller()` **après** `PyCgame.init(...)`.
* Si une manette n'est pas détectée, vérifie les permissions/système ou l'index passé.

### Touches prises en charge (noms acceptés)

* **Boutons principaux :** `a`, `b`, `x`, `y`
* **Système :** `start`, `back`, `select`, `guide`, `home`
* **Sticks cliquables :** `leftstick`, `l3`, `rightstick`, `r3`
* **Bumpers :** `lb`, `l1`, `leftshoulder`, `rb`, `r1`, `rightshoulder`
* **Croix directionnelle (D-Pad) :** `haut`, `up`, `bas`, `down`, `gauche`, `left`, `droite`, `right`
* **Modernes / additionnels :** `share`, `capture`, `paddle1`, `paddle2`, `paddle3`, `paddle4`, `touchpad`

> (Ces noms correspondent à l'implémentation interne évènementielle — si un nom inconnu est donné, la fonction renverra `SDL_CONTROLLER_BUTTON_INVALID` côté C et la vérification renverra `False` côté Python.)

---

## 🖼️ Gestion des images et du texte

```python
PyCgame.ajouter_image(id_="./assets/perso.png", x=10, y=20, w=32, h=32, id_num=2)
PyCgame.ajouter_mot(lien="./assets/police.png", mot="Hello", x=50, y=50, coeff=1, ecart=1, id_num=1)
PyCgame.supprimer_image(1)
PyCgame.modifier_image(x=20, y=30, w=32, h=32, id_num=1)
PyCgame.modifier_texture("./assets/nouvelle_image.png", id_num=2)
PyCgame.ecrire_console("Bonjour le monde !")
```

---

## 🔊 Gestion des sons

```python
#wav obligatoire (pour le moment)
PyCgame.jouer_son("./assets/son.wav", boucle=1, canal=3)
PyCgame.arreter_son("./assets/son.wav")
PyCgame.arreter_canal(3)

# -- nouveaux : pause / reprendre (par canal ou par fichier)
PyCgame.pause_canal(3)
PyCgame.pause_son("./assets/son.wav")
PyCgame.reprendre_canal(3)
PyCgame.reprendre_son("./assets/son.wav")
```

---

## 🧮 Fonctions mathématiques intégrées

```python
PyCgame.abs_val(-5)
PyCgame.clamp(10, 0, 5)
PyCgame.pow(2, 3)
PyCgame.sqrt(16)
PyCgame.sin(3.14)
PyCgame.atan2(1, 1)
```

> Et beaucoup d’autres : `cos`, `tan`, `log`, `exp`, `floor`, `ceil`, `round`, `trunc`, `fmod`, `hypot`, etc.

---

## 🖥️ Redimensionnement

```python
PyCgame.redimensionner_fenetre()
```

---

## 📂 Exemple d’usage

### `exemple.py`

```python
from PyCgame import PyCgame

def update():
    if PyCgame.touche_presser("Espace"):
        print("Espace pressée !")

PyCgame.init(largeur=160, hauteur=90, fps=60, update_func=update)
```

---

## 📝 Créer sa propre police bitmap

1. 📁 Créez un dossier pour votre police :

   * `./mon_dossier`
   * `../mon_dossier`

2. 🖼️ Chaque caractère doit être une image séparée :

   * Nom du fichier = code ASCII du caractère
   * Exemple : `"A" = 65.png`, `"z" = 122.png`

3. 📏 Tous les caractères doivent avoir la même hauteur

4. Exemple final :

```python
jeu.ajouter_image(id_="./mon_dossier", x=10, y=20, w=32, h=32, id_num=2)
```

---

## ✅ Notes importantes

* Les chemins des images et sons doivent être **en rapport au dossier courant**.
* `update_func` doit être **une fonction Python callable**.
* Les images doivent avoir un **id unique** pour pouvoir les modifier ou supprimer.
* **Manette :** appeler `PyCgame.init_controller()` après `PyCgame.init(...)` et `PyCgame.fermer_controller()` avant la fermeture complète si tu as ouvert une manette.

💡 Avec **PyCgame**, vous êtes prêt à créer votre jeu 2D en Python rapidement et proprement !

---

## 📬 Support & suggestions

Pour tout bug ou toute suggestion, merci d’envoyer un mail à :
📧 [Baptiste.guerin34@gmail.com](mailto:Baptiste.guerin34@gmail.com)
