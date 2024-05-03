#include <iostream>

#include <fstream>
#include <string>
#include <sstream>

#include <vector>
#include <algorithm>
#include <unordered_map>
#include <map>
#include <math.h>

#include <ctime>

#define UMBRAL 3
#define PELICULAS_COMUN 2
#define VALOR_NO_DEFINIDO -1234.1234
#define NUMERO_MAX_RECOMENDACIONES 2

// Aun no funciona y no colcoarl ocreo
#define UMBRAL_PELICULAS_COMUN 2

using namespace std;

unordered_map <int , string> nombresPeliculas;
unordered_map < int , unordered_map<int, long double> > ratings;

// Distancia, peliculas en comun y al final por id


//distanciasEuclideana.push_back(make_pair(usuario_iterator, distancias[0]));
// Esta en usuario, luego distancia luego coincidencias

// vector<pair<  int ,pair<long double,int> > >


//Mayor coincidencias
bool compareSecondMenorAMayor(const pair<  int ,pair<long double,int> > &a, const pair<  int ,pair<long double,int> > &b) {
    if(a.second.first != b.second.first)
        return a.second.first < b.second.first;
    if(a.second.second != b.second.second)
        return a.second.second > b.second.second;
    return a.first < b.first;
}

bool compareSecondMayorAMenor(const pair<  int ,pair<long double,int> > &a, const pair<  int ,pair<long double,int> > &b) {
    if(a.second.first != b.second.first)
        return a.second.first > b.second.first;
    if(a.second.second != b.second.second)
        return a.second.second > b.second.second;
    return a.first < b.first;
}

bool compareSecondMayorAMenorRecomendaciones(const pair<string, long double> &a, const pair<string, long double> &b) {
    if(a.second != b.second)
        return a.second > b.second;
    return a.first < b.first;
}

void imprimirRecomendacionesUsuario(vector<pair<string, long double> > &recomendaciones){
    for(int i=0;i<recomendaciones.size();i++){
        if(i<NUMERO_MAX_RECOMENDACIONES)
            cout<<recomendaciones[i].first<<" con rating de: "<<recomendaciones[i].second<<endl;
    }
    cout<<endl;
}

vector<pair<string, long double> > darRecomendacionesUsuario( int usuario1, int usuario2){
    vector<pair<string, long double> > recomendaciones;

    for (const auto& inner_pair : ratings[usuario2]) {
        int idPelicula = inner_pair.first;  
        long double rating = inner_pair.second; 

        bool ratingUsuario1Exists = ratings.count(usuario1) && ratings[usuario1].count(idPelicula);
        bool ratingUsuario2Exists = ratings.count(usuario2) && ratings[usuario2].count(idPelicula);

        if(!ratingUsuario1Exists && ratingUsuario2Exists){
            if(ratings[usuario2][idPelicula]>UMBRAL){
                //Pushear la pelicula y mostrarla si quieremos
                recomendaciones.push_back( make_pair(nombresPeliculas[idPelicula], ratings[usuario2][idPelicula]));
                // cout<<nombresPeliculas[idPelicula]<<endl;
            }
        }
    }

    sort(recomendaciones.begin(),recomendaciones.end(),compareSecondMayorAMenorRecomendaciones);
    imprimirRecomendacionesUsuario(recomendaciones);
    return recomendaciones;
}



vector<vector <string> >  darRecomendacionesUsuariosPesos(int usuario, vector< vector<pair<int ,long double> > > &vecinosMasCercanos){

    vector<vector <string> > recomendaciones;
    vector< pair< string, long double> > puntuacionesPonderadasPelicula;

    //recomendaciones por euclideana
    for (const auto& inner_pair : ratings[vecinosMasCercanos[0][0].first]) {
        int idPelicula = inner_pair.first;  
        long double rating = inner_pair.second; 
        bool alMenosUnRatingNoExiste = false;

        long double sumDistancias = 0;
        for(int i=0;i<vecinosMasCercanos[0].size();i++){
            bool ratingUsuarioExiste = ratings.count(vecinosMasCercanos[0][i].first) && ratings[vecinosMasCercanos[0][i].first].count(idPelicula);
            if(ratingUsuarioExiste == 0){
                alMenosUnRatingNoExiste = true;
                break;
            }
            else{
                sumDistancias += vecinosMasCercanos[0][i].second;
            }
        }

        long double valoracionPonderada = 0;
        if(alMenosUnRatingNoExiste == false){
            cout<<"Distancias total: "<<sumDistancias<<endl;
            for(int i=0;i<vecinosMasCercanos[0].size();i++){
                long double distancia = vecinosMasCercanos[0][i].second;
                long double rating = ratings[vecinosMasCercanos[0][i].first][idPelicula];
                long double peso = distancia/sumDistancias;

                valoracionPonderada += rating * peso;
            }
            puntuacionesPonderadasPelicula.push_back(make_pair(nombresPeliculas[idPelicula],valoracionPonderada));
        }
    }

    for (const auto& puntuacion : puntuacionesPonderadasPelicula) {
        std::cout << "Película: " << puntuacion.first << ", Puntuación: " << puntuacion.second << std::endl;
    }
    return recomendaciones;
}

