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
    cout<<vc.size()<<endl;
    cout<<tc.size()<<endl;
    mainRouteWohutTrailer(camiones,vc,clientes);
    suma_peso(camiones);
    cout<<"Rutas sin trailer"<<endl;
    show_camiones(camiones);
    sort_camiones_min(camiones);
    if (vc.empty()==false)
    {
        cout<<"No se pudo asignar todos los clientes"<<endl;
        add_trailer(camiones,can_tra,cap_tra,cap_cam);
        mainRouteWithTrailer(camiones,vc,clientes);
        suma_peso(camiones);
        cout<<"Rutas con trailer"<<endl;
        show_camiones(camiones);

    }
    
    cout<<"Cliente sin asignar a ruta"<<endl;
    for (auto x:vc){
        cout<<x.numero_cliente<<endl;
    }
    
    return 0;
}

