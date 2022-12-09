#include"header.hh"

vector<Cliente> Leer_arch(string dir, int &can_cam, int &can_tra, float &cap_cam, float &cap_tra, int &n_cli){
    ifstream file(dir);
    string line;
    
    
    if (file.fail()){
        cout<<"No se puede abrir el archivo";
    }
    getline(file,line);
    stringstream linestream(line);
    string data;
    
    linestream >> can_cam >> cap_cam>> can_tra >> cap_tra >> n_cli;
    vector<Cliente> clientes;
    while (!file.eof())
    {
        Cliente cliente;
        getline(file,line);
        stringstream linestream(line);

        linestream>> cliente.numero_cliente >> cliente.pos_x >> cliente.pos_y >> cliente.demanda >> cliente.tipo_cliente;

        clientes.push_back(cliente);

    }

    return clientes;
    
}


vector<vector<float>> euclideanMatrix(vector<Cliente> clientes){
    vector<vector<float>> matrix;
    for (int i = 0; i < clientes.size(); i++)
    {
        vector<float> columna;
        for (int j = 0; j < clientes.size(); j++){
            float x = clientes[i].pos_x - clientes[j].pos_x;
            float y = clientes[i].pos_y - clientes[j].pos_y; 
            float dist = pow(x,2)+pow(y,2);
            dist = sqrt(dist);  
            columna.push_back(dist);
        }
        matrix.push_back(columna);
    }

    return matrix;
    
}


void show_matrix(vector<vector<float>> matrix){
    for (int i = 0; i < matrix.size() ; i++)
    {
    for (int j = 0; j < matrix[i].size(); j++)
    {
        cout << matrix[i][j]  << "\t";
    }
        
    // Newline for new row
    cout << endl;
    }
}

int getIndex(vector<Cliente> v, Cliente K)
{
    for (int i = 0; i < v.size(); i++)
    {
        if (v[i].numero_cliente == K.numero_cliente)
        {
            return i;
        }
        
    }
    return -1;
}

void rutas(vector<Cliente> clientes, vector<Cliente> &vc, vector<Cliente> &tc){
    for (auto x: clientes)
    {
        if (x.tipo_cliente==0)
        {
            vc.push_back(x);
        }else
        {
            tc.push_back(x);
        }    
    }
    
}

void generate_truck(vector<Camion> &camiones, float capacidad, int n_camiones){
    for (int i = 0; i < n_camiones; i++)
    {
        Camion x;
        x.numero_camion=i;
        x.capacidad=capacidad;
        camiones.push_back(x);
    }
    
}

void mainRouteWohutTrailer(vector<Camion> &camiones,vector<Cliente> &vc, vector<Cliente> clientes){
    vc.erase(vc.begin()+0);
    for (auto &camion:camiones)
    {   
        
        if (camion.capacidad>=0)
        {
        if(vc.size()>0){
        vector<Cliente> aux;
        aux.push_back(clientes[0]);

        while ((camion.capacidad >=0)&&(vc.size()>0))
        {
            
            Cliente cli= select_random_int(vc);
            camion.capacidad = camion.capacidad - cli.demanda;
            if (camion.capacidad>=0)
            {
                aux.push_back(cli);
                vc.erase(vc.begin()+getIndex(vc,cli));
            }else
            {   
                camion.capacidad = camion.capacidad + cli.demanda;
                cout<<"Capacidad sin trailer "<<camion.capacidad<<" Camion "<<camion.numero_camion<<endl;
                break;
            }
             
        }
        aux.push_back(clientes[0]);
        vector<vector<Cliente>> aux2;
        aux2.push_back(aux);
        camion.ruta=aux2;
        }
        }
    }
}



Cliente select_random_int(vector<Cliente> v){
    unsigned int random = rand() % v.size();
    Cliente sel_elem = v[random];
    return sel_elem;
}

