#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "heap.cpp"

using namespace std;

#define paradas 7

struct pedido {
    int inicio, fin, pasajeros;
};

bool cmpBeneficio(pedido p1, pedido p2){
    int b1 = (p1.fin - p1.inicio) * p1.pasajeros;
    int b2 = (p2.fin - p2.inicio) * p2.pasajeros;
    return b1 > b2;
}

bool cabePedido(int ocupacion[], pedido p, int tren)
{
    for(int i = p.inicio; i <= p.fin ; i++){
        if(ocupacion[i]+p.pasajeros > tren){
            return false;
        }
    }
    return true;
}

void actOcupacion(int ocupacion[], pedido p)
{
    for (int i = p.inicio; i <= p.fin; i++)
    {
        ocupacion[i] = ocupacion[i] + p.pasajeros;
    }
}

int lowerBound(vector<pedido> lista,int ocupacion[], int ultimo, int beneficio, int tren)
{
    int lb = beneficio;
    struct paux;
    for(int i = ultimo; i < lista.size(); i++){
        paux = lista[i];
        if(cabePedido(ocupacion,paux,tren)){
            lb = lb + (paux.fin - paux.inicio) * paux.pasajeros;
            actOcupacion(ocupacion[], paux);
        }
    }
}

void actOcupacion(int ocupacion[], int ini, int fin, int p)
{
    for (int i = ini; i <= fin; i++)
    {
        ocupacion[i] = ocupacion[i] + p;
    }
}

int faltan(int ocupacion[], int fin, int ini, int tren)
{
    int sum = 0, max = ocupacion[ini];
    for (int i = ini; i <= fin; i++)
    {
        if(max < ocupacion[i]){ max = ocupacion[i]; }
    }
    return tren - max;
}

int costeEstimado(vector<pedido> lista, int ocupacion[], int ultimo, int beneficio, int tren)
{
    int lb = beneficio;
    struct paux;
    for (int i = ultimo; i < lista.size(); i++)
    {
        paux = lista[i];
        if (cabePedido(ocupacion, paux, tren))
        {
            lb = lb + (paux.fin - paux.inicio) * paux.pasajeros;
            actOcupacion(ocupacion[], paux);
        }else{
            int caben = faltan(ocupacion, paux.fin,paux.inicio, tren);
            lb = lb + (paux.fin - paux.inicio) * paux.pasajeros;
            actOcupacion(ocupacion[], paux.inicio, paux.fin, caben);
        }
    }
}

int main(int argc, char *argv[]){
    
    if(argc >= 2){
        
        ifstream entrada(argv[1]);
        if(!entrada.is_open()){
            cerr << "EL fichero de entrada no se ha podido abrir.\n";
            return -1;
        }
        
        int tren, estaciones, pedidos;
        entrada >> tren >> estaciones >> pedidos;

        cout << "Datos problema: " << tren << ", " << estaciones << ", " << pedidos << endl; 
        //struct pedido lista[pedidos];
        vector<pedido> lista(pedidos);

        for(int i = 0; i < pedidos; i++){
            entrada >> lista[i].inicio >> lista[i].fin >> lista[i].pasajeros;
        }

        sort(lista.begin(), lista.end(), cmpBeneficio);
        
        int actualLB = 0;
        Heap nodos_vivos();

        int coste, lower_bound;

        Nodo raiz();

        lower_bound = lowerBound(lista,raiz.ocupacion,0,tren);
        coste = costeEstimado(lista, raiz.ocupacion, 0,tren);

        raiz.coste_estimado = coste;
        raiz.lb = lower_bound;

        nodos_vivos.push(raiz);

        while(!nodos_vivos.esVacia())
        {

        }
        
        //for(int i = 0; i < pedidos; i++){
        //    cout << "{" << (lista[i].fin - lista[i].inicio) * lista[i].pasajeros << "}, ";
        //}

        entrada.close();

    }

    return 0;
}