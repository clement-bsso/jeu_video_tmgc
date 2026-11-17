#ifndef DECOR_HPP
#define DECOR_HPP

#include <SFML/Graphics.hpp>

// Le décor vu de côté : ciel, soleil, nuages, collines, sol et arbre.
class Decor
{
public:
    Decor();

    void mettreAJour(float dt);     // fait avancer les nuages

    // heure : moment de la journée entre 0 (matin) et 1 (soir).
    // Le soleil traverse le ciel et la lumière change en fonction.
    void dessiner(sf::RenderWindow& fenetre, float heure) const;

private:
    void dessinerNuage(sf::RenderWindow& fenetre, float x, float y) const;
    void dessinerArbre(sf::RenderWindow& fenetre, float x) const;

    float xNuage1;
    float xNuage2;
};

#endif
