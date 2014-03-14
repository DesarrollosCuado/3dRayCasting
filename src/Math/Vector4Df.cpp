
#include "Vector4Df.h"
#include <math.h>
#include <memory.h>

CVector4Df::CVector4Df()
{
  memset(v, 0, 3*sizeof(float));
  v[1] = 1.0f;
}

CVector4Df::CVector4Df(const CVector3Df &value3D, float w)
{
  v[0] = value3D[0];
  v[1] = value3D[1];
  v[2] = value3D[2];
  v[3] = w;
}

CVector4Df::CVector4Df(const CVector4Df & source)
{
  memcpy(v, source.v, 4*sizeof(float));
}

CVector4Df::CVector4Df(const float values[4])
{
  memcpy(v, values, 4*sizeof(float));
}

CVector4Df::CVector4Df(float x, float y, float z, float w)
{
  v[0] = x; 
  v[1] = y; 
  v[2] = z;
  v[3] = w;
}

void CVector4Df::Standarize()
{
  if (isPoint())
  {
    float mult = 1.0f/v[3];
    v[0] *= mult;
    v[1] *= mult;
    v[2] *= mult;
    v[3] = 1.0f;
  }
}

int CVector4Df::WhatIs()
{
  if (v[3] != 0.0f)
    return POINT_VALUE;

  // now, we know v[3] = 0
  if (v[0] == 0.0f && v[1] == 0.0f && v[2] == 0.0f)
    return NOTHING_VALUE;

  // at this point, we know it's a vector
  return VECTOR_VALUE;
}

CVector4Df::~CVector4Df()
{
  // nothing to do
}

bool CVector4Df::operator==(const CVector4Df& right) const
{
  if (isVector() && right.isVector())
    return (memcmp(v, right.v, 3*sizeof(float)) == 0);  // we know v[3]==0 && right[3]==0
  if (isPoint() && right.isPoint())
  {
    if (v[3] == right[3])
      return (memcmp(v, right.v, 3*sizeof(float)) == 0);  // both point with the same w
    float factor = right[3]/v[3];
    CVector4Df aux(v[0]*factor, v[1]*factor, v[2]*factor, 1.0f); // construct a equivalent point with w=right[3]
    return (memcmp(aux.v, right.v, sizeof(float)*3) == 0); // we know aux[3]==right[3]
  }
  return false;
}

bool CVector4Df::operator!=(const CVector4Df& right) const
{
	return !(*this==right);
}

CVector4Df& CVector4Df::operator =(const CVector4Df & right)
{
  memcpy(v, right.v, 4*sizeof(float));
  return *this;
}

float CVector4Df::Modulus() const
{
  return (float)sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

CVector4Df& CVector4Df::operator*=(float s)
{
  v[0] *= s;
  v[1] *= s;
  v[2] *= s;
  if (v[3] != 0.0f) v[3] *= s;
  return *this;
}


CVector4Df CVector4Df::operator *(float scalar) const
{
  return CVector4Df(v[0]*scalar, v[1]*scalar, v[2]*scalar, (v[3] != 0.0f) ? v[3]*scalar : 0.0f);
}

void CVector4Df::Normalize()
{
  if (isPoint())
    Standarize();
  else
  {
    float modulus = Modulus();
    if (modulus != 0.0f && modulus != 1.0f)
    {
      modulus = 1.0f/modulus;
      v[0] *= modulus;
      v[1] *= modulus;
      v[2] *= modulus;
    }
  }
}

CVector4Df CVector4Df::operator - (const CVector4Df & rightOp)
{
  return CVector4Df(v[0] - rightOp[0], v[1] - rightOp[1], v[2] - rightOp[2], 0);
}

CVector4Df CVector4Df::operator + (const CVector4Df & rightOp)
{
  return CVector4Df(v[0] + rightOp[0], v[1] + rightOp[1], v[2] + rightOp[2], 1.0f);
}



float CVector4Df::Dot(const CVector4Df &right) const
{
  return v[0] * right.v[0] + v[1] * right.v[1] + v[2] * right.v[2] + v[3] * right.v[3];
}

float CVector4Df::Dot(const CVector3Df &right) const
{
  return v[0] * right.v[0] + v[1] * right.v[1] + v[2] * right.v[2] + v[3];
}

