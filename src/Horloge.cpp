#include "Horloge.hpp"
#include "Parametres.hpp"

Horloge::Horloge()
    : dtReel(0.f), tempsTotal(0.f), vitesse(1.f)
{
}

void Horloge::nouvelleImage()
{
    dtReel = chrono.restart().asSeconds();

    // Si le jeu a été bloqué un moment (fenêtre déplacée par exemple),
    // on évite de faire un énorme saut dans le temps.
    if (dtReel > 0.1f)
        dtReel = 0.1f;

    tempsTotal += dtReel;
}

float Horloge::getDtReel() const
{
    return dtReel;
}

float Horloge::getDtJeu() const
{
    return dtReel * vitesse;
}

float Horloge::getTempsTotal() const
{
    return tempsTotal;
}

float Horloge::getVitesse() const
{
    return vitesse;
}

void Horloge::setVitesse(float nouvelleVitesse)
{
    vitesse = nouvelleVitesse;
    if (vitesse < 1.f)         vitesse = 1.f;
    if (vitesse > VITESSE_MAX) vitesse = VITESSE_MAX;
}

void Horloge::accelerer()
{
    setVitesse(vitesse * 2.f);
}

void Horloge::ralentir()
{
    setVitesse(vitesse / 2.f);
}
