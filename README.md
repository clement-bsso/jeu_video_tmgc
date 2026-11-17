# Cesichi

Un jeu d'animal virtuel en C++ avec SFML. On adopte une créature, on s'occupe de ses besoins
(faim, propreté, bonheur), on joue avec elle, et elle évolue deux fois au cours de sa vie.
La forme qu'elle prend à chaque évolution dépend de la façon dont on s'en est occupé.

Cette version du projet est écrite pour **SFML 3** (3.0 ou plus).

## Compiler

Il faut `g++` (C++17) et SFML 3. Avec MSYS2, SFML s'installe par
`pacman -S mingw-w64-ucrt-x86_64-sfml`. Depuis le dossier du projet :

```
g++ -std=c++17 src/*.cpp -o cesichi -lsfml-graphics -lsfml-window -lsfml-system
```

Ou plus simplement, si `make` est installé :

```
make
```

Si SFML n'est pas installée par MSYS2 mais décompressée à la main (par exemple dans `C:/SFML`),
il faut indiquer son emplacement :

```
g++ -std=c++17 src/*.cpp -o cesichi -I C:/SFML/include -L C:/SFML/lib -lsfml-graphics -lsfml-window -lsfml-system
```

et copier les fichiers `.dll` de `C:/SFML/bin` à côté de `cesichi.exe`.

### Vérifier sa version de SFML

SFML 2 et SFML 3 ne sont pas compatibles. Pour savoir laquelle est installée :

- avec MSYS2 : `pacman -Q | grep sfml` affiche le numéro de version ;
- avec une SFML téléchargée à la main : le numéro est dans le nom du dossier, ou dans
  `include/SFML/Config.hpp` (ligne `SFML_VERSION_MAJOR`).

Si la compilation affiche des erreurs du type `'class sf::Font' has no member named 'openFromFile'`
ou des erreurs sur `pollEvent` et `sf::degrees`, c'est que la machine a SFML 2 :
il faut alors utiliser la variante SFML 2 du projet.

## Lancer

```
./cesichi
```

Le jeu doit être lancé **depuis le dossier du projet**, parce qu'il charge la police
`assets/police.ttf` avec un chemin relatif. Sous Windows, le plus simple est de le lancer
depuis le terminal MSYS2 qui a servi à compiler (il trouve les DLL de SFML tout seul).

## Contrôles

| Action | Commande |
|---|---|
| Choisir le nom de la créature | taper au clavier, puis Entrée (ou bouton Commencer) |
| Nourrir, nettoyer, jouer | clic sur les boutons en bas de l'écran |
| Jouer aux mini-jeux | clic |
| Ouvrir / fermer le mode démo | F1 (ou Tab) |

## Règles du jeu

### Les besoins

La créature a trois jauges, de 0 à 100. Elles démarrent à 70 et baissent toutes seules :

| Besoin | Baisse | Remonte avec |
|---|---|---|
| Faim (100 = rassasiée) | 0,70 point par seconde | Nourrir : +25 |
| Propreté | 0,45 point par seconde | Nettoyer : +30 |
| Bonheur | 0,55 point par seconde | Jouer : +35 (victoire), +20 (égalité), +10 (défaite) |

### Les états

Une jauge est « presque vide » à 20 ou moins et « presque pleine » à 80 ou plus.
La créature peut avoir plusieurs états à la fois, ils sont listés dans le panneau de gauche.

| État | Condition | Ce qu'on voit |
|---|---|---|
| Repue / Propre / Heureuse | la jauge correspondante est presque pleine | grand sourire quand elle est heureuse |
| En pleine forme | les trois jauges sont presque pleines | elle sautille |
| Affamée | faim presque vide | bouche ouverte, « J'ai faim ! » |
| Sale | propreté presque vide | taches marron sur le corps |
| Triste | bonheur presque vide | bouche à l'envers et larme |
| Malade | deux jauges presque vides | teint verdâtre, elle ne marche plus |
| Moribonde | trois jauges presque vides | grise, des croix à la place des yeux |
| Normale | aucun des états ci-dessus | petit sourire |

### Le temps et l'évolution

Un jour de la créature dure 20 secondes. Elle évolue deux fois :

