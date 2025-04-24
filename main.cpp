#include <iostream>
#include "bmp.h"
using namespace std;

int main()
{
    bmp bmp;
    // Definición de rutas y variables para cargar ID
    QString I_D = "../../data/I_D.bmp";
    int height_ID = 0;
    int width_ID = 0;
    unsigned char *ID = bmp.loadPixels(I_D, width_ID, height_ID);

    // Definicion de la ruta y uso de la funcion para contar cuantos archivos .txt hay
    QString ruta = "../../data";
    int n = bmp.contarArchivosMascara(ruta) - 1;

    // Definicion de variables adicionales
    unsigned char *IT = nullptr; // Puntero que va a probar las transformaciones para no afectar ID
    int totalBytes = height_ID * width_ID * 3;

    for (int i = n; i >= 0; i--)
    {
        // Creacion del string con la direccion del archhivo .txt
        string name = "../../data/M" + to_string(i) + ".txt";

        // Aplicacion del XOR y luego verificamos si es la transformacion adecuada
        delete[] IT;
        IT = bmp.XOR(ID);
        if (bmp.verificacion_enmascaramiento(IT, name.c_str()))
        {
            cout << "La transformacion " << n - i + 1 << " fue un XOR" << endl;
        }
        else
        {
            // Si la transformacion no es el XOR, pasa a evaluar las demas transformaciones
            for (int bit = 1; bit <= 8; bit++)
            {
                //Aplicacion del desplazamiento a la derecha (inverso del desplazamiento a la izquierda) y validacion
                delete[] IT;
                IT = bmp.desplazamiento_izquierda(ID, bit, totalBytes);

                if (bmp.verificacion_enmascaramiento(IT, name.c_str()))
                {
                    cout << "La transformacion " << n - i + 1 << " fue un desplazamiento a la izquierda de " << bit << " bits" << endl;
                    break; // Sale del ciclo si la transformacion es correcta
                }

                // Aplicacion del desplazamiento a la izquierda (inverso del desplazamiento a la derecha) y validacion
                delete[] IT;
                IT = bmp.desplazamiento_derecha(ID, bit, totalBytes);

                if (bmp.verificacion_enmascaramiento(IT, name.c_str()))
                {
                    cout << "La transformacion " << n - i + 1 << " fue un desplazamiento a la derecha de " << bit << " bits" << endl;
                    break; // Sale del ciclo si la transformacion es correcta
                }

                // Aplicacion de la rotacion derecha (inverso de la rotacion izquierda) y validacion
                delete[] IT;
                IT = bmp.rotar_derecha(ID, bit, totalBytes);

                if (bmp.verificacion_enmascaramiento(IT, name.c_str()))
                {
                    cout << "La transformacion " << n - i + 1 << " fue una rotacion a la izquierda de " << bit << " bits" << endl;
                    break; // Sale del ciclo si la transformacion es correcta
                }

                // Aplicacion de la rotacion izquierda (inverso de la rotacion derecha) y validacion
                delete[] IT;
                IT = bmp.rotar_izquierda(ID, bit, totalBytes);

                if (bmp.verificacion_enmascaramiento(IT, name.c_str()))
                {
                    cout << "La transformacion " << n - i + 1 << " fue una rotacion a la derecha de " << bit << " bits" << endl;
                    break; // Sale del ciclo si la transformacion es correcta
                }
            }
        }
        // Se elimina ID y le asigno IT, pues en este punto IT ya esta destransformado
        delete[] ID;
        ID = bmp.copiar_arreglo(IT, totalBytes);
    }
    // Se libera IT
    delete[] IT;

    // Exportando ID, en este punto ya es IO
    QString I_O = "../../data/I_O.bmp";
    if (bmp.exportImage(ID, width_ID, height_ID, I_O))
    {
        cout << "Imagen original reconstruida correctamente" << endl;
    }
    else
    {
        cout << "Ocurrio un error al reconstruir la imagen" << endl;
    }

    //Se libera ID
    delete[] ID;

    return 0;
}
