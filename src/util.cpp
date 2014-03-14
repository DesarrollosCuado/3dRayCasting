// Util.cpp: implementation of the CUtil class.
//
//////////////////////////////////////////////////////////////////////

#include "util.h"
#include <fcntl.h>
#include <string.h>
#include <stdarg.h>
#include <io.h>
#include <fcntl.h>


#define DEFAULT_METHOD SCA_DEFAULT
#define MAX_LINE_SIZE 3000
#define FILENAME_SIZE 512

typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALFARPROC)( int );
PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = 0;
int CUtil::m_scaMethod = DEFAULT_METHOD;


CUtil::CUtil()
{

}

CUtil::~CUtil()
{

}

void CUtil::Abort(int ret, char *fmt, ...)
{
  va_list  argptr;
  char str[100];

  va_start(argptr, fmt);
  vsprintf(str, fmt, argptr);
  printf(str);
//  printf("\npress enter to end!...");
//  va_end(argptr);
  //getchar();
 // exit(ret);
}

void CUtil::Vsync(bool enable)
{
  const char *extensions = (const char*)glGetString( GL_EXTENSIONS );
  if( strstr( extensions, "WGL_EXT_swap_control" ) == 0 )
    return; // Error: WGL_EXT_swap_control extension not supported on your computer.\n");
  else
  {
	int interval=enable?1:0;
    wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress( "wglSwapIntervalEXT" );

    if( wglSwapIntervalEXT )
      wglSwapIntervalEXT(interval);
  }

}

char * CUtil::RemoveEnter(char *str)
{
  int length = (int)strlen(str);
  if (length)
    for (int i=length-1; i>=0; i--)
      if (str[i] == 0x0A || str[i] == 0x0D)
        str[i] = 0x00;
  return str;
}

char * CUtil::SkipWord(char *str, char *theword)
{
  int i=0;
  bool enc = false;

  while (*str && *theword)
  {
    if (*str == *theword) 
      enc = true;
    else if (enc == true)
      return NULL;
    str++; 
    if (enc) theword++;
  }
  return (enc==true) ? str : NULL;
}

int CUtil::NextPowerOf2(int value)
{
  if (IsPowerOf2(value) == false)
  	return (1 << (1+(int)( log((double)value) / log(2.0) )));
  return value;
}

bool CUtil::IsPowerOf2(int value)
{
  int n=0, coc=value, res;
  for (;;)
  {
    res = coc & 1;
    if (res)
      if (++n>1) break;
    coc = coc>>1;
    if (coc == 0) break;
  }
  return (n<=1);
}

int CUtil::IsExtensionSupported(const char *extension)
{
  const GLubyte *extensions = NULL;
  const GLubyte *start;
  GLubyte *where, *terminator;

  /* Extension names should not have spaces. */
  where = (GLubyte *) strchr (extension, ' ');
  if (where || *extension == '\0')
    return 0;
  extensions = glGetString(GL_EXTENSIONS);
  /* It takes a bit of care to be fool-proof about parsing the
     OpenGL extensions string. Don't be fooled by sub-strings,
     etc. */
  start = extensions;
	
  for (;;)
  {
    where = (GLubyte *) strstr ((const char *) start, extension);
    if (!where)
      break;
    terminator = where + strlen (extension);
    if (where == start || *(where - 1) == ' ')
      if (*terminator == ' ' || *terminator == '\0')
        return 1;
    start = terminator;
  }
  return 0;
}

void CUtil::DrawBox(float x0, float y0, float z0, float x1, float y1, float z1)
{
  //glEnable(GL_BLEND);
  glBegin(GL_LINES);
    glVertex3f(x0, y0, z0);
    glVertex3f(x0, y0, z1);

    glVertex3f(x0, y0, z1);
    glVertex3f(x0, y1, z1);

    glVertex3f(x0, y1, z1);
    glVertex3f(x0, y1, z0);

    glVertex3f(x0, y1, z0);
    glVertex3f(x0, y0, z0);

    glVertex3f(x0, y0, z0);
    glVertex3f(x1, y0, z0);

    glVertex3f(x1, y0, z0);
    glVertex3f(x1, y0, z1);

    glVertex3f(x1, y0, z1);
    glVertex3f(x1, y1, z1);

    glVertex3f(x1, y1, z1);
    glVertex3f(x1, y1, z0);

    glVertex3f(x1, y1, z0);
    glVertex3f(x1, y0, z0);

    glVertex3f(x0, y1, z0);
    glVertex3f(x1, y1, z0);

    glVertex3f(x0, y1, z1);
    glVertex3f(x1, y1, z1);

    glVertex3f(x0, y0, z1);
    glVertex3f(x1, y0, z1);
  glEnd();
  //glDisable(GL_BLEND);
}


