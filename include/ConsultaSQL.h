#ifndef CONSULTA_SQL_H
#define CONSULTA_SQL_H

#include <iostream>
#include <string>
#include "Registro.h"
#include "ListaRegistros.h"

using namespace std;

class ConsultaSQL {
public:
    static const int MAX_COLUMNAS = 100;
    string archivo;                  // Nombre del archivo extraído de la consulta (con la ruta completa)
    string columnas[15];             // Arreglo para almacenar hasta 10 columnas
    int numColumnas;                 // Número de columnas seleccionadas
    bool seleccionarTodas; 
    ListaRegistros lista;

    ConsultaSQL();

    void procesarConsulta(const std::string& consulta);
    bool consulta();
    void procesarDistinct(const std::string* columna);
    double getMin(const std::string* columna);
    void recorrer() const;
    double getMax(const std::string* columna);
    void extraerColumnas(const std::string columnasStr);
    double getSum(const std::string* columna);
    bool soloSelect(const std::string& consulta);
};

#endif // CONSULTA_SQL_H
