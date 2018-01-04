#ifndef VECTEUR_H_INCLUDED
#define VECTEUR_H_INCLUDED

#include <cmath>
#include <iostream>
#include <fstream>
#include "defines.h"

class vecteur
{
  
	public : 
	double x;
	double y;
	double z;
	
	vecteur();	
	double norme();
	void set(double x1, double y1, double z1);
	~vecteur();
	
	vecteur operator+(const vecteur & a) const;
	vecteur operator*(const double & a) const;
	double operator*(const vecteur & a) const;
	vecteur operator-(const vecteur & a) const;
	vecteur& operator=(const vecteur & a);
	vecteur& operator+=(const vecteur & s);
	friend std::ostream& operator<<(std::ostream & flux, const vecteur & v);
	
};

#endif // VECTEUR_H_INCLUDED