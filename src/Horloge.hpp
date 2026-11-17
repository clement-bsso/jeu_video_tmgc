#ifndef HORLOGE_HPP
#define HORLOGE_HPP

#include <SFML/System.hpp>

// Contrôle le temps du jeu.
// Elle mesure le temps réel entre deux images et applique la vitesse du jeu
// (x1 en temps normal, plus en mode démo).
class Horloge
{
public:
    Horloge();

    void nouvelleImage();       // à appeler une fois par tour de boucle

    float getDtReel() const;    // secondes réelles depuis l'image précédente (animations)
    float getDtJeu() const;     // secondes de jeu : temps réel x vitesse (besoins, âge)
    float getTempsTotal() const;// secondes réelles depuis le lancement du programme

    float getVitesse() const;
    void setVitesse(float nouvelleVitesse);
    void accelerer();           // vitesse x2
    void ralentir();            // vitesse /2

private:
    sf::Clock chrono;
    float dtReel;
    float tempsTotal;
    float vitesse;
};

#endif
