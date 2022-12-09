#include"header.hh"


int main(int argc, char const *argv[])
{
    srand(time(NULL));
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
    suma_peso(camiones, cap_cam, cap_tra);    
    if (vc.empty()==false)
    {
        sort_camiones_min(camiones);
        add_trailer(camiones,can_tra,cap_tra,cap_cam);
        sort_camiones_max(camiones);
        suma_peso(camiones, cap_cam, cap_tra); 
        
        
        
        sort_clientes_tc_max(tc);
        
        
        suma_peso(camiones, cap_cam, cap_tra); 
        sort_camiones_max(camiones);
        if (tc.empty()==false)
        {
            subtour(camiones,costs,tc);
            suma_peso(camiones, cap_cam, cap_tra); 
            sort_camiones_max(camiones);
            if (tc.empty()==false)
            {
                
                add_client_tc(camiones,tc,costs);
                suma_peso(camiones, cap_cam, cap_tra); 
            }
            
        }
        sort_camiones_by_number(camiones);
        cout<<"Rutas finales"<<endl;
        show_camiones(camiones);
    }else{
        
        sort_clientes_tc_max(tc);
        
        cout<<endl;
        sort_camiones_max(camiones);
        
        mainRouteWithTc(camiones,tc,clientes);
        suma_peso(camiones, cap_cam, cap_tra);
        if (tc.empty()==false)
        {   
            add_trailer(camiones,can_tra,cap_tra,cap_cam);
            subtour(camiones,costs,tc);
            suma_peso(camiones, cap_cam, cap_tra); 
            if (tc.empty()==false)
            {   
                
                
                add_client_tc(camiones,tc,costs);
                suma_peso(camiones, cap_cam, cap_tra); 
            }
            
        }
        sort_camiones_by_number(camiones);
        
        
        cout<<"Rutas finales"<<endl;
        suma_peso(camiones, cap_cam, cap_tra); 
        show_camiones(camiones);
    }
    
    
    
    return 0;
}

