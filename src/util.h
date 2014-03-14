#pragma once
#ifndef _util_h_
#define _util_h_

#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<algorithm>
#include <QtOpenGL>
#include "Math/Matrix4x4f.h"
#include "Math/Vector3Df.h"
#include "Math/Vector4Df.h"

#define FAST_INT_CEIL(a,b) ( (a)/(b) + ( (a) % (b) != 0 ) )

using namespace std;
template <class T>
class RGBAt
{
public:
  union
  {
    struct
    {
      T m_r, m_g, m_b, m_a;
    };
    T m_rgba[4];
  };

  RGBAt() {};
  RGBAt(T r, T g, T b, T a)
  {
    m_r = r;
    m_g = g;
    m_b = b;
    m_a = a;
  }

  RGBAt<T> & operator =(const RGBAt<T> &right)
  {
    m_r = right.m_r;
    m_g = right.m_g;
    m_b = right.m_b;
    m_a = right.m_a;
    return *this;
  }

  friend RGBAt<T> operator -(const RGBAt<T> &left, const RGBAt<T> &right)
  {
    return RGBAt<T>(right.m_r-left.m_r, right.m_g-left.m_g, right.m_b-left.m_b, right.m_a-left.m_a);
  }

  inline float Modulus() const
  {
    return sqrt(float(m_r*m_r +  m_g*m_g + m_b*m_b));
  }

  T & operator [] (int i)
  {
    return m_rgba[i];
  }
};

#define RGBA   RGBAt<unsigned char>
#define RGBAus RGBAt<unsigned short>
#define RGBAui RGBAt<unsigned int>
#define RGBAf  RGBAt<float>
#define RGBAd  RGBAt<double>
template class RGBAt<double>;
template class RGBAt<unsigned short>;
template class RGBAt<unsigned int>;
template class RGBAt<unsigned char>;
template class RGBAt<float>;

enum
{
  SCA_DEFAULT,
  SCA_GAINING,
  SCA_OPTIMAL
};

class CUtil  
{
public:
  static int  m_scaMethod;
  static void DrawBox(float x0, float y0, float z0, float x1, float y1, float z1);
  static char * SkipWord(char *str, char *theword);
  static char * RemoveEnter(char *str);
  static void Abort(int ret, char *fmt, ...);
  static int  NextPowerOf2(int value);
  static bool IsPowerOf2(int value);
  static int  IsExtensionSupported(const char *extension);
  static char *TextFileRead(char *fn);

  static void ***Alloc3D(int z, int y, int x, int elemSize);
  static void Free3D(void ***V, int z);
  static long FileSize(FILE *f);
  static void Abort(int code);
  static void Vsync(bool enable);
  static void **Alloc2DCont(int rows, int cols, int elemSize);
  static void Free2DCont(void **data);
  static void Swap(float &a, float &b);
  static void Swap(int   &a, int   &b);
  static void Swap(unsigned char &a, unsigned char &b);
  static void SendMatrixToOpenGL(CMatrix4x4f mat);
  static int  SaveBMP(const char *filename, int nCols, int nRows, unsigned char *buffer);
  static void AddLine(char *filename, char *fmt, ... );
  static void DrawSolidCube(float x0, float y0, float z0, float x1, float y1, float z1);
  static void DrawBackFacedCube(float x0, float y0, float z0, float x1, float y1, float z1, GLenum renderType=GL_LINE);
 static float MinDistance(const CVector4Df &point, const CVector3Df &boxMin, const CVector3Df &boxMax);
 static float MinDistanceEucli(const CVector4Df &point, const CVector4Df &boxMin, const CVector4Df &boxMax);
 static float MinDistance(const CVector4Df &point, const CVector4Df &boxMin, const CVector4Df &boxMax);
 static float MinSqrDistance(const CVector4Df &point, const CVector3Df &boxMin, const CVector3Df &boxMax);
 static float MaxDistance(const CVector4Df &point, const CVector3Df &boxMin, const CVector3Df &boxMax);
  static void RGB2LUV(const CVector3Df &rgb, CVector3Df &luv);
  static float rand01();
  CUtil();
  virtual ~CUtil();

};

#endif 
