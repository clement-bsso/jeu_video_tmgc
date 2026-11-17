#include "ModeDemo.hpp"
#include "Outils.hpp"
#include "Parametres.hpp"

// Numéro de chaque bouton, dans l'ordre où ils sont créés dans le constructeur
enum
{
    FAIM_MOINS, FAIM_PLUS,
    PROPRETE_MOINS, PROPRETE_PLUS,
    BONHEUR_MOINS, BONHEUR_PLUS,
    BESOINS_MAX, BESOINS_MIN,
    AGE_MOINS, AGE_PLUS,
    VITESSE_MOINS, VITESSE_PLUS,
    EVOLUER, REINITIALISER
};

// Position du panneau et de ses lignes
const float X_PANNEAU = 650.f;
const float Y_PANNEAU = 10.f;
const float Y_LIGNES = 50.f;
const float ECART_LIGNES = 38.f;
const float PAS_BESOIN = 20.f;      // points ajoutés ou retirés à chaque clic

ModeDemo::ModeDemo(const sf::Font& police)
    : police(police), actif(false)
{
    // Lignes 0 à 2 : les trois besoins, avec un bouton - et un bouton +
    for (int ligne = 0; ligne < 3; ligne++)
    {
        float y = Y_LIGNES + ligne * ECART_LIGNES;
        boutons.push_back(Bouton(police, "-20", 768.f, y, 54.f, 30.f, 13));
        boutons.push_back(Bouton(police, "+20", 828.f, y, 54.f, 30.f, 13));
    }

    // Ligne 3 : tous les besoins d'un coup
    float y = Y_LIGNES + 3 * ECART_LIGNES;
    boutons.push_back(Bouton(police, "Tout à 100", 660.f, y, 108.f, 30.f, 13));
    boutons.push_back(Bouton(police, "Tout à 10", 774.f, y, 108.f, 30.f, 13));

    // Ligne 4 : l'âge
    y = Y_LIGNES + 4 * ECART_LIGNES;
    boutons.push_back(Bouton(police, "-1 j", 768.f, y, 54.f, 30.f, 13));
    boutons.push_back(Bouton(police, "+1 j", 828.f, y, 54.f, 30.f, 13));

    // Ligne 5 : la vitesse du temps
    y = Y_LIGNES + 5 * ECART_LIGNES;
    boutons.push_back(Bouton(police, "÷2", 768.f, y, 54.f, 30.f, 13));
    boutons.push_back(Bouton(police, "×2", 828.f, y, 54.f, 30.f, 13));

    // Lignes 6 et 7 : évolution immédiate et remise à zéro
    y = Y_LIGNES + 6 * ECART_LIGNES;
    boutons.push_back(Bouton(police, "Évoluer maintenant", 660.f, y, 222.f, 30.f, 13));
    y = Y_LIGNES + 7 * ECART_LIGNES;
    boutons.push_back(Bouton(police, "Réinitialiser", 660.f, y, 222.f, 30.f, 13));
}

void ModeDemo::basculer(Horloge& horloge)
{
    if (actif)
        desactiver(horloge);
    else
        actif = true;
}

// En quittant le mode démo, le temps reprend sa vitesse normale
void ModeDemo::desactiver(Horloge& horloge)
{
    actif = false;
    horloge.setVitesse(1.f);
}

bool ModeDemo::estActif() const
{
    return actif;
}

bool ModeDemo::gererClic(sf::Vector2f souris, Creature& creature, Horloge& horloge)
{
    if (!actif)
        return false;

    for (int i = 0; i < static_cast<int>(boutons.size()); i++)
    {
        if (boutons[i].contient(souris))
        {
            appliquer(i, creature, horloge);
            return true;
        }
    }
    return false;
}

void ModeDemo::appliquer(int numeroBouton, Creature& creature, Horloge& horloge)
{
    switch (numeroBouton)
    {
        case FAIM_MOINS:     creature.getFaim().ajouter(-PAS_BESOIN);     break;
        case FAIM_PLUS:      creature.getFaim().ajouter(PAS_BESOIN);      break;
        case PROPRETE_MOINS: creature.getProprete().ajouter(-PAS_BESOIN); break;
        case PROPRETE_PLUS:  creature.getProprete().ajouter(PAS_BESOIN);  break;
        case BONHEUR_MOINS:  creature.getBonheur().ajouter(-PAS_BESOIN);  break;
        case BONHEUR_PLUS:   creature.getBonheur().ajouter(PAS_BESOIN);   break;

        case BESOINS_MAX:
            creature.getFaim().setValeur(100.f);
            creature.getProprete().setValeur(100.f);
            creature.getBonheur().setValeur(100.f);
            break;

        case BESOINS_MIN:
            creature.getFaim().setValeur(10.f);
            creature.getProprete().setValeur(10.f);
            creature.getBonheur().setValeur(10.f);
            break;

        case AGE_MOINS: creature.setAge(creature.getAge() - 1.f); break;
        case AGE_PLUS:  creature.setAge(creature.getAge() + 1.f); break;

        case VITESSE_MOINS: horloge.ralentir();  break;
        case VITESSE_PLUS:  horloge.accelerer(); break;

        case EVOLUER:
            // On amène la créature pile à l'âge de sa prochaine évolution.
            // C'est ensuite le jeu normal qui la fait évoluer, avec les règles habituelles.
            if (creature.getAgeProchaineEvolution() > 0.f)
                creature.setAge(creature.getAgeProchaineEvolution());
            break;

        case REINITIALISER:
            creature.reinitialiser();
            horloge.setVitesse(1.f);
            break;
    }
}

void ModeDemo::dessiner(sf::RenderWindow& fenetre, const Creature& creature,
                        const Horloge& horloge, sf::Vector2f souris) const
{
    if (!actif)
        return;

    sf::Color noir(40, 40, 40);
    dessinerRectangle(fenetre, X_PANNEAU, Y_PANNEAU, 240.f, 350.f,
                      sf::Color(255, 245, 225, 235), sf::Color(200, 60, 40));
    dessinerTexteCentre(fenetre, police, "MODE DÉMO", 17, sf::Color(200, 60, 40),
                        X_PANNEAU + 120.f, Y_PANNEAU + 20.f);

    // Le texte à gauche des boutons, avec la valeur actuelle
    float x = X_PANNEAU + 10.f;
    dessinerTexte(fenetre, police, "Faim " + enTexte(creature.getFaim().getValeur()),
                  13, noir, x, Y_LIGNES + 7.f);
    dessinerTexte(fenetre, police, "Propreté " + enTexte(creature.getProprete().getValeur()),
                  13, noir, x, Y_LIGNES + ECART_LIGNES + 7.f);
    dessinerTexte(fenetre, police, "Bonheur " + enTexte(creature.getBonheur().getValeur()),
                  13, noir, x, Y_LIGNES + 2 * ECART_LIGNES + 7.f);
    dessinerTexte(fenetre, police, "Âge " + enTexte(creature.getAge(), 1) + " j",
                  13, noir, x, Y_LIGNES + 4 * ECART_LIGNES + 7.f);
    dessinerTexte(fenetre, police, "Vitesse ×" + enTexte(horloge.getVitesse()),
                  13, noir, x, Y_LIGNES + 5 * ECART_LIGNES + 7.f);

    for (const Bouton& bouton : boutons)
        bouton.dessiner(fenetre, souris);
}
