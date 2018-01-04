#include "toolbox.h"
#include <cstring>

using namespace std;

// dist gaussienne centrée en O avec un sigma = 2
double gauss()
{
     static bool dispo = false;
     static double ga;
     double fac, rsq, v1, v2;
    if (!dispo) 
    {
	do 
	{
	  v1 = 2.0 * rand() / double(RAND_MAX) - 1.0;
	  v2 = 2.0 * rand() / double(RAND_MAX) - 1.0;
	  rsq = v1 * v1 + v2 * v2;
	  }while (rsq >= 1.0 || rsq == 0.0);
	    
      fac = sqrt(-2.0 * log(rsq) / rsq);
      ga = v1 * fac;
      dispo = true;
      return v2*fac;
    } 
    else 
      {
	dispo = false;
	return ga;
      }
}

double mesure_temperature(particule *tab)
{
  double vmoy=0;

  for(long int i=0; i<NBRE_PART; i++)
  {
    vmoy+=pow(tab[i].v.norme(),2.);
  }
    vmoy=vmoy/double(NBRE_PART);
    
  return (MASS/(3.*K_BOLTZMANN))*vmoy;
}

double mesure_pression(particule *tab, double T)
{
  double W=0;
  
  for(long int i=0; i<NBRE_PART; i++)
  {
    for(long int j=i+1; j<NBRE_PART; j++)
    {
	W+=tab[i].force_paire(tab[j].r)*(tab[i].r-tab[j].r);
    }
  }  
  return (NBRE_PART*K_BOLTZMANN*T+(1./3.)*W)/(pow(2.0*BOX_SIZE,3.)); 
}

void enregistrer_temperature_sim1(double *tab, char *chemin_rel,double temp_max)
{
  
  char chemin_abs[500];
  char chemin_travail[500];
  char nom_fichier[500];
  char nom_fichier_temp[500];
  char commande_creation_dossier[500];

  getcwd(chemin_travail,400);
  sprintf(chemin_abs,"%s%s/temperature/",chemin_travail,chemin_rel);
  
  sprintf(commande_creation_dossier,"mkdir -p %s",chemin_abs);
  system(commande_creation_dossier);

  sprintf(nom_fichier_temp,"%stemperature_Tsim%g.txt",chemin_abs,TEMPERATURE*(3.1577464e5));
  ofstream f_temp;
  f_temp.open(nom_fichier_temp);
  for(int i=0; i<NBRE_ITERATION; i++)	//On enleve la premiere temperature car elle diverge et ne permet pas d'afficher correctement les autres valeurs
  {
    if(tab[i]*(3.1577464e5)<temp_max+100){f_temp<<i*PAS_INIT<<"\t"<<tab[i]*(3.1577464e5)<<endl;} 
    else{f_temp<<i*PAS_INIT<<"\t"<<" "<<endl;}
  }
  
  f_temp.close();
  
  //On plot avec gnuplot
  char titre[500];
  char out[500];
  char commande[500];
  
  sprintf(titre,"Evolution_temperature_en_fonction_du_temps_Simulation_T=%g",TEMPERATURE*(3.1577464e5));
  sprintf(out,"%stemperature_Tsim%g",chemin_abs,TEMPERATURE*(3.1577464e5));
  sprintf(commande, "./plot_temp.sh %s %s %s", chemin_abs, out, titre);
  system(commande);

}

void enregistrer_temperature_sim2(double *tab, char *chemin_rel, int num_sim, double temp_max)
{
  
  char chemin_abs[500];
  char chemin_travail[500];
  char nom_fichier[500];
  char nom_fichier_temp[500];
  char commande_creation_dossier[500];

  getcwd(chemin_travail,400);
  sprintf(chemin_abs,"%s%s/temperature/",chemin_travail,chemin_rel);
  
  sprintf(commande_creation_dossier,"mkdir -p %s",chemin_abs);
  system(commande_creation_dossier);

  sprintf(nom_fichier_temp,"%stemperature_num%d.txt",chemin_abs,num_sim);
  ofstream f_temp;
  f_temp.open(nom_fichier_temp);
  for(int i=0; i<NBRE_ITERATION; i++)	
  {
    if(tab[i]*(3.1577464e5)<temp_max+100){f_temp<<i*PAS_INIT<<"\t"<<tab[i]*(3.1577464e5)<<endl;} //On enleve les temperature trop hautes car elles ne permettent pas d'afficher correctement les autres valeurs
    else{f_temp<<i*PAS_INIT<<"\t"<<" "<<endl;}
  }
  
  f_temp.close();
  
  //On plot avec gnuplot
  char titre[500];
  char out[500];
  char commande[500];
  
  sprintf(titre,"Evolution_temperature_en_fonction_du_temps_Simulation_%d",num_sim);
  sprintf(out,"%stemperature_num%d",chemin_abs,num_sim);
  sprintf(commande, "./plot_temp.sh %s %s %s", chemin_abs, out, titre);
  system(commande);

}

