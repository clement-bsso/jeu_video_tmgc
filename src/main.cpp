#include <cstdlib>
#include <ctime>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Jeu.hpp"

int main()
{
    // Pour que le hasard des mini-jeux soit différent à chaque lancement
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // La police est chargée une seule fois ici, puis partagée par tout le jeu
    sf::Font police;
    if (!police.openFromFile("assets/police.ttf"))
    {
        std::cerr << "Impossible de charger assets/police.ttf." << std::endl;
        std::cerr << "Le jeu doit etre lance depuis le dossier du projet." << std::endl;
        return 1;
    }

    Jeu jeu(police);
    jeu.lancer();
    return 0;
}
