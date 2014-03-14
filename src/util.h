#ifndef _util_h_
#define _util_h_

#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<algorithm>
#include<QFile>
#include<QFileInfo>

#define FAST_INT_CEIL(a,b) ( (a)/(b) + ( (a) % (b) != 0 ) )

#define CLAMP_RGBA(input, minimum, maximum) \
  if (input.m_r < minimum) input.m_r = minimum; else if (input.m_r > maximum) input.m_r = maximum; \
  if (input.m_g < minimum) input.m_g = minimum; else if (input.m_g > maximum) input.m_g = maximum; \
  if (input.m_b < minimum) input.m_b = minimum; else if (input.m_b > maximum) input.m_b = maximum; \
  if (input.m_a < minimum) input.m_a = minimum; else if (input.m_a > maximum) input.m_a = maximum;

#define TF_BITS 8
#define TF_SIZE (1<<TF_BITS)
#define EMAX_CONTROL_POINTS TF_SIZE

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

#include <QInputDialog>
#include <QFileDialog>

class ReadVolume {
public:
    static bool read(unsigned char **vol, int *w, int *h, int *z, int *bits) {
        QString fileName = QFileDialog::getOpenFileName(NULL, "Open Volume",
                                                         "",
                                                         "Volume (*.pvm *.raw)");

        if(fileName.isEmpty() || fileName.isNull())
            return false;

        QFile f(fileName);
        if(f.open(QIODevice::ReadOnly)){
            QString ext = fileName.mid(fileName.lastIndexOf(".")+1).toLower();
            if(ext=="pvm"){

            }else{
                bool ok;
                *w = QInputDialog::getInt(NULL, "Volumen RAW", "Ancho:", 0, 0, 1024, 1, &ok);
                if(!ok) {
                    f.close();
                    return false;
                }
                *h= QInputDialog::getInt(NULL, "Volumen RAW", "Alto:", 0, 0, 1024, 1, &ok);
                if(!ok) {
                    f.close();
                    return false;
                }
                *z = QInputDialog::getInt(NULL, "Volumen RAW", "Profundidad:", 0, 0, 1024, 1, &ok);
                if(!ok) {
                    f.close();
                    return false;
                }
                *bits = QInputDialog::getInt(NULL, "Volumen RAW", "Bits:", 8, 8, 16, 1, &ok);
                if(!ok) {
                    f.close();
                    return false;
                }
                if(*bits!=8 && *bits!=16) {
                    f.close();
                    return false;
                }
                if((*w)*(*h)*(*z)*((*bits)/8)!=f.size()) {
                    f.close();
                    return false;
                }

                *vol = new unsigned char[f.size()];
                memcpy(*vol, f.readAll().data(), f.size());
                return true;
            }
        }
    }
};

#endif 
