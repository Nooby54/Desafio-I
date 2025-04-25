#ifndef BMP_H
#define BMP_H

#include <QString>

class bmp
{
public:
    bmp();

    unsigned char *loadPixels(QString input, int &width, int &height);
    bool exportImage(unsigned char *pixelData, int width, int height, QString archivoSalida);
    unsigned int *loadSeedMasking(const char *nombreArchivo, int &seed, int &n_pixels);
    unsigned char *copiarArreglo(unsigned char *IT, unsigned int totalBytes);
    unsigned char *rotarIzquierda(unsigned char *ID, unsigned short int bits, unsigned int totalBytes);
    unsigned char *rotarDerecha(unsigned char *ID, unsigned short int bits, unsigned int totalBytes);
    unsigned char *XOR(unsigned char *ID);
    unsigned char* desplazamientoIzquierda(unsigned char* ID, unsigned short int bits, unsigned int totalBytes);
    unsigned char* desplazamientoDerecha(unsigned char* ID, unsigned short int bits, unsigned int totalBytes);
    bool verificarEnmascaramiento(unsigned char *ID, const char* name);
    int contarArchivosMascara(const QString& rutaDirectorio);
};

#endif // BMP_H
