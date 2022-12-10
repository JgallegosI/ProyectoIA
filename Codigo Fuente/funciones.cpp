#include"header.hh"
// Leemos el archivo 
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

// Se obtiene las distancias entre todos los nodos

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

// Funcio de impresion de Matrices
void show_matrix(vector<vector<float>> matrix){
    for (int i = 0; i < matrix.size() ; i++)
    {
    for (int j = 0; j < matrix[i].size(); j++)
    {
        cout << matrix[i][j]  << "\t";
    }
    cout << endl;
    }
}
// Obtener el indice de un elemento en un vector
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
// Separar rutas por clientes
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
// Generar camiones
void generate_truck(vector<Camion> &camiones, float capacidad, int n_camiones){
    for (int i = 0; i < n_camiones; i++)
    {
        Camion x;
        x.numero_camion=i;
        x.capacidad=capacidad;
        camiones.push_back(x);
    }
    
}
// Construir rutas principales de clientes vc sin trailer
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


// Selecciona un elemento aleatorio de un vector
Cliente select_random_int(vector<Cliente> v){
    unsigned int random = rand() % v.size();
    Cliente sel_elem = v[random];
    return sel_elem;
}
// Obtiene el numero del nodo mas cercano a un cliente tc
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
// Verfica peso de los camiones
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
// Suma peso de los camiones
void suma_peso(vector<Camion> &camion,float cap_cam, float cap_tra){
    for (auto &x:camion)
    {
        x.peso = get<0>(check_peso(x, cap_cam, cap_tra));
        x.capacidad = get<1>(check_peso(x,cap_cam, cap_tra));    
    }

}

