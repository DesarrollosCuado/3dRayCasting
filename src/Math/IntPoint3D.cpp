// IntPoint3D.cpp: implementation of the IntPoint3D class.
//
//////////////////////////////////////////////////////////////////////

#include "IntPoint3D.h"
#include <string.h>
#include <math.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



CIntPoint3D::CIntPoint3D()
{
  memset(values, 0, sizeof(int)*3);
}

CIntPoint3D::CIntPoint3D(const CIntPoint3D &source)
{
  memcpy(values, source.values, sizeof(int)*3);
}

CIntPoint3D::CIntPoint3D(int x, int y, int z)
{
  values[0] = x;
  values[1] = y;
  values[2] = z;
}


CIntPoint3D CIntPoint3D::operator -(const CIntPoint3D &right) const
{
  return CIntPoint3D(values[0] - right[0], values[1] - right[1], values[2] - right[2]);
}

CIntPoint3D CIntPoint3D::operator +(const CIntPoint3D &s)
{
  return CIntPoint3D(values[0]+s[0], values[1]+s[1], values[2]+s[2]);
}
CIntPoint3D CIntPoint3D::operator *(int s)
{
  return CIntPoint3D(values[0]*s, values[1]*s, values[2]*s);
}

CIntPoint3D CIntPoint3D::operator /(int s)
{
  return CIntPoint3D(values[0]/s, values[1]/s, values[2]/s);
}

bool CIntPoint3D::operator == (const CIntPoint3D &source) const
{
  return values[0] == source.values[0] &&  values[1] == source.values[1] && values[2] == source.values[2];
}

float CIntPoint3D::Modulus()
{
  return sqrt(values[0]*values[0]+values[1]*values[1]+float(values[2]*values[2]));
}
