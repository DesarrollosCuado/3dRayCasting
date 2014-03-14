
#include "Vector3Df.h"
#include <math.h>
#include <memory.h>

CVector3Df::CVector3Df()
{
  memset(v, 0, 3*sizeof(float));
}

CVector3Df::CVector3Df(const CVector3Df & source)
{
  memcpy(v, source.v, 3*sizeof(float));
}

CVector3Df::CVector3Df(float x, float y, float z)
{
  v[0] = x; 
  v[1] = y; 
  v[2] = z;
}

CVector3Df::CVector3Df(const CVector3Df& p1, const CVector3Df& p2, const CVector3Df& p3)
{
  *this = CVector3Df(p1,p2)*CVector3Df(p2,p3);
}


CVector3Df::CVector3Df(const CVector3Df &p, const CVector3Df &q)
{
  for (int i=0; i<3; i++)
    v[i] = q.v[i] - p.v[i];
}

CVector3Df::~CVector3Df()
{
}

bool CVector3Df::operator==(const CVector3Df& right) const
{
  return (memcmp(v, right.v, 3*sizeof(float)) == 0);
}


CVector3Df& CVector3Df::operator =(const CVector3Df & right)
{
  memcpy(v, right.v, 3*sizeof(float));
  return *this;
}

CVector3Df CVector3Df::operator *(const CVector3Df & right) const
{
  return CVector3Df(v[1]*right.v[2] - v[2]*right.v[1], 
                   v[2]*right.v[0] - v[0]*right.v[2],
		   v[0]*right.v[1] - v[1]*right.v[0]);
}

float CVector3Df::DotProduct(const CVector3Df & w) const
{
  return (v[0]*w.v[0] + v[1]*w.v[1] + v[2]*w.v[2]);
}

float CVector3Df::Modulus() const
{
  return (float)sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}
float CVector3Df::Distance() const
{
  return (float)(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

float CVector3Df::SqrLength() const
{
  return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
}

CVector3Df& CVector3Df::operator +=(const CVector3Df & right)
{
  v[0] += right.v[0];
  v[1] += right.v[1];
  v[2] += right.v[2];
  return *this;
}

CVector3Df& CVector3Df::operator -=(const CVector3Df & right)
{
  v[0] -= right.v[0];
  v[1] -= right.v[1];
  v[2] -= right.v[2];
  return *this;
}

CVector3Df& CVector3Df::operator *=(const CVector3Df & right)
{
  *this = CVector3Df(v[1]*right.v[2] - v[2]*right.v[1], 
                    v[2]*right.v[0] - v[0]*right.v[2],
		    v[0]*right.v[1] - v[1]*right.v[0]);
  return *this;
}

CVector3Df& CVector3Df::operator*=(float s)
{
  v[0] *= s;
  v[1] *= s;
  v[2] *= s;
  return *this;
}

CVector3Df& CVector3Df::operator/=(float s)
{
  v[0] /= s;
  v[1] /= s;
  v[2] /= s;
  return *this;
}


CVector3Df CVector3Df::operator *(float scalar) const
{
  return CVector3Df(v[0]*scalar, v[1]*scalar, v[2]*scalar);
}

CVector3Df CVector3Df::operator +(float scalar) const
{
  return CVector3Df(v[0]+scalar, v[1]+scalar, v[2]+scalar);
}

CVector3Df CVector3Df::operator /(float scalar) const
{
  return CVector3Df(v[0]/scalar, v[1]/scalar, v[2]/scalar);
}

void CVector3Df::Normalize()
{
  float modulus = Modulus();
  if (modulus != 0.0f && modulus != 1.0f)
  {
    v[0] /= modulus;
    v[1] /= modulus;
    v[2] /= modulus;
  }
}

CVector3Df operator+(const CVector3Df& left, const CVector3Df& right)
{
  return CVector3Df(left.v[0]+right.v[0], left.v[1]+right.v[1], left.v[2]+right.v[2]);
}

CVector3Df operator-(const CVector3Df& left, const CVector3Df& right)
{
  return CVector3Df(left.v[0]-right.v[0], left.v[1]-right.v[1], left.v[2]-right.v[2]);
}

void CVector3Df::Scale(float scalar)
{
  v[0] *= scalar;
  v[1] *= scalar;
  v[2] *= scalar;
}

float TriangleArea(CVector3Df & p1, CVector3Df & p2, CVector3Df & p3)
{
  CVector3Df res;
  res = p1*p2+p2*p3+p3*p1;
  return (0.5f*res.Modulus());
}

void CVector3Df::Point2Sphere(int x, int y, int currWidth, int currHeight)
{
  float d, aux;

 // project x,y onto a hemi-sphere centered within width, height
  v[0] = (2.0f*x - currWidth) / currWidth;
  v[1] = (2.0f*y - currHeight) / currHeight;
  aux  = v[0]*v[0] + v[1]*v[1];
  d    = (float)sqrt(aux);
  v[2] = (float)cos((MY_PI*0.5) * ((d < 1.0f) ? d : 1.0f));
  aux  = (float)(1.0 / sqrt(aux + v[2]*v[2]));
  v[0] *= aux;
  v[1] *= aux;
  v[2] *= aux;
}