void enregistrer_energie_sim2(particule *tab, char *chemin_rel, int num_simulation)
{
  char chemin_abs[500];
  char chemin_travail[500];
  char nom_fichier[500];
  char nom_fichier_nrj[500];
  char commande_creation_dossier[500];

  double energie_pot=0;
  double energie_cin=0;
  double distance=0;
  for(int i=0; i<NBRE_PART; i++)
  {
    for(int j=i+1; j<NBRE_PART; j++)
    {
      distance=(tab[i].r-tab[j].r).norme();
      energie_pot+= 4.*NRJ0*(pow(DIST_INTERACTION/distance,12.0) - pow(DIST_INTERACTION/distance,6.0));
    }
//     energie_cin+=0.5*MASS*tab[i].v.norme()*tab[i].v.norme();
  }
  
  getcwd(chemin_travail,400);
  sprintf(chemin_abs,"%s%s/",chemin_travail,chemin_rel);
  
  sprintf(commande_creation_dossier,"mkdir -p %s",chemin_abs);
  system(commande_creation_dossier);

  sprintf(nom_fichier_nrj,"%senergie_finale.txt",chemin_abs);
  ofstream f_nrj; 
  f_nrj.open(nom_fichier_nrj,ios::out | ios::app);
  f_nrj<<energie_cin+energie_pot<<endl;
  f_nrj.close();
}

fichier_position::fichier_position(char *chemin_rel, int num_simulation)
{
  depassement_boite=0;
  
  char chemin_abs[500];
  char chemin_travail[500];
  char nom_fichier[500];
  char nom_fichier_pos[500];
  char commande_creation_dossier[500];

  getcwd(chemin_travail,400);
  sprintf(chemin_abs,"%s%s/position/",chemin_travail,chemin_rel);
  
  sprintf(commande_creation_dossier,"mkdir -p %s",chemin_abs);
  system(commande_creation_dossier);

  //Le fichier de position n'est pas enregiste de la meme maniere dans les 2 simulations
  if(SIMULATION==1) {sprintf(nom_fichier_pos,"%sposition_T%gK.txt",chemin_abs,TEMPERATURE*(3.1577464e5));}
  
  if(SIMULATION==2) {sprintf(nom_fichier_pos,"%sposition_%d.txt",chemin_abs,num_simulation);}
  
  f_pos.open(nom_fichier_pos);
}

void fichier_position::update(particule *tab,int i)
{
  f_pos<<NBRE_PART<<endl;
  f_pos<<"Geometrie n "<<i<<endl;
  for(int j=0; j<NBRE_PART; j++)
  {
    f_pos<<"Ar "<<tab[j].r*0.529177;
    if(tab[j].r.x>4*BOX_SIZE || tab[j].r.y>4*BOX_SIZE || tab[j].r.z>4*BOX_SIZE){depassement_boite=1;}
  }
}

fichier_position::~fichier_position()
{
  f_pos.close();
}


