

#include "Matrix4x4f.h"
#include <memory.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

CMatrix4x4f::CMatrix4x4f()
{
  SetIdentity();
}

CMatrix4x4f::~CMatrix4x4f()
{
}

CMatrix4x4f::CMatrix4x4f(const float m_vector[16])
{
  memcpy(this->m_vector, m_vector, sizeof(float)*16);
}

CMatrix4x4f::CMatrix4x4f(const CMatrix4x4f &rightMatrix)
{
  memcpy(m_vector, rightMatrix.m_vector, sizeof(float)*16);
}

void CMatrix4x4f::SetIdentity()
{
  memset(m_vector, 0x00, 16*sizeof(float));
  m_vector[0] = m_vector[5] = m_vector[10] = m_vector[15]= 1.0f;
}

void CMatrix4x4f::SetMatrix(const float m_vector[16])
{
  memcpy(this->m_vector, m_vector, sizeof(float)*16);
}

void CMatrix4x4f::SetTranslate(float tX, float tY, float tZ)
{
  SetIdentity();
  m_vector[3] = tX;
  m_vector[7] = tY;
  m_vector[11]= tZ;
}

void CMatrix4x4f::SetScale(float s)
{
  memset(m_vector, 0x00, 16*sizeof(float));
  m_vector[0] = m_vector[5] = m_vector[10] = s;
  m_vector[15]= 1.0f;
}

void CMatrix4x4f::SetScale(float sX, float sY, float sZ)
{
  memset(m_vector, 0x00, 16*sizeof(float));
  m_vector[0] = sX;
  m_vector[5] = sY;
  m_vector[10]= sZ;
  m_vector[15]= 1.0f;
}

void CMatrix4x4f::SetRotate(float angle, float x, float y, float z)
{
  float u[3];
  float norma = x*x+y*y+z*z;
  if (norma != 0.0f)
  {
    norma = (float)(1.0/sqrt(norma));
    u[0] = x*norma;
    u[1] = y*norma;
    u[2] = z*norma;
  }
  else
    u[0] = u[1] = u[2] = 0.0f;
  float seno   = (float)sin(angle);
  float coseno = (float)cos(angle);

  float aux;
  float sen_x_u[3];
  sen_x_u[0] = seno*u[0];
  sen_x_u[1] = seno*u[1];
  sen_x_u[2] = seno*u[2];

  aux = u[0]*u[0];  m_vector[0] = aux + coseno * (1.0f - aux);    
  aux = u[0]*u[1];  m_vector[1] = aux - coseno * aux - sen_x_u[2]; 
  aux = u[0]*u[2];  m_vector[2] = aux - coseno * aux + sen_x_u[1]; 
  
  aux = u[1]*u[0];  m_vector[4] = aux - coseno * aux + sen_x_u[2];    
  aux = u[1]*u[1];  m_vector[5] = aux + coseno * (1.0f - aux);       
  aux = u[1]*u[2];  m_vector[6] = aux - coseno * aux - sen_x_u[0];     
  
  aux = u[2]*u[0];  m_vector[8] = aux - coseno * aux - sen_x_u[1];    
  aux = u[2]*u[1];  m_vector[9] = aux - coseno * aux + sen_x_u[0];    
  aux = u[2]*u[2];  m_vector[10]= aux + coseno * (1.0f - aux);

  m_vector[3] = m_vector[7] = m_vector[11] = m_vector[12]= m_vector[13] = m_vector[14] = 0.0f;
  m_vector[15] = 1.0f;
}

void CMatrix4x4f::SetRotate(float angle, char axis)
{
  float s = (float)sin(angle), c = (float)cos(angle);
  SetIdentity();
  switch (axis)
  {
    case 'X':
    case 'x': m_vector[5] = m_vector[10] = c; m_vector[6] = -s; m_vector[9] = s; break;
    case 'Y':
    case 'y': m_vector[0] = m_vector[10]= c; m_vector[2] = -s; m_vector[8] = s; break;
    case 'Z':
    case 'z': m_vector[0] = m_vector[5] = c; m_vector[1] = -s; m_vector[4] = s; break;
    default: printf("Invalid axis in CMatrix4x4f::SetRotate\n"); exit(1);
  }
}

