#ifndef MINI_JEU_HPP
#define MINI_JEU_HPP

#include <string>
#include <SFML/Graphics.hpp>

enum class Resultat { VICTOIRE, EGALITE, DEFAITE };

// Classe de base de tous les mini-jeux.
// Le jeu principal ne connaît que cette classe : il peut donc lancer un Chifoumi ou un Morpion
// exactement de la même façon, et on peut ajouter un nouveau mini-jeu sans toucher à la classe Jeu.
class MiniJeu
{
public:
    MiniJeu(const sf::Font& police);
    virtual ~MiniJeu();

    // Ce que chaque mini-jeu doit obligatoirement savoir faire
    virtual void demarrer() = 0;                        // commence une nouvelle partie
    virtual void gererClic(sf::Vector2f souris) = 0;
    virtual void dessiner(sf::RenderWindow& fenetre, sf::Vector2f souris) const = 0;

    // Commun à tous les mini-jeux
    bool estTermine() const;
    Resultat getResultat() const;

protected:
    void terminer(Resultat resultatFinal);              // à appeler quand la partie est finie

    const sf::Font& police;
    bool termine;
    Resultat resultat;
};

#endif
