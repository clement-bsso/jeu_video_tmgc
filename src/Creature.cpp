#include "Creature.hpp"
#include "Parametres.hpp"

Creature::Creature(const std::string& nom)
    : nom(nom),
      faim("Faim", VITESSE_FAIM),
      proprete("Propreté", VITESSE_PROPRETE),
      bonheur("Bonheur", VITESSE_BONHEUR)
{
    reinitialiser();
}

void Creature::reinitialiser()
{
    faim.setValeur(VALEUR_DEPART);
    proprete.setValeur(VALEUR_DEPART);
    bonheur.setValeur(VALEUR_DEPART);
    age = 0.f;
    stade = Stade::BEBE;
    forme = Forme::NEUTRE;
    vivante = true;
    causeMort = CauseMort::AUCUNE;
    tempsMoribonde = 0.f;
}

// ---------------------------------------------------------------------------
// Le temps qui passe
// ---------------------------------------------------------------------------

bool Creature::mettreAJour(float dt)
{
    if (!vivante)
        return false;

    faim.mettreAJour(dt);
    proprete.mettreAJour(dt);
    bonheur.mettreAJour(dt);
    age += dt / DUREE_JOUR;

    verifierMort(dt);
    if (!vivante)
        return false;

    return verifierEvolution();
}

// Quand la créature atteint l'âge prévu, elle passe au stade suivant.
// Sa forme est décidée à ce moment précis, d'après ses besoins.
bool Creature::verifierEvolution()
{
    if (stade == Stade::BEBE && age >= AGE_EVOLUTION_1)
    {
        forme = calculerForme();
        stade = Stade::ADO;
        return true;
    }
    if (stade == Stade::ADO && age >= AGE_EVOLUTION_2)
    {
        forme = calculerForme();
        stade = Stade::ADULTE;
        return true;
    }
    return false;
}

Forme Creature::calculerForme() const
{
    float f = faim.getValeur();
    float p = proprete.getValeur();
    float b = bonheur.getValeur();

    // Bien nourrie, propre et heureuse : forme favorable
    if (f >= SEUIL_FAVORABLE && p >= SEUIL_FAVORABLE && b >= SEUIL_FAVORABLE)
        return Forme::FAVORABLE;

    // Au moins un besoin vraiment négligé : forme défavorable
    if (f < SEUIL_DEFAVORABLE || p < SEUIL_DEFAVORABLE || b < SEUIL_DEFAVORABLE)
        return Forme::DEFAVORABLE;

    return Forme::NEUTRE;
}

void Creature::verifierMort(float dt)
{
    if (age >= AGE_MORT)
    {
        vivante = false;
        causeMort = CauseMort::VIEILLESSE;
        return;
    }

    // Si elle reste moribonde trop longtemps, elle meurt de négligence
    if (aEtat(Etat::MORIBONDE))
        tempsMoribonde += dt;
    else
        tempsMoribonde = 0.f;

    if (tempsMoribonde >= DELAI_MORT_NEGLIGENCE)
    {
        vivante = false;
        causeMort = CauseMort::NEGLIGENCE;
    }
}

// ---------------------------------------------------------------------------
// Actions du joueur
// ---------------------------------------------------------------------------

void Creature::nourrir()
{
    if (vivante)
        faim.ajouter(GAIN_NOURRIR);
}

void Creature::nettoyer()
{
    if (vivante)
        proprete.ajouter(GAIN_NETTOYER);
}

void Creature::jouer(float gainBonheur)
{
    if (vivante)
        bonheur.ajouter(gainBonheur);
}

// ---------------------------------------------------------------------------
// États
// ---------------------------------------------------------------------------

int Creature::compterBesoinsVides() const
{
    int nombre = 0;
    if (faim.estPresqueVide())     nombre++;
    if (proprete.estPresqueVide()) nombre++;
    if (bonheur.estPresqueVide())  nombre++;
    return nombre;
}