bool compareSecondMayorAMenorRecomendaciones(const string& a, const string& b, const unordered_map<string, int>& frecuencias) {
    return frecuencias.at(a) > frecuencias.at(b);
}

vector<string> ordenarRecomendaciones(unordered_map<string, int>& frecuencias) {
    vector<string> recomendaciones;
    for (const auto& par : frecuencias) {
        recomendaciones.push_back(par.first);
    }
    sort(recomendaciones.begin(), recomendaciones.end(), [&](const string& a, const string& b) {
        return compareSecondMayorAMenorRecomendaciones(a, b, frecuencias);
    });
    return recomendaciones;
}

vector<vector <string> >  darRecomendacionesUsuarios(int usuario, vector< vector<pair<int ,long double> > > &vecinosMasCercanos){

    vector<vector <string> > recomendaciones;
    

    for (const auto& fila : vecinosMasCercanos) {
        unordered_map< string, int> frecuencias;
        for (const auto& par : fila) {
            for (const auto&pelicula : ratings[par.first]){
                bool ratingUsuario1Exists = ratings.count(usuario) && ratings[usuario].count(pelicula.first);
                bool ratingUsuario2Exists = ratings.count(par.first) && ratings[par.first].count(pelicula.first);

                if(ratingUsuario1Exists == 0 && ratingUsuario2Exists && ratings[par.first][pelicula.first]>UMBRAL){
                    frecuencias[nombresPeliculas[pelicula.first]] += 1;
                }
            }
            // std::cout << "(" << par.first << ", " << par.second << ") ";
        }
        vector<string> recomendacionesPorDistancia = ordenarRecomendaciones(frecuencias);
        recomendaciones.push_back(recomendacionesPorDistancia);
        // std::cout << std::endl;
    }
    
    return recomendaciones;
}

void imprimirRecomendacionesUsuarios(vector<vector <string> > recomendaciones){
    string distancias[4] = {"Distancia Euclideana","Distancia Manhattan","Correlacion de Pearson","Similitud del coseno"};
    int cont = 0;
    for (const auto& fila : recomendaciones) {
        cout<<"Recomendaciones por: "<<distancias[cont]<<endl;
        cout<<"----------------------------------------------------------------------------------------------------------------------------------------"<<endl;

        for (const std::string& elemento : fila) {
            std::cout << elemento << endl;
        }
        std::cout << std::endl;
        cont++;
        cout<<"----------------------------------------------------------------------------------------------------------------------------------------"<<endl;
    }
}

