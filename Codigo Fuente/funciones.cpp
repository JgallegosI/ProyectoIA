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

int getIndex(vector<float> v, float K)
{
    auto it = find(v.begin(), v.end(), K);
    if (it != v.end()) 
    {
        int index = it - v.begin();
        return index;
    }
    else {
        return -1;
    }
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

void mainRouteWohutTrailer(vector<Camion> &camiones,vector<Cliente> vc, vector<Cliente> tc, vector<Cliente> clientes){
    
    vector<Cliente> vc1=vc;
    vc1.erase(std::find(vc1.begin(),vc1.end(),clientes[0]));
    for (auto camion:camiones)
    {
        camion.ruta[0].push_back(clientes[0]);

        while (camion.capacidad >=0)
        {
            Cliente cli= select_random_int(vc1);
            camion.capacidad = camion.capacidad - cli.demanda;
            vc1.erase(std::find(vc1.begin(),vc1.end(),cli));
        }
        camion.ruta[0].push_back(clientes[0]);
    }
    
}

Cliente select_random_int(vector<Cliente> v){
    int random = rand() % v.size();
    Cliente sel_elem = v[random];
    return sel_elem;
}

