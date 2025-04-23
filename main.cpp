#include <iostream>
#include "bmp.h"
using namespace std;

int main()
{
    bmp bmp;
    // Definición de rutas de archivos de entrada (Imagen Modificada y Mascara)
    QString I_D = "../../data/I_D.bmp";
    int height_ID = 0;
    int width_ID = 0;

    unsigned char *ID = bmp.loadPixels(I_D, width_ID, height_ID);

    unsigned int n = 2;
    bool transformacion = false;
    unsigned char *IT = nullptr;
    int totalBytes = height_ID * width_ID * 3;
    for (int i = n; i >= 0; i--)
    {
        // Variables para almacenar la semilla y el número de píxeles leídos del archivo de enmascaramiento
        int seed = 0;
        int n_pixels = 0;
        string name = "../../data/M" + to_string(i) + ".txt";
        // Carga los datos de enmascaramiento desde un archivo .txt (semilla + valores RGB)
        unsigned int *maskingData = bmp.loadSeedMasking(name.c_str(), seed, n_pixels);

        // Desenmascarando
        bmp.desenmascarar(ID, seed, totalBytes);
        IT = bmp.XOR(ID);
       if (bmp.verificacion_enmascaramiento(IT, maskingData, seed, n_pixels))
        {
            transformacion = true;
            delete[] ID;
            ID = bmp.copiar_arreglo(IT, totalBytes);
            break;
        }
        else
        {
            for (int bit = 1; bit < 8; bit++)
            {
                if (transformacion == false)
                {
                    delete[] IT;
                    IT = bmp.rotar_derecha(ID, bit, totalBytes);

                    if (bmp.verificacion_enmascaramiento(IT, maskingData, seed, n_pixels))
                    {
                        transformacion = true;
                        delete[] ID;
                        ID = bmp.copiar_arreglo(IT, totalBytes);
                        break;
                    }
                }

                if (transformacion == false)
                {
                    delete[] IT;
                    IT = bmp.rotar_izquierda(ID, bit, totalBytes);

                    if (bmp.verificacion_enmascaramiento(IT, maskingData, seed, n_pixels))
                    {
                        transformacion = true;
                        delete[] ID;
                        ID = bmp.copiar_arreglo(IT, totalBytes);
                        break;
                    }
                }
                if (transformacion == false) 
                {
                    delete[] IT;
                    IT = bmp.desplazamiento_izquierda(ID, bit, totalBytes); 
                    
                    if (bmp.verificacion_enmascaramiento(IT, maskingData, seed, n_pixels)) 
                    {
                        transformacion = true;
                        delete[] ID;
                        ID = bmp.copiar_arreglo(IT, totalBytes);
                        break; 
                    }
                }
                
               if (transformacion == false) 
               {
                    delete[] IT;
                    IT = bmp.desplazamiento_derecha(ID, bit, totalBytes);  // Corregido
                    
                    if (bmp.verificacion_enmascaramiento(IT, maskingData, seed, n_pixels)) 
                    {
                        transformacion = true;
                        delete[] ID;
                        ID = bmp.copiar_arreglo(IT, totalBytes);
                        break; 
                    }
                }
                    
                }
            }
        }
        delete[] maskingData;
    }
    delete[] ID;
    delete[] IT;

    return 0;
}
