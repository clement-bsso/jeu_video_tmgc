#include "Jeu.hpp"
#include "Outils.hpp"
#include "Parametres.hpp"
#include <cmath>
#include <optional>

const sf::Color NOIR(40, 40, 40);
const std::size_t LONGUEUR_MAX_NOM = 12;

Jeu::Jeu(const sf::Font& police)
    : fenetre(sf::VideoMode({LARGEUR_FENETRE, HAUTEUR_FENETRE}), "Cesichi",
              sf::Style::Titlebar | sf::Style::Close),
      police(police),
      creature("Cesichi"),
      affichageCreature(police),
      interfaceJeu(police),
      modeDemo(police),
      chifoumi(police),
      morpion(police),
      miniJeuEnCours(nullptr),
      ecran(Ecran::TITRE),
      boutonCommencer(police, "Commencer", 375.f, 395.f, 150.f, 45.f),
      boutonChifoumi(police, "Chifoumi", 290.f, 290.f, 150.f, 50.f),
      boutonMorpion(police, "Morpion", 460.f, 290.f, 150.f, 50.f),
      boutonAnnuler(police, "Annuler", 375.f, 365.f, 150.f, 40.f),
      boutonRetour(police, "Retour", 375.f, 455.f, 150.f, 45.f),
      boutonRecommencer(police, "Nouvelle partie", 365.f, 225.f, 170.f, 45.f)
{
    fenetre.setFramerateLimit(60);
}

// La boucle principale du jeu
void Jeu::lancer()
{
    while (fenetre.isOpen())
    {
        gererEvenements();
        mettreAJour();
        dessiner();
    }
}

// ---------------------------------------------------------------------------
// Événements clavier et souris
// ---------------------------------------------------------------------------

void Jeu::gererEvenements()
{
    // Avec SFML 3, pollEvent renvoie l'événement suivant (ou rien s'il n'y en a plus),
    // et getIf permet de récupérer ses détails quand il est du type demandé.
    while (const std::optional evenement = fenetre.pollEvent())
    {
        if (evenement->is<sf::Event::Closed>())
        {
            fenetre.close();
        }
        else if (const auto* touche = evenement->getIf<sf::Event::KeyPressed>())
        {
            gererTouche(touche->code);
        }
        else if (const auto* saisie = evenement->getIf<sf::Event::TextEntered>())
        {
            if (ecran == Ecran::TITRE)
                gererSaisieNom(saisie->unicode);
        }
        else if (const auto* clic = evenement->getIf<sf::Event::MouseButtonPressed>())
        {
            if (clic->button == sf::Mouse::Button::Left)
                gererClic(fenetre.mapPixelToCoords(clic->position));
        }
    }
}

void Jeu::gererTouche(sf::Keyboard::Key touche)
{
    if (ecran == Ecran::TITRE && touche == sf::Keyboard::Key::Enter)
        nouvellePartie();

    // F1 (ou Tab) active ou désactive le mode démo, seulement pendant la partie
    bool toucheDemo = (touche == sf::Keyboard::Key::F1 || touche == sf::Keyboard::Key::Tab);
    if (ecran == Ecran::PRINCIPAL && toucheDemo)
        modeDemo.basculer(horloge);
}

// Saisie du nom de la créature sur l'écran titre
void Jeu::gererSaisieNom(char32_t caractere)
{
    const char32_t TOUCHE_EFFACER = 8;

    if (caractere == TOUCHE_EFFACER)
    {
        if (!nomSaisi.empty())
            nomSaisi.pop_back();
    }
    else if (caractere >= 32 && caractere < 127 && nomSaisi.size() < LONGUEUR_MAX_NOM)
    {
        // On garde seulement les caractères simples (lettres sans accent, chiffres, espace)
        nomSaisi += static_cast<char>(caractere);
    }
}