vector<pair<long double, int > > calcularDistanciasUsuarios(int usuario1, int usuario2){

    vector<pair<long double, int > > distancias;
    long double distanciaEuclideana = 0,
            distanciaManhattan = 0,
            distanciaPearson = 0,
            distanciaCoseno = 0;

    long double sumxy = 0, sumx = 0, sumy = 0, n=0, sumx2 = 0, sumy2 = 0;
    int coincidences = 0;

    for (const auto& inner_pair : ratings[usuario2]) {
        int idPelicula = inner_pair.first;  
        long double rating = inner_pair.second; 

        bool ratingUsuario1Exists = ratings.count(usuario1) && ratings[usuario1].count(idPelicula);
        bool ratingUsuario2Exists = ratings.count(usuario2) && ratings[usuario2].count(idPelicula);

        if(ratingUsuario1Exists && ratingUsuario2Exists){
            coincidences++;
            long double ratingUsuario1 = ratings[usuario1][idPelicula];
            long double ratingUsuario2 = ratings[usuario2][idPelicula];

            sumxy += ratingUsuario1 * ratingUsuario2;
            sumx += ratingUsuario1;
            sumy += ratingUsuario2;
            sumx2 += pow(ratingUsuario1,2);
            sumy2 += pow(ratingUsuario2,2);
            n++;

            distanciaEuclideana += pow( ( ratingUsuario1 - ratingUsuario2), 2);
            distanciaManhattan += abs( ( ratingUsuario1 - ratingUsuario2) );
        }
    }

    if (coincidences>PELICULAS_COMUN){
        if(distanciaEuclideana>=0){
            distanciaEuclideana = sqrt(distanciaEuclideana);
            distancias.push_back(make_pair(distanciaEuclideana,coincidences));
        }
        else{
            distanciaEuclideana = VALOR_NO_DEFINIDO;
            distancias.push_back(make_pair(distanciaEuclideana,coincidences));
        }

        distancias.push_back(make_pair(distanciaManhattan,coincidences));

        long double denominador1 = (sumx2 - pow(sumx,2)/n);
        long double denominador2 = (sumy2 - pow(sumy,2)/n);

        if(n>0 && denominador1 > 0 && denominador2 > 0){
            distanciaPearson = (sumxy - (sumx*sumy)/n)/((sqrt( denominador1) * sqrt( denominador2) ));
            if(distanciaPearson>1){
                distancias.push_back(make_pair(1.0,coincidences));
            }
            else if(distanciaPearson < -1.0){
                distancias.push_back(make_pair(-1.0,coincidences));
            }
            else{
                distancias.push_back(make_pair(distanciaPearson,coincidences));
            }
        }
        else{
            distanciaPearson = VALOR_NO_DEFINIDO;
            distancias.push_back(make_pair(distanciaPearson,coincidences));
        }

        if(sumx2>0 && sumy2>0){
            distanciaCoseno = sumxy / (sqrt(sumx2)*sqrt(sumy2));
            distancias.push_back(make_pair(distanciaCoseno,coincidences));
            // cout<<"Todo bien"<<endl;
        }
        else{
            distanciaCoseno = VALOR_NO_DEFINIDO;
            distancias.push_back(make_pair(distanciaCoseno,coincidences));
            // cout<<"Todo mal"<<endl;
        }
    }
    else{
        distancias.push_back(make_pair(VALOR_NO_DEFINIDO,coincidences));
        distancias.push_back(make_pair(VALOR_NO_DEFINIDO,coincidences));
        distancias.push_back(make_pair(VALOR_NO_DEFINIDO,coincidences));
        distancias.push_back(make_pair(VALOR_NO_DEFINIDO,coincidences));
    }

    return distancias;
}


