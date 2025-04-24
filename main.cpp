#include <iostream>
#include "bmp.h"
using namespace std;

int main()
{
    bmp bmp;
    // Definición de rutas de archivos de entrada
    QString I_D = "../../data/I_D.bmp";
    int height_ID = 0;
    int width_ID = 0;

    unsigned char *ID = bmp.loadPixels(I_D, width_ID, height_ID);


    QString ruta = "../../data";
    int n = bmp.contarArchivosMascara(ruta) - 1;

    bool transformacion = false;
    unsigned char *IT = nullptr;
    int totalBytes = height_ID * width_ID * 3;
    for (int i = n; i >= 0; i--)
    {
        delete[] IT;
        IT = bmp.XOR(ID);
        string name = "../../data/M" + to_string(i) + ".txt";
        if (bmp.verificacion_enmascaramiento(IT, name.c_str()))
        {
            transformacion = true;
            cout << "La transformacion " << n-i+1 << " fue un XOR" << endl;
        }
        else
        {
            for (int bit = 1; bit <= 8; bit++)
            {
                if (transformacion == false)
                {
                    delete[] IT;
                    IT = bmp.rotar_derecha(ID, bit, totalBytes);

                    if (bmp.verificacion_enmascaramiento(IT, name.c_str()))
                    {
                        transformacion = true;
                        cout << "La transformacion " << n-i+1 << " fue una rotacion a la izquierda de " << bit << " bits" << endl;
                        break;
                    }
                }

                if (transformacion == false)
                {
                    delete[] IT;
                    IT = bmp.rotar_izquierda(ID, bit, totalBytes);

                    if (bmp.verificacion_enmascaramiento(IT, name.c_str()))
                    {
                        transformacion = true;
                        cout << "La transformacion " << n-i+1 << " fue una rotacion a la derecha de " << bit << " bits" << endl;;
                        break;
                    }
                }
                if (transformacion == false)
                {
                    delete[] IT;
                    IT = bmp.desplazamiento_izquierda(ID, bit, totalBytes);

                    if (bmp.verificacion_enmascaramiento(IT, name.c_str()))
                    {
                        transformacion = true;
                        cout << "La transformacion " << n-i+1 << " fue un desplazamiento a la izquierda de " << bit << " bits" << endl;;
                        break;
                    }
                }

                if (transformacion == false)
                {
                    delete[] IT;
                    IT = bmp.desplazamiento_derecha(ID, bit, totalBytes);  // Corregido

                    if (bmp.verificacion_enmascaramiento(IT, name.c_str()))
                    {
                        transformacion = true;
                        cout << "La transformacion " << n-i+1 << " fue un desplazamiento a la derecha de " << bit << " bits" << endl;
                        break;
                    }
                }
            }
        }
        if(transformacion){
            delete[] ID;
            ID = bmp.copiar_arreglo(IT, totalBytes);
        }
        transformacion = false;
    }
    QString I_O = "../../data/I_O.bmp";
    if(bmp.exportImage(ID,width_ID,height_ID,I_O)){
        cout << "Imagen original reconstruida correctamente" << endl;
    }
    else{
        cout << "Ocurrio un error al reconstruir la imagen" << endl;
    }
    delete[] ID;
    delete[] IT;

    return 0;
}
