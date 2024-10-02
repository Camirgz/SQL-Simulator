// Clase para procesar una consulta SQL y extraer información de un archivo CSV
#include "../include/ConsultaSQL.h"
using namespace std;

// Constructor
ConsultaSQL::ConsultaSQL() : numColumnas(0), seleccionarTodas(false) {}

// Método para procesar la consulta SQL
void ConsultaSQL::procesarConsulta(const string& consulta) {
    
    // Variables de búsqueda de palabras clave en la consulta
    size_t posSelect = consulta.find("SELECT");
    size_t posFrom = consulta.find(" FROM ");
    size_t endPosCSV = consulta.find(".csv", posFrom) + 4; 
    size_t posDistinct = consulta.find("SELECT DISTINCT");
    size_t posMin = consulta.find("SELECT MIN");
    size_t posMax = consulta.find("SELECT MAX");
    size_t posCount = consulta.find("SELECT COUNT");
    size_t posSum = consulta.find("SELECT SUM");
    size_t posAvg = consulta.find("SELECT AVG");
    size_t posWhere = consulta.find(" WHERE ");
    bool existeColumna;
    string comando;

    // Variables para almacenar las columnas y el archivo y validar la consulta
    string columnasStr = "";
    if (posSelect == string::npos || posFrom == string::npos) {
        cerr << "Error: La consulta SQL no es válida." << endl;
        return;
    }

    // Extraer el nombre del archivo CSV
    string archivoStr = consulta.substr(posFrom + 6, endPosCSV - (posFrom + 6));
    archivo = "../db/" + archivoStr;
    cout << "Archivo: " << archivo << endl;
    lista.leerArchivoCSV(archivo);
    
    // Procesar la consulta SQL
    if (soloSelect(consulta)) {
        if (posSelect != string::npos) {
            string columnasStr = consulta.substr(posSelect + 7, posFrom - (posSelect + 7)); // 7 es la longitud de "SELECT "
            // Procesar si es SELECT *
            if (columnasStr == "*") {
                seleccionarTodas = true;
                extraerColumnas("*");
                lista.imprimirLista(numColumnas, columnas, true);
            }
            // Procesar si es SELECT columna, columna
            else {
                seleccionarTodas = false;
                extraerColumnas(columnasStr);
                lista.imprimirLista(numColumnas, columnas, false);
            }
        }
    }

    
    //SELECT DISTINCT columna FROM archivo
    if (posDistinct != string::npos) {
        columnasStr = consulta.substr(posSelect + 16, posFrom - (posSelect + 16));    
        extraerColumnas(columnasStr);
        for (int i = 0; i < numColumnas; ++i) {
            string* columna = lista.getColumna(columnas[i], &existeColumna); 
            if (!existeColumna) {
                continue;
            }
            cout << "Columna: " << columnas[i] << endl;
            procesarDistinct(columna);
        }
        return;
    }  
    //SELECT columna FROM archivo WHERE columna = parametro
    else if (posWhere != string::npos) {
        //Variables para procesar la consulta con WHERE
        size_t posIgual = consulta.find("=");
        string columna = consulta.substr(posWhere + 7, posIgual - (posWhere + 7));
        string parametro = consulta.substr(posIgual + 1);
        //Impresion de variables
        cout << "Columna: " << columna << endl;
        cout << "Parametro: " << parametro << endl;

        string* columnaData = lista.getColumna(columna, &existeColumna);  // Obtener los datos de la columna

        //Ciclo para recorrer la lista de registros y mostrar los datos
        if (existeColumna) {
          for (int i = 0; i < lista.numFilas; ++i) {
            if (columnaData[i] == parametro) {
                string* fila = lista.getFila(columnaData[i]);
                for (int j = 0; j < lista.cabeza->numColumnas; ++j) {
                    cout << fila[j] << " ";
                }
                cout << endl;
                return;
            }
            }
        }
       
    }
    //SELECT MIN(columna) FROM archivo
    else if (posMin != string::npos) {
         columnasStr = consulta.substr(posSelect + 11, posFrom - (posSelect + 11));    
         extraerColumnas(columnasStr);
         for (int i = 0; i < numColumnas; ++i) {
            string* columna = lista.getColumna(columnas[i], &existeColumna); 
            if (!existeColumna) {
                continue;
            }
            double minimo = getMin(columna);
            if (minimo == -999) {
                cout << "La columna *" << columnas[i] << "* no es numerica" << endl;
            } else {
                cout << "El valor mínimo de la columna " << columnas[i] << " es: " << minimo << endl;
            }
         }
    //SELECT MAX(columna) FROM archivo
    } else if (posMax != string::npos) {
        columnasStr = consulta.substr(posSelect + 11, posFrom - (posSelect + 11));    
        extraerColumnas(columnasStr);
        for (int i = 0; i < numColumnas; ++i) {
            string* columna = lista.getColumna(columnas[i], &existeColumna); 
            if (!existeColumna) {
                continue;
            }
            double maximo = getMax(columna);
            if (maximo == -999) {
                cout << "La columna *" << columnas[i] << "* no es numerica" << endl;
            } else {
                cout << "El valor máximo de la columna " << columnas[i] << " es: " << maximo << endl;
            }
        }
    //SELECT COUNT(columna) FROM archivo
    } else if (posCount != string::npos) {
        columnasStr = consulta.substr(posSelect + 13, posFrom - (posSelect + 13));   
        extraerColumnas(columnasStr);
        for (int i = 0; i < numColumnas; ++i) {
            string* columna = lista.getColumna(columnas[i], &existeColumna); 
            if (!existeColumna) {
                continue;
            }
            cout << "El número de registros de la columna " << columnas[i] << " es: " << lista.numFilas << endl;
        } 
    //SELECT SUM(columna) FROM archivo
    } else if (posSum != string::npos) {
        columnasStr = consulta.substr(posSelect + 11, posFrom - (posSelect + 11));   
        extraerColumnas(columnasStr);
        for (int i = 0; i < numColumnas; ++i) {
            string* columna = lista.getColumna(columnas[i], &existeColumna); 
            if (!existeColumna) {
                continue;
            }
            double suma = 0.0;
            suma = getSum(columna);
            if (suma == -999) {
                cout << "La columna *" << columnas[i] << "* no es numerica" << endl;
            } else {
                cout << "La suma de la columna " << columnas[i] << " es: " << suma << endl;
            }
        }
    //SELECT SUM(columna) FROM archivo
    } else if (posSum != string::npos) {
        columnasStr = consulta.substr(posSelect + 11, posFrom - (posSelect + 11));   
        extraerColumnas(columnasStr);
        for (int i = 0; i < numColumnas; ++i) {
            string* columna = lista.getColumna(columnas[i], &existeColumna); 
            if (!existeColumna) {
                continue;
            }
            double suma = 0.0;
            suma = getSum(columna);
            if (suma == -999) {
                cout << "La columna *" << columnas[i] << "* no es numerica" << endl;
            } else {
                cout << "La suma de la columna " << columnas[i] << " es: " << suma << endl;
            }
        }
    //SELECT AVG(columna) FROM archivo
    } else if (posAvg != string::npos) {
        columnasStr  = consulta.substr(posSelect + 11, posFrom - (posSelect + 11));  
        extraerColumnas(columnasStr);
        for (int i = 0; i < numColumnas; ++i) {
            string* columna = lista.getColumna(columnas[i], &existeColumna); 
            if (!existeColumna) {
                continue;
            }
            double suma = 0.0;
            suma = getSum(columna);
            if (suma == -999) {
                cout << "La columna ==" << columnas[i] << "¿== no es numerica" << endl;
            } else {
                cout << "El promedio de la columna " << columnas[i] << " es: " << suma / lista.numFilas << endl;
            }
        }
    }


}

