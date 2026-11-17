#ifndef PARAMETRES_HPP
#define PARAMETRES_HPP

// Tous les réglages du jeu sont regroupés ici.
// Pour rendre le jeu plus facile ou plus difficile, c'est le seul fichier à modifier.

// --- Fenêtre ---
const unsigned int LARGEUR_FENETRE = 900;
const unsigned int HAUTEUR_FENETRE = 600;
const float HAUTEUR_SOL = 460.f;            // position verticale du sol (en pixels)

// --- Temps ---
const float DUREE_JOUR = 20.f;              // un jour de la créature dure 20 secondes de jeu
const float AGE_EVOLUTION_1 = 6.f;          // bébé -> ado à 6 jours (2 minutes)
const float AGE_EVOLUTION_2 = 15.f;         // ado -> adulte à 15 jours (5 minutes)
const float AGE_MORT = 30.f;                // mort de vieillesse à 30 jours (10 minutes)
const float DELAI_MORT_NEGLIGENCE = 30.f;   // secondes passées "moribonde" avant de mourir
const float VITESSE_MAX = 64.f;             // accélération maximale du mode démo

// --- Besoins (valeurs entre 0 et 100) ---
const float VALEUR_DEPART = 70.f;           // valeur des trois besoins au début de la partie
const float VITESSE_FAIM = 0.70f;           // points perdus par seconde
const float VITESSE_PROPRETE = 0.45f;
const float VITESSE_BONHEUR = 0.55f;
const float SEUIL_BAS = 20.f;               // en dessous : jauge "presque vide"
const float SEUIL_HAUT = 80.f;              // au-dessus : jauge "presque pleine"

// --- Actions du joueur ---
const float GAIN_NOURRIR = 25.f;
const float GAIN_NETTOYER = 30.f;
const float GAIN_VICTOIRE = 35.f;           // bonheur gagné selon le résultat du mini-jeu
const float GAIN_EGALITE = 20.f;
const float GAIN_DEFAITE = 10.f;

// --- Évolution ---
const float SEUIL_FAVORABLE = 60.f;         // les trois besoins au moins à 60 : forme favorable
const float SEUIL_DEFAVORABLE = 30.f;       // un besoin en dessous de 30 : forme défavorable

#endif
