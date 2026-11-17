#include "Interface.hpp"
#include "Outils.hpp"
#include "Parametres.hpp"

const sf::Color COULEUR_PANNEAU(255, 255, 255, 235);
const sf::Color COULEUR_TEXTE(40, 40, 40);

Interface::Interface(const sf::Font& police)
    : police(police),
      boutonNourrir(police, "Nourrir", 205.f, 535.f, 150.f, 45.f),
      boutonNettoyer(police, "Nettoyer", 375.f, 535.f, 150.f, 45.f),
      boutonJouer(police, "Jouer", 545.f, 535.f, 150.f, 45.f),
      tempsMessage(0.f)
{
}

Action Interface::gererClic(sf::Vector2f souris) const
{
    if (boutonNourrir.contient(souris))  return Action::NOURRIR;
    if (boutonNettoyer.contient(souris)) return Action::NETTOYER;
    if (boutonJouer.contient(souris))    return Action::JOUER;
    return Action::AUCUNE;
}

void Interface::afficherMessage(const std::string& texte)
{
    message = texte;
    tempsMessage = 5.f;
}

void Interface::effacerMessage()
{
    tempsMessage = 0.f;
}

void Interface::mettreAJour(float dt)
{
    if (tempsMessage > 0.f)
        tempsMessage -= dt;
}

void Interface::dessiner(sf::RenderWindow& fenetre, const Creature& creature, sf::Vector2f souris) const
{
    dessinerRectangle(fenetre, 10.f, 10.f, 250.f, 300.f, COULEUR_PANNEAU, sf::Color(90, 90, 90));
    dessinerInfos(fenetre, creature);

    dessinerJauge(fenetre, creature.getFaim(), 118.f);
    dessinerJauge(fenetre, creature.getProprete(), 146.f);
    dessinerJauge(fenetre, creature.getBonheur(), 174.f);

    dessinerEtats(fenetre, creature);

    boutonNourrir.dessiner(fenetre, souris);
    boutonNettoyer.dessiner(fenetre, souris);
    boutonJouer.dessiner(fenetre, souris);

    dessinerTexte(fenetre, police, "F1 : mode démo", 13, sf::Color(40, 70, 40), 780.f, 575.f);

    if (tempsMessage > 0.f)
        dessinerMessage(fenetre);
}

// Nom, âge, stade et temps restant avant la prochaine évolution
void Interface::dessinerInfos(sf::RenderWindow& fenetre, const Creature& creature) const
{
    dessinerTexte(fenetre, police, creature.getNom(), 22, COULEUR_TEXTE, 22.f, 16.f);
    dessinerTexte(fenetre, police, "Âge : " + enTexte(creature.getAge(), 1) + " jours",
                  14, COULEUR_TEXTE, 22.f, 50.f);

    std::string stade = "Stade : " + texteStade(creature.getStade());
    if (creature.getStade() != Stade::BEBE)
        stade += " (" + texteForme(creature.getForme()) + ")";
    dessinerTexte(fenetre, police, stade, 14, COULEUR_TEXTE, 22.f, 70.f);

    float prochaine = creature.getAgeProchaineEvolution();
    std::string evolution = "Forme finale atteinte";
    if (prochaine > 0.f)
        evolution = "Évolution dans " + enTexte(prochaine - creature.getAge(), 1) + " jours";
    dessinerTexte(fenetre, police, evolution, 14, COULEUR_TEXTE, 22.f, 90.f);
}

// Une jauge : le nom du besoin, une barre colorée et la valeur
void Interface::dessinerJauge(sf::RenderWindow& fenetre, const Besoin& besoin, float y) const
{
    const float X_BARRE = 105.f;
    const float LARGEUR_BARRE = 110.f;
    float valeur = besoin.getValeur();

    // Vert quand tout va bien, orange quand ça baisse, rouge quand c'est presque vide
    sf::Color couleur(70, 190, 90);
    if (valeur <= SEUIL_BAS)     couleur = sf::Color(220, 60, 60);
    else if (valeur < 50.f)      couleur = sf::Color(240, 160, 40);

    dessinerTexte(fenetre, police, besoin.getNom(), 14, COULEUR_TEXTE, 22.f, y);
    dessinerRectangle(fenetre, X_BARRE, y + 2.f, LARGEUR_BARRE, 14.f,
                      sf::Color(225, 225, 225), sf::Color(90, 90, 90));
    dessinerRectangle(fenetre, X_BARRE, y + 2.f, LARGEUR_BARRE * valeur / 100.f, 14.f, couleur);
    dessinerTexte(fenetre, police, enTexte(valeur), 14, COULEUR_TEXTE, X_BARRE + LARGEUR_BARRE + 10.f, y);
}

// La liste des états, avec une pastille verte (bon), rouge (mauvais) ou grise (normal)
void Interface::dessinerEtats(sf::RenderWindow& fenetre, const Creature& creature) const
{
    dessinerTexte(fenetre, police, "États :", 14, COULEUR_TEXTE, 22.f, 206.f);

    float y = 230.f;
    for (Etat etat : creature.getEtats())
    {
        sf::Color couleur(70, 190, 90);
        if (estEtatNegatif(etat))       couleur = sf::Color(220, 60, 60);
        else if (etat == Etat::NORMALE) couleur = sf::Color(150, 150, 150);

        sf::CircleShape pastille(6.f);
        pastille.setPosition({26.f, y + 3.f});
        pastille.setFillColor(couleur);
        fenetre.draw(pastille);

        dessinerTexte(fenetre, police, texteEtat(etat), 14, COULEUR_TEXTE, 46.f, y);
        y += 19.f;
    }
}

void Interface::dessinerMessage(sf::RenderWindow& fenetre) const
{
    float largeur = largeurTexte(police, message, 15) + 30.f;
    float xCentre = 455.f;
    dessinerRectangle(fenetre, xCentre - largeur / 2.f, 14.f, largeur, 34.f,
                      sf::Color(255, 240, 150), sf::Color(150, 120, 30));
    dessinerTexteCentre(fenetre, police, message, 15, COULEUR_TEXTE, xCentre, 31.f);
}
