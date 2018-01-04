#include <iostream>
#include <fstream>

#include "defines.h"
#include "particule.h"
#include "vecteur.h"
#include "world.h"
#include "toolbox.h"

using namespace std;


int main(int argc, char **argv)
{
  //On modifie les entree 
//   sprintf(chemin,"%s%s",argv[1],argv[2]);
//   cout<<argv[1]<<endl;

  //Défintion des variables d'environnement
  Definition_Variable_Simulation();
  
  //Création de l'environnement de la simulation
  world Tworld;
  
  //Création des fichiers de sortie
  fichier_position fichier_pos(argv[1],atof(argv[3]));

//   ofstream f_temp;
//   f_temp.open("Temperature.txt");
  
    
  //Creation constantes
  double W = 0;
  int np = 0;
  double T_simulation=0;
  double T_thermalis=0;
  double P=0;
  int thermalis_counter=0;	//Donne le nombre d'iteration ou la simulation est stable avant de commencer a faire des mesures
  
  //Parametres pour la Simulation 1: Etude a enerie constante: ensemble microcanonique
//   if(SIMULATION==1)
//   {
    //Creation des histogrammes et autres figures
    histogramme_position hist_pos;
    histogramme_vitesse hist_vit(atof(argv[2]));

    double tab_energie[NBRE_ITERATION];	//Tableau de l'énergie moyenne
    double tab_energie2[NBRE_ITERATION];	//Tableau de l'énergie carre moyenne
    
      for(int i=0; i<NBRE_ITERATION; i++)
	{
	  tab_energie[i]=0;
	  tab_energie2[i]=0;
	}
//   }
  
  //Paramètres pour la simulation 2: Etude des réseaux cristallins de l'Argon
//   if(SIMULATION==2)
//   {
    //Creation d'un tableau de temperature pour ensuite l'enregistrer dans un fichier
    double tab_temperature[NBRE_ITERATION];
    for(int i=0; i<NBRE_ITERATION; i++)
	tab_temperature[i]=0;
    
    int counter_simulation_2=0;		//Compteur permettant de compter le nobre d'itérations entre chaque décrementation de la temperature
//   }

  
  for (long int i=0; i<NBRE_ITERATION; i++)
  { 
    
    //On thermalise jusqu'a ce que la temperature soit stable pendant 1000 itérations
    T_simulation=mesure_temperature(Tworld.tab_part);  
    tab_temperature[i]=T_simulation;

    
    if(thermalis_counter<int(atof(argv[4])))
    {
      Tworld.thermalisation();
      T_thermalis=mesure_temperature(Tworld.tab_part);        
    }

    if(T_thermalis-T_thermalis/10.<=T_simulation && T_simulation<=T_thermalis+T_thermalis/10.)
    {
      thermalis_counter++;
    }
    
    //Une fois qu la temperature est stable, on peut commencer a prendre des valeurs
    	if(SIMULATION==1 && thermalis_counter>int(atof(argv[4])))
	{
	  P=mesure_pression(Tworld.tab_part,T_simulation);
	  W += P;
	  np++;
	  hist_pos.update(Tworld.tab_part);
	  hist_vit.update(Tworld.tab_part);
	  for(int k=0; k<NBRE_PART; k++)
	  {
	    tab_energie[i]+=0.5*MASS*Tworld.tab_part[k].v.norme()*Tworld.tab_part[k].v.norme();
	    tab_energie2[i]+=(0.5*MASS*Tworld.tab_part[k].v.norme()*Tworld.tab_part[k].v.norme())*(0.5*MASS*Tworld.tab_part[k].v.norme()*Tworld.tab_part[k].v.norme());
	  }
	  tab_energie[i]=tab_energie[i]/NBRE_PART;
	  tab_energie2[i]=tab_energie2[i]/NBRE_PART;
	  
	}
	
	if(SIMULATION==2 && i>=int(atof(argv[4])) && thermalis_counter>int(atof(argv[4])/2.))
	{
	  P=mesure_pression(Tworld.tab_part,T_simulation);
	  W += P;
	  np++;
	  
	  counter_simulation_2++;
	  if(counter_simulation_2==int(atof(argv[5])) && TEMPERATURE*(3.1577464e5)>5)	//On attend 100 iterations le temps que la simulation s'adapte a sa nouvelle temperature
	  {
	    TEMPERATURE-=(5./(3.1577464e5));
	    Tworld.thermalisation();	//On diminue la temperature de la simulation = on thermalise 1 seule fois!
	    counter_simulation_2=0;
	  }	  
	 }

    
        
    //On prend la position des particule une fois tous le 100 itérations
    if(i%100==0 && JMOL==1 && SIMULATION==1)
    {
      fichier_pos.update(Tworld.tab_part,i);
    }
    
    if(SIMULATION==2 && TEMPERATURE*(3.1577464e5)<=5 && i%100==0)
    {
      fichier_pos.update(Tworld.tab_part,i);
    }
    
    
    if(i*100%NBRE_ITERATION==0)
    {
      counter_simulation(i);
    }
    
    //Intégration numérique pour passer à t+dt
    Tworld.integration_verlet();
    
  }
   // il faut faire la moyenne dans le temps 
    cout<<endl;
    cout<<"Nombre d'iteration SANS thermalisation="<<np<<" / "<<NBRE_ITERATION<<" iterations"<<endl;
    cout<<"Pression(atm)= "<<W/np*2.90361845e+08<<endl;
    cout<<"Température finale(K)= "<<T_simulation*(3.1577464e5)<<endl;
    if(fichier_pos.depassement_boite==1){cout<<"ERREUR: La simulation a divergee!!!"<<endl;}
    
    //On enregistre les fichier et on dessine les courbe
    if(SIMULATION==1)
    {
      hist_pos.enregistrer(argv[1]);
      hist_vit.enregistrer(argv[1]);
      enregistrer_energie_sim1(argv[1], tab_energie, tab_energie2);
      enregistrer_variables_simulation_sim1(argv[1],fichier_pos.depassement_boite, W/np, 2*atof(argv[2]),np);
      enregistrer_temperature_sim1(tab_temperature,argv[1],2*atof(argv[2])); 
    }
    
    if(SIMULATION==2)
    {
      enregistrer_energie_sim2(Tworld.tab_part,argv[1],atof(argv[3]));
      enregistrer_variables_simulation_sim2(argv[1],fichier_pos.depassement_boite, W/np, 2*atof(argv[2]),np,atof(argv[3]));
      enregistrer_temperature_sim2(tab_temperature,argv[1],atof(argv[3]),2*atof(argv[2])); 
    }
    

    

  
  
  return 0;
}
