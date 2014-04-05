#ifndef _util_h_
#define _util_h_

#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<algorithm>
#include<QFile>
#include<QFileInfo>
#include<QDebug>

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

#define DDS_MAXSTR (256)
#define DDS_BLOCKSIZE (1<<20)
#define DDS_INTERLEAVE (1<<24)
#define DDS_RL (7)

class ReadVolume {
private:
    static unsigned int DDS_buffer;
    static int DDS_bufsize;
    static int DDS_bitcnt;
    static int DDS_pos;
    static int DDS_data_size;
    static unsigned char *DDS_data;

    static void DDS_init_buffer() {
        ReadVolume::DDS_buffer = 0;
        ReadVolume::DDS_bufsize = 0;
        ReadVolume::DDS_bitcnt = 0;
        ReadVolume::DDS_pos = 0;
        ReadVolume::DDS_data_size = 0;
        ReadVolume::DDS_data = NULL;
    }

    static size_t DDS_read() {
        ReadVolume::DDS_buffer = 0;
        size_t s = (size_t)std::min(4, ReadVolume::DDS_data_size - ReadVolume::DDS_pos - 1);
        memcpy(&ReadVolume::DDS_buffer, &ReadVolume::DDS_data[DDS_pos], s);
        ReadVolume::DDS_pos += s;
        return s;
    }

    static unsigned int DDS_shiftl(const unsigned int value, const int bits) {
        return ((bits >= 32) ? 0 : value << bits);
    }

    static unsigned int DDS_shiftr(const unsigned int value, const int bits) {
        return ((bits >= 32) ? 0 : value >> bits);
    }

    static int DDS_decode(int bits) {
        return (bits >= 1 ? bits + 1 : bits);
    }

    // deinterleave a byte stream
    static void deinterleave(unsigned char *data, unsigned int bytes, unsigned int skip, unsigned int block = 0, bool restore = false) {
        unsigned int i, j, k;

        unsigned char *data2, *ptr;

        if (skip <= 1)
            return ;

        if (block == 0) {
            if ((data2 = (unsigned char *)malloc(bytes)) == NULL)
                return;

            if (!restore)
                for (ptr = data2, i = 0; i < skip; i++)
                    for (j = i; j < bytes; j += skip)
                        * ptr++ = data[j];
            else
                for (ptr = data, i = 0; i < skip; i++)
                    for (j = i; j < bytes; j += skip)
                        data2[j] = *ptr++;

            memcpy(data, data2, bytes);
        } else {
            if ((data2 = (unsigned char *)malloc((bytes < skip * block) ? bytes : skip * block)) == NULL)
                return;

            if (!restore) {
                for (k = 0; k < bytes / skip / block; k++) {
                    for (ptr = data2, i = 0; i < skip; i++)
                        for (j = i; j < skip*block; j += skip)
                            * ptr++ = data[k * skip * block + j];

                    memcpy(data + k*skip*block, data2, skip*block);
                }

                for (ptr = data2, i = 0; i < skip; i++)
                    for (j = i; j < bytes - k*skip*block; j += skip)
                        * ptr++ = data[k * skip * block + j];

                memcpy(data + k*skip*block, data2, bytes - k*skip*block);
            } else {
                for (k = 0; k < bytes / skip / block; k++) {
                    for (ptr = data + k * skip * block, i = 0; i < skip; i++)
                        for (j = i; j < skip*block; j += skip)
                            data2[j] = *ptr++;

                    memcpy(data + k*skip*block, data2, skip*block);
                }

                for (ptr = data + k * skip * block, i = 0; i < skip; i++)
                    for (j = i; j < bytes - k*skip*block; j += skip)
                        data2[j] = *ptr++;

                memcpy(data + k*skip*block, data2, bytes - k*skip*block);
            }
        }

        free(data2);
    }

    // interleave a byte stream
    static void interleave(unsigned char *data, unsigned int bytes, unsigned int skip, unsigned int block = 0) {
        ReadVolume::deinterleave(data, bytes, skip, block, true);
    }
    static void DDS_swapuint(unsigned int *x) {
        unsigned int tmp = *x;
        *x = ((tmp & 0xff) << 24) |
             ((tmp & 0xff00) << 8) |
             ((tmp & 0xff0000) >> 8) |
             ((tmp & 0xff000000) >> 24);
    }