vector<long double> calcularDistanciasUsuariosConPrint(int usuario1, int usuario2){
    vector<long double> distancias;
    long double distanciaEuclideana = 0,
            distanciaManhattan = 0,
            distanciaPearson = 0,
            distanciaCoseno = 0;

    long double sumxy = 0, sumx = 0, sumy = 0, n=0, sumx2 = 0, sumy2 = 0;
    int coincidences = 0;

    for (const auto& inner_pair : ratings[usuario2]) {
        int idPelicula = inner_pair.first;  
        long double rating = inner_pair.second; 

        bool ratingUsuario1Exists = ratings.count(usuario1) && ratings[usuario1].count(idPelicula);
        bool ratingUsuario2Exists = ratings.count(usuario2) && ratings[usuario2].count(idPelicula);

        if(ratingUsuario1Exists && ratingUsuario2Exists){
            coincidences++;
            long double ratingUsuario1 = ratings[usuario1][idPelicula];
            long double ratingUsuario2 = ratings[usuario2][idPelicula];

            sumxy += ratingUsuario1 * ratingUsuario2;
            sumx += ratingUsuario1;
            sumy += ratingUsuario2;
            sumx2 += pow(ratingUsuario1,2);
            sumy2 += pow(ratingUsuario2,2);
            n++;

            distanciaEuclideana += pow( ( ratingUsuario1 - ratingUsuario2), 2);
            distanciaManhattan += abs( ( ratingUsuario1 - ratingUsuario2) );
        }
    }

    if (coincidences>PELICULAS_COMUN){
        if(distanciaEuclideana>=0){
            distanciaEuclideana = sqrt(distanciaEuclideana);
            distancias.push_back(distanciaEuclideana);
            cout<<"Distancia euclideana: "<<distanciaEuclideana<<endl;
        }
        else{
            distanciaEuclideana = VALOR_NO_DEFINIDO;
            distancias.push_back(distanciaEuclideana);
            cout<<"Distancia euclideana: "<<distanciaEuclideana<<endl;
        }

        distancias.push_back(distanciaManhattan);
        cout<<"Distancia Manhattan: "<<distanciaManhattan<<endl;

        long double denominador1 = (sumx2 - pow(sumx,2)/n);
        long double denominador2 = (sumy2 - pow(sumy,2)/n);

        if(n>0 && denominador1 > 0 && denominador2 > 0){
            distanciaPearson = (sumxy - (sumx*sumy)/n)/((sqrt( denominador1) * sqrt( denominador2) ));
            if(distanciaPearson>1){
                distancias.push_back(1.0);
                cout<<"Distancia Pearson: "<<1<<endl;
            }
            else if(distanciaPearson < -1.0){
                distancias.push_back(-1.0);
                cout<<"Distancia Pearson: "<<-1<<endl;
            }
            else{
                cout<<"Distancia Pearson: "<<distanciaPearson<<endl;
                distancias.push_back(distanciaPearson);
            }
        }
        else{
            distanciaPearson = VALOR_NO_DEFINIDO;
            distancias.push_back(distanciaPearson);
            cout<<"Distancia Pearson: "<<distanciaPearson<<endl;
        }
        if(sumx2>0 && sumy2>0){
            distanciaCoseno = sumxy / (sqrt(sumx2)*sqrt(sumy2));
            distancias.push_back(distanciaCoseno);
            cout<<"Distancia Coseno: "<<distanciaCoseno<<endl;
        }
        else{
            distanciaCoseno = VALOR_NO_DEFINIDO;
            distancias.push_back(distanciaCoseno);
            cout<<"Distancia Coseno: "<<distanciaCoseno<<endl;
        }
    }
    else{
        cout<<"No tienen peliculas en comun"<<endl;
        distancias.push_back(VALOR_NO_DEFINIDO);
        distancias.push_back(VALOR_NO_DEFINIDO);
        distancias.push_back(VALOR_NO_DEFINIDO);
        distancias.push_back(VALOR_NO_DEFINIDO);
    }

    return distancias;
}

vector<pair<  int ,pair<long double,int> > > sacarNPrimerosVector(vector<pair<  int ,pair<long double,int> > > &vec, int n){
    int tam = vec.size();
    int minimo = std::min(n,tam);
    vector<pair<  int ,pair<long double,int> > > primerosElementos(vec.begin(), vec.begin() + minimo);
    return primerosElementos;
}

