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
    void rotar_izquierda(unsigned char *ID, unsigned short int n, unsigned char *transformacion);
    void rotar_derecha(unsigned char *ID, unsigned short int n, unsigned char *transformacion);
};

#endif // BMP_H
