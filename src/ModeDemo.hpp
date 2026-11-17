#ifndef MODE_DEMO_HPP
#define MODE_DEMO_HPP

#include <vector>
#include <SFML/Graphics.hpp>
#include "Bouton.hpp"
#include "Creature.hpp"
#include "Horloge.hpp"

// Le mode démo (ou "triche") : un panneau de boutons pour tester le jeu rapidement.
// Il permet d'accélérer le temps, de changer l'âge et les besoins de la créature,
// et de déclencher tout de suite la prochaine évolution.
// Tant qu'il n'est pas activé, il n'a aucun effet sur la partie.
class ModeDemo
{
public:
    ModeDemo(const sf::Font& police);

    void basculer(Horloge& horloge);    // active ou désactive le mode
    void desactiver(Horloge& horloge);
    bool estActif() const;

    // Renvoie true si le clic était sur un bouton du panneau
    bool gererClic(sf::Vector2f souris, Creature& creature, Horloge& horloge);

    void dessiner(sf::RenderWindow& fenetre, const Creature& creature,
                  const Horloge& horloge, sf::Vector2f souris) const;

private:
    void appliquer(int numeroBouton, Creature& creature, Horloge& horloge);

    const sf::Font& police;
    bool actif;
    std::vector<Bouton> boutons;
};

#endif
