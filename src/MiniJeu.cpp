#include "MiniJeu.hpp"

MiniJeu::MiniJeu(const sf::Font& police)
    : police(police), termine(false), resultat(Resultat::EGALITE)
{
}

MiniJeu::~MiniJeu()
{
}

bool MiniJeu::estTermine() const
{
    return termine;
}

Resultat MiniJeu::getResultat() const
{
    return resultat;
}

void MiniJeu::terminer(Resultat resultatFinal)
{
    termine = true;
    resultat = resultatFinal;
}