int minVal(vector<vector<float>> costos, int i, vector<Cliente> tc){
    float min = INFINITY;
    int vecino;
    for (auto n:tc){
        if (n.numero_cliente != i){
            if (min < costos[i][n.numero_cliente])
            {
                min = costos[i][n.numero_cliente];
                vecino = n.numero_cliente;
            }
        }
    }

    return vecino;
}
tuple<float,float> check_peso(Camion camion,float cap_cam, float cap_tra){
    float peso = 0;
    float capacidad;
    for (auto x:camion.ruta)
    {
        for (int i = 1; i < x.size()-1; i++)
        {
            peso = peso + x[i].demanda;
        }
        
    }
    if(camion.trailer){
        capacidad = cap_cam + cap_tra;
    }else{
        capacidad = cap_cam;
    }
    capacidad = capacidad - peso;
    return make_tuple(peso,capacidad);
}

void suma_peso(vector<Camion> &camion,float cap_cam, float cap_tra){
    for (auto &x:camion)
    {
        x.peso = get<0>(check_peso(x, cap_cam, cap_tra));
        x.capacidad = get<1>(check_peso(x,cap_cam, cap_tra));    
    }

}

void sort_camiones_max(vector<Camion> &camiones){
    sort(camiones.begin(), camiones.end(), [](Camion a, Camion b) { return a.capacidad > b.capacidad; });
}

void sort_camiones_min(vector<Camion> &camiones){
    sort(camiones.begin(), camiones.end(), [](Camion a, Camion b) { return a.capacidad < b.capacidad; });
}

void add_trailer(vector<Camion> &camiones, int can_tra, float cap_tra, float cap_cam){
    for (auto &x:camiones)
    {
        if ((can_tra>0))
        {   
            x.trailer=1;
            x.capacidad = x.capacidad + cap_tra;
            can_tra --;
        }
        
        
    }
    
}

void mainRouteWithTrailer(vector<Camion> &camiones,vector<Cliente> &vc, vector<Cliente> clientes){
    for (auto &camion:camiones)
    {   
        if (camion.capacidad>=0)
        {
        if(vc.size()>0){

        while ((camion.capacidad >=0)&&(vc.size()>0))
        {
            
            Cliente cli= select_random_int(vc);
            camion.capacidad = camion.capacidad - cli.demanda;
            if (camion.capacidad>=0)
            {
                camion.ruta[0].insert(camion.ruta[0].begin()+camion.ruta[0].size()-2,cli);
                vc.erase(vc.begin()+getIndex(vc,cli));
            }else
            {   
                camion.capacidad = camion.capacidad + cli.demanda;
                break;
            }
             
        }
        }
        }
    }
}
void show_camiones(vector<Camion> camiones){
    for (auto x:camiones)
    {
        cout<<"Camion: "<<x.numero_camion<<endl;
        for (auto y:x.ruta)
        {
            for (auto z:y)
            {
                cout<<z.numero_cliente<<" ";
            }
            cout<<endl;
        }
        cout<<"Peso: "<<x.peso<<endl;
        cout<<"Capacidad: "<<x.capacidad<<endl;
        cout<<"Trailer: "<<x.trailer<<endl;
        cout<<endl;
    }
    
}

void mainRouteWithTc(vector<Camion> &camiones, vector<Cliente> &tc,vector<Cliente> clientes){ // ahora tengo agregado los clientes tc a los camiones sin trailer
    for(auto &camion:camiones){
        if ((camion.peso==0) && (camion.trailer == 0))
        {
        vector<Cliente> aux;
        aux.push_back(clientes[0]);

        while ((camion.capacidad >=0)&&(tc.size()>0))
        {
            
            Cliente cli= select_random_int(tc);
            camion.capacidad = camion.capacidad - cli.demanda;
            if (camion.capacidad>=0)
            {
                aux.push_back(cli);
                tc.erase(tc.begin()+getIndex(tc,cli));
            }else
            {   
                camion.capacidad = camion.capacidad + cli.demanda;
                break;
            }
             
        }
        aux.push_back(clientes[0]);
        vector<vector<Cliente>> aux2;
        aux2.push_back(aux);
        camion.ruta=aux2;
        }
        
        
    }
}

