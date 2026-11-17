#include "Bouton.hpp"
#include "Outils.hpp"

Bouton::Bouton(const sf::Font& police, const std::string& libelle,
               float x, float y, float largeur, float hauteur, unsigned int tailleTexte)
    : police(police), libelle(libelle), zone({x, y}, {largeur, hauteur}), tailleTexte(tailleTexte)
{
}

bool Bouton::contient(sf::Vector2f point) const
{
    return zone.contains(point);
}

void Bouton::dessiner(sf::RenderWindow& fenetre, sf::Vector2f souris) const
{
    sf::Color couleur(60, 110, 170);
    if (contient(souris))
        couleur = sf::Color(90, 150, 215);

    dessinerRectangle(fenetre, zone.position.x, zone.position.y, zone.size.x, zone.size.y,
                      couleur, sf::Color(30, 60, 100));
    dessinerTexteCentre(fenetre, police, libelle, tailleTexte, sf::Color::White,
                        zone.position.x + zone.size.x / 2.f, zone.position.y + zone.size.y / 2.f);
}
