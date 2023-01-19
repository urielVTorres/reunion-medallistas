//Para la entrada y salida de datos.
#include<iostream>
//Para la lectura y escritura de ficheros.
#include<fstream>
//Complemento para lectura y escritura de ficheros.
#include<sstream>
//Para arreglos dinamicos
#include<vector>

using namespace std;

template<typename T>
using pointer = T*;

//Estructura de tipo atleta para guardar la información de los deportistas
struct Atleta{
    string name;
    string lastName;
    string nation;
    int medal;
    //constructores
    Atleta():name{""}, lastName{""}, nation{""}, medal{0} {};
    Atleta(string _name, string _lastName, string _nation, int _medal): name{_name}, lastName{_lastName}, nation{_nation}, medal{_medal} {};
};

//Estructura del nodo del árbol que almacenará a los deportistas.
struct Node{
    Atleta dato;
    pointer<Node> izq;
    pointer<Node> der;
    //constructor
    Node(Atleta value) : dato{value}, izq{nullptr}, der{nullptr} {};
};

//Guarda un nuevo deportista en el nodo de un árbol binario.
auto insertar(Atleta value, pointer<Node> &root) -> void {
    if(root == nullptr){
        auto nuevo_nodo = new Node(value);
        root = nuevo_nodo;
        return;
    }
    if(value.lastName < root->dato.lastName){
        insertar(value, root->izq);
    }else{
        insertar(value, root->der);
    }
}

//Retorna la información de un deportista almacenada en un árbol. Si no se encuentra en el árbol binario, retorna un deportista sin nombre.
auto buscar(string name, string lastName, pointer<Node> &root) -> Atleta{
    if(root == nullptr){
        Atleta notfound("No", "Encontrado", "", 0);
        return notfound;
    }else if(root->dato.lastName == lastName && root->dato.name == name){
        return root->dato;
    }else if(lastName < root->dato.lastName)
        return buscar(name, lastName, root->izq);
    else 
        return buscar(name, lastName, root ->der);
}

//Imprime en orden alfabetico la información de los deportistas almacenados en el árbol binario.
auto inorden(pointer<Node> &root) -> void {
    if(root == nullptr){
        return;
    }
    string medalla = root ->dato.medal == 10 ? "Oro" : (root -> dato.medal == 5? "Plata" : "Bronce");
    inorden(root ->izq);
    cout<< "\t" << root ->dato.name << " " << root -> dato.lastName << ", " << root ->dato.nation << endl;
    inorden(root -> der);
}

//Agrega a un arreglo dinámico la información de los deportistas en el árbol binario que comparten la misma nacionalidad.
auto teammates(pointer<Node> &root, vector<Atleta> &compis, string nation) -> void {
    if(root == nullptr){
        return;
    }
    teammates(root ->izq, compis, nation);
    if(nation == root -> dato.nation && root -> dato.lastName != compis[0].lastName) {
        compis.push_back(root -> dato);
    }
    teammates(root -> der, compis, nation);
}

//Gurda la información de un archivo de texto en árboles binarios, según su medalla.
auto readData(string filePath, pointer<Node> &gold, pointer<Node> &silver, pointer<Node> &bronce) -> void {
    ifstream fuente;
    fuente.open(filePath, ios_base::in);
    for(string line; getline(fuente, line);){
        istringstream in(line);
        string name, lastName, nation;
        int medal;
        in >> name >> lastName >> nation >> medal;
        Atleta person(name, lastName, nation, medal);
        if(medal == 10)
            insertar(person, gold);
        else if(medal == 5)
            insertar(person, silver);
        else 
            insertar(person, bronce);
    }
}