void Definition_Variable_Simulation()
{
  ifstream f;
  f.open("Variables_simulation.txt");
    
    f>>RADIUS;
    f>>MASS;
    f>>NBRE_PART_AXE;
    NBRE_PART= NBRE_PART_AXE*NBRE_PART_AXE*NBRE_PART_AXE;
    f>>PAS_INIT;
    f>>NRJ0;
    f>>BOX_SIZE;
    f>>NBRE_ITERATION;
    f>>DIST_INTERACTION;
    f>>K_BOLTZMANN;
    f>>TEMPERATURE;
    EPSILON= TEMPERATURE/10.;
    f>>JMOL;
    f>>SIMULATION;
    RATIO=pow((double(NBRE_PART_AXE)*(DIST_INTERACTION))/(2.*BOX_SIZE),3.);
    
    cout<<"------------VARIABLES DE LA SIMULATION (en unite atomique)--------------"<<endl;
    cout<<"RADIUS "<<RADIUS<<endl;
    cout<<"MASS "<<MASS<<endl;
    cout<<"NBRE_PART_AXE "<<NBRE_PART_AXE<<endl;
    cout<<"NBRE_PART "<<NBRE_PART<<endl;
    cout<<"PAS_INIT "<<PAS_INIT<<endl;
    cout<<"NRJ0 "<<NRJ0<<endl;
    cout<<"BOX_SIZE "<<BOX_SIZE<<endl;
    cout<<"NBRE_ITERATION "<<NBRE_ITERATION<<endl;
    cout<<"DIST_INTERACTION "<<DIST_INTERACTION<<endl;
    cout<<"K_BOLTZMANN "<<K_BOLTZMANN<<endl;
    cout<<"TEMPERATURE "<<TEMPERATURE<<endl;
    cout<<"EPSILON "<<EPSILON<<endl;
    cout<<"JMOL "<<JMOL<<endl;
    cout<<"RATIO "<<RATIO<<endl;
    cout<<"SIMULATION "<<SIMULATION<<endl;
    cout<<"------------------------------------------------------------------------"<<endl;
   
   f.close();
}

void enregistrer_variables_simulation_sim1(char *chemin_rel, bool depassement_boite, double pression, double temperature_finale, int np)
{
    char chemin_abs[500];
    char chemin_travail[500];
    char nom_fichier_variables[500];
    
    getcwd(chemin_travail,400);
    sprintf(chemin_abs,"%s%s/",chemin_travail,chemin_rel);
    
    sprintf(nom_fichier_variables,"%sresume_simulations.txt",chemin_abs);
    fstream fichier_variables;
    fichier_variables.open(nom_fichier_variables,ios::out | ios::app);
    
    fichier_variables<<endl;
    fichier_variables<<"------------VARIABLES DE LA SIMULATION TEMPERATURE="<<TEMPERATURE*(3.1577464e5)<<" K (en unite MKSA)--------------"<<endl;
    fichier_variables<<"Nombre de particule par axe= "<<NBRE_PART_AXE<<endl;
    fichier_variables<<"Nombre de particules total= "<<NBRE_PART<<endl;
    fichier_variables<<"Pas de Temps= "<<PAS_INIT*(2.42e-2)<<" fs"<<endl;
    fichier_variables<<"Cote de la boite= "<<BOX_SIZE*(0.529)<<" Angstrom"<<endl;
    fichier_variables<<"Nombre d'iterations sans thermalisation= "<<np<<" / "<<NBRE_ITERATION<<" Iterations"<<endl;
    fichier_variables<<"Ratio=(densite/densite critique)³= "<<RATIO<<endl;
    fichier_variables<<"Pression= "<<pression*(2.90361845e+08)<<" atm"<<endl;
    fichier_variables<<"Temperature de la derniere iteration= "<<temperature_finale<<" K"<<endl;
    fichier_variables<<"La Simulation a Divergee: ";
    if(depassement_boite==1){fichier_variables<<"OUI"<<endl;}
    else{fichier_variables<<"NON"<<endl;}
    fichier_variables<<"------------------------------------------------------------------------"<<endl;    
    fichier_variables<<endl;
    
    fichier_variables.close();
}

void enregistrer_variables_simulation_sim2(char *chemin_rel, bool depassement_boite, double pression, double temperature_finale, int np, int num_sim)
{
    char chemin_abs[500];
    char chemin_travail[500];
    char nom_fichier_variables[500];
    
    getcwd(chemin_travail,400);
    sprintf(chemin_abs,"%s%s/",chemin_travail,chemin_rel);
    
    sprintf(nom_fichier_variables,"%sresume_simulations.txt",chemin_abs);
    fstream fichier_variables;
    fichier_variables.open(nom_fichier_variables,ios::out | ios::app);
    
    fichier_variables<<endl;
    fichier_variables<<"------------VARIABLES DE LA SIMULATION "<<num_sim<<" (en unite MKSA)--------------"<<endl;
    fichier_variables<<"Nombre de particule par axe= "<<NBRE_PART_AXE<<endl;
    fichier_variables<<"Nombre de particules total= "<<NBRE_PART<<endl;
    fichier_variables<<"Pas de Temps= "<<PAS_INIT*(2.42e-2)<<" fs"<<endl;
    fichier_variables<<"Cote de la boite= "<<BOX_SIZE*(0.529)<<" Angstrom"<<endl;
    fichier_variables<<"Nombre d'iterations sans thermalisation= "<<np<<" / "<<NBRE_ITERATION<<" Iterations"<<endl;
    fichier_variables<<"Ratio=(densite/densite critique)³= "<<RATIO<<endl;
    fichier_variables<<"Pression= "<<pression*(2.90361845e+08)<<" atm"<<endl;
    fichier_variables<<"Temperature de la derniere iteration= "<<temperature_finale<<" K"<<endl;
    fichier_variables<<"La Simulation a Divergee: ";
    if(depassement_boite==1){fichier_variables<<"OUI"<<endl;}
    else{fichier_variables<<"NON"<<endl;}
    fichier_variables<<"------------------------------------------------------------------------"<<endl;    
    fichier_variables<<endl;
    
    fichier_variables.close();
}

