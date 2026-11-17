#include "AffichageCreature.hpp"
#include "Outils.hpp"
#include "Parametres.hpp"
#include <cmath>
#include <algorithm>
#include <cstdint>

// Zone dans laquelle la créature se promène
const float X_MIN = 345.f;
const float X_MAX = 555.f;
const float VITESSE_MARCHE = 40.f;      // pixels par seconde

// Mélange deux couleurs (proportion = 0 : que la première, 1 : que la seconde)
static sf::Color melanger(sf::Color a, sf::Color b, float proportion)
{
    return sf::Color(static_cast<std::uint8_t>(a.r + (b.r - a.r) * proportion),
                     static_cast<std::uint8_t>(a.g + (b.g - a.g) * proportion),
                     static_cast<std::uint8_t>(a.b + (b.b - a.b) * proportion));
}

static sf::Color assombrir(sf::Color couleur)
{
    return melanger(couleur, sf::Color::Black, 0.25f);
}

AffichageCreature::AffichageCreature(const sf::Font& police)
    : police(police), x(450.f), direction(1), marche(false),
      temps(0.f), tempsReaction(0.f), tempsEvolution(0.f)
{
}

void AffichageCreature::replacerAuCentre()
{
    x = 450.f;
    direction = 1;
    tempsReaction = 0.f;
    tempsEvolution = 0.f;
}

void AffichageCreature::reagir(const std::string& texte)
{
    texteReaction = texte;
    tempsReaction = 2.f;
}

void AffichageCreature::lancerAnimationEvolution()
{
    tempsEvolution = 2.f;
}

// ---------------------------------------------------------------------------
// Animation
// ---------------------------------------------------------------------------

void AffichageCreature::mettreAJour(float dt, const Creature& creature)
{
    temps += dt;
    if (tempsReaction > 0.f)  tempsReaction -= dt;
    if (tempsEvolution > 0.f) tempsEvolution -= dt;

    // La créature se promène, sauf si elle va trop mal ou qu'elle est en train d'évoluer
    marche = creature.estVivante()
          && !creature.aEtat(Etat::MALADE)
          && !creature.aEtat(Etat::MORIBONDE)
          && tempsEvolution <= 0.f;

    if (marche)
    {
        x += direction * VITESSE_MARCHE * dt;
        if (x > X_MAX) { x = X_MAX; direction = -1; }   // demi-tour au bord
        if (x < X_MIN) { x = X_MIN; direction = 1; }
    }
}

// ---------------------------------------------------------------------------
// Dessin
// ---------------------------------------------------------------------------

void AffichageCreature::dessiner(sf::RenderWindow& fenetre, const Creature& creature) const
{
    float taille = choisirTaille(creature.getStade());

    // Elle sautille quand elle réagit à une action ou qu'elle est en pleine forme
    float saut = 0.f;
    if (tempsReaction > 0.f)
        saut = std::abs(std::sin(temps * 10.f)) * 25.f;
    else if (creature.aEtat(Etat::EN_PLEINE_FORME))
        saut = std::abs(std::sin(temps * 6.f)) * 10.f;

    // Ombre au sol (elle ne saute pas, elle)
    ellipse(fenetre, sf::Transform::Identity, x, HAUTEUR_SOL + 4.f,
            50.f * taille, 8.f * taille, sf::Color(0, 0, 0, 60));

    // Le repère place la créature, la grossit selon son stade
    // et la retourne quand elle marche vers la gauche.
    sf::Transform repere;
    repere.translate({x, HAUTEUR_SOL - saut});
    repere.scale({direction * taille, taille});

    sf::Color couleur = choisirCouleur(creature);
    dessinerDos(fenetre, repere, creature, couleur);
    dessinerCorps(fenetre, repere, creature, couleur);
    dessinerTete(fenetre, repere, creature, couleur);
    dessinerVisage(fenetre, repere, creature);

    dessinerBulle(fenetre, creature, saut + 150.f * taille);
}

float AffichageCreature::choisirTaille(Stade stade) const
{
    if (stade == Stade::BEBE) return 0.65f;
    if (stade == Stade::ADO)  return 0.90f;
    return 1.20f;
}