- à **6 jours** (2 minutes) : bébé → ado ;
- à **15 jours** (5 minutes) : ado → adulte.

À chaque échéance, le jeu regarde les trois jauges à cet instant précis :

| Forme | Condition | Apparence |
|---|---|---|
| Favorable | les trois besoins sont à 60 ou plus | dorée, avec des ailes (et une auréole une fois adulte) |
| Défavorable | au moins un besoin est en dessous de 30 | violette, avec des cornes et des piques |
| Neutre | tous les autres cas | bleue |

La créature grossit à chaque stade : l'ado a des oreilles, l'adulte a en plus une queue.

### La fin de partie

La créature meurt de vieillesse à 30 jours (10 minutes), ou de négligence si elle reste
moribonde 30 secondes de suite. On peut alors recommencer avec une nouvelle créature.

### Les mini-jeux

Le bouton Jouer propose deux mini-jeux. Le temps de la créature est en pause pendant qu'on joue.

- **Chifoumi** : pierre-feuille-ciseaux, le premier à 2 points gagne.
- **Morpion** : le joueur a les croix et commence. La créature gagne si elle peut, bloque
  sinon, et joue au hasard le reste du temps.

## Mode démo

F1 (ou Tab) ouvre un panneau à droite de l'écran. Tant qu'il est fermé, il n'a aucun effet
sur la partie, et le fermer remet le temps à vitesse normale.

| Bouton | Effet |
|---|---|
| -20 / +20 | modifie un besoin |
| Tout à 100 / Tout à 10 | règle les trois besoins d'un coup |
| -1 j / +1 j | recule ou avance l'âge d'un jour |
| ÷2 / ×2 | ralentit ou accélère le temps (de ×1 à ×64) |
| Évoluer maintenant | amène la créature à l'âge de sa prochaine évolution |
| Réinitialiser | redonne un bébé tout neuf, à vitesse normale |

« Évoluer maintenant » ne choisit pas la forme : il avance seulement l'âge, et c'est la règle
normale du jeu qui décide ensuite d'après les jauges.

### Déroulé possible pour la soutenance

1. Lancer le jeu, donner un nom, montrer Nourrir, Nettoyer et un mini-jeu.
2. F1, puis **Tout à 100** et **Évoluer maintenant** : évolution favorable (ado doré).
   Recliquer sur **Évoluer maintenant** : adulte favorable.
3. **Réinitialiser**, **Tout à 10**, **Évoluer maintenant** : évolution défavorable.
   On voit au passage les états Moribonde, Affamée, Sale et Triste.
4. **Réinitialiser**, trois clics sur **-20** (un par besoin), **Évoluer maintenant** : forme neutre.
5. Pour montrer le temps accéléré : **×2** plusieurs fois, les jauges et le soleil s'emballent.
6. Pour montrer la mort : **Tout à 10** et vitesse ×64, ou **+1 j** jusqu'à 30 jours.

## Organisation du code

| Fichiers | Rôle |
|---|---|
| `main.cpp` | charge la police et lance le jeu |
| `Jeu` | fenêtre, boucle principale, passage d'un écran à l'autre |
| `Creature` | besoins, âge, états, évolution, mort (aucun affichage) |
| `Besoin` | une jauge qui baisse avec le temps |
| `Horloge` | mesure du temps et vitesse du jeu |
| `AffichageCreature` | dessin et animations de la créature |
| `Decor` | ciel, soleil, nuages, sol, arbre |
| `Interface` | panneau d'informations, jauges, états, boutons d'action |
| `Bouton` | bouton cliquable |
| `MiniJeu` | classe de base abstraite des mini-jeux |
| `Chifoumi`, `Morpion` | les deux mini-jeux, qui héritent de `MiniJeu` |
| `ModeDemo` | panneau de triche |
| `Outils` | petites fonctions de dessin (texte, rectangle) |
| `Parametres.hpp` | toutes les constantes de réglage |

Le diagramme de classes est dans `docs/diagramme_classes.png` (source PlantUML à côté).

Pour changer l'équilibrage (vitesse des jauges, âges d'évolution, seuils), tout est dans
`src/Parametres.hpp`.

## Police

`assets/police.ttf` est la police libre DejaVu Sans. Sa licence est dans
`assets/LICENCE_police.txt`.