void counter_simulation(int i)
{
  
  if(i==0)
  {
    cout<<"0%       10%       20%       30%       40%       50%       60%       70%       80%       90%       100%"<<endl;
    cout<<"|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|"<<endl;
  }
  
  if(i!=0 && i !=NBRE_ITERATION-1)
  {
    cout<<"\b *"<<flush;
  }
}


histogramme_position::histogramme_position()
{
  min=0;
  max=BOX_SIZE;
  pas=(BOX_SIZE)/double(int(DIST_INTERACTION*20));
  
  taille=int(DIST_INTERACTION*20);	//=DIST_INTERACTION*20
  tab_histo=new double[taille];
  for(int i=0; i<taille; i++)
    tab_histo[i]=0;
}

histogramme_position::~histogramme_position()
{
  delete[] tab_histo;
}

void histogramme_position::update(particule *tab_part)
{
  double dist;
  
  for(int i=0; i<NBRE_PART; i++)
  {
    for(int j=i+1; j<NBRE_PART; j++)
    {
      dist=(tab_part[i].r-tab_part[j].r).norme();
      if(dist>BOX_SIZE){dist=dist-BOX_SIZE;}
      if(min<=dist && dist<=max){tab_histo[int((dist-min)/pas)]++;}   
    }
  }
}

histogramme_vitesse::histogramme_vitesse(double temp_max)
{
  double vlim=sqrt(3.*NBRE_PART*K_BOLTZMANN*(temp_max/(3.1577464e5))/MASS);
  
  min=-vlim;
  max=+vlim;
  taille=100+NBRE_PART;
  pas=(max-min)/double(taille);
  
  tab_histo=new double[taille];
  for(int i=0; i<taille; i++)
    tab_histo[i]=0;
}

histogramme_vitesse::~histogramme_vitesse()
{
  delete[] tab_histo;
}

void histogramme_vitesse::update(particule *tab_part)
{

  for(int i=0; i<NBRE_PART; i++)
  {
    if(min<=tab_part[i].v.x && tab_part[i].v.x<=max){tab_histo[int((tab_part[i].v.x-min)/pas)]++;}
  }
}


void histogramme_position::enregistrer(char *chemin_rel)
{
  char chemin_abs[500];
  char chemin_travail[500];
  char nom_fichier[500];
  char nom_abs[500];
  char commande_creation_dossier[500];
  getcwd(chemin_travail,400);
  sprintf(chemin_abs,"%s%s/distribution_radiale/",chemin_travail,chemin_rel);
  sprintf(nom_fichier,"distrib_radiale_ratio_densite%g_T%g",RATIO, TEMPERATURE*(3.1577464e5));
  sprintf(nom_abs,"%s%s.txt", chemin_abs,nom_fichier);
  
  //On cree le dossier de travail
  sprintf(commande_creation_dossier,"mkdir -p %s",chemin_abs);
  system(commande_creation_dossier);
  
  //On écrit dans le dossier
  ofstream fichier;
  fichier.open(nom_abs);
  
  for(int i=0; i<taille; i++)
  {
    fichier<<min+i*pas<<"\t"<<tab_histo[i]<<endl; 
  }
  
  //On execute gnuplot
    char commande[500];
    char titre[500];
    char label[500];
    
    sprintf(label,"Distance_en_ua");
    sprintf(titre,"Distribution_radiale_avec:_Ratio_densite=_%g_,_T=_%g_K",RATIO, TEMPERATURE*(3.1577464e5));
    sprintf(commande, "./plot_histo.sh %s %e %e %s %s %s", chemin_abs, min, max, nom_fichier, titre,label);
    system(commande);
    fichier.close();
}

