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
    srand(time(NULL));
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
float check_peso(Camion camion){
    float peso = 0;
    for (auto x:camion.ruta)
    {
        for (auto y:x)
        {
            peso = peso + y.demanda;
        }
        
    }
    return peso;
}

void suma_peso(vector<Camion> &camion){
    for (auto &x:camion)
    {
        x.peso = check_peso(x);
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
            x.trailer=true;
            x.capacidad = x.capacidad + cap_tra;
            can_tra --;
        }
        
        
    }
    
}

void mainRouteWithTrailer(vector<Camion> &camiones,vector<Cliente> &vc, vector<Cliente> clientes){
    srand(time(NULL));
    for (auto &camion:camiones)
    {   
        cout<<"Capacidad con trailer: "<<camion.capacidad<< "\tCamion: "<< camion.numero_camion <<endl;
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
        cout<<endl;
    }
    
}