// Método para procesar la consulta DISTINCT
void ConsultaSQL::procesarDistinct(const string* columna) {
    for (int i = 0; i < lista.numFilas; ++i) {
        bool repetido = false;
        for (int j = 0; j < i; ++j) {
            if (columna[i] == columna[j]) {
                repetido = true;
                break;
            }
        }
        if (!repetido) {
            cout << columna[i] << endl;
        }
    }
    cout << endl;
}

// Método para imprimir la consulta SQL
bool ConsultaSQL::consulta() {
    cout << "Archivo: " << archivo << endl;
    if (seleccionarTodas) {
        cout << "Columnas: Todas (*)" << endl;
    } else {
        cout << "Columnas seleccionadas: ";
        for (int i = 0; i < numColumnas; ++i) {
            cout << columnas[i] << " ";
        }
        cout << "\n" << endl;
    }
    return true;
}

// Método para recorrer la lista de registros y mostrar el mínimo
double ConsultaSQL::getMin(const string* columnasStr) {
    double minimo = 0.0;
    int contador = 0;
    try {
        minimo = stod(columnasStr[0]);
        double valorActual = 0.0;

        for (int i=0; i< lista.numFilas; ++i) {
            valorActual = stod(columnasStr[i]);
            if (valorActual < minimo) {
                minimo = valorActual;
            }
            contador++;
        }
    } catch(...) {
        // cout << columnasStr[contador]<< " no es numero" << endl;
        return -999;}
    return minimo;
}