void Jeu::gererClic(sf::Vector2f souris)
{
    switch (ecran)
    {
        case Ecran::TITRE:
            if (boutonCommencer.contient(souris))
                nouvellePartie();
            break;

        case Ecran::PRINCIPAL:
            gererClicPrincipal(souris);
            break;

        case Ecran::CHOIX_MINI_JEU:
            if (boutonChifoumi.contient(souris))     lancerMiniJeu(&chifoumi);
            else if (boutonMorpion.contient(souris)) lancerMiniJeu(&morpion);
            else if (boutonAnnuler.contient(souris)) ecran = Ecran::PRINCIPAL;
            break;

        case Ecran::MINI_JEU:
            if (miniJeuEnCours->estTermine())
            {
                if (boutonRetour.contient(souris))
                    terminerMiniJeu();
            }
            else
            {
                miniJeuEnCours->gererClic(souris);
            }
            break;

        case Ecran::FIN:
            if (boutonRecommencer.contient(souris))
                ecran = Ecran::TITRE;
            break;
    }
}

void Jeu::gererClicPrincipal(sf::Vector2f souris)
{
    // Le panneau du mode démo passe en premier s'il est ouvert
    if (modeDemo.gererClic(souris, creature, horloge))
        return;

    Action action = interfaceJeu.gererClic(souris);
    if (action == Action::NOURRIR)
    {
        creature.nourrir();
        affichageCreature.reagir("Miam !");
    }
    else if (action == Action::NETTOYER)
    {
        creature.nettoyer();
        affichageCreature.reagir("Toute propre !");
    }
    else if (action == Action::JOUER)
    {
        ecran = Ecran::CHOIX_MINI_JEU;
    }
}

// ---------------------------------------------------------------------------
// Changements d'écran
// ---------------------------------------------------------------------------

void Jeu::nouvellePartie()
{
    if (nomSaisi.empty())
        nomSaisi = "Cesichi";

    creature = Creature(nomSaisi);
    affichageCreature.replacerAuCentre();
    interfaceJeu.effacerMessage();
    modeDemo.desactiver(horloge);
    ecran = Ecran::PRINCIPAL;
}

void Jeu::lancerMiniJeu(MiniJeu* miniJeu)
{
    miniJeuEnCours = miniJeu;
    miniJeuEnCours->demarrer();
    ecran = Ecran::MINI_JEU;
}

// Retour au jeu principal : le résultat du mini-jeu fait monter le bonheur
void Jeu::terminerMiniJeu()
{
    Resultat resultat = miniJeuEnCours->getResultat();

    if (resultat == Resultat::VICTOIRE)
    {
        creature.jouer(GAIN_VICTOIRE);
        affichageCreature.reagir("Bravo, bien joué !");
    }
    else if (resultat == Resultat::EGALITE)
    {
        creature.jouer(GAIN_EGALITE);
        affichageCreature.reagir("Match nul, c'était sympa !");
    }
    else
    {
        creature.jouer(GAIN_DEFAITE);
        affichageCreature.reagir("J'ai gagné, hi hi !");
    }

    ecran = Ecran::PRINCIPAL;
}

// ---------------------------------------------------------------------------
// Mise à jour
// ---------------------------------------------------------------------------

void Jeu::mettreAJour()
{
    horloge.nouvelleImage();
    float dtReel = horloge.getDtReel();

    // Les animations tournent toujours à vitesse normale
    decor.mettreAJour(dtReel);
    interfaceJeu.mettreAJour(dtReel);
    affichageCreature.mettreAJour(dtReel, creature);

    // Le temps de la créature n'avance que sur l'écran principal :
    // il est en pause pendant les mini-jeux et sur les autres écrans.
    if (ecran != Ecran::PRINCIPAL)
        return;

    bool aEvolue = creature.mettreAJour(horloge.getDtJeu());
    if (aEvolue)
    {
        affichageCreature.lancerAnimationEvolution();
        interfaceJeu.afficherMessage("Évolution " + texteForme(creature.getForme()) + " : "
                                     + creature.getNom() + " devient "
                                     + texteStade(creature.getStade()) + " !");
    }

    if (!creature.estVivante())
        ecran = Ecran::FIN;
}

// ---------------------------------------------------------------------------
// Dessin
// ---------------------------------------------------------------------------