int Creature::compterBesoinsPleins() const
{
    int nombre = 0;
    if (faim.estPresquePlein())     nombre++;
    if (proprete.estPresquePlein()) nombre++;
    if (bonheur.estPresquePlein())  nombre++;
    return nombre;
}

// Renvoie la liste des états actuels, les plus graves en premier.
// Le premier de la liste sert d'état "principal" pour l'affichage.
std::vector<Etat> Creature::getEtats() const
{
    std::vector<Etat> etats;

    int vides = compterBesoinsVides();
    if (vides == 3)      etats.push_back(Etat::MORIBONDE);
    else if (vides == 2) etats.push_back(Etat::MALADE);

    if (faim.estPresqueVide())     etats.push_back(Etat::AFFAMEE);
    if (proprete.estPresqueVide()) etats.push_back(Etat::SALE);
    if (bonheur.estPresqueVide())  etats.push_back(Etat::TRISTE);

    if (compterBesoinsPleins() == 3) etats.push_back(Etat::EN_PLEINE_FORME);
    if (bonheur.estPresquePlein())   etats.push_back(Etat::HEUREUSE);
    if (faim.estPresquePlein())      etats.push_back(Etat::REPUE);
    if (proprete.estPresquePlein())  etats.push_back(Etat::PROPRE);

    if (etats.empty())
        etats.push_back(Etat::NORMALE);

    return etats;
}

bool Creature::aEtat(Etat etat) const
{
    for (Etat e : getEtats())
    {
        if (e == etat)
            return true;
    }
    return false;
}

// ---------------------------------------------------------------------------
// Accesseurs
// ---------------------------------------------------------------------------

const std::string& Creature::getNom() const { return nom; }

Besoin& Creature::getFaim()     { return faim; }
Besoin& Creature::getProprete() { return proprete; }
Besoin& Creature::getBonheur()  { return bonheur; }

const Besoin& Creature::getFaim() const     { return faim; }
const Besoin& Creature::getProprete() const { return proprete; }
const Besoin& Creature::getBonheur() const  { return bonheur; }

float Creature::getAge() const { return age; }

void Creature::setAge(float jours)
{
    age = jours;
    if (age < 0.f)
        age = 0.f;
}

float Creature::getAgeProchaineEvolution() const
{
    if (stade == Stade::BEBE) return AGE_EVOLUTION_1;
    if (stade == Stade::ADO)  return AGE_EVOLUTION_2;
    return -1.f;    // un adulte n'évolue plus
}

Stade Creature::getStade() const { return stade; }
Forme Creature::getForme() const { return forme; }
bool Creature::estVivante() const { return vivante; }
CauseMort Creature::getCauseMort() const { return causeMort; }

// ---------------------------------------------------------------------------
// Textes
// ---------------------------------------------------------------------------

std::string texteEtat(Etat etat)
{
    switch (etat)
    {
        case Etat::MORIBONDE:       return "Moribonde";
        case Etat::MALADE:          return "Malade";
        case Etat::AFFAMEE:         return "Affamée";
        case Etat::SALE:            return "Sale";
        case Etat::TRISTE:          return "Triste";
        case Etat::EN_PLEINE_FORME: return "En pleine forme";
        case Etat::HEUREUSE:        return "Heureuse";
        case Etat::REPUE:           return "Repue";
        case Etat::PROPRE:          return "Propre";
        default:                    return "Normale";
    }
}

std::string texteStade(Stade stade)
{
    switch (stade)
    {
        case Stade::BEBE: return "bébé";
        case Stade::ADO:  return "ado";
        default:          return "adulte";
    }
}

std::string texteForme(Forme forme)
{
    switch (forme)
    {
        case Forme::FAVORABLE:   return "favorable";
        case Forme::DEFAVORABLE: return "défavorable";
        default:                 return "neutre";
    }
}

bool estEtatNegatif(Etat etat)
{
    return etat == Etat::MORIBONDE || etat == Etat::MALADE || etat == Etat::AFFAMEE
        || etat == Etat::SALE || etat == Etat::TRISTE;
}
