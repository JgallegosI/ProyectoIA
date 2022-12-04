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
    for (int i = 0; i < clientes.size(); i++){
        cout << clientes[i].numero_cliente << " " << clientes[i].pos_x<<" "<< clientes[i].pos_y<< " "<< clientes[i].demanda << " "<< clientes[i].tipo_cliente<<endl;
    }

    vector<vector<float>> meuc = euclideanMatrix(clientes);
    cout << "Matriz de Costos "<<endl;
    show_matrix(meuc);
    cout << endl<<endl;

    generate_truck(camiones,cap_cam,can_cam);
    vector<Cliente> vc;
    vector<Cliente> tc;

    rutas(clientes,vc,tc);

    for (auto x:vc)
    {
        cout<<x.numero_cliente<<endl;
    }
    
    for (auto x:tc)
    {
        cout<<x.numero_cliente<<endl;
    }

    for (auto x:camiones)
    {
        cout<<x.numero_camion<<endl;   
        for (auto y:x.ruta)
    {
        for (auto w:y)
        {
        cout<<w.numero_cliente<<endl;
        }
    }
    
    }
    
    return 0;
}

