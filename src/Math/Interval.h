#ifndef _INTERVAL_h_
#define _INTERVAL_h_

class CInterval  
{
public:
  float a,b;
	CInterval();
	CInterval(float a, float b);
  bool Intersect(const CInterval &op);
  bool Intersect(CInterval *op);
  bool Intersect(float &ra, float &rb);
  bool IsInsideOf(float ra, float rb);
  bool IsInsideOf(const CInterval &op);
  bool IsInside(float ra, float rb);
  bool IsInside(const CInterval &op);
  void operator = (const CInterval &op);
  float & operator [](int i) { return (i==0)?a:b; };
	virtual ~CInterval();

};

#endif 