// Ordena camiones de mayor a menor capacidad
void sort_camiones_max(vector<Camion> &camiones){
    sort(camiones.begin(), camiones.end(), [](Camion a, Camion b) { return a.capacidad > b.capacidad; });
}
// Ordena camiones de menor a mayor capacidad
void sort_camiones_min(vector<Camion> &camiones){
    sort(camiones.begin(), camiones.end(), [](Camion a, Camion b) { return a.capacidad < b.capacidad; });
}
// Añade trailers de acuerdo a la capacidad de los camiones
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
// Ruta principal de clientes vc con trailer
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
// Funcion de Impresion de Camiones
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
// Ruta principal de clientes tc sin trailer
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
// Ordena clientes de mayor a menor demanda
void sort_clientes_tc_max(vector<Cliente> &tc){
    sort(tc.begin(), tc.end(), [](Cliente a, Cliente b) { return a.demanda < b.demanda; });
}
// Funcion de Impresion de Clientes
void show_clientes(vector<Cliente> clientes){
    for (auto x:clientes)
    {
        cout<<x.numero_cliente<<" ";
    }
    cout<<endl;
    
}
// Establece los subtures de los camiones con trailer
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
// en caso de que no se pueda agregar un cliente a un subtour se agrega a la ruta principal en una ruta tc o 
// en un subtour con trailer
void add_client_tc(vector<Camion> &camiones, vector<Cliente> &tc, vector<vector<float>> costos){
    for (auto &x:camiones)
    {   
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
// Fencion que retorna indice de cliente con menor distancia a un cliente
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
// Funcion que ordena camiones por numero de camion
void sort_camiones_by_number(vector<Camion> &camiones){
    sort(camiones.begin(), camiones.end(), [](Camion a, Camion b) { return a.numero_camion < b.numero_camion; });
}

// Hill CLimmbing con Mejor Mejora
void hill_climbing(vector<Camion> camiones, vector<vector<float>> costos, vector<Cliente> clientes,float cap_cam,float cap_tra,int MAX_ITER){
    vector<Vecino> vecinos;
    Vecino vecino, mejor_vecino;
    vecino.camiones = camiones;
    vecino.costo = costo_ruta(camiones,costos);
    vecinos.push_back(vecino);
    vector<Vecino> vecinos_aceptados;
    int i=0;
    while(i<MAX_ITER){
        vector<Camion> SOL_INI = camiones;
        float Costo_SOL_INI = costo_ruta(camiones,costos);
        for (int j = 0; j < camiones.size(); j++)
        {
            for (int k = 0; k < camiones[j].ruta.size(); k++)
            {   
                
                for (int l = 1; l < camiones[j].ruta[k].size()-1; l++)
                {   
                    
                    for(int x =0; x<camiones.size(); x++)
                    {
                        for (int y = 0; y < camiones[x].ruta.size(); y++)
                        {
                                for (int z = 1; z < camiones[x].ruta[y].size()-1; z++)
                                {   
                                    vector<Camion> camiones_aux = camiones;
                                    if ((l!=z)&&(camiones[j].ruta[k][l].tipo_cliente==1))
                                    {   //cambio posicion de cliente tc entre subrutas
                                            if (((y>0)||(k>0))&&(camiones_aux[x].trailer==1)&&(camiones[x].ruta[y][z].tipo_cliente==1))
                                            {        
                                                //cambio entre subrutas                                
                                            swap(camiones_aux[j].ruta[k][l],camiones_aux[x].ruta[y][z]);
                                                if (check_factibility(camiones_aux[j],cap_cam,cap_tra))
                                                {
                                                    vecino.camiones = camiones_aux;
                                                    vecino.costo = costo_ruta(camiones_aux,costos);
                                                    vecinos.push_back(vecino);
                                                }
                                            }else if (((y==0)||(k==0))&&((camiones_aux[x].trailer==0)||(camiones_aux[j].trailer==0)))
                                            {   //cambio subruta con ruta
                                                swap(camiones_aux[j].ruta[k][l],camiones_aux[x].ruta[y][z]);
                                                if (check_factibility(camiones_aux[x],cap_cam,cap_tra))
                                                {
                                                    vecino.camiones = camiones_aux;
                                                    vecino.costo = costo_ruta(camiones_aux,costos);
                                                    vecinos.push_back(vecino);
                                                }
                                            }
                                        
                                    }else if ((l!=z)&&(camiones[j].ruta[k][l].tipo_cliente==0))
                                    { if(inSubRuta(camiones[j].ruta[k],camiones[j].ruta[k][l]))
                                        {
                                            swap(camiones_aux[j].ruta[k][l],camiones_aux[x].ruta[y][z]);
                                            if (check_factibility(camiones_aux[j],cap_cam,cap_tra))
                                            {
                                                vecino.camiones = camiones_aux;
                                                vecino.costo = costo_ruta(camiones_aux,costos);
                                                vecinos.push_back(vecino);
                                            }
                                            
                                        }else
                                        {
                                            swap(camiones_aux[j].ruta[0][l],camiones_aux[x].ruta[0][z]);
                                            if (check_factibility(camiones_aux[j],cap_cam,cap_tra))
                                            {
                                                vecino.camiones = camiones_aux;
                                                vecino.costo = costo_ruta(camiones_aux,costos);
                                                vecinos.push_back(vecino);
                                            }
                                        }
                                        
                                        

                                    }
                            }
                        }
                    }                
                }   
            }
        }
    // Seleccionar el mejor vecino
    select_best(vecinos);
        mejor_vecino = vecinos[0];
        if (mejor_vecino.costo > Costo_SOL_INI)
        {
            break;
        }
        vecinos_aceptados.push_back(mejor_vecino);
        camiones = mejor_vecino.camiones;
        i++;
        vecinos.clear();
}
        
    // Selccionar el mejor vecino de los vecinos aceptados
    cout<<"Solucion HCMM"<<endl;
     cout<<"--------------------------@--------------------------"<<endl;
    select_best(vecinos_aceptados);
    mejor_vecino = vecinos_aceptados[0];
    show_camiones(mejor_vecino.camiones);
    cout<<"Costo: "<<costo_ruta(mejor_vecino.camiones,costos)<<endl;
}
// Funcion Objetivo para calcular el costo de la ruta
float costo_ruta(vector<Camion> camiones, vector<vector<float>> costos){
    float costo = 0;
    float sobre_peso = 0;
    for (auto x:camiones)
    {
        for (auto y:x.ruta)
        {
            for (int i = 0; i < y.size()-1; i++)
            {
                costo = costo + costos[y[i].numero_cliente][y[i+1].numero_cliente];
            }
            
        }
        
    }

    for(auto x:camiones){
        if (x.capacidad < 0){
            sobre_peso = sobre_peso + x.capacidad;
        }
    }
    costo = costo + (abs(sobre_peso)*30);
    return costo;
}
// Selecciona Mejor Vecino
void select_best(vector<Vecino> &vecinos){
    sort(vecinos.begin(), vecinos.end(), [](Vecino a, Vecino b) { return a.costo < b.costo; });
}
// Muestra solucion HCMM
void print_vecino(Vecino vecino,float cap_cam, float cap_tra){
    suma_peso(vecino.camiones,cap_cam,cap_tra);
    show_camiones(vecino.camiones);
    cout<<"Costo: "<<vecino.costo<<endl;
}
// Chequea si vecino es factible
bool check_factibility(Camion &camion1,float cap_cam, float cap_tra){
    suma_peso_per_camion(camion1,cap_cam,cap_tra);
    if ((camion1.capacidad >=0)){
        return true;
    }
    else{
        return false;
    }
}
// Suma peso de cada camion
void suma_peso_per_camion(Camion &x,float cap_cam, float cap_tra){
    
        x.peso = get<0>(check_peso(x, cap_cam, cap_tra));
        x.capacidad = get<1>(check_peso(x,cap_cam, cap_tra));    

}
// Chequea si nodo es raiz de subruta
bool inSubRuta(vector<Cliente> RutaxCamion,Cliente cliente){
    for (auto x:RutaxCamion){
        if (x.numero_cliente == cliente.numero_cliente){
            return true;
        }
    }
    return false;
}