char *CUtil::TextFileRead(char *fn) 
{
	FILE *fp;
	char *content = NULL;

	int f;
	f = _open(fn, _O_RDONLY);
  if (!f)
    return NULL;

	size_t count = _lseek(f, 0, SEEK_END);
	_close(f);

	fp = fopen(fn,"rt");
	if (fp != NULL) 
  {
		if (count > 0) 
    {
			content = (char *)malloc(sizeof(char) * (count+1));
			count = fread(content,sizeof(char),count,fp);
			content[count] = '\0';
		}
		fclose(fp);
	}
	return content;
}

void ***CUtil::Alloc3D(int z, int y, int x, int elemSize)
{
  if (x<=0 || y<=0 || z<=0) return NULL;
  char ***V = (char ***)malloc(z * sizeof(char **));
  if (!V) 
    return NULL;
  int size = x*y*z*elemSize;
  char *data = (char *)malloc(size);
  if (!data)
  {
    free(V);
    return NULL;
  }
  memset(data,0,size);  // para borrar las zonas fuera de la data original
  for (int i=0; i<z; i++)
  {
    V[i] = (char **)malloc(sizeof(char *) * y);
    for (int j=0; j<y; j++)
      V[i][j] = &data[(x*y*i + x*j)*elemSize];
  }
  return (void ***)V;
}

void CUtil::Free3D(void ***V, int z)
{
  if (!V) 
    return;
  char ***a = (char ***)V;
  free(&a[0][0][0]);  ///erase the data
  for (int i=0; i<z; i++)
    free(a[i]);
  free(a);
}

long CUtil::FileSize(FILE *f)
{ 
   long curpos, length;

   curpos = ftell(f);
   fseek(f, 0L, SEEK_END);
   length = ftell(f);
   fseek(f, curpos, SEEK_SET);
   return length;
}

void CUtil::Abort(int code)
{
  exit(code);
}

void ** CUtil::Alloc2DCont(int rows, int cols, int elemSize)
{
  unsigned char *q = (unsigned char *)malloc(rows*cols*elemSize);
  void **p = (void **)malloc(rows*sizeof(void *));
  for (int i=0; i<rows; i++)
      p[i] = (void *)&q[i*cols*elemSize];
  return p;
}

void CUtil::Free2DCont(void ** data)
{
  if (data)
  {
    free(data[0]);
    free(data);
  }
}

void CUtil::Swap(float &a, float &b)
{
  float aux;
  aux = a;
  a = b;
  b = aux;
}

void CUtil::Swap(int   &a, int   &b)
{
  int aux;
  aux = a;
  a = b;
  b = aux;
}

void CUtil::Swap(unsigned char &a, unsigned char &b)
{
  unsigned char aux;
  aux = a;
  a = b;
  b = aux;
}

void CUtil::SendMatrixToOpenGL(CMatrix4x4f mat)
{
  mat.Transpose();
  glLoadMatrixf((float *)mat.m_vector);
}

int CUtil::SaveBMP(const char *filename, int nCols, int nRows, unsigned char *buffer)
{
  char bfType[3]="BM";        // ASCII "BM"
  int bfSize;           // Tama~no en bytes del archivo (Longword)
  int bfOffBits=54;        // Distancia en bytes desde el fin del header.. 
  // (byte 14) donde comienza la imagen como tal			                  
  int ErrorCode=0;		 // Entero que denota el codigo de error al leer
			 // del archivo (ver tabla de errores)
  int biSize=40;          // Tama~no del Header (40 bytes)
	int biWidth=nCols;         // Ancho de la imagen en pixeles
	int biHeight=nRows;        // Largo de la imagen en pixeles
  short int  biPlanes=1;        // Numero de planos de la imagen. Debe ser 1
	short int  biBitCount=24;      // Bits por pixel
  int  biCompression=0;   // Tipo de compresion
	int biSizeImage=0;     // Tamano en bytes de la imagen comprimida
	int biXPelsPerMeter=0; // Resolucion horizontal en pixeles/metros
	int biYPelsPerMeter=0; // Vertical horizontal en pixeles/metros
	int biClrUsed=0;       // Numero de colores usados
	int biClrImportant=0;  // Numero de colores "de importancia"
	int PaletteSize=0;     // Numero de entradas en color map. Se deduce
  BYTE *bmiColors=NULL; 	 // Color Map (blue,green,red,0) ...
	unsigned char *imagen=buffer;	 // Bytes correspondientes a la imagen

  FILE *f;
  int lineWidth = biWidth*3;
  if (lineWidth & 3)
    lineWidth += 4-(lineWidth & 3);
  biSizeImage=lineWidth*biHeight;
  bfSize = bfOffBits+biSizeImage;
  f = fopen(filename, "wb");
  if (f == NULL)
  {
    ErrorCode = 1;
    return ErrorCode;
  }
  if (fwrite(&bfType, 1, 2, f) != 2 || fwrite(&bfSize, 4, 1, f) != 1)
  {
    ErrorCode = 2;
    fclose(f);
    return ErrorCode;
  }
  int cero = 0;
  if (fwrite(&cero, 4, 1, f) != 1)
  {
    ErrorCode = 2;
    fclose(f);
    return ErrorCode;
  }


  if (fwrite(&bfOffBits, 4, 1, f)       != 1  ||
      fwrite(&biSize, 4, 1, f)          != 1  ||
      fwrite(&biWidth, 4, 1, f)         != 1  ||
      fwrite(&biHeight, 4, 1, f)        != 1  ||
      fwrite(&biPlanes, 2, 1, f)        != 1  ||
      fwrite(&biBitCount, 2, 1, f)      != 1  ||
      fwrite(&biCompression, 4, 1, f)   != 1  ||
      fwrite(&biSizeImage, 4, 1, f)     != 1  ||
      fwrite(&biXPelsPerMeter, 4, 1, f) != 1  ||
      fwrite(&biYPelsPerMeter, 4, 1, f) != 1  ||
      fwrite(&biClrUsed, 4, 1, f)       != 1  ||
      fwrite(&biClrImportant, 4, 1, f)  != 1)
  {
    ErrorCode = 5;
    fclose(f);
    return ErrorCode;
  }

  if (fwrite(imagen, 1, biSizeImage, f) != biSizeImage)
  {
      ErrorCode = 10;
      fclose(f);
      return ErrorCode;
  }
  if (fclose(f) == -1)
  {
    ErrorCode = 3;
    fclose(f);
    return ErrorCode;
  }
  ErrorCode = 0;
  return ErrorCode;
}

