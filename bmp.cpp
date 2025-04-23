#include "bmp.h"

#include <fstream>
#include <iostream>
#include <QCoreApplication>
#include <QImage>

using namespace std;

bmp::bmp() {}



unsigned char *bmp::loadPixels(QString input, int &width, int &height)
{
    /*
     * @brief Carga una imagen BMP desde un archivo y extrae los datos de píxeles en formato RGB.
     *
     * Esta función utiliza la clase QImage de Qt para abrir una imagen en formato BMP, convertirla al
     * formato RGB888 (24 bits: 8 bits por canal), y copiar sus datos de píxeles a un arreglo dinámico
     * de tipo unsigned char. El arreglo contendrá los valores de los canales Rojo, Verde y Azul (R, G, B)
     * de cada píxel de la imagen, sin rellenos (padding).
     *
     * @param input Ruta del archivo de imagen BMP a cargar (tipo QString).
     * @param width Parámetro de salida que contendrá el ancho de la imagen cargada (en píxeles).
     * @param height Parámetro de salida que contendrá la altura de la imagen cargada (en píxeles).
     * @return Puntero a un arreglo dinámico que contiene los datos de los píxeles en formato RGB.
     *         Devuelve nullptr si la imagen no pudo cargarse.
     *
     * @note Es responsabilidad del usuario liberar la memoria asignada al arreglo devuelto usando `delete[]`.
     */

    // Cargar la imagen BMP desde el archivo especificado (usando Qt)
    QImage imagen(input);

    // Verifica si la imagen fue cargada correctamente
    if (imagen.isNull())
    {
        cout << "Error: No se pudo cargar la imagen BMP." << std::endl;
        return nullptr; // Retorna un puntero nulo si la carga falló
    }

    // Convierte la imagen al formato RGB888 (3 canales de 8 bits sin transparencia)
    imagen = imagen.convertToFormat(QImage::Format_RGB888);

    // Obtiene el ancho y el alto de la imagen cargada
    width = imagen.width();
    height = imagen.height();

    // Calcula el tamaño total de datos (3 bytes por píxel: R, G, B)
    int dataSize = width * height * 3;

    // Reserva memoria dinámica para almacenar los valores RGB de cada píxel
    unsigned char *pixelData = new unsigned char[dataSize];

    // Copia cada línea de píxeles de la imagen Qt a nuestro arreglo lineal
    for (int y = 0; y < height; ++y)
    {
        const uchar *srcLine = imagen.scanLine(y);          // Línea original de la imagen con posible padding
        unsigned char *dstLine = pixelData + y * width * 3; // Línea destino en el arreglo lineal sin padding
        memcpy(dstLine, srcLine, width * 3);                // Copia los píxeles RGB de esa línea (sin padding)
    }

    // Retorna el puntero al arreglo de datos de píxeles cargado en memoria
    return pixelData;
}

bool bmp::exportImage(unsigned char *pixelData, int width, int height, QString archivoSalida)
{
    /*
     * @brief Exporta una imagen en formato BMP a partir de un arreglo de píxeles en formato RGB.
     *
     * Esta función crea una imagen de tipo QImage utilizando los datos contenidos en el arreglo dinámico
     * `pixelData`, que debe representar una imagen en formato RGB888 (3 bytes por píxel, sin padding).
     * A continuación, copia los datos línea por línea a la imagen de salida y guarda el archivo resultante
     * en formato BMP en la ruta especificada.
     *
     * @param pixelData Puntero a un arreglo de bytes que contiene los datos RGB de la imagen a exportar.
     *                  El tamaño debe ser igual a width * height * 3 bytes.
     * @param width Ancho de la imagen en píxeles.
     * @param height Alto de la imagen en píxeles.
     * @param archivoSalida Ruta y nombre del archivo de salida en el que se guardará la imagen BMP (QString).
     *
     * @return true si la imagen se guardó exitosamente; false si ocurrió un error durante el proceso.
     *
     * @note La función no libera la memoria del arreglo pixelData; esta responsabilidad recae en el usuario.
     */

    // Crear una nueva imagen de salida con el mismo tamaño que la original
    // usando el formato RGB888 (3 bytes por píxel, sin canal alfa)
    QImage outputImage(width, height, QImage::Format_RGB888);

    // Copiar los datos de píxeles desde el buffer al objeto QImage
    for (int y = 0; y < height; ++y)
    {
        // outputImage.scanLine(y) devuelve un puntero a la línea y-ésima de píxeles en la imagen
        // pixelData + y * width * 3 apunta al inicio de la línea y-ésima en el buffer (sin padding)
        // width * 3 son los bytes a copiar (3 por píxel)
        memcpy(outputImage.scanLine(y), pixelData + y * width * 3, width * 3);
    }

    // Guardar la imagen en disco como archivo BMP
    if (!outputImage.save(archivoSalida, "BMP"))
    {
        // Si hubo un error al guardar, mostrar mensaje de error
        cout << "Error: No se pudo guardar la imagen BMP modificada.";
        return false; // Indica que la operación falló
    }
    else
    {
        // Si la imagen fue guardada correctamente, mostrar mensaje de éxito
        cout << "Imagen BMP modificada guardada como " << archivoSalida.toStdString() << endl;
        return true; // Indica éxito
    }
}

