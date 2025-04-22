#include <iostream>
#include "bmp.h"
using namespace std;
bmp bmp;

int main()
{
    // Definición de rutas de archivos de entrada (Imagen Modificada y Mascara)
    QString I_D = "../../data/I_D.bmp";
    int height_ID = 0;
    int width_ID = 0;

    unsigned char *ID = bmp.loadPixels(I_D, width_ID, height_ID);

    unsigned int n = 2;
    bool transformacion = false;
    unsigned char *IT = nullptr;
    for (int i = n; i >= 0; i--)
    {
        // Variables para almacenar la semilla y el número de píxeles leídos del archivo de enmascaramiento
        int seed = 0;
        int n_pixels = 0;
        string name = "../../data/M" + to_string(i) + ".txt";
        // Carga los datos de enmascaramiento desde un archivo .txt (semilla + valores RGB)
        unsigned int *maskingData = bmp.loadSeedMasking(name.c_str(), seed, n_pixels);

        //Desenmascarando
        bmp.desenmascarar(ID, seed, height_ID * width_ID * 3);
        IT = bmp.XOR(ID);
        if(bmp.verificacion_enmascaramiento(IT,maskingData,n_pixels)){
            transformacion = true;
            //Copiar datos de IT a ID
        }
        else{
        for (int bit = 1; bit <= 8; bit++)
        {
            if (transformacion == false)
            {
                delete[] IT;
                IT = bmp.rotar_derecha(ID, bit, height_ID * width_ID * 3);

                if (bmp.verificacion_enmascaramiento(IT, maskingData, n_pixels))
                {
                    transformacion = true;
                    //Copiar datos de IT a ID
                }
            }
        }}
        delete[] maskingData;
    }
    delete[] ID;
    delete[] IT;

    return 0;
}
