#ifndef BOUTON_HPP
#define BOUTON_HPP

#include <string>
#include <SFML/Graphics.hpp>

// Un bouton cliquable : un rectangle avec un texte au milieu.
class Bouton
{
public:
    Bouton(const sf::Font& police, const std::string& libelle,
           float x, float y, float largeur, float hauteur, unsigned int tailleTexte = 16);

    bool contient(sf::Vector2f point) const;    // vrai si le point est sur le bouton

    // Le bouton s'éclaircit quand la souris passe dessus
    void dessiner(sf::RenderWindow& fenetre, sf::Vector2f souris) const;

private:
    const sf::Font& police;
    std::string libelle;
    sf::FloatRect zone;             // position et taille du bouton
    unsigned int tailleTexte;
};

#endif
