#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <time.h>
#include <iomanip>
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

bool cabePedido(const vector<int>& ocupacion, pedido p, int tren)
{
    for(int i = p.inicio; i <= p.fin ; i++){
        if(ocupacion[i]+p.pasajeros > tren){
            return false;
        }
    }
    return true;
}

void actOcupacion(vector<int>& ocupacion, pedido p)
{
    for (int i = p.inicio; i <= p.fin; i++)
    {
        ocupacion[i] = ocupacion[i] + p.pasajeros;
    }
}

int lowerBound(vector<pedido> lista, vector<int> ocupacion, int ultimo, int beneficio, int tren)
{
    int lb = beneficio;
    struct pedido paux;
    for(int i = ultimo; i < lista.size(); i++){
        paux = lista[i];
        if(cabePedido(ocupacion,paux,tren)){
            lb = lb + (paux.fin - paux.inicio) * paux.pasajeros;
            actOcupacion(ocupacion, paux);
        }
    }
    return lb;
}

void actOcupacion(vector<int>& ocupacion, int ini, int fin, int p)
{
    for (int i = ini; i <= fin; i++)
    {
        ocupacion[i] = ocupacion[i] + p;
    }
}

int faltan(const vector<int>& ocupacion, int fin, int ini, int tren)
{
    int sum = 0, max = ocupacion[ini];
    for (int i = ini; i <= fin; i++)
    {
        if(max < ocupacion[i]){ max = ocupacion[i]; }
    }
    return tren - max;
}

int costeEstimado(vector<pedido> lista, vector<int> ocupacion, int ultimo, int beneficio, int tren)
{
    int lb = beneficio;
    struct pedido paux;
    for (int i = ultimo; i < lista.size(); i++)
    {
        paux = lista[i];
        if (cabePedido(ocupacion, paux, tren))
        {
            lb = lb + (paux.fin - paux.inicio) * paux.pasajeros;
            actOcupacion(ocupacion, paux);
        }else{
            int caben = faltan(ocupacion, paux.fin,paux.inicio, tren);
            lb = lb + (paux.fin - paux.inicio) * paux.pasajeros;
            actOcupacion(ocupacion, paux.inicio, paux.fin, caben);
        }
    }
    return lb;
}

bool factible(const vector<int>& oc, int tren){
    for (int i = 0; i < oc.size(); i++)
    {
        if (oc[i] > tren)
        {
            return false;
        }
    }
    return true;
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

        //Inicio real de la resolucion del programa
        clock_t start, end;
        start = clock();

        sort(lista.begin(), lista.end(), cmpBeneficio);
        
        int actualLB = 0, mejor_sol = 0;
        Heap nodos_vivos;

        int coste, lower_bound;
        vector<int> ocupacion_aux(7,0);

        Nodo raiz(0, 0, 0, ocupacion_aux, 0);

        lower_bound = lowerBound(lista,raiz.ocupacion,0,0,tren);
        coste = costeEstimado(lista, raiz.ocupacion, 0,0,tren);

        cout << "Primer coste estimado: " << coste << endl;
        cout << "Primer LB: " << lower_bound << endl;

        actualLB = lower_bound;

        raiz.coste_estimado = coste;
        raiz.lb = lower_bound;

        nodos_vivos.push(raiz);

        int ben_aux, i = 0, iter = 0;
        Nodo aux, izq, der;
        struct pedido paux;

        while(!nodos_vivos.esVacia() /*&& iter < 6*/)
        {
            iter++;
            //cout << "En el bucle\n";
            aux = nodos_vivos.cima(); nodos_vivos.pop();

            i = aux.order;

            //creacion hijo izquierdo, es decir, se acepta siguiente peticion
            //cout << "-------Hijo izq----------\n";
            paux = lista[i];
            ocupacion_aux = aux.ocupacion;
            actOcupacion(ocupacion_aux,paux);
            /*for(int i = 0; i < 7; i++)
            {
                cout << "{" << ocupacion_aux[i] << "}";
            }
            cout << endl;*/
            if(factible(ocupacion_aux,tren)){
                //cout << "Nodo factible\n";
                ben_aux = aux.beneficio_actual + ((paux.fin - paux.inicio) * paux.pasajeros);
                coste = costeEstimado(lista,ocupacion_aux,i+1,ben_aux,tren);
                lower_bound = lowerBound(lista,ocupacion_aux,i+1,ben_aux,tren);
                //cout << "Hijo izq coste estimado: " << coste << endl;
                //cout << "Hijo izq LB: " << lower_bound << endl;
                if (coste >= actualLB){
                    if(i < (pedidos-1)){
                        izq = Nodo(coste, lower_bound, i+1, ocupacion_aux, ben_aux);
                        nodos_vivos.push(izq);
                    }
                    if (lower_bound > actualLB){
                        actualLB = lower_bound;
                    }
                    //if (i == (pedidos-1) && mejor_sol < lower_bound)
                    if (coste == lower_bound)
                    {
                        mejor_sol = coste;
                    }
                }
            }
            //cout << "---------------------\n";

            //creacion hijo derecho, es decir, se rechaza siguiente peticion
            //cout << "-------Hijo der----------\n";
            ocupacion_aux = aux.ocupacion;
            /*for (int i = 0; i < 7; i++)
            {
                cout << "{" << ocupacion_aux[i] << "}";
            }
            cout << endl;*/
            ben_aux = aux.beneficio_actual;
            coste = costeEstimado(lista, ocupacion_aux, i+1, ben_aux, tren);
            lower_bound = lowerBound(lista, ocupacion_aux, i+1, ben_aux, tren);
            //cout << "Hijo der coste estimado: " << coste << endl;
            //cout << "Hijo der LB: " << lower_bound << endl;
            if (coste >= actualLB)
            {
                if (i < (pedidos - 1))
                {
                    der = Nodo(coste, lower_bound, i+1, ocupacion_aux, ben_aux);
                    nodos_vivos.push(der);
                }
                if (lower_bound > actualLB)
                {
                    actualLB = lower_bound;
                }
                //if (i == (pedidos - 1) && mejor_sol < lower_bound)
                if (coste == lower_bound)
                {
                    mejor_sol = coste;
                }
            }
            //cout << "---------------------\n";
        }
        end = clock();

        double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
        cout << "Time taken by program is : " << fixed
             << time_taken << setprecision(10);
        cout << " sec " << endl;

        entrada.close();

        cout << "La mejor solucion encontrada ha sido: " << mejor_sol << endl;
    }

    return 0;
}