unsigned int *bmp::loadSeedMasking(const char *nombreArchivo, int &seed, int &n_pixels)
{
    /*
     * @brief Carga la semilla y los resultados del enmascaramiento desde un archivo de texto.
     *
     * Esta función abre un archivo de texto que contiene una semilla en la primera línea y,
     * a continuación, una lista de valores RGB resultantes del proceso de enmascaramiento.
     * Primero cuenta cuántos tripletes de píxeles hay, luego reserva memoria dinámica
     * y finalmente carga los valores en un arreglo de enteros.
     *
     * @param nombreArchivo Ruta del archivo de texto que contiene la semilla y los valores RGB.
     * @param seed Variable de referencia donde se almacenará el valor entero de la semilla.
     * @param n_pixels Variable de referencia donde se almacenará la cantidad de píxeles leídos
     *                 (equivalente al número de líneas después de la semilla).
     *
     * @return Puntero a un arreglo dinámico de enteros que contiene los valores RGB
     *         en orden secuencial (R, G, B, R, G, B, ...). Devuelve nullptr si ocurre un error al abrir el archivo.
     *
     * @note Es responsabilidad del usuario liberar la memoria reservada con delete[].
     */

    // Abrir el archivo que contiene la semilla y los valores RGB
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open())
    {
        // Verificar si el archivo pudo abrirse correctamente
        cout << "No se pudo abrir el archivo." << endl;
        return nullptr;
    }

    // Leer la semilla desde la primera línea del archivo
    archivo >> seed;

    int r, g, b;

    // Contar cuántos grupos de valores RGB hay en el archivo
    // Se asume que cada línea después de la semilla tiene tres valores (r, g, b)
    while (archivo >> r >> g >> b)
    {
        n_pixels++; // Contamos la cantidad de píxeles
    }

    // Cerrar el archivo para volver a abrirlo desde el inicio
    archivo.close();
    archivo.open(nombreArchivo);

    // Verificar que se pudo reabrir el archivo correctamente
    if (!archivo.is_open())
    {
        cout << "Error al reabrir el archivo." << endl;
        return nullptr;
    }

    // Reservar memoria dinámica para guardar todos los valores RGB
    // Cada píxel tiene 3 componentes: R, G y B
    unsigned int *RGB = new unsigned int[n_pixels * 3];

    // Leer nuevamente la semilla desde el archivo (se descarta su valor porque ya se cargó antes)
    archivo >> seed;

    // Leer y almacenar los valores RGB uno por uno en el arreglo dinámico
    for (int i = 0; i < n_pixels * 3; i += 3)
    {
        archivo >> r >> g >> b;
        RGB[i] = r;
        RGB[i + 1] = g;
        RGB[i + 2] = b;
    }

    // Cerrar el archivo después de terminar la lectura
    archivo.close();

    // Mostrar información de control en consola
    cout << "Semilla: " << seed << endl;
    cout << "Cantidad de píxeles leídos: " << n_pixels << endl;

    // Retornar el puntero al arreglo con los datos RGB
    return RGB;
}