void CMatrix4x4f::SetPerspective(float nearClippingPlane)
{
  SetIdentity();
  m_vector[0] = m_vector[5] = m_vector[10] = -nearClippingPlane;
  m_vector[15] = 0;
  m_vector[14] = -1;
}

void CMatrix4x4f::SetShearXY(float sx, float sy)
{
  SetIdentity(); 
  m_vector[2] = sx;
  m_vector[6] = sy;
}

void CMatrix4x4f::SetShearXZ(float sx, float sz)
{
  SetIdentity(); 
  m_vector[1] = sx;
  m_vector[9] = sz;
}

void CMatrix4x4f::SetShearYZ(float sy, float sz)
{
  SetIdentity(); 
  m_vector[4] = sy;
  m_vector[8] = sz;
}

void CMatrix4x4f::PreMult(const CMatrix4x4f & leftMatrix)
{
  CMatrix4x4f res;
  for (int i=0; i<4; i++)
    for (int j=0; j<4; j++)
    {
      res[i][j] = 0;
      for (int k=0; k<4; k++)
        res[i][j] += m_vector[(k<<2)+j]*leftMatrix[i][k];
    }
  *this = res;
}

void CMatrix4x4f::PostMult(const CMatrix4x4f & rightMatrix)
{
  *this *= rightMatrix;
}

void CMatrix4x4f::Translate(float tX, float tY, float tZ)
{
  m_vector[3] = m_vector[0] * tX + m_vector[1] * tY + m_vector[2] * tZ + m_vector[3];
  m_vector[7] = m_vector[4] * tX + m_vector[5] * tY + m_vector[6] * tZ + m_vector[7];
  m_vector[11]= m_vector[8] * tX + m_vector[9] * tY + m_vector[10]* tZ + m_vector[11];
  m_vector[15]= m_vector[12]* tX + m_vector[13]* tY + m_vector[14]* tZ + m_vector[15];
}

void CMatrix4x4f::Scale(float sX, float sY, float sZ)
{
  for (int i=0; i<=12; i+=4)
  {
    m_vector[i+0] *= sX; // Colum[0]*sX
    m_vector[i+1] *= sY; // Colum[1]*sY
    m_vector[i+2] *= sZ; // Colum[2]*sZ
  }
}

void CMatrix4x4f::Rotate(float angle, char axis)
{
  float s = (float)sin(angle), c = (float)cos(angle), aux;
  int w;
  switch (axis)
  {
    case 'X':
    case 'x': for (w=1; w<16; w+=4)
              {
                aux = m_vector[w];
                m_vector[w]   =  aux*c + m_vector[w+1]*s; 
                m_vector[w+1] = -aux*s + m_vector[w+1]*c; 
              }
              break;
    case 'Y':
    case 'y': for (w=0; w<16; w+=4)
              {
                aux = m_vector[w];
                m_vector[w]   =  aux*c + m_vector[w+2]*s; 
                m_vector[w+2] = -aux*s + m_vector[w+2]*c; 
              }
              break;
    case 'Z':
    case 'z': for (w=0; w<16; w+=4)
              {
                aux = m_vector[w];
                m_vector[w]   =  aux*c + m_vector[w+1]*s; 
                m_vector[w+1] = -aux*s + m_vector[w+1]*c; 
              }
              break;
    default: printf("Invalid axis in CMatrix4x4f::SetRotate\n"); exit(1);
  }
}

void CMatrix4x4f::shearXY(float sx, float sy)
{
  // 1  0  sx 0
  // 0  1  sy 0
  // 0  0  1  0
  // 0  0  0  1
  for (int i=2; i<16; i+=4)
    m_vector[i] += sx*m_vector[i-2]+sy*m_vector[i-1];
}

void CMatrix4x4f::shearXZ(float sx, float sz)
{
  // 1  sx 0  0
  // 0  1  0  0
  // 0  sz 1  0
  // 0  0  0  1
  for (int i=1; i<16; i+=4)
    m_vector[i] += sx*m_vector[i-1]+sz*m_vector[i+1];
}