vector<vector<pair<int ,pair<long double,int> > > > KNN(int usuario,int k){
    vector<vector<pair<int ,pair<long double,int> > > > vecinosMasCercanos;

    vector<pair<  int ,pair<long double,int> > > distanciasEuclideana,
                                distanciasManhattan,
                                distanciasPearson,
                                distanciasCoseno;

    for (const auto& rating : ratings) {
        int usuario_iterator = rating.first;
        if(usuario_iterator != usuario){

            long double distanciaEuclideana,
                distanciaManhattan,
                distanciaPearson,
                distanciaCoseno;
            
            vector<pair<long double, int > > distancias = calcularDistanciasUsuarios(usuario,usuario_iterator);
            for(int i=0;i<distancias.size();i++){
                if(distancias[i].first!=VALOR_NO_DEFINIDO){
                    if(i==0){
                        // distanciaEuclideana = distancias[0].first;
                        distanciasEuclideana.push_back(make_pair(usuario_iterator, distancias[0]));
                    }
                    else if(i==1){
                        // distanciaManhattan = distancias[1].first;
                        distanciasManhattan.push_back(make_pair(usuario_iterator, distancias[1]));
                    }
                    else if(i==2){
                        distanciaPearson = distancias[2].first;
                        if(distanciaPearson>=-1 && distanciaPearson<=1)
                            distanciasPearson.push_back(make_pair(usuario_iterator, distancias[2]));
                    }
                    else if(i==3){
                        // cout<< "Estoy entrando"<<endl;
                        distanciaCoseno = distancias[3].first;
                        if(distanciaCoseno>=-1 && distanciaCoseno<=1)
                            distanciasCoseno.push_back(make_pair(usuario_iterator, distancias[3]));
                        // else{
                        //     cout<<"No estoy agregando"<< distancias[3].first<<endl;
                        // }
                    }
                }
            }
        }
    }

    sort(distanciasEuclideana.begin(), distanciasEuclideana.end(), compareSecondMenorAMayor); 
    sort(distanciasManhattan.begin(),distanciasManhattan.end(), compareSecondMenorAMayor);
    sort(distanciasPearson.begin(), distanciasPearson.end(), compareSecondMayorAMenor); 
    sort(distanciasCoseno.begin(),distanciasCoseno.end(), compareSecondMayorAMenor);

    vecinosMasCercanos.push_back(sacarNPrimerosVector(distanciasEuclideana,k));
    vecinosMasCercanos.push_back(sacarNPrimerosVector(distanciasManhattan,k));
    vecinosMasCercanos.push_back(sacarNPrimerosVector(distanciasPearson,k));
    vecinosMasCercanos.push_back(sacarNPrimerosVector(distanciasCoseno,k));

    return vecinosMasCercanos;
}

void guardarNombresPeliculas(string rutaArchivoMovies){
    ifstream file(rutaArchivoMovies);

    string line;
    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        string cell;
        int cont=0;
        int indice;
        string nombre;
        while (getline(ss, cell, ',')) {
            cont++;
            if(cont == 1)
                indice = stoi(cell);
            else if(cont == 2)
                nombre = cell;
            else {
                break;
            }
            
        }
        nombresPeliculas[indice] = nombre;
    }

    file.close();
}

void leerRatings(string rutaArchivo){
    ifstream file(rutaArchivo);

    string line;
    getline(file, line);
    while (getline(file, line)) {
        stringstream ss(line);
        string cell;

        int cont=0,indiceUsuario,indicePelicula;
        long double rating;

        while (getline(ss, cell, ',')) {
            cont++;
            if(cont == 1)
                indiceUsuario = stoi(cell);
            else if(cont == 2)
                indicePelicula = stoi(cell);
            else if(cont == 3)
                rating = stof(cell);
            else {
                break;
            }
        }

        ratings[indiceUsuario][indicePelicula] = rating;
    }

    file.close();
}

