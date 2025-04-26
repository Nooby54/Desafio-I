#include "bmp.h"

#include <fstream>
#include <iostream>
#include <QCoreApplication>
#include <QImage>
#include <QDir>

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
        return false; // Indica que la operación falló
    }
    else
    {
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
        // cout << "No se pudo abrir el archivo." << endl;
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

    // Retornar el puntero al arreglo con los datos RGB
    return RGB;
}

unsigned char *bmp::copiarArreglo(unsigned char *IT, unsigned int totalBytes)
{
    /*
     * @brief Copia la informacion de un arreglo a otro almacenado en el heap.
     *
     * Esta función crea un arreglo dinámico 'ID', que copia la informacion del arreglo 'IT'.
     *
     * @param IT Puntero a un arreglo dinamico.
     * @param totalBytes Tamaño del arreglo.
     *
     * @return Puntero al nuevo arreglo dinámico que contiene la copia de los datos de 'IT'.
     *
     * @note La función no libera la memoria del arreglo IT; esta responsabilidad recae en el usuario.
     */

    // Reserva memoria dinamica para almacenar la copia de 'IT'
    unsigned char *ID = new unsigned char[totalBytes];

    // Copiar la informacion de 'IT' a 'ID'
    for (unsigned int i = 0; i < totalBytes; i++)
    {
        ID[i] = IT[i];
    }

    // Retorna el puntero al arreglo con los datos de 'IT' copiados
    return ID;
}

unsigned char *bmp::rotarIzquierda(unsigned char *ID, unsigned short int bits, unsigned int totalBytes)
{
    /*
     * @brief Rota el arreglo 'ID' y lo almacena en un arreglo almacenado en el heap.
     *
     * Esta función copia el arreglo dinámico 'ID' a 'transformacion' y, rota cada byte del arreglo
     * una cantidad n (bits) de bits a la izquierda.
     *
     * @param ID Puntero a un arreglo dinamico con los valores RGB de la imagen BMP.
     * @param bits Cantidad de bits a rotar.
     * @param totalBytes Tamaño del arreglo.
     *
     * @return Puntero al nuevo arreglo dinámico que contiene la rotacion de 'ID'.
     *
     * @note La función no libera la memoria del arreglo ID; esta responsabilidad recae en el usuario.
     */

    // Copia 'ID' a 'transformacion' invocando la funcion copiar arreglo
    unsigned char *transformacion = copiarArreglo(ID, totalBytes);

    // Itera sobre 'transformacion' para rotar cada byte
    for (unsigned int t = 0; t < totalBytes; t++)
    {
        // Rota cada byte a la izquierda la cantidad de veces que se requiere
        for (int i = 0; i < bits; ++i)
        {
            unsigned char bit = (transformacion[t] & 0b10000000) >> 7;
            transformacion[t] = (transformacion[t] << 1) | bit;
        }
    }

    // Retorna el puntero al arreglo con los datos de 'ID' rotados
    return transformacion;
}

unsigned char *bmp::rotarDerecha(unsigned char *ID, unsigned short int bits, unsigned int totalBytes)
{
    /*
     * @brief Rota el arreglo 'ID' y lo almacena en un arreglo almacenado en el heap.
     *
     * Esta función copia el arreglo dinámico 'ID' a 'transformacion' y, rota cada byte del arreglo
     * una cantidad n (bits) de bits a la derecha.
     *
     * @param ID Puntero a un arreglo dinamico con los valores RGB de la imagen BMP.
     * @param bits Cantidad de bits a rotar.
     * @param totalBytes Tamaño del arreglo.
     *
     * @return Puntero al nuevo arreglo dinámico que contiene la rotacion de 'ID'.
     *
     * @note La función no libera la memoria del arreglo ID; esta responsabilidad recae en el usuario.
     */

    // Copia 'ID' a 'transformacion' invocando la funcion copiar arreglo
    unsigned char *transformacion = copiarArreglo(ID, totalBytes);

    // Itera sobre 'transformacion' para rotar cada byte
    for (unsigned int t = 0; t < totalBytes; t++)
    {
        // Rota cada byte a la derecha la cantidad de veces que se requiere
        for (int i = 0; i < bits; ++i)
        {
            unsigned char bit = (transformacion[t] & 0b00000001) << 7;
            transformacion[t] = (transformacion[t] >> 1) | bit;
        }
    }

    // Retorna el puntero al arreglo con los datos de 'ID' rotados
    return transformacion;
}

unsigned char *bmp::XOR(unsigned char *ID)
{
    /*
     * @brief Aplica una operación XOR al arreglo 'ID' usando una máscara cargada desde archivo.
     *
     * Esta función recibe el arreglo dinámico 'ID', luego carga la máscara 'IM' en un arreglo dinámico,
     * y aplica una operación XOR entre ambos arreglos. El resultado se almacena en un nuevo arreglo
     * dinámico llamado 'transformacion'.
     *
     * @param ID Puntero a un arreglo dinámico con los valores RGB de la imagen BMP.
     *
     * @return Puntero al nuevo arreglo dinámico que contiene el resultado del XOR entre 'ID' e 'IM'.
     *
     * @note La función no libera la memoria del arreglo 'ID'; esta responsabilidad recae en el usuario.
     */

    // Se carga IM
    QString I_M = "../../data/I_M.bmp";
    int height_IM = 0;
    int width_IM = 0;
    unsigned char *IM = loadPixels(I_M, width_IM, height_IM);

    // Reserva memoria dinamica para almacenar el XOR de 'ID' con 'IM'
    unsigned char *transformacion = new unsigned char[height_IM * width_IM * 3];

    // Itera sobre el tamaño de 'IM' y aplica el XOR en cada byte
    for (int i = 0; i < height_IM * width_IM * 3; i++)
    {
        transformacion[i] = ID[i] ^ IM[i];
    }

    // Libera la memoria de 'IM'
    delete[] IM;

    // Retorna el puntero al arreglo con el resultado del XOR entre 'ID' e 'IM'
    return transformacion;
}