float CUtil::rand01()
{
  return (float)rand()/(float)RAND_MAX;
}

void CUtil::AddLine(char *filename, char *fmt, ... )
{
  va_list  argptr;
  char str[1024];

  va_start(argptr, fmt);
  vsprintf(str, fmt, argptr);
  FILE *f = fopen(filename, "at");
  if (!f)
  {
    f = fopen(filename, "wt");
    if (!f)
    {
      printf("Error opening %s\n", filename);
      va_end(argptr);
      return;
    }
  }
  fprintf(f, str);
  //printf("Writting %s\n", str);
  fclose(f);
  va_end(argptr);
}

void CUtil::DrawSolidCube(float x0, float y0, float z0, float x1, float y1, float z1)
{
  glBegin(GL_QUADS);
    // tapa inferior
    glVertex3f(x0, y0, z0);
    glVertex3f(x0, y0, z1);
    glVertex3f(x1, y0, z1);
    glVertex3f(x1, y0, z0);
    // tapa superior
    glVertex3f(x0, y1, z0);
    glVertex3f(x1, y1, z0);
    glVertex3f(x1, y1, z1);
    glVertex3f(x0, y1, z1);
    // tapa izquierda
    glVertex3f(x0, y0, z0);
    glVertex3f(x0, y1, z0);
    glVertex3f(x0, y1, z1);
    glVertex3f(x0, y0, z1);
    // tapa derecha
    glVertex3f(x1, y0, z0);
    glVertex3f(x1, y0, z1);
    glVertex3f(x1, y1, z1);
    glVertex3f(x1, y1, z0);
    // tapa frontal
    glVertex3f(x0, y0, z0);
    glVertex3f(x1, y0, z0);
    glVertex3f(x1, y1, z0);
    glVertex3f(x0, y1, z0);
    // tapa trasera
    glVertex3f(x0, y0, z1);
    glVertex3f(x0, y1, z1);
    glVertex3f(x1, y1, z1);
    glVertex3f(x1, y0, z1);
  glEnd();

}