void sort_clientes_tc_max(vector<Cliente> &tc){
    sort(tc.begin(), tc.end(), [](Cliente a, Cliente b) { return a.demanda < b.demanda; });
}

void show_clientes(vector<Cliente> clientes){
    for (auto x:clientes)
    {
        cout<<x.numero_cliente<<" ";
    }
    cout<<endl;
    
}

void subtour(vector<Camion> &camiones, vector<vector<float>> costos, vector<Cliente> &tc){
    for (auto &x:camiones)
    {
        if ((x.capacidad>=0)&&(x.trailer==1))
        {
            if (tc.size()!=0)
            {   int n_subtours = 0;
                while ((tc.size()!=0 && n_subtours<4))
                {
                    int i=0;
                vector<Cliente> camiones_aux;
                Cliente cli= tc.back(); 
                x.capacidad = x.capacidad - cli.demanda;
                int position = most_near_client(x.ruta[0],costos,cli);
                if (position == 0)
                {
                    position = 1;
                }
                
                if((x.capacidad>=0)&&(tc.size()!=0)){
                camiones_aux.push_back(x.ruta[0][position]);
                camiones_aux.push_back(cli);
                tc.pop_back();
                
                
                while ((x.capacidad >=0)&&(tc.size()!=0)&&(i<5))
                {
                    Cliente cli= tc.back();
                    x.capacidad = x.capacidad - cli.demanda;    
                    if (x.capacidad>=0)
                    {
                        camiones_aux.push_back(cli);
                        tc.pop_back();
                        i++;
                    }else
                    {   
                        x.capacidad = x.capacidad + cli.demanda;
                        break;
                    }
                    
                }
                camiones_aux.push_back(camiones_aux[0]);
                x.ruta.push_back(camiones_aux);
                n_subtours++;
                }
                else{
                    x.capacidad = x.capacidad + cli.demanda;
                    break;
                }
                }
                
                
        
            }
            
        }
        
    }
    
}

void add_client_tc(vector<Camion> &camiones, vector<Cliente> &tc, vector<vector<float>> costos){
    for (auto &x:camiones)
    {   
        cout<<"Camion: "<<x.numero_camion<<endl;
        if(tc.size()!=0){
        for(auto cli:tc){
         cli= tc.back();
        if ((x.trailer==1)&&(cli.tipo_cliente==1))
        {
            if (tc.size()!=0)
            {
                
                int position = most_near_client(x.ruta.back(),costos,cli);
                if (position == 0)
                {
                    position = 1;
                }
                x.ruta.back().insert(x.ruta.back().begin()+position,cli);
                tc.pop_back();
            }
            
        }else if((x.trailer==0)){
            if (tc.size()!=0)
            {
                int position = most_near_client(x.ruta[0],costos,cli);
                if (position == 0)
                {
                    position = 1;
                }
                x.ruta[0].insert(x.ruta[0].begin()+position,cli);
                tc.pop_back();
            }
        }
        }
    }
    }
}

int  most_near_client(vector<Cliente> &clientes, vector<vector<float>> costos, Cliente cli){
    int index = 0;
    float min = 999999999;
    for (int i = 0; i < clientes.size(); i++)
    {
        if (costos[cli.numero_cliente][clientes[i].numero_cliente]<min)
        {
            min = costos[cli.numero_cliente][clientes[i].numero_cliente];
            index = i;
        }
        
    }
    return index;
}

void sort_camiones_by_number(vector<Camion> &camiones){
    sort(camiones.begin(), camiones.end(), [](Camion a, Camion b) { return a.numero_camion < b.numero_camion; });
}

// Hill CLimmbing con Mejor Mejora