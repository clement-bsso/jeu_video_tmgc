#ifndef JEU_HPP
#define JEU_HPP

#include <string>
#include <SFML/Graphics.hpp>
#include "Horloge.hpp"
#include "Decor.hpp"
#include "Creature.hpp"
#include "AffichageCreature.hpp"
#include "Interface.hpp"
#include "ModeDemo.hpp"
#include "Chifoumi.hpp"
#include "Morpion.hpp"
#include "Bouton.hpp"

// Les différents écrans du jeu
enum class Ecran { TITRE, PRINCIPAL, CHOIX_MINI_JEU, MINI_JEU, FIN };

// La classe principale : elle possède la fenêtre et tous les éléments du jeu,
// et fait tourner la boucle (événements -> mise à jour -> dessin).
class Jeu
{
public:
    Jeu(const sf::Font& police);
    void lancer();

private:
    // Les trois étapes de la boucle principale
    void gererEvenements();
    void mettreAJour();
    void dessiner();

    // Événements
    void gererTouche(sf::Keyboard::Key touche);
    void gererSaisieNom(char32_t caractere);
    void gererClic(sf::Vector2f souris);
    void gererClicPrincipal(sf::Vector2f souris);

    // Changements d'écran
    void nouvellePartie();
    void lancerMiniJeu(MiniJeu* miniJeu);
    void terminerMiniJeu();

    // Dessin des écrans autres que l'écran principal
    void dessinerPanneau(float x, float y, float largeur, float hauteur);
    void dessinerEcranTitre(sf::Vector2f souris);
    void dessinerChoixMiniJeu(sf::Vector2f souris);
    void dessinerMiniJeu(sf::Vector2f souris);
    void dessinerEcranFin(sf::Vector2f souris);

    sf::RenderWindow fenetre;
    const sf::Font& police;

    Horloge horloge;
    Decor decor;
    Creature creature;
    AffichageCreature affichageCreature;
    Interface interfaceJeu;
    ModeDemo modeDemo;

    Chifoumi chifoumi;
    Morpion morpion;
    MiniJeu* miniJeuEnCours;        // pointe sur chifoumi ou morpion selon le choix du joueur

    Ecran ecran;
    std::string nomSaisi;           // nom tapé sur l'écran titre

    Bouton boutonCommencer;
    Bouton boutonChifoumi;
    Bouton boutonMorpion;
    Bouton boutonAnnuler;
    Bouton boutonRetour;
    Bouton boutonRecommencer;
};

#endif
