#ifndef BESOIN_HPP
#define BESOIN_HPP

#include <string>

// Un besoin de la créature (faim, propreté ou bonheur).
// Sa valeur va de 0 (très mauvais) à 100 (parfait) et baisse toute seule avec le temps.
class Besoin
{
public:
    Besoin(const std::string& nom, float vitesseDegradation);

    void mettreAJour(float dt);         // fait baisser la valeur (dt = temps écoulé en secondes)
    void ajouter(float quantite);       // quantité positive ou négative
    void setValeur(float nouvelleValeur);

    float getValeur() const;
    const std::string& getNom() const;
    bool estPresqueVide() const;
    bool estPresquePlein() const;

private:
    std::string nom;
    float valeur;
    float vitesseDegradation;           // points perdus par seconde
};

#endif