unsigned char *bmp::desplazamientoDerecha(unsigned char *ID, unsigned short int bits, unsigned int totalBytes)
{
    /*
     * @brief Desplaza a la derecha el arreglo 'ID' y lo almacena en un arreglo almacenado en el heap.
     *
     * Esta función desplaza a la derecha cada byte del arreglo 'ID' una cantidad n (bits) de bits a la derecha.
     *
     * @param ID Puntero a un arreglo dinamico con los valores RGB de la imagen BMP.
     * @param bits Cantidad de bits a desplazar.
     * @param totalBytes Tamaño del arreglo.
     *
     * @return Puntero al nuevo arreglo dinámico que contiene el desplazamiento de 'ID'.
     *
     * @note La función no libera la memoria del arreglo ID; esta responsabilidad recae en el usuario.
     */

    // Reserva memoria dinamica para almacenar el desplazamiento de 'ID'
    unsigned char *transformacion = new unsigned char[totalBytes];

    // Desplaza a la derecha cada byte de 'ID' y lo almacena en transformacion
    for (unsigned int i = 0; i < totalBytes; ++i)
    {
        transformacion[i] = ID[i] >> bits;
    }

    // Retorna el puntero al arreglo con los datos de 'ID' desplazados
    return transformacion;
}

unsigned char *bmp::desplazamientoIzquierda(unsigned char *ID, unsigned short int bits, unsigned int totalBytes)
{
    /*
     * @brief Desplaza a la izquierda el arreglo 'ID' y lo almacena en un arreglo almacenado en el heap.
     *
     * Esta función desplaza  a la izquierda cada byte del arreglo 'ID' una cantidad n (bits) de bits a la derecha.
     *
     * @param ID Puntero a un arreglo dinamico con los valores RGB de la imagen BMP.
     * @param bits Cantidad de bits a desplazar.
     * @param totalBytes Tamaño del arreglo.
     *
     * @return Puntero al nuevo arreglo dinámico que contiene el desplazamiento de 'ID'.
     *
     * @note La función no libera la memoria del arreglo ID; esta responsabilidad recae en el usuario.
     */

    // Reserva memoria dinamica para almacenar el desplazamiento de 'ID'
    unsigned char *transformacion = new unsigned char[totalBytes];

    // Desplaza a la izquierda cada byte de 'ID' y lo almacena en transformacion
    for (unsigned int i = 0; i < totalBytes; ++i)
    {
        transformacion[i] = ID[i] << bits & 0xFF;
    }

    // Retorna el puntero al arreglo con los datos de 'ID' desplazados
    return transformacion;
}

bool bmp::verificarEnmascaramiento(unsigned char *ID, const char *name)
{
    /*
     * @brief Valida que la transformacion sea la adecuada.
     *
     *Esta función carga una máscara BMP 'IM' y un archivo de datos .txt 'maskingData' en arreglos dinámicos.
     *Luego, aplica una la funcion por enmascaramiento con el arreglo 'ID' y verifica que el resultado coincida
     *con los valores esperados almacenados en el archivo de texto.
     *
     * @param ID Puntero a un arreglo dinámico con los valores RGB de la imagen BMP.
     * @param name Nombre del archivo .txt.
     *
     * @return true si todos los valores transformados coinciden con los esperados; false en caso contrario.
     *
     * @note La función no libera la memoria del arreglo 'ID'; esta responsabilidad recae en el usuario.
     */

    // Se carga la mascara
    QString M = "../../data/M.bmp";
    int height_M = 0;
    int width_M = 0;
    unsigned char *mascara = loadPixels(M, width_M, height_M);

    // Se abre el archivo .txt
    int seed = 0;
    int n_pixels = 0;
    unsigned int *maskingData = loadSeedMasking(name, seed, n_pixels);

    // Se itera sobre el arreglo 'maskingData' que contiene la informacion del archivo .txt
    for(int k = 0; k < n_pixels*3;k++){
        // Calcula la transformacion sobre 'ID'
        unsigned int enmascaramiento = ID[seed + k] + mascara[k];

        // Valida si la transformacion no es igual al resultado almacenado en 'maskingData'
        if(maskingData[k] != enmascaramiento){
            // Se librera 'maskingData' y 'mascara' y se retorna falso dado que todos los valores transformados no son iguales a los esperados
            delete[] maskingData;
            delete[] mascara;
            return false;
        }
    }
    // Se librera 'maskingData' y 'mascara' y se retorna true dado que todos los valores transformados son iguales a los esperados
    delete[] maskingData;
    delete[] mascara;
    return true;
}

int bmp::contarArchivosMascara(const QString &rutaDirectorio)
{
    /*
     * @brief Cuenta la cantidad de archivos de texto con nombre que inicia por 'M' en un directorio dado.
     *
     * Esta función recorre un directorio especificado y cuenta cuántos archivos de texto existen
     * cuyo nombre comienza con 'M' y tiene extensión '.txt'.
     *
     * @param rutaDirectorio Ruta del directorio a explorar.
     *
     * @return Número de archivos encontrados que cumplen con el patrón 'M*.txt'.
     */

    // Carga el directorio
    QDir directorio(rutaDirectorio);

    // Genera una lista con los archivos que comiencen por 'M' y terminen por '.txt'
    QStringList archivos = directorio.entryList(QStringList() << "M*.txt", QDir::Files);

    // Retorna el tamaño de la lista (Cantidad de archivos que comiencen por 'M' y terminen por '.txt')
    return archivos.size();
}
