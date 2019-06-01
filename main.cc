#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <time.h>
#include <iomanip>
#include "heap.cpp"

using namespace std;

#define paradas 7

/*
 * Estructura que representa un pedido mediante sua parada de inicio, de fin
 * y el número de pasajeros que contiene.
 */
struct pedido {
    int inicio, fin, pasajeros;
};

/*
 * Pre: p1 != null && p2 != null
 * Post: Devuelve verdadero si el beneficio de p1 es mayor al de p2, falso
 * en caso contrario.
 */
bool cmpBeneficio(pedido p1, pedido p2){
    int b1 = (p1.fin - p1.inicio) * p1.pasajeros;
    int b2 = (p2.fin - p2.inicio) * p2.pasajeros;
    return b1 > b2;
}

/*
 * Pre: !ocupacion.isEmpty() && ocupacion.size() <= (p.fin - p.inicio)
 * Post: Devuelve verdadero en caso de que añadir el pedido a la ocupación no
 * exceda la capacidad del tren, falso en caso contrario.
 */
bool cabePedido(const vector<int>& ocupacion, pedido p, int tren)
{
    for(int i = p.inicio; i <= p.fin ; i++){
        if(ocupacion[i]+p.pasajeros > tren){
            return false;
        }
    }
    return true;
}

/*
 * Pre: ocupacion != null
 * Post: añade el pedido p a la ocupación del tren
 */
void actOcupacion(vector<int>& ocupacion, pedido p)
{
    for (int i = p.inicio; i < p.fin; i++)
    {
        ocupacion[i] = ocupacion[i] + p.pasajeros;
    }
}

/*
 * Pre: lista != null && ocupacion != null
 * Post: Devuelve el coste de añadir secuencialmente todos los pedidos de lista
 * mientras no se exceda la capacidad del tren
 */
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

// ESTE HACE FALTA ??
void actOcupacion(vector<int>& ocupacion, int ini, int fin, int p)
{
    for (int i = ini; i <= fin; i++)
    {
        ocupacion[i] = ocupacion[i] + p;
    }
}

/*
 * Pre: true
 * Post: Devuelve el número de plazas libres hasta el momento en el punto de
 * mayor ocupación entre las paradas ini y fin
 */
int faltan(const vector<int>& ocupacion, int fin, int ini, int tren)
{
    int sum = 0, max = ocupacion[ini];
    for (int i = ini; i <= fin; i++)
    {
        if(max < ocupacion[i]){ max = ocupacion[i]; }
    }
    return tren - max;
}

/*
 * Pre: lista != null && ocupacion != null
 * Post: Devuelve el coste de añadir secuencialmente todos los pedidos de lista
 * mientras no se exceda la capacidad del tren, permitiendo fraccionar pedidos
 */
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

/*
 * Pre: vector != null
 * Post: Devuelve verdadero en caso de que ninguna de las componentes de oc 
 * (ocupacion) exceda la capacidad del tren.
 */
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

/*
 * Pre: entrada.is_open() && pedidos <= entrada.numLines()
 * Post: Dada una lista de pedidos de una estación individual para un tren de
 * capacidad = tren, número de estaciones = estaciones y número de pedidos
 * = pedidos, calcula mediante un algoritmo de ramificación y poda el máximo
 * ingreso obtenible que no exceda las capacidades de la línea.
 */