// La couleur dépend d'abord de la forme, puis elle est modifiée par la santé.
sf::Color AffichageCreature::choisirCouleur(const Creature& creature) const
{
    sf::Color gris(150, 150, 150);
    if (!creature.estVivante())
        return gris;

    // Pendant une évolution, elle clignote en blanc
    if (tempsEvolution > 0.f && static_cast<int>(tempsEvolution * 10.f) % 2 == 0)
        return sf::Color::White;

    sf::Color couleur(140, 210, 120);                           // bébé : vert clair
    if (creature.getStade() != Stade::BEBE)
    {
        if (creature.getForme() == Forme::FAVORABLE)        couleur = sf::Color(255, 200, 60);   // doré
        else if (creature.getForme() == Forme::DEFAVORABLE) couleur = sf::Color(125, 75, 155);   // violet
        else                                                couleur = sf::Color(90, 160, 230);   // bleu
    }

    if (creature.aEtat(Etat::MORIBONDE))
        couleur = melanger(couleur, gris, 0.7f);
    else if (creature.aEtat(Etat::MALADE))
        couleur = melanger(couleur, sf::Color(150, 170, 80), 0.5f);     // teint verdâtre

    return couleur;
}

// Ce qui est derrière le corps : ailes, queue, piques
void AffichageCreature::dessinerDos(sf::RenderWindow& fenetre, const sf::Transform& repere,
                                    const Creature& creature, sf::Color couleur) const
{
    bool evoluee = creature.getStade() != Stade::BEBE;

    // Queue des adultes, qui remue
    if (creature.getStade() == Stade::ADULTE)
    {
        float remue = std::sin(temps * 5.f) * 6.f;
        triangle(fenetre, repere, sf::Vector2f(-45.f, -45.f), sf::Vector2f(-92.f, -62.f + remue),
                 sf::Vector2f(-48.f, -20.f), assombrir(couleur));
    }

    // Forme favorable : des ailes blanches qui battent
    if (evoluee && creature.getForme() == Forme::FAVORABLE)
    {
        float battement = 14.f + std::sin(temps * 8.f) * 6.f;
        ellipse(fenetre, repere, -52.f, -78.f, 32.f, battement, sf::Color(250, 250, 255));
    }

    // Forme défavorable : des piques dans le dos
    if (evoluee && creature.getForme() == Forme::DEFAVORABLE)
    {
        sf::Color rouge(150, 30, 40);
        triangle(fenetre, repere, sf::Vector2f(-30.f, -98.f), sf::Vector2f(-62.f, -108.f), sf::Vector2f(-44.f, -80.f), rouge);
        triangle(fenetre, repere, sf::Vector2f(-48.f, -74.f), sf::Vector2f(-80.f, -70.f), sf::Vector2f(-54.f, -52.f), rouge);
        triangle(fenetre, repere, sf::Vector2f(-50.f, -44.f), sf::Vector2f(-74.f, -30.f), sf::Vector2f(-42.f, -24.f), rouge);
    }
}

void AffichageCreature::dessinerCorps(sf::RenderWindow& fenetre, const sf::Transform& repere,
                                      const Creature& creature, sf::Color couleur) const
{
    // Pieds : ils se lèvent chacun leur tour quand elle marche
    float pied1 = 0.f;
    float pied2 = 0.f;
    if (marche)
    {
        pied1 = std::max(0.f, std::sin(temps * 9.f)) * 7.f;
        pied2 = std::max(0.f, -std::sin(temps * 9.f)) * 7.f;
    }
    ellipse(fenetre, repere, -22.f, -7.f - pied1, 17.f, 8.f, assombrir(couleur));
    ellipse(fenetre, repere,  24.f, -7.f - pied2, 17.f, 8.f, assombrir(couleur));

    // Corps : il gonfle un peu, comme si elle respirait
    float respiration = std::sin(temps * 3.f) * 2.f;
    ellipse(fenetre, repere, 0.f, -56.f, 55.f, 50.f + respiration, couleur);

    // Taches de saleté
    if (creature.aEtat(Etat::SALE))
    {
        sf::Color marron(110, 80, 40);
        ellipse(fenetre, repere, -24.f, -38.f, 8.f, 6.f, marron);
        ellipse(fenetre, repere, -10.f, -86.f, 6.f, 5.f, marron);
        ellipse(fenetre, repere, -34.f, -64.f, 6.f, 7.f, marron);
        ellipse(fenetre, repere,   2.f, -22.f, 6.f, 4.f, marron);
    }
}