void CMatrix4x4f::shearYZ(float sy, float sz)
{
  // 1  0  0  0
  // sy 1  0  0
  // sz 0  1  0
  // 0  0  0  1
  for (int i=0; i<16; i+=4)
    m_vector[i] += sy*m_vector[i+1]+sz*m_vector[i+2];
}

CMatrix4x4f& CMatrix4x4f::Transpose()
{
  for (int i=0; i<4; i++)
    for (int j=i+1; j<4; j++)
    {
      float aux = m_vector[(i<<2)+j];
      m_vector[(i<<2)+j] = m_vector[(j<<2)+i];
      m_vector[(j<<2)+i] = aux;
    }
  return *this;
}

CMatrix4x4f& CMatrix4x4f::FullInvert()
{
  float t[3][3];
  float m[4][4];
  float det;
  int nx,ny,sx,sy,tx,ty;

  for(ny=0; ny<4; ny++)
    for(nx=0; nx<4; nx++)
    {
      //make 3x3
      ty=0;
      for(sy=0; sy<4; sy++)
      {
        tx=0;
        for(sx=0;sx<4;sx++)
        {
          if( sx!=nx && sy!=ny)
          {
            t[ty][tx]=m_vector[(sy<<2)+sx];
            tx++;
          }
        }
        if(sy!=ny)
          ty++;
      }//
      m[ny][nx] = getDet3(t);
    }
  int i;
  det=0;
  for (i=0; i<4; i++)
    det +=m_vector[(0<<2)+i]*m[0][i];

  //printf("determinante: %f %f\n",det);

  for (i=0; i<4; i++)
  {
    m_vector[(i<<2)+0] =  m[0][i];
    m_vector[(i<<2)+1] = -m[1][i];
    m_vector[(i<<2)+2] =  m[2][i];
    m_vector[(i<<2)+3] = -m[3][i];
  }

  if(det==0.0) 
    det=0.000001f;

  det=1.0f/det;

  for (i=0; i<4; i++)
  {
    m_vector[(0<<2)+i] *= det;
    m_vector[(1<<2)+i] *= det;
    m_vector[(2<<2)+i] *= det;
    m_vector[(3<<2)+i] *= det;
  }
  return *this;
}

void CMatrix4x4f::operator +=(const CMatrix4x4f & rightMatrix)
{
  for (int i=0; i<16; i++) m_vector[i] += rightMatrix.m_vector[i];
}

void CMatrix4x4f::operator -=(const CMatrix4x4f & rightMatrix)
{
  for (int i=0; i<16; i++) m_vector[i] -= rightMatrix.m_vector[i];
}

void CMatrix4x4f::operator *=(const CMatrix4x4f & rightMatrix)
{
  CMatrix4x4f res;
  for (int i=0; i<4; i++)
    for (int j=0; j<4; j++)
    {
      res[i][j] = 0;
      for (int k=0; k<4; k++)
        res[i][j] += (*this)[i][k]*rightMatrix[k][j];
    }
  *this = res;
}

void CMatrix4x4f::operator /= (float c)
{
  c = 1/c;
  for (int i=0; i<16; i++) m_vector[i] *= c;
}

CMatrix4x4f& CMatrix4x4f::operator =(const CMatrix4x4f & rightMatrix)
{
  memcpy(m_vector, rightMatrix.m_vector, sizeof(float)*16);
  return *this;
}


void CMatrix4x4f::Print()
{
  for (int i=0; i<4; i++)
  {
    for (int j=0; j<4; j++)
      printf("%4.4f ", m_vector[i*4+j]);
    printf("\n");
  }
  printf("\n");
}

CMatrix4x4f operator + (const CMatrix4x4f &A, const CMatrix4x4f &B)
{
  CMatrix4x4f C;
  for (int i=0; i<16; i++) C.m_vector[i] = A.m_vector[i] + B.m_vector[i];
  return C;
}

CMatrix4x4f operator - (const CMatrix4x4f &A, const CMatrix4x4f &B)
{
  CMatrix4x4f C;
  for (int i=0; i<16; i++) C.m_vector[i] = A.m_vector[i] - B.m_vector[i];
  return C;
}