void CUtil::DrawBackFacedCube(float x0, float y0, float z0, float x1, float y1, float z1, GLenum renderType)
{
  glPolygonMode(GL_FRONT_AND_BACK, renderType);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
      DrawSolidCube(x0,y0,z0,x1,y1,z1);
    glDisable(GL_CULL_FACE);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

float CUtil::MinDistance(const CVector4Df &point, const CVector3Df &boxMin, const CVector3Df &boxMax)
{
  CVector3Df res;
  for (int i=0; i<3; i++)
  {
    if (boxMax[i] < point[i])
      res[i] = point[i] - boxMax[i];
    else if (point[i]   < boxMin[i])
      res[i] = boxMin[i] - point[i];
    else
      res[i] = 0.0f;
  }  
  return (res[0] || res[1] || res[2]) ? res.Modulus() : 0.0f;
}
float CUtil::MinDistanceEucli(const CVector4Df &point, const CVector4Df &boxMin, const CVector4Df &boxMax)
{
  CVector3Df res;
  for (int i=0; i<3; i++)
  {
    if (boxMax[i] < point[i])
      res[i] = point[i] - boxMax[i];
    else if (point[i]   < boxMin[i])
      res[i] = boxMin[i] - point[i];
    else
      res[i] = 0.0f;
  }  
  return (res[0] || res[1] || res[2]) ? res.Distance() : 0.0f;
}

float CUtil::MinDistance(const CVector4Df &point, const CVector4Df &boxMin, const CVector4Df &boxMax)
{
  CVector3Df res;
  for (int i=0; i<3; i++)
  {
    if (boxMax[i] < point[i])
      res[i] = point[i] - boxMax[i];
    else if (point[i]   < boxMin[i])
      res[i] = boxMin[i] - point[i];
    else
      res[i] = 0.0f;
  }  
  return (res[0] || res[1] || res[2]) ? res.Modulus() : 0.0f;
}

float CUtil::MinSqrDistance(const CVector4Df &point, const CVector3Df &boxMin, const CVector3Df &boxMax)
{
  CVector3Df res;
  for (int i=0; i<3; i++)
  {
    if (boxMax[i] < point[i])
      res[i] = point[i] - boxMax[i];
    else if (point[i]   < boxMin[i])
      res[i] = boxMin[i] - point[i];
    else
      res[i] = 0.0f;
  }  
  return (res[0] || res[1] || res[2]) ? res[0]*res[0]+res[1]*res[1]+res[2]*res[2] : 0.0f;
}

float CUtil::MaxDistance(const CVector4Df &point, const CVector3Df &boxMin, const CVector3Df &boxMax)
{
  CVector3Df res1, res2;
  res1[0] = fabs(boxMin[0] - point[0]);
  res2[0] = fabs(boxMax[0] - point[0]);
  res1[1] = fabs(boxMin[1] - point[1]);
  res2[1] = fabs(boxMax[1] - point[1]);
  res1[2] = fabs(boxMin[2] - point[2]);
  res2[2] = fabs(boxMax[2] - point[2]);
  return CVector3Df(max(res1[0],res2[0]), max(res1[1],res2[1]), max(res1[2],res2[2])).Modulus();
}


void CUtil::RGB2LUV(const CVector3Df &rgb, CVector3Df &luv)
{
  static CVector3Df xyz_white(0.412453*1.0f + 0.357580*1.0f + 0.180423*1.0f, 
                              0.212671*1.0f + 0.715160*1.0f + 0.072169*1.0f,
                              0.019334*1.0f + 0.119193*1.0f + 0.950227*1.0f);
  static float un_prima = (4.0f*xyz_white[0])/(xyz_white[0]+15.0f*xyz_white[1]+3.0f*xyz_white[2]);
  static float vn_prima = (9.0f*xyz_white[1])/(xyz_white[0]+15.0f*xyz_white[1]+3.0f*xyz_white[2]);
  CVector3Df xyz;

  if (rgb[0]==0 && rgb[1]==0 && rgb[2]==0)
  {
    luv = CVector3Df(0,0,0);
    return;
  }
  xyz[0] = 0.412453*rgb[0] + 0.357580*rgb[1] + 0.180423*rgb[2];
  xyz[1] = 0.212671*rgb[0] + 0.715160*rgb[1] + 0.072169*rgb[2];
  xyz[2] = 0.019334*rgb[0] + 0.119193*rgb[1] + 0.950227*rgb[2];

  float aux = 1.0f/(xyz[0]+15.0f*xyz[1]+3.0f*xyz[2]);
  float u = (4.0f*xyz[0])*aux;
  float v = (9.0f*xyz[1])*aux;
  aux = xyz[1]/xyz_white[1];
  luv[0] = (aux > 0.008856f) ? 116.0f*pow(aux,0.3333333333f) - 16.0f : 903.3*aux;
  aux = 13.0f * luv[0];
  luv[1] = aux * (u - un_prima);
  luv[2] = aux * (v - vn_prima);
}


#ifdef _DEBUG
  void TestOpenGLError()
  {
    //if (glGetError()!=GL_NO_ERROR)
      //printf("CUtil::TestOpenGLError\n");  
      //CUtil::Abort(0, "CUtil::TestOpenGLError\n");  
    CheckErrorsGL(NULL);
  }

	void CheckErrorsGL(const char* location, ostream& ostr) 
	{
	  GLuint errnum;
	  const char *errstr;
	  int counter = 0;
	  while (errnum = glGetError()) 
	  {
		errstr = reinterpret_cast<const char *>(gluErrorString(errnum));
		if(errstr) 
		{
		  ostr << errstr; 
		}
		else 
		{
		  ostr << "Error " << errnum;
		}
	    
		if(location) 
		  ostr << " at " << location;		
		ostr << endl;
		counter++;
	  }
	 if (counter)
		  CUtil::Abort(0, "Application caanot continue!\n");

	  return;
	}


#endif
