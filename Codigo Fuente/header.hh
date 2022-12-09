#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<vector> 
#include<math.h>
#include<algorithm>
#include <bits/stdc++.h>
#include<time.h>
#include<tuple>
using namespace std;

struct Cliente
{
    int numero_cliente;
    float pos_x;
    float pos_y;
    float demanda;
    int tipo_cliente;
    bool nodo_subruta;
};



struct Camion
{
    int numero_camion;
    float capacidad;
    float peso;
    vector<vector<Cliente>> ruta;
    int trailer=0;
};

vector<Cliente> Leer_arch(string dir, int &can_cam, int &can_tra, float &cap_cam, float &cap_tra, int &n_cli);
vector<vector<float>> euclideanMatrix(vector<Cliente> );
void show_matrix(vector<vector<float>>);
int getIndex(vector<Cliente> , Cliente );
void rutas(vector<Cliente> clientes, vector<Cliente> &vc, vector<Cliente> &tc);
void generate_truck(vector<Camion> &camiones, float capacidad, int n_camiones);
Cliente select_random_int(vector<Cliente> );
void mainRouteWohutTrailer(vector<Camion> &camiones,vector<Cliente> &vc, vector<Cliente> clientes);
int minVal(vector<vector<float>> costos, int i, vector<Cliente> tc);
tuple<float,float> check_peso(Camion camion,float cap_cam, float cap_tra);
void suma_peso(vector<Camion> &camion,float cap_cam, float cap_tra);
void sort_camiones_max(vector<Camion> &camiones);
void add_trailer(vector<Camion> &camiones, int can_tra, float cap_tra, float cap_cam);
void mainRouteWithTrailer(vector<Camion> &camiones,vector<Cliente> &vc, vector<Cliente> clientes);
void sort_camiones_min(vector<Camion> &camiones);
void show_camiones(vector<Camion> camiones);
void mainRouteWithTc(vector<Camion> &camiones,vector<Cliente> &tc, vector<Cliente> clientes);
void sort_clientes_tc_max(vector<Cliente> &tc);
void show_clientes(vector<Cliente> clientes);
void subtour(vector<Camion> &camiones, vector<vector<float>> costos, vector<Cliente> &tc);
int  most_near_client(vector<Cliente> &clientes, vector<vector<float>> costos, Cliente cli);
void add_client_tc(vector<Camion> &camiones, vector<Cliente> &tc, vector<vector<float>> costos);
void sort_camiones_by_number(vector<Camion> &camiones);