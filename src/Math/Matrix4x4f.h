#ifndef CMatrix4x4f_h
#define CMatrix4x4f_h

#include "Vector3Df.h"
#include "Vector4Df.h"

class CMatrix4x4f
{
public: // attributes

  float m_vector[16];

public: // methods
	void Rotate(float angle, const CVector4Df &axis);
	void Rotate(float angle, float x, float y, float z);

  // default constructor. The the Identity matrix
  CMatrix4x4f();

  // constructor with all the values by rows [f0][f1][f2][f3]
  CMatrix4x4f(const float m_vector[16]);

  // copy constructor
  CMatrix4x4f(const CMatrix4x4f &rightMatrix);

  // destructor
 ~CMatrix4x4f();

  // common settings. These functions replace the current matrix by another matrix
  void SetIdentity();
  void SetMatrix(const float m_vector[16]);
  void SetTranslate(float tX, float tY, float tZ);
	void SetScale(float s);
  void SetScale(float sX, float sY, float sZ);
	void SetRotate(float angle, float x, float y, float z);
  void SetRotate(float angle, char axis);
	void SetPerspective(float nearClippingPlane); // ..............revisar..................
	void SetPerspective(float L, float R, float B, float T, float N, float F); 
  void SetPerspective(float fovY, float N, float F, float viewportWidth, float viewportHeight);
  void SetShearXY(float sx, float sy);
  void SetShearXZ(float sx, float sz);
  void SetShearYZ(float sy, float sz);

  // mutiplies the current matrix by other matrix
  void PreMult (const CMatrix4x4f & leftMatrix);
  void PostMult(const CMatrix4x4f & rightMatrix);

  // traslates. It post-mutiplies the current matrix by a traslation matrix T(tX,tY,tZ)
  void Translate(float tX, float tY, float tZ);

  // post-multiplies the current matrix by the scale matrix S(sX,sY,sZ)
  void Scale(float sX, float sY, float sZ);

  // post-mutiplies the matrix by the standar rotate matrix around 'x', 'y',  or 'z'
  void Rotate(float angle, char axis);

  // post-multiplie the matrix by shear matrix
  void shearXY   (float sx, float sy);
  void shearXZ   (float sx, float sz);
  void shearYZ   (float sy, float sz);

  // Transpose the matrix, i.e. the new matrix[i][j] = the old matrix[j][i]
  CMatrix4x4f& Transpose();

  // Invert the current matrix, and return this inverse matrix
  CMatrix4x4f& FullInvert();

  // operators overload
  void operator += (const CMatrix4x4f & rightMatrix);
  void operator -= (const CMatrix4x4f & rightMatrix);
  void operator *= (const CMatrix4x4f & rightMatrix);
  void operator *= (float v);
  void operator /= (float c);
  CMatrix4x4f& operator = (const CMatrix4x4f &A);
  float * operator [](int i);
  const float * operator [](int i) const;

  // print the matrix to standar output
  void Print();

  // friend functions
  friend CMatrix4x4f operator + (const CMatrix4x4f &A, const CMatrix4x4f &B);     
  friend CMatrix4x4f operator - (const CMatrix4x4f &A, const CMatrix4x4f &B);     
  friend CMatrix4x4f operator * (const CMatrix4x4f &A, const CMatrix4x4f &B);     
  friend CMatrix4x4f operator * (float c, const CMatrix4x4f &A);
  friend CMatrix4x4f operator * (const CMatrix4x4f &A, float c);
  friend CVector4Df  operator * (const CMatrix4x4f &A, const CVector4Df &v);
  friend CVector3Df  operator * (const CMatrix4x4f &A, const CVector3Df &v);
  friend CVector4Df  operator * (const CVector4Df  &v, const CMatrix4x4f &A);       
  friend CMatrix4x4f operator / (const CMatrix4x4f &A, float c);
  friend bool operator ==(const CMatrix4x4f &A, const CMatrix4x4f &B);
  friend bool operator !=(const CMatrix4x4f &A, const CMatrix4x4f &B);

  float  getDet3(float t[3][3]);

  // calculates the PseudoInverse of the matrix, and returns it
  CMatrix4x4f& PseudoInverse();
};

#endif