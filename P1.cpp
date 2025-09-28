#include <iostream>
#include <string>
#include <functional>
#include <map>
#include <cstdlib>
using namespace std;

int main(int argc, char* argv[]) {
    if(argc != 4) {
        cerr << "USO: ./operacion <num1> <operador> <num2>\n";
        return 1;
    }

    try {
        float num1 = stof(argv[1]);
        string op = argv[2];
        float num2 = stof(argv[3]);

        map<string,function<float(float,float)>> operaciones;
        operaciones["+"] = [](float a,float b){ return a+b; };
        operaciones["-"] = [](float a,float b){ return a-b; };
        operaciones["*"] = [](float a,float b){ return a*b; };
        operaciones["x"] = [](float a,float b){ return a*b; };
        operaciones["/"] = [](float a,float b){ 
            if(b==0) throw runtime_error("Error: División entre cero");
            return a/b; 
        };

        if(!operaciones.count(op)) {
            cerr << "Operador inválido. Use + - * x /\n";
            return 1;
        }

        float resultado = operaciones[op](num1,num2);
        cout << "El resultado de la operacion es " << resultado << endl;

    } catch(exception& e) {
        cerr << "Ha ingresado valores inadecuados: " << e.what() << endl;
        cerr << "USO ./operacion <num1> <operador> <num2>\n";
        return 1;
    }
    return 0;
}
