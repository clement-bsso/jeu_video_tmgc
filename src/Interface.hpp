#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <string>
#include <SFML/Graphics.hpp>
#include "Bouton.hpp"
#include "Creature.hpp"

// Ce que le joueur peut demander en cliquant sur un bouton
enum class Action { AUCUNE, NOURRIR, NETTOYER, JOUER };

// Tout ce qui s'affiche par-dessus la scène pendant la partie :
// les informations sur la créature, les jauges, les états et les boutons d'action.
class Interface
{
public:
    Interface(const sf::Font& police);

    Action gererClic(sf::Vector2f souris) const;    // quel bouton a été cliqué ?

    void afficherMessage(const std::string& texte); // bandeau temporaire en haut de l'écran
    void effacerMessage();
    void mettreAJour(float dt);

    void dessiner(sf::RenderWindow& fenetre, const Creature& creature, sf::Vector2f souris) const;

private:
    void dessinerInfos(sf::RenderWindow& fenetre, const Creature& creature) const;
    void dessinerJauge(sf::RenderWindow& fenetre, const Besoin& besoin, float y) const;
    void dessinerEtats(sf::RenderWindow& fenetre, const Creature& creature) const;
    void dessinerMessage(sf::RenderWindow& fenetre) const;

    const sf::Font& police;
    Bouton boutonNourrir;
    Bouton boutonNettoyer;
    Bouton boutonJouer;
    std::string message;
    float tempsMessage;         // temps d'affichage restant pour le message
};

#endif
