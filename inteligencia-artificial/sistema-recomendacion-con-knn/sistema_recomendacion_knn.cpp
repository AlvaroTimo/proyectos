#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <math.h>
#include <algorithm>
#include <unordered_map>

using namespace std;

vector<vector<string> > datos;
unordered_map<string, int> indices;

void imprimirDatos(const vector<vector<string>>& data) {
    for (const auto& row : data) {
        for (const string& value : row) {
            cout << value << " ";
        }
        cout << endl;
    }
}

vector<float> distanciaEntre2Usuarios( int usuario1, int usuario2){

    vector<float> distancias;
    float distanciaEuclideana = 0;
    float distanciaManhattan = 0;

    for (int i = 1; i<datos.size();i++){
        float dato1 = stof(datos[i][usuario1]);
        float dato2 = stof(datos[i][usuario2]);
        if(dato1 != -1 && dato2 != -1){
            distanciaEuclideana += pow( (dato1 - dato2), 2);
            distanciaManhattan += abs( (dato1 - dato2 ) );
        }
    }

    distanciaEuclideana = sqrt(distanciaEuclideana);
    distancias.push_back(distanciaEuclideana);
    distancias.push_back(distanciaManhattan);

    return distancias;
}

vector<string> darRecomendaciones( int usuario1, int usuario2){
    vector<string> recomendaciones;
    float umbral=3;

    for (int i = 1; i<datos.size();i++){
        float dato1 = stof(datos[i][usuario1]);
        float dato2 = stof(datos[i][usuario2]);

        if(dato1 == -1 && dato2 > umbral){
            recomendaciones.push_back(datos[i][0]);
            cout << datos[i][0] << endl;
        }
    }
    return recomendaciones;
}

bool compareSecond(const pair<string, float> &a, const pair<string, float> &b) {
    return a.second < b.second;
}

vector< vector<pair<string ,float> > >calcularKNN( int usuario1){
    vector< vector<pair<string ,float> > > distancias;
    vector<pair<string ,float> > distanciasEuclideanas;
    vector<pair<string ,float> > distanciasManhattan;

    for (int i = 1; i<datos[0].size();i++){
        if(i!= usuario1){
            vector<float> distancias = distanciaEntre2Usuarios(usuario1,i);
            float distanciaEuclideana = distancias[0];
            float distanciaManhattan = distancias[1];


            distanciasEuclideanas.push_back(make_pair (datos[0][i],distanciaEuclideana));
            distanciasManhattan.push_back(make_pair(datos[0][i],distanciaManhattan));

            indices[datos[0][i]] = i;
        }
    }

    sort(distanciasEuclideanas.begin(), distanciasEuclideanas.end(), compareSecond); 
    sort(distanciasManhattan.begin(),distanciasManhattan.end(), compareSecond);

    distancias.push_back(distanciasEuclideanas);
    distancias.push_back(distanciasManhattan);

    return distancias;
}

int main() {
    ifstream file("archivo.csv");
    string line;
    

    while (getline(file, line)) {
        stringstream ss(line);
        string cell;
        vector<string> row;

        while (getline(ss, cell, ',')) {
            row.push_back(cell.empty() ? "-1" : cell);
        }

        datos.push_back(row);
    }

    file.close();

    // Llamar a la función para imprimir los datos
    
    //imprimirDatos(datos);
    // distanciaEntre2Usuarios(1,2);
    // darRecomendaciones(1,2);

    vector< vector<pair<string ,float> > > knn = calcularKNN(25);


    //Recomendaciones por distancia Euclideana
    cout<<"Recomendaciones por distancia Euclideana"<<endl;
    for(int i=0;i<1;i++){
        for(int j=0;j<5;j++){
            cout << knn[i][j].first <<"     "<< knn[i][j].second<<endl; 
            darRecomendaciones(25,indices[knn[i][j].first]);
            cout<<endl;
        }
    }

    //Recomendaciones por distancia Manhattan
    cout<<"Recomendaciones por distancia Manhattan"<<endl;
    for(int i=1;i<2;i++){
        for(int j=0;j<5;j++){
            cout << knn[i][j].first <<"     "<< knn[i][j].second<<endl; 
            
            darRecomendaciones(25,indices[knn[i][j].first]);
            cout<<endl;
        }
    }

    return 0;
}
