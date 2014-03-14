#ifndef _TRANSFER_FUNCTION1_h_
#define _TRANSFER_FUNCTION1_h_

#include "util.h"

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