CVector4Df  operator * (const CVector4Df& v,  const CMatrix4x4f& A)
{
  CVector4Df res;
  for (int j=0; j<4; j++)
  {
    res[0] = 0.0f;
    for (int i=0; i<4; i++)
      res[j] += v[i]*A.m_vector[(i<<2)+j];
  }
  return res;
}

CMatrix4x4f operator * (float c, const CMatrix4x4f &A)
{
  CMatrix4x4f res;
  for (int i=0; i<16; i++) res.m_vector[i]=c*A.m_vector[i];
  return res;
}

float* CMatrix4x4f::operator [](int i)
{
  return &m_vector[i<<2];
}

const float * CMatrix4x4f::operator [](int i) const
{
  return &m_vector[i<<2];
}

void CMatrix4x4f::operator *=(float v)
{
  for (register int i=0; i<16; i++) m_vector[i] *= v;
}

CMatrix4x4f operator * (const CMatrix4x4f &A, const CMatrix4x4f &B)
{
  CMatrix4x4f C;
  int i,j,k;
  for (i=0; i<4; i++)
    for (j=0; j<4; j++)
    {
      C[i][j] = 0.0f;
      for (k=0; k<4; k++) C[i][j] += A[i][k]*B[k][j];
    }
  return C;
}

CMatrix4x4f operator * (const CMatrix4x4f& A, float c)
{
  CMatrix4x4f B;
  for (int i=0; i<16; i++) B.m_vector[i] = A.m_vector[i] * c;
  return B;
}

CVector4Df  operator * (const CMatrix4x4f &A, const CVector4Df &v)
{
  CVector4Df res;
  int fila,i,j;
  for (i=0; i<4; i++)
  {
    fila = 4*i;
    res[i] = 0.0f;
    for (j=0; j<4; j++)
      res[i] += A.m_vector[fila+j]*v[j];
  }
  return res;
}

CVector3Df  operator * (const CMatrix4x4f &A, const CVector3Df &v)
{
  CVector4Df res;
  int fila,i,j;
  for (i=0; i<4; i++)
  {
    fila = 4*i;
    res[i] = 0.0f;
    for (j=0; j<3; j++)
      res[i] += A.m_vector[fila+j]*v[j];
    res[i] += A.m_vector[fila+j];
  }
  res.Standarize();
  return CVector3Df(res[0], res[1], res[2]);
}

CMatrix4x4f operator / (const CMatrix4x4f &A, float c)
{
  CMatrix4x4f res;
  for (int i=0; i<16; i++) res.m_vector[i]=A.m_vector[i]/c;
  return res;
}

bool operator ==(const CMatrix4x4f &A, const CMatrix4x4f &B)
{
  return memcmp(A.m_vector, B.m_vector, sizeof(float)*16) == 0;
}

bool operator !=(const CMatrix4x4f &A, const CMatrix4x4f &B)
{
  return memcmp(A.m_vector, B.m_vector, sizeof(float)*16) == 0;
}

float CMatrix4x4f::getDet3(float t[3][3])
{
  float det=0.0f;

  det =t[0][0]*t[1][1]*t[2][2];
  det+=t[0][1]*t[1][2]*t[2][0];
  det+=t[0][2]*t[1][0]*t[2][1];
  det-=t[0][2]*t[1][1]*t[2][0];
  det-=t[0][1]*t[1][0]*t[2][2];
  det-=t[0][0]*t[1][2]*t[2][1];
  return det;
}