//Busca en los árboles binarios la información del deportista del nombre recibido, y retorna un arreglo dinámico de deportistas olimpicos que comparten la misma nacionlidad.
auto teamwork(string name, string lastName, pointer<Node> &gold, pointer<Node> &silver, pointer<Node> &bronce) -> vector<Atleta> {
    vector<Atleta> panas;
    //busca en el árbol binario de medallas de oro
    if(buscar(name, lastName, gold).medal != 0){
        Atleta one = buscar(name, lastName, gold);
        panas.push_back(one),
        teammates(silver, panas, one.nation);
        teammates(bronce, panas, one.nation);
        teammates(gold, panas, one.nation);
    }
    //busca en el árbol binario de medallas de plata
    else if(buscar(name, lastName, silver).medal != 0){
        Atleta one = buscar(name, lastName, silver);
        panas.push_back(one),
        teammates(gold, panas, one.nation);
        teammates(bronce, panas, one.nation);
        teammates(silver, panas, one.nation);
    }
    //busca en el árbol binario de medallas de bronce
    else if(buscar(name, lastName, bronce).medal != 0){
        Atleta one = buscar(name, lastName, bronce);
        panas.push_back(one),
        teammates(gold, panas, one.nation);
        teammates(silver, panas, one.nation);
        teammates(bronce, panas, one.nation);
    }

    return panas;
}

auto main() -> int {
    //Creación de los árboles binarios según su medalla.
    pointer<Node> Gold = nullptr;
    pointer<Node> Silver = nullptr;
    pointer<Node> Bronce = nullptr;

    //Lectura del fichero de dados, y llenado de los árboles binarios.
    readData("Atletas.txt", Gold, Silver, Bronce);
    
    //Menú interactivo.
    int temp;
    do{
        //variables que se ocuparán.
        string nombre, apellido;
        vector<Atleta> elegido;
        int suma = 0;

        cout << "Bienvenido" << endl;
        cout << "\n\t1. Mostrar la lista de deportistas.\n\t2.Buscar un deportista y formar un equipo.\n\t0.Salir.\n\nSeleccione una opcion: ";
        cin >> temp;
        system("cls");
        
        switch (temp){
            //Mostrar la lista de deportistas
            case 1:
                
                cout << "\n\nDeportistas con medalla de oro: " << endl;
                inorden(Gold);
                cout << "\n\nDeportistas con medalla de plata: " << endl;
                inorden(Silver);
                cout << "\n\nDeportistas con medalla de bronce: " << endl;
                inorden(Bronce),
                cout << endl;
                system("PAUSE");
                break;

            //Buscar un deportista y formar un equipo
            case 2:

                cout << "Introduzca el nombre completo del deportista (en minusculas): ";
                cin >> nombre >> apellido;
                system("cls");

                elegido = teamwork(nombre, apellido, Gold, Silver, Bronce);
                if(elegido.size() == 0){
                    cout << "No se ha encontrado al deportista."<< endl << endl;
                    system("PAUSE");
                    break;
                }
                cout << "El mejor equipo para " << elegido[0].nation << " esta conformado por: "<<endl;
                for(int i = 0; i < 7; i++){
                    suma += elegido[i].medal;
                    string medalla = elegido[i].medal == 10 ? "Oro" : (elegido[i].medal == 5 ? "Plata" : "Bronce");
                    cout << "\t" << elegido[i].name << " " << elegido[i].lastName << ", \t" << medalla << endl;

                }
                /*
                for(auto  i: elegido){
                    suma += i.medal;
                    string medalla = i.medal == 10 ? "Oro" : (i.medal == 5 ? "Plata" : "Bronce");
                    cout << i.name << " " << i.lastName << ", \t\t" << medalla << endl;
                }
                */
                cout << "\nPuntaje total: \t" << suma << endl << endl;
                system("PAUSE");
                break;

            //Salir
            case 0:
                
                cout << "Hasta pronto!" << endl;
                break;

            //Si no elige ninguna de las opciones
            default:
                
                cout << "Opcion no valida, intente de nuevo." << endl;
                break;

        }
    }while(temp != 0);

    return 0;
}