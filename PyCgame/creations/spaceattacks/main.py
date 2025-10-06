""""CODE EN PARTIE GENERE AVEC CLAUDE IA -> IA EST LARGEMENT CAPABLE DE GENERER DU CODE FONCTIONNEL A PARTIR DE LA BIBLIOTHEQUE"""

from PyCgame import PyCgame

# ==================== VARIABLES D'ÉTAT DU JEU ====================
init_mannette = False
vies = 3
score = 0
game_over = False

# Position et état du vaisseau du joueur
vaisseau_x = 176
vaisseau_y = 340
vaisseau_degats = 0  # Niveau de dégâts visuels (0-3)
invincible_timer = 0  # Temps d'invincibilité restant après collision

# ==================== GESTION DES PROJECTILES ====================
projectiles = []  # Liste des projectiles actifs
projectile_anim_timer = 0  # Timer pour l'animation des projectiles
projectile_anim_frame = 1  # Frame actuelle de l'animation (1-3)
tir_cooldown = 0.5  # Temps avant de pouvoir tirer à nouveau

# ==================== GESTION DES MONSTRES ====================
monstres = []  # Liste des monstres actifs à l'écran
monstre_spawn_timer = 0  # Timer pour le spawn des monstres
monstre_anim_timer = 0  # Timer pour l'animation des monstres
monstre_anim_frame = 1  # Frame actuelle de l'animation (1-6)

# ==================== GESTION DES EXPLOSIONS ====================
explosions = []  # Liste des explosions en cours d'animation

# ==================== CONSTANTES DE GAMEPLAY ====================
VAISSEAU_VITESSE = 200.0  # Pixels par seconde
PROJECTILE_VITESSE = 200.0  # Pixels par seconde
MONSTRE_VITESSE = 120.0  # Pixels par seconde
MONSTRE_SPAWN_DELAY = 0.4  # Secondes entre chaque spawn
INVINCIBLE_DURATION = 1.0  # Durée d'invincibilité après dégât
FIRE_RATE = 0.5  # Délai minimum entre deux tirs (secondes)

# ==================== HITBOXES PRÉCISES ====================
# Définit les zones de collision réelles des sprites (offset depuis le coin supérieur gauche)
VAISSEAU_HITBOX = {'w': 38, 'h': 38, 'offset_x': 5, 'offset_y': 5}
MONSTRE_HITBOX = {'w': 47, 'h': 31, 'offset_x': 8, 'offset_y': 16}
PROJECTILE_HITBOX = {'w': 8, 'h': 24, 'offset_x': 12, 'offset_y': 4}


def collision_rect(x1, y1, w1, h1, x2, y2, w2, h2):
    """
    Détecte une collision entre deux rectangles (AABB - Axis-Aligned Bounding Box)
    
    Args:
        x1, y1: Position du premier rectangle
        w1, h1: Dimensions du premier rectangle
        x2, y2: Position du second rectangle
        w2, h2: Dimensions du second rectangle
    
    Returns:
        bool: True si les rectangles se chevauchent
    """
    return (x1 < x2 + w2 and x1 + w1 > x2 and
            y1 < y2 + h2 and y1 + h1 > y2)


def spawn_monstre():
    """Fait apparaître un nouveau monstre en haut de l'écran à une position aléatoire"""
    x = PyCgame.random(0, 340)
    y = -70  # Spawn au-dessus de l'écran pour apparition progressive
    monstre = {
        'x': x,
        'y': y
    }
    monstres.append(monstre)


def creer_explosion(x, y):
    """
    Crée une animation d'explosion à la position donnée
    
    Args:
        x, y: Position de l'explosion
    """
    explosion = {
        'x': x,
        'y': y,
        'frame': 1,  # Commence à la première frame
        'timer': 0   # Timer pour changer de frame
    }
    explosions.append(explosion)
    PyCgame.jouer_son("./assets/explosion.wav", boucle=1, canal=1)


def tirer_projectile():
    """Crée un nouveau projectile à partir de la position actuelle du vaisseau"""
    global projectiles
    proj = {
        'x': vaisseau_x + 8,  # Centré sur le vaisseau
        'y': vaisseau_y - 10  # Légèrement au-dessus
    }
    projectiles.append(proj)
    PyCgame.jouer_son("./assets/tir.wav", boucle=1, canal=2)


