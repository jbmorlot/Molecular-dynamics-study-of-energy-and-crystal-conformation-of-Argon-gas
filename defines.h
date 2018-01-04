#ifndef DEFINES_H_INCLUDED
#define DEFINES_H_INCLUDED

//---------------VARIABLES GLOBALES---------------------//
//Systeme d'unité: ATOMIQUE

  extern double RADIUS;			//Rayon des particules (-> pour l'affichage en OpenGL) -> N'est pas utilise finalement
  extern double MASS;			//Masse des particules
  extern int NBRE_PART_AXE;		//Nombre de particules par axes
  extern int NBRE_PART;			//Nombre de particules totales (= NBRE_PART_AXE³)
  extern double PAS_INIT;		//Pas de la simulation
  extern double NRJ0;			//Energie du puit de potentiel
  extern double BOX_SIZE;		//Taille de la boite/2
  extern int NBRE_ITERATION;		//Nombre d'itérations
  extern double DIST_INTERACTION;	//Distance d'intéraction

  extern double K_BOLTZMANN;		//Constante de Boltzmann
  extern double TEMPERATURE;		//Température du systeme
  extern double EPSILON;		//Ecart entre la temperature de la simulation et celle de la thermalisation pour qu'il y ait thermalisation
  extern int JMOL;			//Bool pour enregistrer un fichier texte pour jmol ou pas
  extern double RATIO;			//Ratio de la (densite/densite critique)³ (avec densite critique=distance d'interaction et densite=NBRE_PART_AXE)
  extern int SIMULATION;		//Constante permettant de selectionner la simulation que l'on souhaite executer

//---------------------------------------------------------//

#endif