int main(){
    // // Dataset muy pequeño
    // string rutaPeliculas = "/home/alvaro/Documents/Universidad/5toA/TopicosCienciaDeDatos/SistemaRecomendacion/Movielens/3.-profesora/movies.csv";
    // string rutaRatings = "/home/alvaro/Documents/Universidad/5toA/TopicosCienciaDeDatos/SistemaRecomendacion/Movielens/3.-profesora/ratings.csv";

    // // Dataset pequeño (100 000)
    // string rutaPeliculas = "/home/alvaro/Documents/Universidad/5toA/TopicosCienciaDeDatos/SistemaRecomendacion/Movielens/1.-ml-latest-small/movies.csv";
    // string rutaRatings = "/home/alvaro/Documents/Universidad/5toA/TopicosCienciaDeDatos/SistemaRecomendacion/Movielens/1.-ml-latest-small/ratings.csv";

    // // Dataset 20M
    // string rutaPeliculas = "/home/alvaro/Documents/Universidad/5toA/TopicosCienciaDeDatos/SistemaRecomendacion/Movielens/4.-ml-20m/movies.csv";
    // string rutaRatings = "/home/alvaro/Documents/Universidad/5toA/TopicosCienciaDeDatos/SistemaRecomendacion/Movielens/4.-ml-20m/ratings.csv";

    // Dataset grande 33 M
    string rutaPeliculas = "/home/alvaro/Documents/Universidad/5toA/TopicosCienciaDeDatos/SistemaRecomendacion/Movielens/2.-ml-latest/movies.csv";
    string rutaRatings = "/home/alvaro/Documents/Universidad/5toA/TopicosCienciaDeDatos/SistemaRecomendacion/Movielens/2.-ml-latest/ratings.csv";

    // nombresPeliculas.reserve(577966);
    // ratings.reserve(661950);

    guardarNombresPeliculas(rutaPeliculas);
    leerRatings(rutaRatings);


    // Distancias
    // calcularDistanciasUsuarios(11400,100);
    // calcularDistanciasUsuarios(159444,149444);

    // calcularDistanciasUsuariosConPrint(150000, 2428);
    // calcularDistanciasUsuariosConPrint(150000, 21659);
    // calcularDistanciasUsuariosConPrint(150000, 26923);
    // calcularDistanciasUsuariosConPrint(150000, 28015);
    // calcularDistanciasUsuariosConPrint(150000, 88905);

    int usuario1 = 1;
    int k = 10;

    unsigned t0, t1;

    t0=clock();

    vector< vector<pair<int ,pair<long double,int > > > > vecinosMasCercanos = KNN(usuario1,k);

    t1 = clock();

    double time = (double(t1-t0)/CLOCKS_PER_SEC);
    cout << "Tiempo de ejecucion del KNN: " << time << endl<<endl<< endl<<endl;

    cout<<"Recomendaciones por distancia Euclideana"<<endl;
    cout<<"----------------------------------------------------------------------------------------------------------------------------------------"<<endl;
    for(int j=0;j<vecinosMasCercanos[0].size();j++){
        // if(j == 4){
        cout << "Usuario: "<<vecinosMasCercanos[0][j].first <<"    con distancia: "<< vecinosMasCercanos[0][j].second.first<<" con coincidencias: "<< vecinosMasCercanos[0][j].second.second <<endl; 
        darRecomendacionesUsuario(usuario1, vecinosMasCercanos[0][j].first);
        // }
        
    }
    cout<<"----------------------------------------------------------------------------------------------------------------------------------------"<<endl;


    cout<<"Recomendaciones por distancia Manhattan"<<endl;
    cout<<"----------------------------------------------------------------------------------------------------------------------------------------"<<endl;
    for(int j=0;j<vecinosMasCercanos[1].size();j++){
        cout << "Usuario: "<< vecinosMasCercanos[1][j].first <<"    con distancia: "<< vecinosMasCercanos[1][j].second.first << " con coincidencias: "<< vecinosMasCercanos[1][j].second.second<<endl; 
        darRecomendacionesUsuario(usuario1, vecinosMasCercanos[1][j].first);
    }
    cout<<"----------------------------------------------------------------------------------------------------------------------------------------"<<endl;

    cout<<"Recomendaciones por correlacion de Pearson"<<endl;
    cout<<"----------------------------------------------------------------------------------------------------------------------------------------"<<endl;
    for(int j=0;j<vecinosMasCercanos[2].size();j++){
        cout << "Usuario: "<< vecinosMasCercanos[2][j].first <<"    con correlacion de Pearson de: "<< vecinosMasCercanos[2][j].second.first<<" con coincidencias: "<< vecinosMasCercanos[2][j].second.second<<endl; 
        darRecomendacionesUsuario(usuario1, vecinosMasCercanos[2][j].first);
    }
    cout<<"----------------------------------------------------------------------------------------------------------------------------------------"<<endl;

    cout<<"Recomendaciones por similitud del Coseno"<<endl;
    cout<<"----------------------------------------------------------------------------------------------------------------------------------------"<<endl;
    for(int j=0;j<vecinosMasCercanos[3].size();j++){
        cout << "Usuario: "<< vecinosMasCercanos[3][j].first <<"    con similitud del coseno: "<< vecinosMasCercanos[3][j].second.first << " con coincidencias: "<< vecinosMasCercanos[3][j].second.second<<endl; 
        darRecomendacionesUsuario(usuario1, vecinosMasCercanos[3][j].first);
    }
    cout<<"----------------------------------------------------------------------------------------------------------------------------------------"<<endl;


    // vector<vector<string> > recomendacionesKNN = darRecomendacionesUsuarios(usuario1,vecinosMasCercanos);
    // imprimirRecomendacionesUsuarios(recomendacionesKNN);
    return 0;
}


//Imprimir comunes y no comunes
// y un if para colocar solo las recomendaciones y no solo las comunes


//Si distancia es igual, busqueda mayor por comun
//