unsigned char *bmp::copiar_arreglo(unsigned char *IT, unsigned int totalBytes)
{
    unsigned char *ID = new unsigned char[totalBytes];
    for (unsigned int i = 0; i < totalBytes; i++)
    {
        ID[i] = IT[i];
    }

    return ID;
}

unsigned char *bmp::rotar_izquierda(unsigned char *ID, unsigned short int bits, unsigned int totalBytes)
{
    unsigned char *transformacion = copiar_arreglo(ID, totalBytes);

    for (unsigned int t = 0; t < totalBytes; t++)
    {
        for (int i = 0; i < bits; ++i)
        {
            unsigned char bit = (transformacion[t] & 0b10000000) >> 7;
            transformacion[t] = (transformacion[t] << 1) | bit;
        }
    }

    return transformacion;
}

unsigned char *bmp::rotar_derecha(unsigned char *ID, unsigned short int bits, unsigned int totalBytes)
{
    unsigned char *transformacion = copiar_arreglo(ID, totalBytes);
    for (unsigned int t = 0; t < totalBytes; t++)
    {
        for (int i = 0; i < bits; ++i)
        {
            unsigned char bit = (transformacion[t] & 0b00000001) << 7;
            transformacion[t] = (transformacion[t] >> 1) | bit;
        }
    }
    return transformacion;
}

unsigned char *bmp::XOR(unsigned char *ID)
{
    // Cargando IM
    QString I_M = "../../data/I_M.bmp";
    int height_IM = 0;
    int width_IM = 0;
    unsigned char *IM = loadPixels(I_M, width_IM, height_IM);
    unsigned char *transformacion = new unsigned char[height_IM * width_IM * 3];
    for (int i = 0; i < height_IM * width_IM * 3; i++)
    {
        transformacion[i] = ID[i] ^ IM[i];
    }
    delete[] IM;
    return transformacion;
}

unsigned char* bmp::desenmascarar(unsigned char *S, unsigned int semilla, int totalPixeles)
{
    QString M = "../../data/M.bmp";
    int height_M = 0;
    int width_M = 0;

    unsigned char *mascara = loadPixels(M, width_M, height_M);
    if (!mascara)
        return nullptr;

    if (semilla + (height_M * width_M * 3) > totalPixeles)
    {
        cout << "Advertencia: El desplazamiento y tamaño de la máscara exceden el tamaño de la imagen enmascarada.";
        delete[] mascara;
        return nullptr;
    }

    unsigned char* nomask = copiar_arreglo(S,totalPixeles);
    for (int k = 0; k < (height_M * width_M * 3); ++k)
    {
        nomask[k + semilla] = S[k] - mascara[k];
    }

    // Liberar la memoria de la máscara
    delete[] mascara;
    return nomask;
}

unsigned char* bmp::desplazamiento_derecha(unsigned char* entrada, unsigned short int bits, unsigned int totalBytes)
{
    unsigned char* salida = new unsigned char[totalBytes];
    for (int i = 0; i < totalBytes; ++i)
    {
        salida[i] = entrada[i] >> bits;
    }
    return salida;
}

unsigned char* bmp::desplazamiento_izquierda(unsigned char* entrada, unsigned short int bits, unsigned int totalBytes)
{
    unsigned char* salida = new unsigned char[totalBytes];
    for (int i = 0; i < totalBytes; ++i)
    {
        salida[i] = entrada[i] << bits & 0xFF;
    }
    return salida;
}

bool bmp::verificacion_enmascaramiento( const unsigned char *enmascarado, const unsigned int *datos_txt, int semilla, int totalPixeles)
{
    for (int i = 0; i < totalPixeles * 3; i += 3) {
        int pos = semilla + i; 
        
        if (static_cast<unsigned int>(enmascarado[pos]) != datos_txt[i] || 
            static_cast<unsigned int>(enmascarado[pos + 1]) != datos_txt[i + 1] || 
            static_cast<unsigned int>(enmascarado[pos + 2]) != datos_txt[i + 2])
        { 
            return false;
        }
    }
    return true;
}