void Jeu::dessiner()
{
    sf::Vector2f souris = fenetre.mapPixelToCoords(sf::Mouse::getPosition(fenetre));

    // L'heure de la journée est la partie décimale de l'âge (âge 3,25 -> un quart de la journée)
    float heure = creature.getAge() - std::floor(creature.getAge());

    fenetre.clear();
    decor.dessiner(fenetre, heure);

    if (ecran == Ecran::TITRE)
    {
        dessinerEcranTitre(souris);
    }
    else
    {
        // La scène reste visible derrière les autres écrans
        affichageCreature.dessiner(fenetre, creature);
        interfaceJeu.dessiner(fenetre, creature, souris);
        modeDemo.dessiner(fenetre, creature, horloge, souris);

        if (ecran == Ecran::CHOIX_MINI_JEU) dessinerChoixMiniJeu(souris);
        else if (ecran == Ecran::MINI_JEU)  dessinerMiniJeu(souris);
        else if (ecran == Ecran::FIN)       dessinerEcranFin(souris);
    }

    fenetre.display();
}

// Assombrit la scène et dessine un panneau clair par-dessus
void Jeu::dessinerPanneau(float x, float y, float largeur, float hauteur)
{
    dessinerRectangle(fenetre, 0.f, 0.f, LARGEUR_FENETRE, HAUTEUR_FENETRE, sf::Color(0, 0, 0, 120));
    dessinerRectangle(fenetre, x, y, largeur, hauteur, sf::Color(250, 250, 245), sf::Color(60, 60, 60));
}

void Jeu::dessinerEcranTitre(sf::Vector2f souris)
{
    dessinerPanneau(225.f, 130.f, 450.f, 340.f);
    dessinerTexteCentre(fenetre, police, "CESICHI", 48, sf::Color(60, 110, 170), 450.f, 190.f);
    dessinerTexteCentre(fenetre, police, "Comment s'appelle ta créature ?", 18, NOIR, 450.f, 260.f);

    // Zone de saisie, avec un curseur qui clignote au bout du nom
    bool curseurVisible = std::fmod(horloge.getTempsTotal(), 1.f) < 0.5f;
    std::string texte = nomSaisi;
    if (curseurVisible)
        texte += "_";
    dessinerRectangle(fenetre, 330.f, 290.f, 240.f, 44.f, sf::Color::White, sf::Color(60, 110, 170));
    dessinerTexte(fenetre, police, texte, 22, NOIR, 342.f, 298.f);

    dessinerTexteCentre(fenetre, police, "Tape un nom puis appuie sur Entrée", 14,
                        sf::Color(110, 110, 110), 450.f, 362.f);
    boutonCommencer.dessiner(fenetre, souris);
}

void Jeu::dessinerChoixMiniJeu(sf::Vector2f souris)
{
    dessinerPanneau(250.f, 190.f, 400.f, 240.f);
    dessinerTexteCentre(fenetre, police, "À quoi veux-tu jouer ?", 22, NOIR, 450.f, 240.f);
    boutonChifoumi.dessiner(fenetre, souris);
    boutonMorpion.dessiner(fenetre, souris);
    boutonAnnuler.dessiner(fenetre, souris);
}

void Jeu::dessinerMiniJeu(sf::Vector2f souris)
{
    dessinerPanneau(200.f, 80.f, 500.f, 440.f);
    miniJeuEnCours->dessiner(fenetre, souris);

    if (miniJeuEnCours->estTermine())
        boutonRetour.dessiner(fenetre, souris);
}

void Jeu::dessinerEcranFin(sf::Vector2f souris)
{
    std::string cause = "de vieillesse";
    if (creature.getCauseMort() == CauseMort::NEGLIGENCE)
        cause = "de négligence";

    // Le panneau est placé en haut pour qu'on voie encore la créature en dessous
    dessinerPanneau(230.f, 90.f, 440.f, 200.f);
    dessinerTexteCentre(fenetre, police, creature.getNom() + " est morte " + cause, 22, NOIR, 450.f, 135.f);
    dessinerTexteCentre(fenetre, police,
                        "Elle a vécu " + enTexte(creature.getAge(), 1) + " jours (stade "
                        + texteStade(creature.getStade()) + ")",
                        16, NOIR, 450.f, 180.f);
    boutonRecommencer.dessiner(fenetre, souris);
}
