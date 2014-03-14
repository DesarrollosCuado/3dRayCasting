#ifndef __VECTOR3Df_h__
#define __VECTOR3Df_h__

#define MY_PI 3.14159265

class CVector3Df  
{
public:
    void Point2Sphere(int x, int y, int currWidth, int currHeight);
    float v[3];

    CVector3Df();
    CVector3Df(float x, float y, float z);
    CVector3Df(const CVector3Df& source);
    CVector3Df(const CVector3Df& p, const CVector3Df& q);
    // Normal al plano generado por los tres puntos p1,p2,p3
    CVector3Df(const CVector3Df& p1, const CVector3Df& p2, const CVector3Df& p3);
    ~CVector3Df();
    float Modulus() const;
    float Distance() const;
    float SqrLength() const;
    float DotProduct(const CVector3Df& w) const;
    CVector3Df  operator+(float scalar) const;
    CVector3Df  operator*(float scalar) const;
    CVector3Df  operator/(float scalar) const;
    CVector3Df  operator*(const CVector3Df& right) const;
    CVector3Df& operator=(const CVector3Df& right);
    CVector3Df& operator+=(const CVector3Df& right);
    CVector3Df& operator-=(const CVector3Df& right);
    CVector3Df& operator*=(const CVector3Df& right);
    CVector3Df& operator*=(float s);
    CVector3Df& operator/=(float s);
    friend CVector3Df operator+(const CVector3Df& left, const CVector3Df& right);
    friend CVector3Df operator-(const CVector3Df& left, const CVector3Df& right);
    bool operator==(const CVector3Df& right) const;
    void Normalize();
    inline float & operator [](int i) { return v[i]; };
    inline float   operator [](int i) const { return v[i]; };
    void Scale(float scalar);
    friend float TriangleArea(CVector3Df & p1, CVector3Df & p2, CVector3Df & p3);
};

#endif 
