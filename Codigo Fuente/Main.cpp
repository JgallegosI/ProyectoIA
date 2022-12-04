#include"header.hh"


int main(int argc, char const *argv[])
{
    int can_cam;
    int can_tra;
    float cap_cam;
    float cap_tra;
    int n_cli;
    vector<Cliente> clientes=Leer_arch(argv[1],can_cam, can_tra, cap_cam, cap_tra, n_cli);
    vector<Camion> camiones;

    //Calculo matriz de costos
    vector<vector<float>> costs = euclideanMatrix(clientes);

    generate_truck(camiones,cap_cam,can_cam);
    //Separacion de clientes or tc y vc
    vector<Cliente> vc;
    vector<Cliente> tc;
    rutas(clientes,vc,tc);
    mainRouteWohutTrailer(camiones,vc,clientes);
    cout<<"Rutas"<<endl;
    for (auto x:camiones)
    {

            for (int i = 0; i < x.ruta.size() ; i++)
        {
        for (int j = 0; j < x.ruta[i].size(); j++)
        {
            cout<< x.ruta[i][j].numero_cliente << "\t";
        }
            
        // Newline for new row
        cout << endl;
        }
    
    }
    cout<<"Cliente sin asignar a ruta"<<endl;
    for (auto x:vc){
        cout<<x.numero_cliente<<endl;
    }
    
    return 0;
}

