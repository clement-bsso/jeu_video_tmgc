#ifndef AFFICHAGE_CREATURE_HPP
#define AFFICHAGE_CREATURE_HPP

#include <string>
#include <SFML/Graphics.hpp>
#include "Creature.hpp"

// Dessine la créature et gère ses animations.
// Elle est dessinée uniquement avec des formes simples (ellipses, triangles, traits),
// dans un repère local : le point (0, 0) est entre ses pieds et elle regarde vers la droite.
// Une transformation SFML la place ensuite au bon endroit, à la bonne taille et dans le bon sens.
class AffichageCreature
{
public:
    AffichageCreature(const sf::Font& police);

    void mettreAJour(float dt, const Creature& creature);
    void dessiner(sf::RenderWindow& fenetre, const Creature& creature) const;

    void reagir(const std::string& texte);  // petite bulle + sauts après une action du joueur
    void lancerAnimationEvolution();        // la créature clignote pendant qu'elle évolue
    void replacerAuCentre();

private:
    // Les différentes parties du dessin
    void dessinerDos(sf::RenderWindow& fenetre, const sf::Transform& repere,
                     const Creature& creature, sf::Color couleur) const;
    void dessinerCorps(sf::RenderWindow& fenetre, const sf::Transform& repere,
                       const Creature& creature, sf::Color couleur) const;
    void dessinerTete(sf::RenderWindow& fenetre, const sf::Transform& repere,
                      const Creature& creature, sf::Color couleur) const;
    void dessinerVisage(sf::RenderWindow& fenetre, const sf::Transform& repere,
                        const Creature& creature) const;
    void dessinerBulle(sf::RenderWindow& fenetre, const Creature& creature, float hauteur) const;

    sf::Color choisirCouleur(const Creature& creature) const;
    float choisirTaille(Stade stade) const;
    std::string choisirTexteBulle(const Creature& creature) const;

    // Formes de base
    void ellipse(sf::RenderWindow& fenetre, const sf::Transform& repere,
                 float cx, float cy, float rayonX, float rayonY, sf::Color couleur) const;
    void triangle(sf::RenderWindow& fenetre, const sf::Transform& repere,
                  sf::Vector2f a, sf::Vector2f b, sf::Vector2f c, sf::Color couleur) const;
    void trait(sf::RenderWindow& fenetre, const sf::Transform& repere,
               float cx, float cy, float longueur, float angle, sf::Color couleur) const;
    void demiDisque(sf::RenderWindow& fenetre, const sf::Transform& repere,
                    float cx, float cy, float rayon, bool versLeBas, sf::Color couleur) const;

    const sf::Font& police;
    float x;                    // position horizontale de la créature
    int direction;              // 1 : va vers la droite, -1 : vers la gauche
    bool marche;
    float temps;                // temps écoulé, sert de base à toutes les animations
    float tempsReaction;        // temps restant pour la réaction en cours
    std::string texteReaction;
    float tempsEvolution;       // temps restant pour l'animation d'évolution
};

#endif
