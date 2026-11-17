#include "Outils.hpp"
#include <sstream>
#include <iomanip>

sf::String utf8(const std::string& texte)
{
    return sf::String::fromUtf8(texte.begin(), texte.end());
}

std::string enTexte(float valeur, int decimales)
{
    std::ostringstream flux;
    flux << std::fixed << std::setprecision(decimales) << valeur;
    return flux.str();
}

void dessinerTexte(sf::RenderWindow& fenetre, const sf::Font& police, const std::string& contenu,
                   unsigned int taille, sf::Color couleur, float x, float y)
{
    sf::Text texte(police, utf8(contenu), taille);
    texte.setFillColor(couleur);
    texte.setPosition({x, y});
    fenetre.draw(texte);
}

void dessinerTexteCentre(sf::RenderWindow& fenetre, const sf::Font& police, const std::string& contenu,
                         unsigned int taille, sf::Color couleur, float x, float y)
{
    sf::Text texte(police, utf8(contenu), taille);
    texte.setFillColor(couleur);

    // On place l'origine au milieu du texte, puis ce milieu sur le point demandé
    sf::FloatRect cadre = texte.getLocalBounds();
    texte.setOrigin({cadre.position.x + cadre.size.x / 2.f, cadre.position.y + cadre.size.y / 2.f});
    texte.setPosition({x, y});
    fenetre.draw(texte);
}

float largeurTexte(const sf::Font& police, const std::string& contenu, unsigned int taille)
{
    sf::Text texte(police, utf8(contenu), taille);
    return texte.getLocalBounds().size.x;
}

void dessinerRectangle(sf::RenderWindow& fenetre, float x, float y, float largeur, float hauteur,
                       sf::Color couleur, sf::Color contour)
{
    sf::RectangleShape rectangle(sf::Vector2f(largeur, hauteur));
    rectangle.setPosition({x, y});
    rectangle.setFillColor(couleur);
    if (contour != sf::Color::Transparent)
    {
        rectangle.setOutlineThickness(2.f);
        rectangle.setOutlineColor(contour);
    }
    fenetre.draw(rectangle);
}
