# 🎮 PyCgame

**PyCgame** est un module Python pour créer facilement des jeux 2D avec images, sons, clavier/souris et fonctions mathématiques intégrées.



👉 Actuellement disponible pour **Windows 64 bits** (et 32 bits).  
👉 Une version **Linux** arrive bientôt.  

---

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
    largeur=160,#largeur
    hauteur=90,#hauteur
    fps=60,#actualisation
    coeff=3,#coeff de lecran sans redimensionner ici 3x160,3x90
    chemin_image="./assets",#ici dossier courant puis dossier assets -> si rien mis, les images doivent etre dans le meme dossier dexecution  on peut aussi mettre : ../assets etc....
    chemin_son="./assets",
    dessiner=True,#estce que je dessine un fond quand jactualise ?
    bande_noir=True,#estce que je dessine des bandes noirs si ma fenetre en plein ecran nest pas proportionnel a lecran ?
    r=0, g=0, b=0,# couleur de lactualisation
    update_func=Update # nom de la fonction a actualiser
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

| Propriété         | Description                          |
| ------------------| -----------------------              |
| `jeu.largeur`     | largeur virtuelle                    |
| `jeu.hauteur`     | hauteur virtuelle                    |
| `jeu.dt`          | delta time entre frames              |
| `jeu.fps`         | FPS actuel                           |
| `jeu.time`        | temps écoulé                         |
| `jeu.run`         | bool : le jeu tourne ?               |
| `jeu.decalage_x`  | decalage en x du jeu en plein ecran  | 
| `jeu.decalage_y`  | decalage en y du jeu en plein ecran  | 
---

## ⌨️ Gestion du clavier
### Touches supportées
(temporaire)
- **Touches spéciales :** Espace, Entrée, Échap, Escape, Tab, Maj, Shift
- **Touches fonction :** F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12
- **Lettres :** A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z
- **Chiffres :** 0, 1, 2, 3, 4, 5, 6, 7, 8, 9

```python
PyCgame.touche_presser("Z")
PyCgame.touche_enfoncee("Z")
```

---

## 🖼️ Gestion des images et du texte

```python
PyCgame.ajouter_image(id_="./assets/perso.png", x=10, y=20, w=32, h=32, id_num=2)
PyCgame.ajouter_mot(lien="./assets/police.png", mot="Hello", x=50, y=50, coeff=1, ecart=1, id_num=1)
PyCgame.supprimer_image(1)
PyCgame.modifier_image(x=20, y=30, w=32, h=32, id_num=1)
# impossible de modifier la texture sur des caracteres (pour le moment)
PyCgame.modifier_texture("./assets/nouvelle_image.png", id_num=2)
PyCgame.ecrire_console("Bonjour le monde !")
```

---

## 🔊 Gestion des sons

```python
#wav obligatoire (pour le moment)
PyCgame.jouer_son("./assets/son.wav", boucle=1, canal=3) # boucle =  0 sera infini 1 = 1 tour etc...
PyCgame.arreter_son("./assets/son.wav")
PyCgame.arreter_canal(3)
```

---

## 🧮 Fonctions mathématiques intégrées

```python
jeu.abs_val(-5)
jeu.clamp(10, 0, 5)
jeu.pow(2, 3)
jeu.sqrt(16)
jeu.sin(3.14)
jeu.atan2(1, 1)
```

> Et beaucoup d’autres : `cos`, `tan`, `log`, `exp`, `floor`, `ceil`, `round`, `trunc`, `fmod`, `hypot`, etc.

---

## 🖥️ Redimensionnement

```python
jeu.redimensionner_fenetre()
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

   * `./mon_dossier` → dans le dossier courant
   * `../mon_dossier` → un niveau au-dessus

2. 🖼️ Chaque caractère doit être une image séparée :

   * Nom du fichier = code ASCII du caractère
   * Exemple : `"A" = 65.png`, `"z" = 122.png`

3. 📏 Tous les caractères doivent avoir la même hauteur :

   * Ajoutez de l’espace en haut ou en bas si nécessaire
   * Tout reste bien aligné à l’écran
   * 
4. exemple final : jeu.ajouter_image(id_="./mon_dossier", x=10, y=20, w=32, h=32, id_num=2)
---

## ✅ Notes importantes

* Les chemins des images et sons doivent être **en rapport au dossier courant**.
* `update_func` doit être **une fonction Python callable**.
* Les images doivent avoir un **id unique** pour pouvoir les modifier ou supprimer.

💡 Avec **PyCgame**, vous êtes prêt à créer votre jeu 2D en Python rapidement et proprement !


---


## 📬 Support & suggestions

Pour tout bug ou toute suggestion, merci d’envoyer un mail à :
📧 Baptiste.guerin34@gmail.com
