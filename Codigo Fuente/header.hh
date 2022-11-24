#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<vector> 
#include<math.h>
#include<algorithm>
#include <bits/stdc++.h>
using namespace std;

struct Cliente
{
    int numero_cliente;
    float pos_x;
    float pos_y;
    float demanda;
    int tipo_cliente;
};



struct Camion
{
    int numero_camion;
    float capacidad;
};

vector<Cliente> Leer_arch(string dir, int &can_cam, int &can_tra, float &cap_cam, float &cap_tra, int &n_cli);
vector<vector<float>> euclideanMatrix(vector<Cliente> );
void show_matrix(vector<vector<float>>);
vector<vector<float>> dijMatrix(vector<vector<float>>, vector<Cliente> , int );
int getIndex(vector<float> , float );
void allseeds(vector<vector<float>> Costos, vector<int> &seeds);
float sumcolumna(vector<vector<float>> matrizDistancesSeed, int column );
