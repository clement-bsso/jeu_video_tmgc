#include "Decor.hpp"
#include "Outils.hpp"
#include "Parametres.hpp"
#include <cmath>
#include <cstdint>

Decor::Decor()
    : xNuage1(150.f), xNuage2(560.f)
{
}

void Decor::mettreAJour(float dt)
{
    xNuage1 += 12.f * dt;
    xNuage2 += 7.f * dt;

    // Un nuage sorti à droite revient par la gauche
    if (xNuage1 > LARGEUR_FENETRE + 80.f) xNuage1 = -120.f;
    if (xNuage2 > LARGEUR_FENETRE + 80.f) xNuage2 = -120.f;
}

void Decor::dessiner(sf::RenderWindow& fenetre, float heure) const
{
    // lumiere vaut 0 le matin et le soir, 1 à midi
    float lumiere = std::sin(heure * PI);
    float clarte = 0.55f + 0.45f * lumiere;

    // Ciel : plus ou moins clair selon l'heure
    sf::Color ciel(static_cast<std::uint8_t>(135 * clarte),
                   static_cast<std::uint8_t>(206 * clarte),
                   static_cast<std::uint8_t>(235 * clarte));
    dessinerRectangle(fenetre, 0.f, 0.f, LARGEUR_FENETRE, HAUTEUR_FENETRE, ciel);

    // Soleil : il va de gauche à droite en montant puis en redescendant
    sf::CircleShape soleil(35.f);
    soleil.setOrigin({35.f, 35.f});
    soleil.setPosition({50.f + heure * 800.f, 300.f - 220.f * lumiere});
    soleil.setFillColor(sf::Color(255, 220, 80));
    fenetre.draw(soleil);

    dessinerNuage(fenetre, xNuage1, 90.f);
    dessinerNuage(fenetre, xNuage2, 150.f);

    // Deux collines au loin (de grands cercles dont on ne voit que le haut)
    sf::CircleShape colline(320.f, 100);    // 100 points pour un contour bien rond
    colline.setFillColor(sf::Color(95, 170, 95));
    colline.setPosition({-120.f, 390.f});
    fenetre.draw(colline);
    colline.setPosition({430.f, 370.f});
    fenetre.draw(colline);

    // Sol
    dessinerRectangle(fenetre, 0.f, HAUTEUR_SOL, LARGEUR_FENETRE, HAUTEUR_FENETRE - HAUTEUR_SOL,
                      sf::Color(110, 190, 90));
    dessinerRectangle(fenetre, 0.f, HAUTEUR_SOL, LARGEUR_FENETRE, 8.f, sf::Color(80, 150, 70));

    dessinerArbre(fenetre, 770.f);
}

// Un nuage, c'est trois cercles blancs collés
void Decor::dessinerNuage(sf::RenderWindow& fenetre, float x, float y) const
{
    sf::CircleShape boule(26.f);
    boule.setFillColor(sf::Color(255, 255, 255, 230));

    boule.setPosition({x, y});
    fenetre.draw(boule);
    boule.setPosition({x + 30.f, y - 14.f});
    fenetre.draw(boule);
    boule.setPosition({x + 62.f, y});
    fenetre.draw(boule);
}

void Decor::dessinerArbre(sf::RenderWindow& fenetre, float x) const
{
    // Tronc
    dessinerRectangle(fenetre, x - 12.f, HAUTEUR_SOL - 110.f, 24.f, 115.f, sf::Color(120, 80, 45));

    // Feuillage
    sf::CircleShape feuilles(48.f);
    feuilles.setOrigin({48.f, 48.f});
    feuilles.setFillColor(sf::Color(50, 135, 60));

    feuilles.setPosition({x - 35.f, HAUTEUR_SOL - 125.f});
    fenetre.draw(feuilles);
    feuilles.setPosition({x + 35.f, HAUTEUR_SOL - 125.f});
    fenetre.draw(feuilles);
    feuilles.setPosition({x, HAUTEUR_SOL - 170.f});
    fenetre.draw(feuilles);
}
