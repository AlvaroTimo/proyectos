#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <math.h>
#include <algorithm>
#include <map>

using namespace std;

vector<vector<string> > datos;
map<string, int> indices;
map<int, string> nombres;
map<string, int> indices_nombres;

map< int, map<int, float> > ratings;
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

string convertirIndiceAString(int indice){
    return nombres[indice];
}

int convertirStringAIndice(string nombre){
    return indices_nombres[nombre];
}

void formatear(){
    ofstream archivo("./Movielens/3.-profesora/movies.csv");
    archivo<<"movieId,title,genres"<<endl;

    ofstream archivo2("./Movielens/3.-profesora/ratings.csv");
    archivo2<<"userId,movieId,rating,timestamp"<<endl;

    if (archivo.is_open()) {
        int cont=0;
        for (const auto& row : datos) {
            int cont2 = 0;
            for (const string& value : row) {
                if(cont2==0 && cont!=0){
                    string cadenaSinComillas = value.substr(1, value.size() - 2);
                    archivo<<cont<<","<<cadenaSinComillas<<",,"<<endl;
                }
                cont2++;
            }
            cont++;
        }
    } else {
        cerr << "No se pudo abrir el archivo." << endl;
    }

    if (archivo2.is_open()) {
        for (const auto& pair : ratings) {
            for (const auto& inner_pair : ratings[pair.first]) {
                archivo2<<pair.first<<","<<inner_pair.first<<","<<inner_pair.second<<",,"<<endl;
            }    
        }
    }
}

int main() {
    ifstream file("archivo.csv");
    string line;
    

    bool namesInitialized = false;
    int indicePeliculas =0;
    while (getline(file, line)) {
        stringstream ss(line);
        string cell;
        vector<string> row;

        int indice=0;
        while (getline(ss, cell, ',')) {
            row.push_back(cell.empty() ? "-1" : cell);
            if(namesInitialized == false){
                nombres[indice] = cell;
                indices_nombres[cell] = indice;
            }
            else if (indice!=0){
                if(row[row.size()-1] != "-1"){
                    // cout<<"Imrpimiendo usuario "<<indice<<endl;
                    // cout<<"Imprimiendo indice Pelicula "<<indicePeliculas<<endl;
                    // cout<<"Imprimiendo rating "<<row[row.size()-1]<<endl;
                    ratings[indice][indicePeliculas] = stof(row[row.size()-1]);
                }
            }
            indice++;
        }
        namesInitialized = true;

        datos.push_back(row);
        indicePeliculas++;
    }

    file.close();

    // Llamar a la función para imprimir los datos
    
    // imprimirDatos(datos);
    formatear();

    // distanciaEntre2Usuarios(1,2);
    // darRecomendaciones(1,2);

    // int usuario1 = convertirStringAIndice("\"Bryan\"");
    // int k = 1;
    // vector< vector<pair<string ,float> > > knn = calcularKNN(usuario1);

    // //Recomendaciones por distancia Euclideana
    // cout<<"Recomendaciones por distancia Euclideana"<<endl;
    // for(int i=0;i<1;i++){
    //     for(int j=0;j<k;j++){
    //         cout << knn[i][j].first <<"     "<< knn[i][j].second<<endl; 
    //         darRecomendaciones(usuario1,indices[knn[i][j].first]);
    //         cout<<endl;
    //     }
    // }

    // //Recomendaciones por distancia Manhattan
    // cout<<"Recomendaciones por distancia Manhattan"<<endl;
    // for(int i=1;i<2;i++){
    //     for(int j=0;j<k;j++){
    //         cout << knn[i][j].first <<"     "<< knn[i][j].second<<endl; 
            
    //         darRecomendaciones(usuario1,indices[knn[i][j].first]);
    //         cout<<endl;
    //     }
    // }

    return 0;
}
