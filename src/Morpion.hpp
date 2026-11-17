#ifndef MORPION_HPP
#define MORPION_HPP

#include "MiniJeu.hpp"

enum class Case { VIDE, JOUEUR, CREATURE };

// Morpion contre la créature : le joueur a les croix et commence, la créature a les ronds.
class Morpion : public MiniJeu
{
public:
    Morpion(const sf::Font& police);

    void demarrer() override;
    void gererClic(sf::Vector2f souris) override;
    void dessiner(sf::RenderWindow& fenetre, sf::Vector2f souris) const override;

private:
    bool aGagne(Case joueur) const;
    bool grillePleine() const;
    int chercherCoupGagnant(Case joueur);   // numéro de la case qui fait gagner, -1 s'il n'y en a pas
    void jouerCreature();

    // Les 9 cases, numérotées de gauche à droite puis de haut en bas :
    //  0 1 2
    //  3 4 5
    //  6 7 8
    Case grille[9];
    std::string message;
};

#endif