// Ce qui est sur la tête : oreilles ou cornes, auréole
void AffichageCreature::dessinerTete(sf::RenderWindow& fenetre, const sf::Transform& repere,
                                     const Creature& creature, sf::Color couleur) const
{
    if (creature.getStade() == Stade::BEBE)
        return;     // le bébé n'a rien sur la tête

    if (creature.getForme() == Forme::DEFAVORABLE)
    {
        // Cornes rouges
        sf::Color rouge(190, 40, 40);
        triangle(fenetre, repere, sf::Vector2f(-28.f, -95.f), sf::Vector2f(-34.f, -136.f), sf::Vector2f(-8.f, -103.f), rouge);
        triangle(fenetre, repere, sf::Vector2f(10.f, -104.f), sf::Vector2f(32.f, -138.f), sf::Vector2f(32.f, -96.f), rouge);
    }
    else
    {
        // Oreilles
        triangle(fenetre, repere, sf::Vector2f(-32.f, -92.f), sf::Vector2f(-22.f, -128.f), sf::Vector2f(-4.f, -103.f), assombrir(couleur));
        triangle(fenetre, repere, sf::Vector2f(8.f, -104.f), sf::Vector2f(24.f, -128.f), sf::Vector2f(34.f, -93.f), assombrir(couleur));
    }

    // Auréole de l'adulte favorable : une ellipse vide avec un contour jaune
    if (creature.getStade() == Stade::ADULTE && creature.getForme() == Forme::FAVORABLE)
    {
        sf::CircleShape aureole(24.f);
        aureole.setOrigin({24.f, 24.f});
        aureole.setScale({1.f, 0.3f});
        aureole.setPosition({4.f, -142.f});
        aureole.setFillColor(sf::Color::Transparent);
        aureole.setOutlineThickness(5.f);
        aureole.setOutlineColor(sf::Color(255, 235, 90));
        fenetre.draw(aureole, repere);
    }
}

// Les yeux et la bouche changent selon l'état de la créature
void AffichageCreature::dessinerVisage(sf::RenderWindow& fenetre, const sf::Transform& repere,
                                       const Creature& creature) const
{
    sf::Color noir(40, 40, 40);
    bool tresMal = !creature.estVivante() || creature.aEtat(Etat::MORIBONDE);
    bool cligne = std::fmod(temps, 3.f) < 0.15f;        // un clignement toutes les 3 secondes

    // --- Yeux ---
    if (tresMal)
    {
        // Des croix à la place des yeux
        trait(fenetre, repere, 10.f, -70.f, 18.f,  45.f, noir);
        trait(fenetre, repere, 10.f, -70.f, 18.f, -45.f, noir);
        trait(fenetre, repere, 34.f, -68.f, 16.f,  45.f, noir);
        trait(fenetre, repere, 34.f, -68.f, 16.f, -45.f, noir);
    }
    else if (cligne)
    {
        trait(fenetre, repere, 10.f, -70.f, 20.f, 0.f, noir);
        trait(fenetre, repere, 34.f, -68.f, 16.f, 0.f, noir);
    }
    else
    {
        // Pupilles rouges pour la forme défavorable
        sf::Color pupille = noir;
        if (creature.getStade() != Stade::BEBE && creature.getForme() == Forme::DEFAVORABLE)
            pupille = sf::Color(200, 30, 30);

        ellipse(fenetre, repere, 10.f, -70.f, 12.f, 12.f, sf::Color::White);
        ellipse(fenetre, repere, 34.f, -68.f, 10.f, 10.f, sf::Color::White);
        ellipse(fenetre, repere, 14.f, -69.f, 5.f, 5.f, pupille);
        ellipse(fenetre, repere, 37.f, -67.f, 5.f, 5.f, pupille);
    }

    // --- Bouche ---
    sf::Color bouche(110, 35, 45);
    if (tresMal)
        trait(fenetre, repere, 24.f, -40.f, 16.f, 0.f, bouche);                 // trait droit
    else if (tempsReaction > 0.f)
        demiDisque(fenetre, repere, 24.f, -46.f, 12.f, true, bouche);           // grand sourire
    else if (creature.aEtat(Etat::TRISTE) || creature.aEtat(Etat::MALADE))
        demiDisque(fenetre, repere, 24.f, -36.f, 10.f, false, bouche);          // bouche à l'envers
    else if (creature.aEtat(Etat::AFFAMEE))
        ellipse(fenetre, repere, 24.f, -40.f, 6.f, 8.f, bouche);                // bouche ouverte
    else if (creature.aEtat(Etat::HEUREUSE))
        demiDisque(fenetre, repere, 24.f, -46.f, 12.f, true, bouche);           // grand sourire
    else
        demiDisque(fenetre, repere, 24.f, -44.f, 7.f, true, bouche);            // petit sourire

    // Une larme qui coule quand elle est triste
    if (!tresMal && creature.aEtat(Etat::TRISTE))
    {
        float chute = std::fmod(temps * 14.f, 14.f);
        ellipse(fenetre, repere, 40.f, -54.f + chute, 3.f, 5.f, sf::Color(90, 170, 255));
    }
}

