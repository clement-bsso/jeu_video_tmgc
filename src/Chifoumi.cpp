#include "Chifoumi.hpp"
#include "Outils.hpp"
#include <cstdlib>

const std::string NOMS_CHOIX[3] = { "Pierre", "Feuille", "Ciseaux" };
const int POINTS_POUR_GAGNER = 2;

Chifoumi::Chifoumi(const sf::Font& police)
    : MiniJeu(police)
{
    boutons.push_back(Bouton(police, NOMS_CHOIX[0], 235.f, 360.f, 130.f, 50.f));
    boutons.push_back(Bouton(police, NOMS_CHOIX[1], 385.f, 360.f, 130.f, 50.f));
    boutons.push_back(Bouton(police, NOMS_CHOIX[2], 535.f, 360.f, 130.f, 50.f));
    demarrer();
}

void Chifoumi::demarrer()
{
    termine = false;
    scoreJoueur = 0;
    scoreCreature = 0;
    choixJoueur = -1;
    choixCreature = -1;
    message = "Choisis : pierre, feuille ou ciseaux ?";
}

void Chifoumi::gererClic(sf::Vector2f souris)
{
    if (termine)
        return;

    for (int i = 0; i < 3; i++)
    {
        if (boutons[i].contient(souris))
            jouerManche(i);
    }
}

void Chifoumi::jouerManche(int choix)
{
    choixJoueur = choix;
    choixCreature = std::rand() % 3;    // la créature joue au hasard

    // Dans l'ordre pierre(0), feuille(1), ciseaux(2), chaque choix bat celui d'avant
    // (la feuille bat la pierre, les ciseaux battent la feuille, la pierre bat les ciseaux).
    // La différence modulo 3 donne donc directement le gagnant.
    int difference = (choixJoueur - choixCreature + 3) % 3;

    if (difference == 0)
    {
        message = "Égalité, on rejoue la manche !";
    }
    else if (difference == 1)
    {
        scoreJoueur++;
        message = "Tu gagnes la manche !";
    }
    else
    {
        scoreCreature++;
        message = "Ta créature gagne la manche !";
    }

    if (scoreJoueur == POINTS_POUR_GAGNER)
    {
        message = "Victoire ! Tu as gagné la partie.";
        terminer(Resultat::VICTOIRE);
    }
    else if (scoreCreature == POINTS_POUR_GAGNER)
    {
        message = "Perdu ! Ta créature a gagné la partie.";
        terminer(Resultat::DEFAITE);
    }
}

void Chifoumi::dessiner(sf::RenderWindow& fenetre, sf::Vector2f souris) const
{
    sf::Color noir(40, 40, 40);

    dessinerTexteCentre(fenetre, police, "Chifoumi", 30, noir, 450.f, 115.f);
    dessinerTexteCentre(fenetre, police, "Le premier à 2 points gagne", 15, noir, 450.f, 150.f);

    dessinerTexteCentre(fenetre, police,
                        "Toi  " + enTexte(scoreJoueur) + "  -  " + enTexte(scoreCreature) + "  Créature",
                        24, noir, 450.f, 205.f);

    // Les deux choix de la dernière manche
    if (choixJoueur != -1)
    {
        dessinerTexteCentre(fenetre, police, "Toi : " + NOMS_CHOIX[choixJoueur], 18, noir, 330.f, 262.f);
        dessinerTexteCentre(fenetre, police, "Créature : " + NOMS_CHOIX[choixCreature], 18, noir, 570.f, 262.f);
    }

    dessinerTexteCentre(fenetre, police, message, 18, sf::Color(30, 90, 160), 450.f, 312.f);

    // Une fois la partie finie, on ne peut plus choisir
    if (!termine)
    {
        for (const Bouton& bouton : boutons)
            bouton.dessiner(fenetre, souris);
    }
}
