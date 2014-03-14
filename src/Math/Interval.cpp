#include "Interval.h"

CInterval::CInterval()
{
  a = b = 0;
}

CInterval::~CInterval()
{

}

CInterval::CInterval(float a, float b)
{
  this->a = a;
  this->b = b;
}

bool CInterval::Intersect(const CInterval &op)
{
  if (b <= op.a) return false;
  if (op.b <= a) return false;
  return true;
}

bool CInterval::Intersect(CInterval *op)
{
  if (b <= op->a) return false;
  if (op->b <= a) return false;

  // maximo izquierdo
  op->a = (a > op->a) ? a : op->a;

  // minimo derecho
  op->b = (b > op->b) ? op->b : b;
  return true;
}

bool CInterval::Intersect(float &ra, float &rb)
{
  if (b <= ra) return false;
  if (rb <= a) return false;

  // maximo izquierdo
  if (a>ra) ra = a;

  // minimo derecho
  if (b<rb) rb = b;
  return true;
}

bool CInterval::IsInsideOf(float ra, float rb)
{
  return ra<=a && b<=rb;
}

bool CInterval::IsInsideOf(const CInterval &op)
{
  return op.a<=a && b<=op.b;
}

bool CInterval::IsInside(float ra, float rb)
{
  return a<=ra && rb<=b;
}

bool CInterval::IsInside(const CInterval &op)
{
  return a<=op.a && op.b<=b;
}

void CInterval::operator =(const CInterval &op)
{
  a = op.a;
  b = op.b;
}


