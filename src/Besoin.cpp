#include "Besoin.hpp"
#include "Parametres.hpp"

Besoin::Besoin(const std::string& nom, float vitesseDegradation)
    : nom(nom), valeur(VALEUR_DEPART), vitesseDegradation(vitesseDegradation)
{
}

void Besoin::mettreAJour(float dt)
{
    setValeur(valeur - vitesseDegradation * dt);
}

void Besoin::ajouter(float quantite)
{
    setValeur(valeur + quantite);
}

// Toutes les modifications passent par ici : la valeur reste donc toujours entre 0 et 100.
void Besoin::setValeur(float nouvelleValeur)
{
    valeur = nouvelleValeur;
    if (valeur < 0.f)   valeur = 0.f;
    if (valeur > 100.f) valeur = 100.f;
}

float Besoin::getValeur() const
{
    return valeur;
}

const std::string& Besoin::getNom() const
{
    return nom;
}

bool Besoin::estPresqueVide() const
{
    return valeur <= SEUIL_BAS;
}

bool Besoin::estPresquePlein() const
{
    return valeur >= SEUIL_HAUT;
}