def update_jeu():
    """Boucle principale du jeu - appelée à chaque frame"""
    global init_mannette, vies, score, game_over
    global vaisseau_x, vaisseau_y, vaisseau_degats, invincible_timer
    global projectiles, projectile_anim_timer, projectile_anim_frame
    global monstres, monstre_spawn_timer, monstre_anim_timer, monstre_anim_frame
    global explosions, tir_cooldown
    
    dt = PyCgame.dt  # Delta time - temps écoulé depuis la dernière frame
    
    # Basculer en plein écran avec F3
    if PyCgame.touche_presser("F3"):
        PyCgame.redimensionner_fenetre()
    
    # Initialiser la manette une seule fois
    if not init_mannette:
        PyCgame.init_mannette()
        init_mannette = True
    
    # Dessiner l'arrière-plan
    PyCgame.dessiner_image("./assets/fond.png", 0, 0, 400, 400)
    
    # ==================== ÉCRAN GAME OVER ====================
    if game_over:
        PyCgame.dessiner_mot("./assets/police", "GAME OVER", 130, 200, 2, 3)
        PyCgame.dessiner_mot("./assets/police", f"SCORE: {score}", 165, 250, 1, 3)
        PyCgame.dessiner_mot("./assets/police", "APPUIE B RESTART", 140, 300, 1, 2)
        
        # Redémarrer le jeu avec le bouton B
        if PyCgame.touche_mannette_juste_presse("B"):
            # Réinitialiser toutes les variables
            vies = 3
            score = 0
            game_over = False
            vaisseau_x = 176
            vaisseau_y = 340
            vaisseau_degats = 0
            invincible_timer = 0
            projectiles.clear()
            monstres.clear()
            explosions.clear()
        return
    
    # ==================== TIMER D'INVINCIBILITÉ ====================
    if invincible_timer > 0:
        invincible_timer -= dt
        if invincible_timer < 0:
            invincible_timer = 0
    
    # ==================== DÉPLACEMENT DU VAISSEAU ====================
    joysticks = PyCgame.renvoie_joysticks()
    joy_x = joysticks[0]  # Stick gauche horizontal
    joy_y = joysticks[1]  # Stick gauche vertical
    
    # Déplacement basé sur le joystick
    vaisseau_x += joy_x * VAISSEAU_VITESSE * dt
    vaisseau_y += joy_y * VAISSEAU_VITESSE * dt
    
    # Empêcher le vaisseau de sortir de l'écran
    vaisseau_x = max(0, min(vaisseau_x, 352))
    vaisseau_y = max(0, min(vaisseau_y, 352))
    
    # ==================== SYSTÈME DE TIR ====================
    if tir_cooldown > 0:
        tir_cooldown -= dt
    
    # Tirer avec le bouton A si le cooldown est terminé
    if PyCgame.touche_mannette_enfoncee("A") and tir_cooldown <= 0:
        tirer_projectile()
        tir_cooldown = FIRE_RATE
    
    # ==================== ANIMATION DES PROJECTILES ====================
    projectile_anim_timer += dt
    if projectile_anim_timer > 0.15:
        projectile_anim_timer = 0
        projectile_anim_frame = (projectile_anim_frame % 3) + 1  # Cycle 1->2->3->1
    
    # ==================== MISE À JOUR DES PROJECTILES ====================
    projectiles_a_supprimer = []
    for proj in projectiles:
        # Déplacer le projectile vers le haut
        proj['y'] -= PROJECTILE_VITESSE * dt
        
        # Supprimer si hors écran
        if proj['y'] < -20:
            projectiles_a_supprimer.append(proj)
    
    # Nettoyer les projectiles hors écran
    for proj in projectiles_a_supprimer:
        projectiles.remove(proj)
    
    # ==================== SPAWN DES MONSTRES ====================
    monstre_spawn_timer += dt
    if monstre_spawn_timer > MONSTRE_SPAWN_DELAY:
        monstre_spawn_timer = 0
        spawn_monstre()
    
    # ==================== ANIMATION DES MONSTRES ====================
    monstre_anim_timer += dt
    if monstre_anim_timer > 0.2:
        monstre_anim_timer = 0
        monstre_anim_frame = (monstre_anim_frame % 6) + 1  # Cycle 1->2->3->4->5->6->1
    
    # ==================== MISE À JOUR DES MONSTRES ====================
    monstres_a_supprimer = []
    for monstre in monstres:
        # Déplacer le monstre vers le bas
        monstre['y'] += MONSTRE_VITESSE * dt
        
        # Supprimer si hors écran
        if monstre['y'] > 400:
            monstres_a_supprimer.append(monstre)
            continue
        
        # ========== COLLISION MONSTRE ↔ VAISSEAU ==========
        if invincible_timer == 0 and collision_rect(
            monstre['x'] + MONSTRE_HITBOX['offset_x'], 
            monstre['y'] + MONSTRE_HITBOX['offset_y'], 
            MONSTRE_HITBOX['w'], 
            MONSTRE_HITBOX['h'],
            vaisseau_x + VAISSEAU_HITBOX['offset_x'], 
            vaisseau_y + VAISSEAU_HITBOX['offset_y'], 
            VAISSEAU_HITBOX['w'], 
            VAISSEAU_HITBOX['h']
        ):
            # Le vaisseau prend des dégâts
            vies -= 1
            vaisseau_degats = min(vaisseau_degats + 1, 3)
            invincible_timer = INVINCIBLE_DURATION
            creer_explosion(monstre['x'], monstre['y'])
            monstres_a_supprimer.append(monstre)
            PyCgame.jouer_son("./assets/degat.wav", boucle=1, canal=3)
            
            # Vérifier game over
            if vies <= 0:
                game_over = True
            continue
        
        # ========== COLLISION MONSTRE ↔ PROJECTILES ==========
        touche = False
        for proj in projectiles:
            if collision_rect(
                monstre['x'] + MONSTRE_HITBOX['offset_x'], 
                monstre['y'] + MONSTRE_HITBOX['offset_y'], 
                MONSTRE_HITBOX['w'], 
                MONSTRE_HITBOX['h'],
                proj['x'] + PROJECTILE_HITBOX['offset_x'], 
                proj['y'] + PROJECTILE_HITBOX['offset_y'], 
                PROJECTILE_HITBOX['w'], 
                PROJECTILE_HITBOX['h']
            ):
                # Monstre détruit
                score += 100
                creer_explosion(monstre['x'], monstre['y'])
                monstres_a_supprimer.append(monstre)
                projectiles_a_supprimer.append(proj)
                touche = True
                break
    
    # Nettoyer les monstres détruits ou hors écran
    for monstre in monstres_a_supprimer:
        if monstre in monstres:
            monstres.remove(monstre)
    
    for proj in projectiles_a_supprimer:
        if proj in projectiles:
            projectiles.remove(proj)

    # ==================== ANIMATION DES EXPLOSIONS ====================
    explosions_a_supprimer = []
    for exp in explosions:
        exp['timer'] += dt
        if exp['timer'] > 0.1:  # Changer de frame toutes les 0.1 secondes
            exp['timer'] = 0
            exp['frame'] += 1
            if exp['frame'] > 3:  # L'animation a 3 frames
                explosions_a_supprimer.append(exp)

    # Nettoyer les explosions terminées
    for exp in explosions_a_supprimer:
        if exp in explosions:
            explosions.remove(exp)
    
    # ==================== RENDU VISUEL ====================
    # Ordre de dessin : fond -> explosions -> monstres -> projectiles -> vaisseau -> HUD
    
    # Dessiner les explosions
    for exp in explosions:
        PyCgame.dessiner_image(f"./assets/explosion {exp['frame']}.png", exp['x'], exp['y'], 62, 62)
    
    # Dessiner les monstres avec animation
    for monstre in monstres:
        PyCgame.dessiner_image(f"./assets/monstre {monstre_anim_frame}.png", monstre['x'], monstre['y'], 62, 62)
    
    # Dessiner les projectiles avec animation
    for proj in projectiles:
        PyCgame.dessiner_image(f"./assets/projectile {projectile_anim_frame}.png", proj['x'], proj['y'], 32, 32)
    
    # Dessiner le vaisseau (clignotement pendant l'invincibilité)
    if invincible_timer == 0 or int(invincible_timer * 10) % 2 == 0:
        frame_vaisseau = min(vaisseau_degats + 1, 4)  # Frames 1-4 selon les dégâts
        PyCgame.dessiner_image(f"./assets/vaisseau {frame_vaisseau}.png", vaisseau_x, vaisseau_y, 48, 48)
    
    # ==================== HUD (INTERFACE) ====================
    PyCgame.dessiner_mot("./assets/police", f"VIES: {vies}", 10, 10, 1, 2)
    PyCgame.dessiner_mot("./assets/police", f"SCORE: {score}", 300, 10, 1, 2)


# ==================== INITIALISATION DU JEU ====================
PyCgame.init(
    largeur=400,
    hauteur=400,
    fps=120,
    coeff=2,
    chemin_image="./assets",
    chemin_son="./assets",
    dessiner=True,
    bande_noir=True,
    r=0, g=0, b=0,
    update_func=update_jeu,
    nom_fenetre="Space Attacks",
    debug=False
)