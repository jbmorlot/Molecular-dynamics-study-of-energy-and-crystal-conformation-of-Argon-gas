#ifndef TOOLBOX_H_INCLUDED
#define TOOLBOX_H_INCLUDED

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h> 
#include <cmath>

#include "defines.h"
#include "particule.h"
#include "vecteur.h"


double gauss ();											//Permet de définir une distribution gausienne
double mesure_temperature(particule *tab);								//Permet de mesurer la temperatre de la simulation
double mesure_pression(particule *tab, double T);							//Permet de mesurer la pression
void enregistrer_temperature_sim1(double *tab, char *chemin_rel,double temp_max);			//Permet d'enregitre la temperature dans un fichier POUR LA SIMULATION 1
void enregistrer_temperature_sim2(double *tab, char *chemin_rel, int num_sim, double temp_max);		//Permet d'enregitre la temperature dans un fichier POUR LA SIMULATION 2
void enregistrer_energie_sim2(particule *tab, char *chemin_rel, int num_simulation);
void enregistrer_energie_sim1(char *chemin_rel, double *tab1, double *tab2);
void Definition_Variable_Simulation();									//Permet de charger les variables globales depuis un fichier dans la simulation
void enregistrer_variables_simulation_sim1(char *chemin_rel, bool depassement_boite, double pression, double temperature_finale, int np);		//Permet d'enregistrer un resume des variables de la simulation
void enregistrer_variables_simulation_sim2(char *chemin_rel, bool depassement_boite, double pression, double temperature_finale, int np, int num_sim);

void counter_simulation(int i);		//Affichage d'une barre de progression dans le terminal


//Enregistrement des positions dans un fichier texte pour Jmol
class fichier_position
{
  public:
  std::ofstream f_pos;
  bool depassement_boite;		//Ce booleen passe à 1 lorque une particule s'est retrouve loin en dehors de la boite -> La simulation a diverge  
  fichier_position(char *chemin_rel, int num_simulation);
  ~fichier_position();
  void update(particule *tab,int i);
};


//Construction des histogrammes
class histogramme_position
{
    public: 
    int taille;
    double *tab_histo;
    double min;
    double max;
    double pas;
    
    histogramme_position();
    void update(particule *tab);
    void enregistrer(char *chemin_rel);
    ~histogramme_position();
};

class histogramme_vitesse
{
  public: 
  int taille;
  double *tab_histo;
  double min;
  double max;
  double pas;
  
  histogramme_vitesse(double temp_max);  
  void update(particule *tab);
  void enregistrer(char *chemin_rel);
  ~histogramme_vitesse();

};




#endif // TOOLBOX_H_INCLUDED
