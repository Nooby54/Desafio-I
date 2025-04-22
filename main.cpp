#include <iostream>
#include "bmp.h"
using namespace std;
bmp bmp;

int main()
{
    // Definición de rutas de archivos de entrada (Imagen Modificada y Mascara)
    QString I_D = "I_D.bmp";
    QString I_M = "I_M.bmp";

    // Variables para almacenar las dimensiones de las imagenes
    int height_ID = 0;
    int width_ID = 0;
    int height_IM = 0;
    int width_IM = 0;

    // Carga las imagenes BMP en memoria dinámica y obtiene ancho y alto
    unsigned char *ID = bmp.loadPixels(I_D, width_ID, height_ID);
    unsigned char *IM = bmp.loadPixels(I_M, width_IM, height_IM);

    unsigned int n = 2;
    bool transformacion = false;
    for (int i = n; i >= 0; i--)
    {
        // Variables para almacenar la semilla y el número de píxeles leídos del archivo de enmascaramiento
        int seed = 0;
        int n_pixels = 0;
        string name = "M" + to_string(n) + ".txt";

        // Carga los datos de enmascaramiento desde un archivo .txt (semilla + valores RGB)
        unsigned int *maskingData = bmp.loadSeedMasking(name.c_str(), seed, n_pixels);

        for (int b = 1; b <= 8; b++)
        {
            unsigned char *IT = ID;
            if (transformacion == false)
            {
                IT = bmp.rotar_derecha(IT, b, height_ID * width_ID * 3);
                IT = bmp.Enmascaramiento(IT, IM, seed, height_ID * width_ID * 3);
                if (bmp.verificacion_enmascaramiento(IT, maskingData, n_pixels))
                {
                    transformacion = true;
                    ID = IT;
                }
            }
        }
    }
    return 0;
}
