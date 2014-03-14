#ifndef _INT_POINT3D_h
#define _INT_POINT3D_h

class CIntPoint3D  
{
public:
	CIntPoint3D();
  CIntPoint3D(const CIntPoint3D &source);
  CIntPoint3D(int x, int y, int z);
  virtual ~CIntPoint3D() {};
  inline int& operator[](int i) { return values[i]; }
  inline int operator[](int i) const { return values[i]; }
  CIntPoint3D operator +(const CIntPoint3D &s);
  CIntPoint3D operator *(int s);
  CIntPoint3D operator /(int s);
  bool operator == (const CIntPoint3D &source) const;
  CIntPoint3D operator -(const CIntPoint3D &right) const;
  float Modulus();
  int values[3];
};

#endif 