    static unsigned int readbits(int bits) {
        unsigned int value;

        if (bits < 0 || bits > 32)
            return 0;

        if (bits == 0)
            return (0);

        if (bits < ReadVolume::DDS_bufsize) {
            ReadVolume::DDS_bufsize -= bits;
            value = DDS_shiftr(ReadVolume::DDS_buffer, ReadVolume::DDS_bufsize);
        } else {
            value = DDS_shiftl(ReadVolume::DDS_buffer, bits - ReadVolume::DDS_bufsize);

            size_t read = ReadVolume::DDS_read();
            //size_t read = fread(&ReadVolume::DDS_buffer, 1, 4, file);
            if (read == 0)
                return value;//ERRORMSG();  //FIXME: we ignore this error, necessary to load the
                                            //       16-bit bonsai PVM files from The Volume  Library. joerg
            DDS_swapuint(&DDS_buffer);
            DDS_bufsize += 32 - bits;
            value |= DDS_shiftr(DDS_buffer, DDS_bufsize);
        }

        DDS_buffer &= DDS_shiftl(1, DDS_bufsize) - 1;
        DDS_bitcnt += bits;

        return (value);
    }
    static unsigned char* readDDSFile(unsigned char *dataFile, int size, int *bytes) {
        int version = 1;
        const char DDS_ID[] = "DDS v3d\n";
        const char DDS_ID2[] = "DDS v3e\n";

         unsigned int skip, strip;

         unsigned char *data = 0, *ptr = 0;

         unsigned int cnt, cnt1, cnt2;
         int bits, act;

         for (cnt = 0; DDS_ID[cnt] != '\0'; cnt++)
             if (dataFile[cnt] != DDS_ID[cnt]) {
                 version = 0;
                 break;
             }

         if (version == 0) {
             for (cnt = 0; DDS_ID2[cnt] != '\0'; cnt++)
                 if (dataFile[cnt]  != DDS_ID2[cnt]) {
                     return (NULL);
                 }
             version = 2;
         }

         ReadVolume::DDS_init_buffer();
         ReadVolume::DDS_pos += cnt;
         ReadVolume::DDS_data = dataFile;
         ReadVolume::DDS_data_size = size;

         skip = readbits(2) + 1;
         strip = readbits(16) + 1;

         data = NULL;
         cnt = act = 0;

         while ((cnt1 = ReadVolume::readbits(DDS_RL)) != 0) {
             bits = ReadVolume::DDS_decode(readbits(3));

             for (cnt2 = 0; cnt2 < cnt1; cnt2++) {
                 if (cnt <= strip)
                     act += ReadVolume::readbits(bits) - (1 << bits) / 2;
                 else
                     act += *(ptr - strip) - *(ptr - strip - 1) + ReadVolume::readbits(bits) - (1 << bits) / 2;

                 while (act < 0)
                     act += 256;
                 while (act > 255)
                     act -= 256;

                 if (cnt % DDS_BLOCKSIZE == 0) {
                     if (data == NULL) {
                         if ((data = (unsigned char *)malloc(DDS_BLOCKSIZE)) == NULL)
                             return NULL;
                     } else
                         if ((data = (unsigned char *)realloc(data, cnt + DDS_BLOCKSIZE)) == NULL)
                             return NULL;

                     ptr = &data[cnt];
                 }

                 *ptr++ = act;
                 cnt++;
             }
         }

         if (cnt == 0)
             return (NULL);

         if ((data = (unsigned char *)realloc(data, cnt)) == NULL)
             return NULL;

         if (version == 1)
             interleave(data, cnt, skip);
         else
             interleave(data, cnt, skip, DDS_INTERLEAVE);

         *bytes = cnt;

         return (data);
    }

public:
    static bool readPVMvolume(QFile &f, unsigned char **vol,
                                 int *width, int *height, int *depth, int *bits,
                                 float *scalex, float *scaley, float *scalez) {
        unsigned char *data, *ptr;
        unsigned int bytes, numc;

        int version = 1;

        unsigned char *volume;

        float sx = 1.0f, sy = 1.0f, sz = 1.0f;

        unsigned int len1 = 0, len2 = 0, len3 = 0, len4 = 0;

        bytes = f.size();
        data = new unsigned char[bytes+1];
        if (data == NULL)
            return false;

        memcpy(data, f.readAll().data(), bytes);
        if (strncmp((char *)data, "DDS", 3) == 0 ) {
            int s=0;
            unsigned char *result = ReadVolume::readDDSFile(data, bytes, &s);
            if(s > 0 && result) {
                delete data;
                bytes = s;
                data = new unsigned char[bytes+1];
                memcpy(data, result, bytes);
                data[bytes] = '\0';
                free(result);
            }
        }

        if (bytes < 5) {
            delete data;
            return false;
        }

        if (strncmp((char *)data, "PVM\n", 4) != 0) {
            version = 1;
            if (strncmp((char *)data, "PVM2\n", 5) == 0)
                version = 2;
            else if (strncmp((char *)data, "PVM3\n", 5) == 0)
                version = 3;
            else {
                delete data;
                return false;
            }


            #ifdef __unix__
                // need to switch to default locale "C" to prevent problems when current locale uses comma
                // instead of decimal point
                const char* oldlocale = setlocale(LC_NUMERIC, "C");
            #endif

            if (sscanf((char *)&data[5], "%d %d %d\n%g %g %g\n", width, height, depth, &sx, &sy, &sz) != 6) {
                #ifndef __unix__
                    setlocale(LC_NUMERIC, oldlocale);
                #endif
                delete data;
                return false;
            }
            #ifndef __unix__
                setlocale(LC_NUMERIC, oldlocale);
            #endif

            if (*width < 1 || *height < 1 || *depth < 1 || sx <= 0.0f || sy <= 0.0f || sz <= 0.0f) {
                delete data;
                return false;
            }
            ptr = (unsigned char *)strchr((char *) & data[5], '\n') + 1;
        } else {
            if (sscanf((char *)&data[4], "%d %d %d\n", width, height, depth) != 3) {
                delete data;
                return false;
            }
            if (*width < 1 || *height < 1 || *depth < 1) {
                delete data;
                return false;
            }
            ptr = &data[4];
        }

        if (scalex != NULL && scaley != NULL && scalez != NULL) {
            *scalex = sx;
            *scaley = sy;
            *scalez = sz;
        }

        ptr = (unsigned char *)strchr((char *)ptr, '\n') + 1;
        if (sscanf((char *)ptr, "%d\n", &numc) != 1) {
            delete data;
            return false;
        }
        if (numc < 1) {
            delete data;
            return false;
        }

        *bits = numc*8;

       if (numc != 1 && numc != 2){
            delete data;
            return false;
        }

        ptr = (unsigned char *)strchr((char *)ptr, '\n') + 1;
        if ((volume = (unsigned char *)malloc((*width) * (*height) * (*depth) * numc)) == NULL){
            delete data;
            return false;
        }

        memcpy(volume, ptr, (*width)*(*height)*(*depth)*numc);
        delete data;
        *vol = volume;
        return true;
    }

