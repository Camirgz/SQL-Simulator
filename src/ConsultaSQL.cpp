#include "../include/ConsultaSQL.h"

using namespace std;

ConsultaSQL::ConsultaSQL() : numColumnas(0), seleccionarTodas(false) {}

// Método para procesar la consulta SQL
void ConsultaSQL::procesarConsulta(const string& consulta) {
    
    // Variables de apoyo
    size_t posSelect = consulta.find("SELECT ");
    size_t posFrom = consulta.find(" FROM ");
    size_t posDistinct = consulta.find("SELECT DISTINCT");
    size_t posMin = consulta.find("SELECT MIN");
    size_t posMax = consulta.find("SELECT MAX");
    size_t posCount = consulta.find("SELECT COUNT");
    size_t posSum = consulta.find("SELECT SUM");
    size_t posAvg = consulta.find("SELECT AVG");
    bool existeColumna;
    string comando;

    string columnasStr = "";

    if (posSelect == string::npos || posFrom == string::npos) {
        cerr << "Error: La consulta SQL no es válida." << endl;
        return;
    }
    string archivoStr = consulta.substr(posFrom + 6); // 6 caracteres de " FROM "

    // Añadir la ruta de ../db/ al nombre del archivo
    archivo = "../db/" + archivoStr;

    lista.leerArchivoCSV(archivo);
    
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
                cout << "La columna *" << columnas[i] << "* no es numerica" << endl;
            } else {
                cout << "El promedio de la columna " << columnas[i] << " es: " << suma / lista.numFilas << endl;
            }
        }
    }


}

void ConsultaSQL::procesarDistinct(const std::string* columna) {
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


double ConsultaSQL::getMin(const std::string* columnasStr) {
    double minimo = 0.0;
    int contador = 0;
    try {
        minimo = std::stod(columnasStr[0]);
        double valorActual = 0.0;

        for (int i=0; i< lista.numFilas; ++i) {
            valorActual = std::stod(columnasStr[i]);
            if (valorActual < minimo) {
                minimo = valorActual;
            }
            contador++;
        }
    } catch(...) {
        // std::cout << columnasStr[contador]<< " no es numero" << std::endl;
        return -999;}
    return minimo;
}

void ConsultaSQL::extraerColumnas(const std::string columnasStr) {
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

double ConsultaSQL::getMax(const std::string* columnasStr) {
    double maximo = 0.0;
    int contador = 0;
    try {
        maximo = std::stod(columnasStr[0]);
        double valorActual = 0.0;

        for (int i=0; i< lista.numFilas; ++i) {
            valorActual = std::stod(columnasStr[i]);
            if (valorActual > maximo) {
                maximo = valorActual;
            }
            contador++;
        }
    } catch(...) {
        // std::cout << columnasStr[contador]<< " no es numero" << std::endl;
        return -999;}
    return maximo;
}

double ConsultaSQL::getSum(const std::string* columnasStr) {
    double suma = 0.0;
    int contador = 0;
    try {
        double valorActual = 0.0;

        for (int i=0; i< lista.numFilas; ++i) {
            valorActual = std::stod(columnasStr[i]);
            suma += valorActual;
            contador++;
        }
    } catch(...) {
        // std::cout << columnasStr[contador]<< " no es numero" << std::endl;
        return -999;}
    return suma;
}