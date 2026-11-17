#ifndef CREATURE_HPP
#define CREATURE_HPP

#include <string>
#include <vector>
#include "Besoin.hpp"

enum class Stade { BEBE, ADO, ADULTE };
enum class Forme { FAVORABLE, NEUTRE, DEFAVORABLE };
enum class CauseMort { AUCUNE, VIEILLESSE, NEGLIGENCE };

// Les états possibles. La créature peut en avoir plusieurs en même temps.
enum class Etat
{
    MORIBONDE, MALADE, AFFAMEE, SALE, TRISTE,
    EN_PLEINE_FORME, HEUREUSE, REPUE, PROPRE,
    NORMALE
};

// Texte à afficher pour un état, un stade ou une forme
std::string texteEtat(Etat etat);
std::string texteStade(Stade stade);
std::string texteForme(Forme forme);
bool estEtatNegatif(Etat etat);

// La créature : ses besoins, son âge, son stade d'évolution.
// Cette classe ne contient que la logique du jeu, aucun affichage.
class Creature
{
public:
    Creature(const std::string& nom);

    // Fait avancer le temps de dt secondes. Renvoie true si la créature vient d'évoluer.
    bool mettreAJour(float dt);
    void reinitialiser();               // redevient un bébé tout neuf

    // Actions du joueur
    void nourrir();
    void nettoyer();
    void jouer(float gainBonheur);      // appelée à la fin d'un mini-jeu

    // États
    std::vector<Etat> getEtats() const; // du plus grave au moins grave
    bool aEtat(Etat etat) const;

    // Accès aux informations
    const std::string& getNom() const;
    Besoin& getFaim();
    Besoin& getProprete();
    Besoin& getBonheur();
    const Besoin& getFaim() const;
    const Besoin& getProprete() const;
    const Besoin& getBonheur() const;
    float getAge() const;               // en jours
    void setAge(float jours);           // utilisé par le mode démo
    float getAgeProchaineEvolution() const;  // -1 s'il n'y a plus d'évolution
    Stade getStade() const;
    Forme getForme() const;
    bool estVivante() const;
    CauseMort getCauseMort() const;

private:
    bool verifierEvolution();
    Forme calculerForme() const;
    void verifierMort(float dt);
    int compterBesoinsVides() const;
    int compterBesoinsPleins() const;

    std::string nom;
    Besoin faim;                        // 100 = rassasiée, 0 = affamée
    Besoin proprete;
    Besoin bonheur;
    float age;                          // en jours
    Stade stade;
    Forme forme;
    bool vivante;
    CauseMort causeMort;
    float tempsMoribonde;               // secondes passées à la suite dans l'état moribonde
};

#endif
