#ifndef OUTILS_HPP
#define OUTILS_HPP

#include <string>
#include <SFML/Graphics.hpp>

// Petites fonctions de dessin utilisées un peu partout,
// pour ne pas réécrire dix fois les mêmes lignes SFML.

const float PI = 3.14159265f;

// Convertit un texte avec accents pour que SFML l'affiche correctement
sf::String utf8(const std::string& texte);

// Transforme un nombre en texte, avec le nombre de décimales voulu
std::string enTexte(float valeur, int decimales = 0);

// Écrit un texte dont le coin haut-gauche est en (x, y)
void dessinerTexte(sf::RenderWindow& fenetre, const sf::Font& police, const std::string& contenu,
                   unsigned int taille, sf::Color couleur, float x, float y);

// Écrit un texte centré sur le point (x, y)
void dessinerTexteCentre(sf::RenderWindow& fenetre, const sf::Font& police, const std::string& contenu,
                         unsigned int taille, sf::Color couleur, float x, float y);

// Largeur en pixels qu'occupera un texte à l'écran
float largeurTexte(const sf::Font& police, const std::string& contenu, unsigned int taille);

// Dessine un rectangle plein, avec un contour si on en donne un
void dessinerRectangle(sf::RenderWindow& fenetre, float x, float y, float largeur, float hauteur,
                       sf::Color couleur, sf::Color contour = sf::Color::Transparent);

#endif
