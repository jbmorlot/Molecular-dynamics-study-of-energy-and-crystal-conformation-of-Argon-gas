#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>

using namespace std;

int main(int argc, char **argv)
{

  double tab_variable_simulation[12];
  
  tab_variable_simulation[0]=1.339;		//RADIUS
  tab_variable_simulation[1]=7.282e4;		//MASS
  tab_variable_simulation[2]=4;			//NBRE_PART_AXE
  tab_variable_simulation[3]=20.0;		//PAS_INIT
  tab_variable_simulation[4]=3.793e-4;		//NRJ0
  tab_variable_simulation[5]=20;		//BOX_SIZE
  tab_variable_simulation[6]=20000;		//NBRE_ITERATION
  tab_variable_simulation[7]=6.424;		//DIST_INTERACTION
  tab_variable_simulation[8]=1.;		//K_BOLTZMANN
  tab_variable_simulation[9]=9.50044657e-03;	//TEMPERATURE
  tab_variable_simulation[10]=0;		//BOOL POUR ENREGISTRER UN FICHIER TEXTE POUR JMOL OU PAS (0:NON ; 1:OUI)
  tab_variable_simulation[11]=atof(argv[1]);	//SIMULATION
  
  double RATIO=0;
  //-----------------SIMULATION 1---------------------------
  
  if(tab_variable_simulation[11]==1)
  {
    //Evolution de la temperature
    char commande[100];
    char chemin_simulation1[100];
    int counter=0;
    int counter_max=0;
    double Temperature_max=1000;
    int palier1=5000;		//nombre d'itération avec thermalisation avant la simulation
    int bin=0;			//une constante qui sert pour la simulation 2 uniquement
    
    if(palier1>=tab_variable_simulation[6])
    {
      cout<<"ERREUR: palier>NBRE_ITERATION"<<endl;
    }
    
    //Initialisation du compteur pour savoir le nombre de calcul on s'aprete a faire
    for(double n=2; n<=9; n++)
    {
      double var=0;
      for(double i=0.1; i<Temperature_max; i+=var)
	{
	  if(i<4){var=0.4;}
	  if(i<50){var=5;}
	  if(i>=50){var=50;}
	  if(i>100){var=100;}
	  counter_max++;
	} 
    }
    
    
    //On change le nombre de particules
    for(double n=2; n<=9; n++)
    {
      //On change le nombre de particules dans la boite
      tab_variable_simulation[2]=n;
      
      //On change la temperature
      double var=0;
      for(double i=0.1; i<Temperature_max; i+=var)
      {
	if(i<4){var=0.4;}
	if(i<50){var=5;}
	if(i>=50){var=50;}
	if(i>100){var=100;}
	counter++;
	cout<<"***************************************************************************"<<endl;
	cout<<"\t-------------------SIMULATION "<<counter<<" / "<<counter_max<<" ------------------"<<endl;
	cout<<"***************************************************************************"<<endl;
	
	//On écrit le fichier des varibles globales
	ofstream fichier;
	fichier.open("Variables_simulation.txt");
	
	tab_variable_simulation[9]=i/(3.1577464e5);
	    
	for(int j=0; j<12; j++)
	{
	  if(j==2 || j==6 || j==11)
	  {
	    fichier<<int(tab_variable_simulation[j])<<endl;
	  }
	  else
	  {
	    fichier<<tab_variable_simulation[j]<<endl;
	  }
	}
	
	//On lance la simulation avec les nouvelles variables
	RATIO=pow((double(tab_variable_simulation[2])*(tab_variable_simulation[7]))/(2.*tab_variable_simulation[5]),3.);
	
	sprintf(chemin_simulation1,"/Simulation1_Etude_systeme_Energie_constante/Ratio_%g",RATIO);
	sprintf(commande,"./dynamique_mol %s %g %d %d",chemin_simulation1,Temperature_max/2.,counter,palier1, bin);
	system(commande);
	fichier.close();
      }
    }
  }
  
  
  //------------------------SIMULATION 2------------------------------------
  
  
  if(tab_variable_simulation[11]==2)
  {
    //Evolution de la temperature
    char commande[100];
    char chemin_simulation1[100];
    int counter=0;
    double Temperature_max=500;
    RATIO=pow((double(tab_variable_simulation[2])*(tab_variable_simulation[7]))/(2.*tab_variable_simulation[5]),3.);
    int counter_max=10;
    int palier1=5000;	//Nombre d'iteration pour la thermalisation
    int palier2=100;	//Nombre d'itération par palier de temperature
    int palier3=5000;	//Nomre d'iteration une fois la temperature a 0.1K
    
    //On regle le nombre d'itérations 
    tab_variable_simulation[6]=palier1 + int(1+Temperature_max/5.)*palier2 + palier3;
    tab_variable_simulation[9]=Temperature_max/(3.1577464e5);
    
     for(int i=0; i<counter_max; i++)
     {
	cout<<"***************************************************************************"<<endl;
	cout<<"\t-------------------SIMULATION "<<counter<<" / "<<counter_max<<" ------------------"<<endl;
	cout<<"***************************************************************************"<<endl;
	
	counter++;
	
	tab_variable_simulation[10]=1;		//On active JMOL
	
	//On écrit le fichier des varibles globales
	ofstream fichier;
	fichier.open("Variables_simulation.txt");
	for(int j=0; j<12; j++)
	{
	  if(j==2 || j==6 || j==11)
	  {
	    fichier<<int(tab_variable_simulation[j])<<endl;
	  }
	  else
	  {
	    fichier<<tab_variable_simulation[j]<<endl;
	  }
	}
	
	//On lance la simulation avec les nouvelles variables
	sprintf(chemin_simulation1,"/Simulation2_Etude_reseaux_cristallins_argon/NBRE_PART_%d_RATIO_%g",int(pow(tab_variable_simulation[2],3.)),RATIO);
	sprintf(commande,"./dynamique_mol %s %g %d %d %d",chemin_simulation1,Temperature_max/2.,i,palier1, palier2);
	system(commande);
	fichier.close();
     }	    
	//On recherche ensuite la simulation avec la plus basse energie
	char nom_fichier1[400];
	char nom_fichier2[400];
	char chemin_travail[400];
	char chemin_abs[400];
	
	getcwd(chemin_travail,400);
	sprintf(chemin_abs,"%s%s",chemin_travail,chemin_simulation1);
  
	sprintf(nom_fichier1,"%s/energie_finale.txt",chemin_abs);
	sprintf(nom_fichier2,"%s/energie_finale_classement.txt",chemin_abs);
	
	ifstream f_energie;
	f_energie.open(nom_fichier1);
	
	ofstream f_classement;
	f_classement.open(nom_fichier2);
	
	double tab_energie[counter_max];
	for(int i=0; i<counter_max; i++)
	{
	  f_energie>>tab_energie[i];
	}
	
	  //On trie le tableau et on enregistre le classement
	  double min=tab_energie[0];
	  int num_min=0;
	  f_classement<<"Num_simulation\t\tEnergie"<<endl;
	  for(int i=0; i<counter_max; i++)
	  {
	    for(int j=0; j<counter_max; j++)
	    {
	      if(tab_energie[j]<min) 
	      {
		min=tab_energie[j];
		num_min=j;
	      }
	    }  
	    f_classement<<num_min<<"\t\t"<<min<<endl;
	    tab_energie[num_min]=100000;	//On met le min qu l'on vien de trouver à une valeur importante pour qu'il ne soit pas dans les prochains tirages
	    min=100000;
	  }
	
	
	f_energie.close();
	f_classement.close();
   }
 
  
  return 0;
}
