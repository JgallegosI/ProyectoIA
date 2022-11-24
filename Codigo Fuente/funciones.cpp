
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

vector<vector<float>> dijMatrix(vector<vector<float>> Costos, vector<Cliente> clientes, int n_cam){
    vector<Cliente> sj;
    vector<float> costos_aux;
    vector<vector<float>> Costos_copia = Costos;
    sort(Costos_copia[0].begin(),Costos_copia[0].end(),greater<float>());

    for (auto  x : Costos_copia[0]){
        costos_aux.push_back(x);
    }

    vector<int> seeds;
    seeds.push_back(0);
    int p = getIndex(Costos[0],costos_aux[0]);
    seeds.push_back(p);


    for (int i = 1; i < n_cam; i++)
    {
        allseeds(Costos,seeds);
    }
    
   
    vector<vector<float>> dij;

    seeds.erase(seeds.begin());
    for (int i = 0; i < Costos[1].size(); i++)
    {   
        vector<float> aux;
        for (auto x : seeds){
            
                float dij_aux = Costos[0][i] + Costos[i][x] - Costos[0][x];
                aux.push_back(dij_aux);
        }
        dij.push_back(aux);
    }
    
    return  dij;
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

void allseeds(vector<vector<float>> Costos, vector<int> &seeds){
    vector<vector<float>> newdistance;
    for(auto x : seeds){
        newdistance.push_back(Costos[x]);
    }
    
    vector<float> maxsum;
    for (int i = 0; i < newdistance[0].size(); i++)
    {
        float sum = sumcolumna(newdistance, i);
        maxsum.push_back(sum);

    }
    float max_seed_dist = *max_element(maxsum.begin(), maxsum.end());
    vector<float> ms_aux = maxsum;
    sort(maxsum.begin(),maxsum.end(),greater<float>());

    for(auto x : maxsum){
        int n = getIndex(ms_aux, x);
        if(find(seeds.begin(), seeds.end(), n) == seeds.end()){
             seeds.push_back(n);
             break;
        }
    }
    
   
    
}

float sumcolumna(vector<vector<float>> matrizDistancesSeed, int column ){
  float sum=0.0;
  for (int i=0;i<matrizDistancesSeed.size();i++){
    sum += matrizDistancesSeed[i][column];
  }
  return sum;

}
//