    static bool read(unsigned char **vol, int *w, int *h, int *d, float *sx, float *sy, float *sz, int *bits) {
        QString fileName = QFileDialog::getOpenFileName(NULL, "Open Volume",
                                                         "",
                                                         "Volume (*.pvm *.raw *.t3d)");

        if(fileName.isEmpty() || fileName.isNull())
            return false;

        QFile f(fileName);
        if(f.open(QIODevice::ReadOnly)){
            QString ext = fileName.mid(fileName.lastIndexOf(".")+1).toLower();
            bool ok;
            if(ext=="t3d"){
                char line[100];
                while(f.readLine(line, 100)) {
                    QString l(line);
                    QString key(l.mid(0, l.indexOf('=')));
                    QString value(l.mid(l.indexOf('=')+1));
                    ok=false;
                    if(key=="sizex")        *w=value.toInt(&ok);
                    else if(key=="sizey")   *h=value.toInt(&ok);
                    else if(key=="sizez")   *d=value.toInt(&ok);
                    else if(key=="scalex")  *sx=value.toFloat(&ok);
                    else if(key=="scaley")  *sy=value.toFloat(&ok);
                    else if(key=="scalez")  *sz=value.toFloat(&ok);
                    else if(key=="fileraw") {
                        QFile fileRAW(value);
                        if(fileRAW.open(QIODevice::ReadOnly)) {
                            *vol = new unsigned char[fileRAW.size()];
                            memcpy(*vol, fileRAW.readAll().data(), fileRAW.size());
                        }else ok=false;
                    }

                    if(!ok) {
                        f.close();
                        return false;
                    }
                }
                return true;
            } else if(ext=="pvm"){
                bool result = ReadVolume::readPVMvolume(f, vol, w, h, d, bits, sx, sy, sz);
                f.close();
                return result;
            }else{
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
                *d = QInputDialog::getInt(NULL, "Volumen RAW", "Profundidad:", 0, 0, 1024, 1, &ok);
                if(!ok) {
                    f.close();
                    return false;
                }

                *sx = QInputDialog::getDouble(NULL, "Volumen RAW", "Escala X:", 0, 0.01, 1024, 1, &ok);
                if(!ok) {
                    f.close();
                    return false;
                }

                *sy = QInputDialog::getDouble(NULL, "Volumen RAW", "Escala Y:", 0, 0.01, 1024, 1, &ok);
                if(!ok) {
                    f.close();
                    return false;
                }

                *sz = QInputDialog::getDouble(NULL, "Volumen RAW", "Escala Z:", 0, 0.01, 1024, 1, &ok);
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
                if((*w)*(*h)*(*d)*((*bits)/8)!=f.size()) {
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
