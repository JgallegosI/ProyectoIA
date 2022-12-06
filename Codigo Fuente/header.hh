#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<vector> 
#include<math.h>
#include<algorithm>
#include <bits/stdc++.h>
#include<time.h>
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
    bool trailer;
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
float check_peso(Camion camion);
void suma_peso(vector<Camion> &camion);
void sort_camiones_max(vector<Camion> &camiones);
void add_trailer(vector<Camion> &camiones, int can_tra, float cap_tra, float cap_cam);
void mainRouteWithTrailer(vector<Camion> &camiones,vector<Cliente> &vc, vector<Cliente> clientes);
void sort_camiones_min(vector<Camion> &camiones);
void show_camiones(vector<Camion> camiones);