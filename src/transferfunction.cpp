#include "transferfunction.h"

CTransferFunction::CTransferFunction(void)
{
  m_nNodes = 2;
  m_nodes  = new CTFNode[EMAX_CONTROL_POINTS];
  m_nodes[0].m_x = 0;
  m_nodes[0].m_y = 0;
  m_nodes[0].m_r = 0;
  m_nodes[0].m_g = 0;
  m_nodes[0].m_b = 0;
  m_nodes[2].m_x = 1;
  m_nodes[2].m_y = 1;
  m_nodes[2].m_r = 1;
  m_nodes[2].m_g = 1;
  m_nodes[2].m_b = 1;
}


CTransferFunction ::CTransferFunction(CTFNode *pNodes, int nNodes)
{
  m_nodes = NULL;
  Set(pNodes, nNodes);
}


CTransferFunction ::~CTransferFunction(void)
{
  if (m_nodes)
    delete [] m_nodes;
}


void CTransferFunction ::Set(CTFNode *pNodes, int nNodes)
{
  if (nNodes>0)
  {
    if (!m_nodes)
      m_nodes = new CTFNode[EMAX_CONTROL_POINTS];
    memcpy(m_nodes, pNodes, sizeof(CTFNode)*nNodes);
    m_nNodes = nNodes;
  }
  else
    m_nNodes = 0;
}

void CTransferFunction ::GetPalette(RGBAf *palette)
{

  RGBAf *p = &palette[0];

  int i;
  for (i=0; i<m_nNodes-1; i++)
  {
    // computing rect between point i and i+1
    // first, checking if they are colinear in x
    float x0 = m_nodes[i].m_x, x1 = m_nodes[i+1].m_x;
    float y0 = m_nodes[i].m_y, y1 = m_nodes[i+1].m_y;
    if (x0 == x1) 
      continue; // skip

    // alpha component
    float m = (y1 - y0)/(x1 - x0);
    float b = -m * x0 + y0;
    float x,incX = 1.0f/(TF_SIZE-1.0f);
    int start = int(x0*(TF_SIZE-1.0f) + 0.5f);
    int end   = int(x1*(TF_SIZE-1.0f) + 0.5f);
    int pos;
    for (x=x0, pos=start; pos<=end; pos++, x+=incX)
      p[pos][3] = m * x + b;

    for (int j=0; j<3; j++) // r,g and b
    {
      float c0 = m_nodes[i][j];
      float c1 = m_nodes[i+1][j];

      if (c0 == c1)
      {
        // c constant
        for (pos=start; pos<=end; pos++)
          p[pos][j] = c0;
      }
      else
      {
        float t = 0.0f;
        float diff = c1-c0;
        float dt= diff / (end-start);
        for (pos=start; pos<=end; pos++,t+=dt)
          p[pos][j] = (c0 + t);
      }
    }
  }
}

void CTransferFunction ::GetPostClassificationMap(RGBAf *mapa)
{
  float deltaTF = 1.0f/(TF_SIZE-1.0f);
  RGBAf b[TF_SIZE];
  RGBAf m[TF_SIZE];

  for (int i=0; i<m_nNodes-1; i++)
  {
    float x0 = m_nodes[i].m_x, x1 = m_nodes[i+1].m_x;
    if (x0 == x1) 
      continue; // skip
    int start = int(x0 * (TF_SIZE-1.0f) + 0.5f);
    int stop  = int(x1 * (TF_SIZE-1.0f) + 0.5f);

    for (int curve=0; curve<4; curve++)
    {
      float y0= m_nodes[i][curve],   y1= m_nodes[i+1][curve];
      m[start][curve] = (y1 - y0)/(x1 - x0);
      b[start][curve] =  -m[start][curve] * x0 + y0;
    }
    for (int j=start+1; j<=stop; j++)
    {
      m[j] = m[start];
      b[j] = b[start];
    }
  }
  float s = 0;
  for (int i=0; i<TF_SIZE; i++, s+=deltaTF) 
  {
    mapa[i][0] = m[i][0]*s + b[i][0];
    mapa[i][1] = m[i][1]*s + b[i][1];
    mapa[i][2] = m[i][2]*s + b[i][2];
    mapa[i][3] = m[i][3]*s + b[i][3];//exp(-(m[i][3]*s + b[i][3]));
    CLAMP_RGBA(mapa[i], 0.0f, 1.0f);
 }
}

