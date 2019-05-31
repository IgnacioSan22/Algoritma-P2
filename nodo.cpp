#include <iostream>
#include <vector>

using namespace std;

#ifndef _NODO_
#define _NODO_

class Nodo {
public:
    int coste_estimado;
    int lb;
    int order;
    vector<int> ocupacion;
    int beneficio_actual;

    Nodo(){
        
    }

    Nodo(int coste_estimado, int lb, int order, vector<int> ocupacion, int beneficio ){
        this->coste_estimado = coste_estimado;
        this->lb = lb;
        this->order = order;
        this->ocupacion = ocupacion;
        this->beneficio_actual = beneficio;
    }

    bool operator>(const Nodo& n){
        return this->coste_estimado > n.coste_estimado;
    }
};

#endif