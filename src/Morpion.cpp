#include "Morpion.hpp"
#include "Outils.hpp"
#include <cstdlib>
#include <vector>

// Position et taille de la grille à l'écran
const float X_GRILLE = 330.f;
const float Y_GRILLE = 160.f;
const float TAILLE_CASE = 80.f;

// Les 8 alignements gagnants : 3 lignes, 3 colonnes et 2 diagonales
const int ALIGNEMENTS[8][3] = {
    {0, 1, 2}, {3, 4, 5}, {6, 7, 8},
    {0, 3, 6}, {1, 4, 7}, {2, 5, 8},
    {0, 4, 8}, {2, 4, 6}
};

Morpion::Morpion(const sf::Font& police)
    : MiniJeu(police)
{
    demarrer();
}

void Morpion::demarrer()
{
    termine = false;
    for (int i = 0; i < 9; i++)
        grille[i] = Case::VIDE;
    message = "À toi de jouer (tu as les croix)";
}

void Morpion::gererClic(sf::Vector2f souris)
{
    if (termine)
        return;

    // On retrouve la case cliquée à partir de la position de la souris
    if (souris.x < X_GRILLE || souris.y < Y_GRILLE)
        return;
    int colonne = static_cast<int>((souris.x - X_GRILLE) / TAILLE_CASE);
    int ligne = static_cast<int>((souris.y - Y_GRILLE) / TAILLE_CASE);
    if (colonne > 2 || ligne > 2)
        return;

    int numero = ligne * 3 + colonne;
    if (grille[numero] != Case::VIDE)
        return;

    // Coup du joueur
    grille[numero] = Case::JOUEUR;
    if (aGagne(Case::JOUEUR))
    {
        message = "Victoire ! Tu as aligné trois croix.";
        terminer(Resultat::VICTOIRE);
        return;
    }
    if (grillePleine())
    {
        message = "Match nul !";
        terminer(Resultat::EGALITE);
        return;
    }

    // Réponse de la créature
    jouerCreature();
    if (aGagne(Case::CREATURE))
    {
        message = "Perdu ! Ta créature a aligné trois ronds.";
        terminer(Resultat::DEFAITE);
    }
    else if (grillePleine())
    {
        message = "Match nul !";
        terminer(Resultat::EGALITE);
    }
}

bool Morpion::aGagne(Case joueur) const
{
    for (int i = 0; i < 8; i++)
    {
        if (grille[ALIGNEMENTS[i][0]] == joueur
            && grille[ALIGNEMENTS[i][1]] == joueur
            && grille[ALIGNEMENTS[i][2]] == joueur)
            return true;
    }
    return false;
}

bool Morpion::grillePleine() const
{
    for (int i = 0; i < 9; i++)
    {
        if (grille[i] == Case::VIDE)
            return false;
    }
    return true;
}

// Essaie chaque case vide pour voir si elle ferait gagner ce joueur
int Morpion::chercherCoupGagnant(Case joueur)
{
    for (int i = 0; i < 9; i++)
    {
        if (grille[i] == Case::VIDE)
        {
            grille[i] = joueur;             // on pose le pion pour tester...
            bool gagne = aGagne(joueur);
            grille[i] = Case::VIDE;         // ...et on le retire
            if (gagne)
                return i;
        }
    }
    return -1;
}

// La créature joue simplement : elle gagne si elle peut, sinon elle bloque le joueur,
// sinon elle prend une case au hasard. On peut donc la battre en étant malin.
void Morpion::jouerCreature()
{
    int coup = chercherCoupGagnant(Case::CREATURE);
    if (coup == -1)
        coup = chercherCoupGagnant(Case::JOUEUR);

    if (coup == -1)
    {
        std::vector<int> casesVides;
        for (int i = 0; i < 9; i++)
        {
            if (grille[i] == Case::VIDE)
                casesVides.push_back(i);
        }
        coup = casesVides[std::rand() % casesVides.size()];
    }

    grille[coup] = Case::CREATURE;
}

void Morpion::dessiner(sf::RenderWindow& fenetre, sf::Vector2f souris) const
{
    sf::Color noir(40, 40, 40);
    dessinerTexteCentre(fenetre, police, "Morpion", 30, noir, 450.f, 115.f);

    for (int i = 0; i < 9; i++)
    {
        float x = X_GRILLE + (i % 3) * TAILLE_CASE;
        float y = Y_GRILLE + (i / 3) * TAILLE_CASE;
        float xCentre = x + TAILLE_CASE / 2.f;
        float yCentre = y + TAILLE_CASE / 2.f;

        // Une case vide s'éclaire quand la souris passe dessus
        sf::Color fond(245, 245, 245);
        bool survolee = sf::FloatRect({x, y}, {TAILLE_CASE, TAILLE_CASE}).contains(souris);
        if (survolee && grille[i] == Case::VIDE && !termine)
            fond = sf::Color(205, 225, 250);
        dessinerRectangle(fenetre, x, y, TAILLE_CASE, TAILLE_CASE, fond, noir);

        if (grille[i] == Case::JOUEUR)
        {
            // Une croix : deux barres inclinées dans des sens opposés
            sf::RectangleShape barre(sf::Vector2f(60.f, 8.f));
            barre.setOrigin({30.f, 4.f});
            barre.setPosition({xCentre, yCentre});
            barre.setFillColor(sf::Color(40, 110, 200));
            barre.setRotation(sf::degrees(45.f));
            fenetre.draw(barre);
            barre.setRotation(sf::degrees(-45.f));
            fenetre.draw(barre);
        }
        else if (grille[i] == Case::CREATURE)
        {
            // Un rond : un cercle vide avec un contour épais
            sf::CircleShape rond(22.f);
            rond.setOrigin({22.f, 22.f});
            rond.setPosition({xCentre, yCentre});
            rond.setFillColor(sf::Color::Transparent);
            rond.setOutlineThickness(8.f);
            rond.setOutlineColor(sf::Color(220, 80, 60));
            fenetre.draw(rond);
        }
    }

    dessinerTexteCentre(fenetre, police, message, 18, sf::Color(30, 90, 160), 450.f, 425.f);
}
