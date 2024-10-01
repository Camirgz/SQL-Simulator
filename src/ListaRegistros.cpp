#include "../include/ListaRegistros.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

// Método para leer el archivo CSV y crear registros dinámicamente
void ListaRegistros::leerArchivoCSV(const string &rutaArchivo)
{
    ifstream archivo(rutaArchivo);
    if (!archivo.is_open())
    {
        cerr << "No se pudo abrir el archivo." << endl;
        return;
    }

    string linea;
    bool esPrimeraLinea = true;
    int numColumnas = 0;
    Registro *actual = nullptr;
    numFilas = 0; // Inicializar el contador de filas

    // Leer el archivo línea por línea
    while (getline(archivo, linea))
    {
        stringstream ss(linea);
        string valor;

        // Si es la primera línea, contamos las columnas (encabezados)
        if (esPrimeraLinea)
        {
            while (getline(ss, valor, ','))
            {
                numColumnas++;
            }
            esPrimeraLinea = false;

            // Resetear el stream para procesar la primera línea de nuevo
            ss.clear();
            ss.str(linea);
        }

        // Crear un nuevo objeto Registro con el número de columnas
        Registro *nuevoRegistro = new Registro(numColumnas);

        // Asignar los valores de la línea actual al nuevo registro
        for (int i = 0; i < numColumnas; ++i)
        {
            getline(ss, nuevoRegistro->valores[i], ',');
        }

        // Conectar el nuevo registro a la lista
        if (cabeza == nullptr)
        {
            cabeza = nuevoRegistro; // Primer registro en la lista
        }
        else
        {
            actual->siguiente = nuevoRegistro; // Conectar al siguiente nodo
            numFilas++;
        }
        actual = nuevoRegistro; // Actualizar el puntero actual

        // Incrementar el contador de filas
       
    }
    this->numFilas = numFilas;

    archivo.close();
}

// Método para imprimir toda la lista de registros
void ListaRegistros::imprimirLista(int numcolumnas, string *columnas, bool imprimirTodas) const
{
    if (imprimirTodas)
    {
        Registro *actual = cabeza;
        while (actual != nullptr)
        {
            actual->imprimir();
            actual = actual->siguiente;
        }
    }
    else
    {
        // Imprimir las columnas seleccionadas
        for (int i = 0; i < numcolumnas; ++i)

        {

            // imprimir el nombre de la columna

            cout << columnas[i] << "\n"

                 << endl;

            Registro *actual = cabeza;

            // Encontrar la columna seleccionada en la primera linea

            int contador = 0;

            for (int j = 0; j < numcolumnas; ++j)

            {

                // si el valor de la columna no es igual a la columna seleccionada se aumenta el contador

                if (actual->valores[j] != columnas[i])

                {

                    contador++;
                }

                else

                {

                    break;
                }
            }

            // saltarse la primera linea

            actual = actual->siguiente;

            // Imprimir los registros uno por uno usando el contador para saber que columna imprimir

            while (actual != nullptr)

            {

                actual->imprimir(contador);

                actual = actual->siguiente;
            }

            cout << endl;
        }
    }
}

// Método para obtener una columna específica de la lista de registros
std::string* ListaRegistros::getColumna(std::string nombreColumna, bool* hola) const {
    *hola = true;
    // Encontrar la columna seleccionada en la primera fila (nombre de las columnas)
    Registro *actual = cabeza;
    int numColumna = -1;  // Inicializa en -1 para detectar si no se encuentra la columna

    for (int j = 0; j < actual->numColumnas; ++j) {
        // Si el valor de la columna es el que se busca, almacenamos el índice y salimos
        if (actual->valores[j] == nombreColumna) {
            numColumna = j;
            break;
        }
    }

    // Verifica si se encontró la columna
    if (numColumna == -1) {
        *hola = false;
        std::cerr << "Error: No se encontró la columna *" << nombreColumna << "*"<< std::endl;
        return nullptr;
    }

    // Saltarse la primera fila (nombres de columnas) para obtener los valores de los registros
    actual = actual->siguiente;

    

    // Crear un arreglo dinámico para almacenar los valores de la columna
    std::string *columnaArray = new std::string[numFilas];
    int index = 0;

    // Almacenar los valores de la columna en el arreglo
    while (actual != nullptr) {
        columnaArray[index++] = actual->valores[numColumna];
        actual = actual->siguiente;
    }

    return columnaArray;
}

