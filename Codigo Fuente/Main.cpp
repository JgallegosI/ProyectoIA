#include"header.hh"

int main(int argc, char const *argv[])
{
    int can_cam;
    int can_tra;
    float cap_cam;
    float cap_tra;
    int n_cli;
    vector<Cliente> clientes=Leer_arch(argv[1],can_cam, can_tra, cap_cam, cap_tra, n_cli);
    
    for (int i = 0; i < clientes.size(); i++){
        cout << clientes[i].numero_cliente << " " << clientes[i].pos_x<<" "<< clientes[i].pos_y<< " "<< clientes[i].demanda << " "<< clientes[i].tipo_cliente<<endl;
    }

    vector<vector<float>> meuc = euclideanMatrix(clientes);
    cout << "Matriz de Costos "<<endl;
    show_matrix(meuc);
    cout << endl<<endl;
    vector<vector<float>> dij = dijMatrix(meuc, clientes, can_cam);
    cout << "Matriz dij"<<endl;
    show_matrix(dij);
    cout << endl;
    return 0;
}