// Método para extraer las columnas de la consulta SQL en un SELECT
void ConsultaSQL::extraerColumnas(const string columnasStr) {
    
    // Extraer las columnas entre SELECT y FROM
    if (columnasStr == "*") {
        seleccionarTodas = true;
        numColumnas = lista.cabeza->numColumnas;
        for (int i = 0; i < numColumnas; ++i) {
            columnas[i] = lista.cabeza->valores[i];
        }

    } else {
        seleccionarTodas = false;
        size_t start = 0;
        size_t end = 0;
        numColumnas = 0;

        // Extraer las columnas individuales
        while ((end = columnasStr.find(",", start)) != string::npos) {
            columnas[numColumnas++] = columnasStr.substr(start, end - start);
            start = end + 1;  // Avanzar al siguiente carácter después de la coma
        }
        columnas[numColumnas++] = columnasStr.substr(start); // Para la última columna
        this->numColumnas = numColumnas;
    }
    
}

// Método para recorrer la lista de registros y mostrar el máximo
double ConsultaSQL::getMax(const string* columnasStr) {
    double maximo = 0.0;
    int contador = 0;
    try {
        maximo = stod(columnasStr[0]);
        double valorActual = 0.0;

        for (int i=0; i< lista.numFilas; ++i) {
            valorActual = stod(columnasStr[i]); //Stod: string to double
            if (valorActual > maximo) {
                maximo = valorActual;
            }
            contador++;
        }
    } catch(...) {
        return -999;} //Si no es un numero
    return maximo;
}

// Método para recorrer la lista de registros y mostrar la suma
double ConsultaSQL::getSum(const string* columnasStr) {
    double suma = 0.0;
    int contador = 0;
    try {
        double valorActual = 0.0;

        for (int i=0; i< lista.numFilas; ++i) {
            valorActual = stod(columnasStr[i]);
            suma += valorActual;
            contador++;
        }
    } catch(...) {
        return -999;} //Si no es un numero
    return suma;
}

bool ConsultaSQL::soloSelect(const string& consulta) {
    size_t posSelect = consulta.find("SELECT");
    size_t posFrom = consulta.find(" FROM ");
    size_t posDistinct = consulta.find("SELECT DISTINCT");
    size_t posMin = consulta.find("SELECT MIN");
    size_t posMax = consulta.find("SELECT MAX");
    size_t posCount = consulta.find("SELECT COUNT");
    size_t posSum = consulta.find("SELECT SUM");
    size_t posAvg = consulta.find("SELECT AVG");
    size_t posWhere = consulta.find(" WHERE ");

    if (posSelect != string::npos && posFrom != string::npos && posDistinct == string::npos 
    && posMin == string::npos && posMax == string::npos && posCount == string::npos && 
    posSum == string::npos && posAvg == string::npos && posWhere == string::npos) {
        return true;
    }
    return false;
}