// Bulle de texte au-dessus de la créature
void AffichageCreature::dessinerBulle(sf::RenderWindow& fenetre, const Creature& creature, float hauteur) const
{
    std::string texte = choisirTexteBulle(creature);
    if (texte.empty())
        return;

    float largeur = largeurTexte(police, texte, 15) + 24.f;
    float y = HAUTEUR_SOL - hauteur - 30.f;
    dessinerRectangle(fenetre, x - largeur / 2.f, y - 15.f, largeur, 30.f,
                      sf::Color::White, sf::Color(60, 60, 60));
    dessinerTexteCentre(fenetre, police, texte, 15, sf::Color(40, 40, 40), x, y);
}

std::string AffichageCreature::choisirTexteBulle(const Creature& creature) const
{
    if (!creature.estVivante())
        return "";
    if (tempsReaction > 0.f)
        return texteReaction;

    // Sinon elle se plaint de son problème le plus grave
    switch (creature.getEtats().front())
    {
        case Etat::MORIBONDE: return "Je me sens très mal...";
        case Etat::MALADE:    return "Je suis malade...";
        case Etat::AFFAMEE:   return "J'ai faim !";
        case Etat::SALE:      return "Je suis toute sale !";
        case Etat::TRISTE:    return "Je m'ennuie...";
        default:              return "";
    }
}

// ---------------------------------------------------------------------------
// Formes de base
// ---------------------------------------------------------------------------

// Une ellipse est un cercle qu'on étire dans un sens
void AffichageCreature::ellipse(sf::RenderWindow& fenetre, const sf::Transform& repere,
                                float cx, float cy, float rayonX, float rayonY, sf::Color couleur) const
{
    sf::CircleShape forme(rayonX);
    forme.setOrigin({rayonX, rayonX});
    forme.setScale({1.f, rayonY / rayonX});
    forme.setPosition({cx, cy});
    forme.setFillColor(couleur);
    fenetre.draw(forme, repere);
}

void AffichageCreature::triangle(sf::RenderWindow& fenetre, const sf::Transform& repere,
                                 sf::Vector2f a, sf::Vector2f b, sf::Vector2f c, sf::Color couleur) const
{
    sf::ConvexShape forme(3);
    forme.setPoint(0, a);
    forme.setPoint(1, b);
    forme.setPoint(2, c);
    forme.setFillColor(couleur);
    fenetre.draw(forme, repere);
}

// Un trait épais centré sur (x, y), incliné de "angle" degrés
void AffichageCreature::trait(sf::RenderWindow& fenetre, const sf::Transform& repere,
                              float cx, float cy, float longueur, float angle, sf::Color couleur) const
{
    sf::RectangleShape forme(sf::Vector2f(longueur, 4.f));
    forme.setOrigin({longueur / 2.f, 2.f});
    forme.setPosition({cx, cy});
    forme.setRotation(sf::degrees(angle));
    forme.setFillColor(couleur);
    fenetre.draw(forme, repere);
}

// Un demi-disque, utilisé pour la bouche : arrondi vers le bas pour un sourire,
// vers le haut pour une bouche triste.
void AffichageCreature::demiDisque(sf::RenderWindow& fenetre, const sf::Transform& repere,
                                   float cx, float cy, float rayon, bool versLeBas, sf::Color couleur) const
{
    const int NB_POINTS = 12;
    float sens = versLeBas ? 1.f : -1.f;

    sf::ConvexShape forme(NB_POINTS);
    for (int i = 0; i < NB_POINTS; i++)
    {
        float angle = PI * i / (NB_POINTS - 1);     // de 0 à 180 degrés
        forme.setPoint(i, sf::Vector2f(rayon * std::cos(angle), sens * rayon * std::sin(angle)));
    }
    forme.setPosition({cx, cy});
    forme.setFillColor(couleur);
    fenetre.draw(forme, repere);
}
