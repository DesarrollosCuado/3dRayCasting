#pragma once
#ifndef _TRANSFER_FUNCTION1_h_
#define _TRANSFER_FUNCTION1_h_

#ifdef __unix__
    #include <GL/glext.h>
#endif
#include <GL/glu.h>

#include <QtGui>
#include <QtOpenGL>

#include "glextensions.h"
#include "glshaders.h"
#include "util.h"
#include <vector>

#define TF_BITS 8
#define TF_SIZE (1<<TF_BITS)
#define EMAX_CONTROL_POINTS TF_SIZE
#define GRvoxel unsigned char

using namespace std;

typedef struct CTFNode
{
  union
  {
    struct
    {
      float m_r, m_g, m_b, m_y, m_x;
    };
    float m_values[5];
  };
  inline float & operator [](int i)
  {
    return m_values[i];
  }
} ENode;

class CTransferFunction
{
public:
  CTransferFunction(void);
  CTransferFunction(CTFNode *pNodes, int nNodes);
  ~CTransferFunction(void);

  void Set(CTFNode *pNodes, int nNodes);
  void GetPalette(RGBAf *palette);
  void GetPostClassificationMap(RGBAf *mapa);

  RGBAf *GetPalettePointer();

private:
  CTFNode *m_nodes;
  int   m_nNodes;
};


#endif