CMatrix4x4f& CMatrix4x4f::PseudoInverse()
{
  int i,j,k,c,f,w;
  float div, acum;
  bool NullVector = true;

  float P[4];
  float VC[4];
  float m[4][4],SI[4][4];

  for (i=0;i<4;i++)
    for (j=0; j<4; j++)
      m[i][j] = m_vector[(i<<2)+j];

  //calculo de peseudoinversa(m)
  for (i=0; i<4; i++)
    if (m[i][0] != 0.0f) 
    {
      NullVector = false;
      break;
    }

  if (NullVector)
    for (j=0; j<4; j++) SI[0][j] = 0.0f;
  else
  {
    div = 0.0f;
    for (i=0; i<4; i++) div += (m[i][0]*m[i][0]);
    div = 1.0f/div;
    for (j=0; j<4; j++) SI[0][j] = m[j][0]*div;
  }

  for (k=1; k<4; k++)   // for all the columns
  {
    //calculo PkT 
    NullVector = true;
    for (f=0; f<4; f++)
    {
      P[f] = 0.0;
      for (c=0; c<4; c++)
      {
        acum = 0.0;
        for (w=0; w<4; w++) acum += m[f][w] * SI[w][c];
        acum = (f == c) ? 1.0f-acum : -acum;
        P[f] += acum * m[c][k];
      }
      if (fabs(P[f]) > 0.5e-5) NullVector = false;
    }

    if (!NullVector)
    {
      acum = 0.0f;
      for (i=0; i<4; i++) acum += P[i]*P[i];
      acum = 1.0f/acum;
      for (i=0; i<4; i++) P[i] *= acum;
    }
    else
    {
      float normaVC=0;
      for (f=0; f<k; f++)
      {
        acum = 0.0f;
        for (c=0; c<4; c++) acum += SI[f][c] * m[c][k];
        VC[f] = acum;
        normaVC += VC[f]*VC[f];
      }
      div = 1.0f/(1.0f+normaVC);
      for (f=0; f<4; f++)
      {
        acum = 0.0f;
        for (c=0; c<k; c++)  acum+= SI[c][f]*VC[c];
        P[f] = acum*div;
      }
    }
    // Pk listo --- hasta aqui verified 
    for (f=0; f<k; f++)
    {
      memcpy(VC, SI[f], 4*sizeof(float));
      for (c=0; c<4; c++)
      {
        acum = 0.0f;
        for (w=0; w<4; w++)
          acum += (c==w) ? VC[w]*(1.0f-m[w][k]*P[c]) : -VC[w]*m[w][k]*P[c];
        SI[f][c] = acum;
      }
    }
    memcpy(SI[k],P,4*sizeof(float));
  }
  memcpy(m_vector,SI,sizeof(float)*16);
  return *this;
}


void CMatrix4x4f::Rotate(float angle, const CVector4Df &axis)
{
  CMatrix4x4f aux;
  aux.SetRotate(angle, axis[0], axis[1], axis[2]);
  PostMult(aux);
}

void CMatrix4x4f::Rotate(float angle, float x, float y, float z)
{
  CMatrix4x4f aux;
  aux.SetRotate(angle, x, y, z);
  PostMult(aux);
}

void CMatrix4x4f::SetPerspective(float fovY, float N, float F, float viewportWidth, float viewportHeight)
{
  float cotangent = 1.0f/tan(0.5f * fovY);
  float aspect = viewportWidth/float(viewportHeight);
  m_vector[0] = cotangent/aspect;
  m_vector[1] = 0;
  m_vector[2] = 0;
  m_vector[3] = 0;
  m_vector[4] = 0;
  m_vector[5] = cotangent;
  m_vector[6] = 0;
  m_vector[7] = 0;
  m_vector[8] = 0;
  m_vector[9] = 0;
  m_vector[10]= -(F+N)/(F-N);
  m_vector[11]= -2*F*N/(F-N);
  m_vector[12]= 0;
  m_vector[13]= 0;
  m_vector[14]= -1;
  m_vector[15]= 0;
}

void CMatrix4x4f::SetPerspective(float L, float R, float B, float T, float N, float F)
{
  m_vector[0] = 2*N/(R-L);  
  m_vector[1] = 0;
  m_vector[2] = (R+L)/(R-L);
  m_vector[3] = 0;
  m_vector[4] = 0;
  m_vector[5] = 2*N/(T-B);
  m_vector[6] = (T+B)/(T-B);
  m_vector[7] = 0;
  m_vector[8] = 0;
  m_vector[9] = 0;
  m_vector[10]= -(F+N)/(F-N);
  m_vector[11]= -2*F*N/(F-N);
  m_vector[12]= 0;
  m_vector[13]= 0;
  m_vector[14]= -1;
  m_vector[15]= 0;
}



