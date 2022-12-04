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
    cout<<"entre"<<endl;
    vc.erase(vc.begin()+0);
    for (auto &camion:camiones)
    {   
        if(vc.size()>0){
        cout<<"Camion"<<endl;
        vector<Cliente> aux;
        aux.push_back(clientes[0]);

        while ((camion.capacidad >=0)&&(vc.size()>0))
        {
            
            Cliente cli= select_random_int(vc);
            camion.capacidad = camion.capacidad - cli.demanda;
            cout<<cli.numero_cliente<<endl;
            aux.push_back(cli);
            vc.erase(vc.begin()+getIndex(vc,cli)); 
        }
        aux.push_back(clientes[0]);
        vector<vector<Cliente>> aux2;
        aux2.push_back(aux);
        camion.ruta=aux2;
        }
    }
    
}

Cliente select_random_int(vector<Cliente> v){
    cout<<"Calculo random"<<endl;
    cout<<v.size()<<endl;
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