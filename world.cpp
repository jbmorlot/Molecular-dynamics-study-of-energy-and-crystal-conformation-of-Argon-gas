#include "world.h"

using namespace std;

world::world()
{
    dt=PAS_INIT;
    srand48(time(NULL));
    //Création du tableau de particules
    tab_part=new particule [NBRE_PART];
    
      //Initialisation des valeurs du tableau de particules
      vecteur r1;
      int counter=0;
      if(SIMULATION==1)
      {
	for(int i=0; i<NBRE_PART_AXE; i++)
	{
	  for(int j=0; j<NBRE_PART_AXE; j++)
	  {
		for(int k=0; k<NBRE_PART_AXE; k++)
	      {
		r1.set(-BOX_SIZE+i*(2.*BOX_SIZE/NBRE_PART_AXE),-BOX_SIZE+j*(2.*BOX_SIZE/NBRE_PART_AXE),-BOX_SIZE+k*(2.*BOX_SIZE/NBRE_PART_AXE));
		tab_part[counter].set(r1);
		counter++;
	      }
	  }
	}     
      }
      
      if(SIMULATION==2)
      {
	double randx;	//Variables qui vont rajouter un petite composante aléatoire pour la simulation 2
	double randy;	//Car sinon on aurait toujours les meme resultats
	double randz;
	
	for(int i=0; i<NBRE_PART_AXE; i++)
	{
	  for(int j=0; j<NBRE_PART_AXE; j++)
	  {
		for(int k=0; k<NBRE_PART_AXE; k++)
	      {
		randx=(drand48()*0.8)+0.1;
		randy=(drand48()*0.8)+0.1;
		randz=(drand48()*0.8)+0.1;
		r1.set(-BOX_SIZE+(i+randx)*(2.*BOX_SIZE/NBRE_PART_AXE),-BOX_SIZE+(j+randy)*(2.*BOX_SIZE/NBRE_PART_AXE),-BOX_SIZE+(k+randz)*(2.*BOX_SIZE/NBRE_PART_AXE));
		tab_part[counter].set(r1);
		counter++;
	      }
	  }
	}      
      }
            
      
      distribution_normale_vitesse();		//On applique une distribution normale des vitesses autour de la temprérature définie

}

world::~world()
{
    delete[] tab_part;  
}



void world::calcul_force_particules()
{
  vecteur force;
  for(long int i=0; i<NBRE_PART; i++)
    tab_part[i].force.set(0.0,0.0,0.0);

  for(long int i=0; i<NBRE_PART; i++)
  {
    for(long int j=i+1; j<NBRE_PART; j++)
    {
      force=tab_part[i].force_paire(tab_part[j].r);
      tab_part[i].force= tab_part[i].force+force;
      tab_part[j].force= tab_part[j].force-force;
    }
  }
    
}

void world::integration_verlet()
{
	for(long int i=0; i<NBRE_PART; i++)
	{

	  tab_part[i].v=tab_part[i].v+tab_part[i].force*(1./tab_part[i].masse)*dt*(1./2.);
	  tab_part[i].r=tab_part[i].r+tab_part[i].v*dt;
	  
	  //Vérification des conditions aux limites
	  conditions_limites_BVK(tab_part[i].r);
	}
	
	calcul_force_particules();	//On calcule la force à t+dt d ou l acceleration

	
	for(long int i=0; i<NBRE_PART; i++)
	{
	  tab_part[i].v=tab_part[i].v+tab_part[i].force*(1./tab_part[i].masse)*dt*(1./2.);
	}
	
}



void world::conditions_limites_BVK(vecteur& r)
{
  
  if(r.x>BOX_SIZE){r.x-=2*BOX_SIZE;}
    
  if(r.x<-BOX_SIZE){r.x+=2*BOX_SIZE;}
    
  if(r.y>BOX_SIZE){r.y-=2*BOX_SIZE;}
    
  if(r.y<-BOX_SIZE){r.y+=2*BOX_SIZE;}
    
  if(r.z>BOX_SIZE){r.z-=2*BOX_SIZE;}
    
  if(r.z<-BOX_SIZE){r.z+=2*BOX_SIZE;}
    
}

void world::distribution_normale_vitesse()
{
  srand48(time(NULL));
 
  for(long int i=0; i<NBRE_PART; i++)
    tab_part[i].v.set(gauss(),gauss(),gauss());

  vecteur VCM;
  VCM.set(0,0,0);
  double mt = 0;;
  for(long int i=0; i<NBRE_PART; i++)
  {
       	VCM = VCM + tab_part[i].v*tab_part[i].masse;
 	mt += tab_part[i].masse;
  }
      VCM = VCM*(1.0/mt);
  for(long int i=0; i<NBRE_PART; i++)
       	tab_part[i].v = tab_part[i].v - VCM;
}


void world::thermalisation()
{
  double T_simulation;
  T_simulation=mesure_temperature(tab_part);
  
    for(int i=0; i<NBRE_PART; i++)
    {
      tab_part[i].v=tab_part[i].v*sqrt(TEMPERATURE/T_simulation);	//v1=sqrt(T1/T2)*v2
    }

}



