#include "particule.h"

using namespace std;


particule::particule()
{
    r.set(0.,0.,0.);
    v.set(0.,0.,0.);
    force.set(0.,0.,0.);
    rayon=RADIUS;
    masse=MASS;
    dist_int6=pow(DIST_INTERACTION,6.0);
    
}

void particule::set(vecteur r1)
{
    r=r1;
}


particule& particule::operator=(const particule& a)
{
  r = a.r;
  v = a.v;
  force = a.force;
  return *this;
}

particule::~particule()
{}

vecteur particule::force_paire(vecteur r2)
{
  vecteur r2p;
  r2p=r2;	
  
  if((r2-r).x>=BOX_SIZE){r2p.x=r2.x-2*BOX_SIZE;}
  if((r2-r).y>=BOX_SIZE){r2p.y=r2.y-2*BOX_SIZE;}
  if((r2-r).z>=BOX_SIZE){r2p.z=r2.z-2*BOX_SIZE;}

  if((r2-r).x<=-BOX_SIZE){r2p.x=r2.x+2*BOX_SIZE;}
  if((r2-r).y<=-BOX_SIZE){r2p.y=r2.y+2*BOX_SIZE;}
  if((r2-r).z<=-BOX_SIZE){r2p.z=r2.z+2*BOX_SIZE;}

  
  double distance=(r-r2p).norme();
  double distance_7=pow(distance,7.0);
  return (r - r2p)*(-4.*NRJ0*(dist_int6/distance_7)*(-12.*dist_int6/(distance_7) + 6./(distance)));

  
}

std::ostream& operator<<(std::ostream& flux, const particule & p)
{
  flux<<"r= "<<p.r<<endl<<"v= "<<p.v<<endl<<"force= "<<p.force<<endl;
  return flux;	
}

