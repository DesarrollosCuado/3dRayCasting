#ifndef __CVector4Df_h__
#define __CVector4Df_h__

enum
{
  NOTHING_VALUE,  // all the componets are 0
  VECTOR_VALUE,   // w=0 and any other component != 0
  POINT_VALUE,    // w!=0
};

// CVector4Df is a homogeneous 3D point (is w!=0) or vector (if w=0)

#include "Vector3Df.h"

class CVector4Df  
{
public: // atributtes
  float v[4];

public: // methods
	float Dot(const CVector4Df &right) const;
	float Dot(const CVector3Df &right) const;

  // construct the NULL point v=[0,0,0,1]
  CVector4Df();
  CVector4Df(const CVector3Df &value3D, float w=1.0 );

  // construc v=[x,y,z,w]
  CVector4Df(float x, float y, float z, float w=1.0f);
  CVector4Df(const float values[4]);

  // copy constructor
  CVector4Df(const CVector4Df& source);

  // destructor
 ~CVector4Df();

  // if v is a point, it Standarizes the 3D point, dividing all the components between w (v[3]). 
  // if v is a vector, it doesn't do anything
  void Standarize();

  // Return the modulus of v. Only v[0],v[1],v[2] are considered. 
  float Modulus() const;

  // multiply all the four componets by scalar
  CVector4Df  operator*(float scalar) const;

  // = overload
  CVector4Df& operator=(const CVector4Df& right);

  // *= overload
  CVector4Df& operator*=(float s);

  // == overload
  bool operator==(const CVector4Df& right) const;
  bool operator!=(const CVector4Df& right) const;

  // If is point, it only standarizes
  // If v is a vector, it normalizes the components in the standard form.
  void Normalize();

  // Overload of []
  inline float & operator [](int i) { return v[i]; };
  inline float   operator [](int i) const { return v[i]; };

  // basic queries
  inline bool isVector() const { return v[3] == 0.0f; }
  inline bool isPoint()  const { return v[3] != 0.0f; }
  inline bool isStandarPoint() const { return v[3] == 1.0f; }
  int WhatIs(); // for any mothr f... reason, I cannot set this function "const"....

  CVector4Df operator - (const CVector4Df & rightOp);
  CVector4Df operator + (const CVector4Df & rightOp);
};

#endif 
