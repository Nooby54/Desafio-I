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
    unsigned char* desenmascarar(unsigned char *S, unsigned int semilla, int totalPixeles);
    unsigned char* desplazamiento_izquierda(unsigned char* entrada, int bits, int totalBytes);
    unsigned char* desplazamiento_derecha(unsigned char* entrada, int bits, int totalBytes);
    bool verificacion_enmascaramiento( const unsigned char *enmascarado, const unsigned int *datos_txt, int semilla, int totalPixeles);
    int contarArchivosMascara(const QString& rutaDirectorio);
    QStringList encontrarArchivosMascara(const QString& rutaDirectorio);
};

#endif // BMP_H
