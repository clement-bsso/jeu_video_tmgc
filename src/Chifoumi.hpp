#ifndef CHIFOUMI_HPP
#define CHIFOUMI_HPP

#include <vector>
#include "MiniJeu.hpp"
#include "Bouton.hpp"

// Pierre-feuille-ciseaux contre la créature. Le premier à 2 points gagne la partie.
class Chifoumi : public MiniJeu
{
public:
    Chifoumi(const sf::Font& police);

    void demarrer() override;
    void gererClic(sf::Vector2f souris) override;
    void dessiner(sf::RenderWindow& fenetre, sf::Vector2f souris) const override;

private:
    void jouerManche(int choix);

    std::vector<Bouton> boutons;    // dans l'ordre : pierre, feuille, ciseaux
    int scoreJoueur;
    int scoreCreature;
    int choixJoueur;                // 0 = pierre, 1 = feuille, 2 = ciseaux, -1 = rien encore
    int choixCreature;
    std::string message;
};

#endif
