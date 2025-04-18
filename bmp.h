#ifndef BMP_H
#define BMP_H

#include <QString>

class bmp
{
public:
    bmp();

    unsigned char* loadPixels(QString input, int &width, int &height);
    bool exportImage(unsigned char* pixelData, int width,int height, QString archivoSalida);
    unsigned int* loadSeedMasking(const char* nombreArchivo, int &seed, int &n_pixels);
    unsigned char* rotar_izquierda(unsigned char *ID, unsigned short int n);
    unsigned char* rotar_derecha(unsigned char *ID, unsigned short int n);
    unsigned char* Enmascaramiento(unsigned char* transformacion, unsigned char* mascara, unsigned int semilla, int totalPixeles);
};

#endif // BMP_H