void resolver(int tren, int estaciones, int pedidos, ifstream& entrada) {
    cout << "-----------------\n";
    cout << "Datos problema: " << tren << ", " << estaciones << ", " << pedidos << endl; 
    //struct pedido lista[pedidos];

    /* Declaración de variables */
    vector<pedido> lista;
    int begin, fin, pasj, elem = 0;
    struct pedido p_auxiliar;

    /* Si una lista incluye más de 0 pedidos, se hace el cálculo */
    if(pedidos > 0){

        /* Creación de la lista de pedidos */
        for(int i = 0; i < pedidos; i++){
            entrada >> begin >> fin >> pasj;
            if(pasj <= tren){
                p_auxiliar.inicio = begin; p_auxiliar.fin = fin; p_auxiliar.pasajeros = pasj;
                lista.push_back(p_auxiliar);
            }
        }
        pedidos = lista.size();
        
        clock_t start, end;
        start = clock();

        /* Si se puede aceptar algún pedido, se hace el cálculo */
        if(pedidos > 0){
        //Inicio real de la resolucion del programa

            /* Se ordena la lista por beneficio */
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
        //if(lower_bound > 0){
            /* Creación del árbol como cola de prioridades y parametrización */
            actualLB = lower_bound;
            raiz.coste_estimado = coste;
            raiz.lb = lower_bound;

            nodos_vivos.push(raiz);

            int ben_aux, i = 0, iter = 0;
            Nodo aux, izq, der;
            struct pedido paux;

            /* La búsqueda termina cuando se han podado todas las opciones */
            while(!nodos_vivos.esVacia())
            {
                iter++;
                aux = nodos_vivos.cima(); nodos_vivos.pop();

                i = aux.order;

                /* Creación hijo izquierdo, es decir, se acepta siguiente petición */
                paux = lista[i];
                ocupacion_aux = aux.ocupacion;
                actOcupacion(ocupacion_aux,paux);

                /* Si es factible, se añade a la solución y se actualizan los parámetros */
                if(factible(ocupacion_aux,tren)){
                    ben_aux = aux.beneficio_actual + ((paux.fin - paux.inicio) * paux.pasajeros);
                    coste = costeEstimado(lista,ocupacion_aux,i+1,ben_aux,tren);
                    lower_bound = lowerBound(lista,ocupacion_aux,i+1,ben_aux,tren);
                    if (coste >= actualLB){
                        if(i < (pedidos-1)){
                            izq = Nodo(coste, lower_bound, i+1, ocupacion_aux, ben_aux);
                            nodos_vivos.push(izq);
                        }
                        if (lower_bound > actualLB){
                            actualLB = lower_bound;
                        }
                        if (i == (pedidos-1) && mejor_sol < lower_bound){
                            mejor_sol = coste;
                        }
                    }
                }

                /* Creacion hijo derecho, es decir, se rechaza siguiente peticion */
                ocupacion_aux = aux.ocupacion;
                ben_aux = aux.beneficio_actual;
                coste = costeEstimado(lista, ocupacion_aux, i+1, ben_aux, tren);
                lower_bound = lowerBound(lista, ocupacion_aux, i+1, ben_aux, tren);
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
                    if (i == (pedidos - 1) && mejor_sol < lower_bound){
                        mejor_sol = coste;
                    }
                }
            }
            end = clock();

            /* Se muestra la solcución por la salida estándar */
            double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
            cout << "Time taken by program is : " << fixed
                << time_taken << setprecision(10);
            cout << " sec " << endl;

            cout << "La mejor solucion encontrada ha sido: " << mejor_sol << endl;
        } 
        /* Si NO se puede aceptar ningúngún pedido, no se hace el cálculo */
        else {
            end = clock();
            
            /* Se muestra la solcución por la salida estándar */
            double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
            cout << "Time taken by program is : " << fixed
                << time_taken << setprecision(10);
            cout << " sec " << endl;

            cout << "La mejor solucion encontrada ha sido: " << 0 << " (no cabe ningun pedido)" << endl;
        }

        
    } 
    /* Si una lista NO incluye más de 0 pedidos, no se hace el cálculo */
    else {
        clock_t start, end;
        start = clock();
        end = clock();

        /* Se muestra la solcución por la salida estándar */
        double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
        cout << "Time taken by program is : " << fixed
             << time_taken << setprecision(10);
        cout << " sec " << endl;

        cout << "La mejor solucion encontrada ha sido: " << 0 << " (sin pedidos)" << endl;
    }
    cout << "-----------------\n";
}

/*
 * Pre: argc >=2, argv[1] es un fichero que incluye los datos del problema
 * Post: Escribe en el fichero argv[2] la solución óptima de selección de 
 * pedidos para cada una de las instancias de problema.
 */
int main(int argc, char *argv[]){
    
    if(argc >= 2){
        
        ifstream entrada(argv[1]);
        if(!entrada.is_open()){
            cerr << "EL fichero de entrada no se ha podido abrir.\n";
            return -1;
        }

        ofstream salida(argv[2]);
        if(!salida.is_open()){
            cerr << "EL fichero de salida no se ha podido abrir.\n";
            return -1;
        }
        
        cout.rdbuf(salida.rdbuf()); //Redirigir stdout al fichero de salida

        int tren, estaciones, pedidos;
        entrada >> tren >> estaciones >> pedidos;

        while(!(tren == 0 && estaciones == 0 && pedidos == 0)){
            cout << "----------------------------------------------------" << endl;
            resolver(tren,estaciones,pedidos,entrada);
            entrada >> tren >> estaciones >> pedidos;
        }

        entrada.close();
        salida.close();

    }

    return 0;
}