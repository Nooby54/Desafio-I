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
    unsigned char *copiar_arreglo(unsigned char *IT, unsigned int totalBytes);
    unsigned char *rotar_izquierda(unsigned char *ID, unsigned short int bits, unsigned int totalBytes);
    unsigned char *rotar_derecha(unsigned char *ID, unsigned short int bits, unsigned int totalBytes);
    unsigned char *XOR(unsigned char *ID);
    void desenmascarar(unsigned char *S, unsigned int semilla, int totalPixeles);
    void desplazamiento_derecha(const unsigned char *entrada, unsigned char *salida, int totalBytes, int bits);
    void desplazamiento_izquierda(const unsigned char *entrada, unsigned char *salida, int totalBytes, int bits);
    bool verificacion_enmascaramiento(const unsigned char *enmascarado, const unsigned int *datos_txt, int totalPixeles);
};

#endif // BMP_H