void histogramme_vitesse::enregistrer(char *chemin_rel)
{
  char chemin_abs[500];
  char chemin_travail[500];
  char nom_fichier[500];
  char nom_abs[500];
  char commande_creation_dossier[500];

  getcwd(chemin_travail,400);
  sprintf(chemin_abs,"%s%s/distribution_vitesse/",chemin_travail,chemin_rel);
  sprintf(nom_fichier,"distrib_vitesse_ratio_densite%g_T%g",RATIO, TEMPERATURE*(3.1577464e5));
  sprintf(nom_abs,"%s%s.txt", chemin_abs,nom_fichier);
  
  //On cree le dossier de travail
  sprintf(commande_creation_dossier,"mkdir -p %s",chemin_abs);
  system(commande_creation_dossier);
  
  //On écrit dans le dossier
  ofstream fichier;
  fichier.open(nom_abs);
  
  for(int i=0; i<taille; i++)
  {
    fichier<<min+i*pas<<"\t"<<tab_histo[i]<<endl; 
  }
    
  
  //On execute gnuplot
    char commande[500];
    char titre[500];
    char label[500];
    
    sprintf(label,"Vitesse_en_ua");
    sprintf(titre,"Distribution_des_vitesses_avec:_ratio_densite=_%g_,_T=_%g_K",RATIO, TEMPERATURE*(3.1577464e5));
    sprintf(commande, "./plot_histo.sh %s %e %e %s %s %s", chemin_abs, min, max, nom_fichier, titre,label);
    system(commande);
  
  fichier.close();
}

void enregistrer_energie_sim1(char *chemin_rel, double *tab1, double *tab2)
{
  char chemin_abs[500];
  char chemin_travail[500];
  char nom_fichier[500];
  char nom_abs[500];
  char commande_creation_dossier[500];
  
  getcwd(chemin_travail,400);
  sprintf(chemin_abs,"%s%s/energie/",chemin_travail,chemin_rel);
  sprintf(nom_fichier,"energie_ratio_densite%g_T%g",RATIO, TEMPERATURE*(3.1577464e5));
  sprintf(nom_abs,"%s%s.txt", chemin_abs,nom_fichier);
  
  //On cree le dossier de travail
  sprintf(commande_creation_dossier,"mkdir -p %s",chemin_abs);
  system(commande_creation_dossier);
  
  //On écrit dans le dossier
  ofstream fichier;
  fichier.open(nom_abs);
  
  //on retravaille un peu le tableau avant de l'enregistrer:    
  
  double E_moy=0;
  double sigma_moy=0;
  
  for(int i=0; i<NBRE_ITERATION; i++)
  {
    fichier<<i*PAS_INIT<<"\t"<<tab1[i]<<"\t"<<sqrt(tab2[i]-tab1[i]*tab1[i])<<endl;
    E_moy+=tab1[i];
    sigma_moy+=sqrt(tab2[i]-tab1[i]*tab1[i]);
  }
    
  E_moy=E_moy/NBRE_ITERATION;
  sigma_moy=sigma_moy/NBRE_ITERATION;
  cout<<"Emoy "<<E_moy<<endl;
  cout<<"Sigma_moy "<<sigma_moy<<endl;
    
  //on execute gnuplot
    char commande[500];
    char titre[500];
    char titre2[500];
    char Emoy[500];
    char sigmamoy[500];
        
    sprintf(Emoy,"Energie_Moyenne=%g",E_moy);
    sprintf(sigmamoy,"Ecart_type_Moyen=%g",sigma_moy);
    
    sprintf(titre,"Energie_moyenne_en_fonction_du_temps_avec:_ratio_densite=_%g_,_T=_%g_K",RATIO, TEMPERATURE*(3.1577464e5));
    sprintf(titre2,"Ecart-Type_moyen_en_fonction_du_temps_avec:_ratio_densite=_%g_,_T=_%g_K",RATIO, TEMPERATURE*(3.1577464e5));
    sprintf(commande, "./plot_energie.sh %s %s %s %s %f %s %s", chemin_abs, nom_fichier, titre,titre2,double(NBRE_ITERATION*PAS_INIT),Emoy,sigmamoy);
    system(commande);
  
  
  fichier.close();
}


