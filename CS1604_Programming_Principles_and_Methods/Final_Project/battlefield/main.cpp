#include <iostream>
#include <fstream>
#include "field.h"
#include "units.h"
#include "engine.h"
using namespace std;

//int main()
//{
    /*string filename = "field.txt";
    ofstream ofs;
    ofs.open(filename.c_str());
    if (!ofs) {
        cout << "Cannot open the file: " << filename << endl;
        return -1;
    }*/
    //Field f = loadMap(cin);
    //Field f(11, 11);

    // Set units
    /*addFootman(f, true, 2, 0);
    addFootman(f, true, 2, 1);
    addFootman(f, true, 2, 2);
    addFootman(f, true, 2, 7);

    addFootman(f, false, 7, 4);
    addFootman(f, false, 8, 3);
    addFootman(f, false, 7, 3);
    addFootman(f, false, 9, 5);*/


    // displayField(cout, f);

    //play(f, cin, cout);

    // ofs.close();
    //return 0;
//}

int main()
{
    string filename = "map.txt";
    ifstream ifs;
    ifs.open(filename.c_str());
    if (!ifs) {
        cout << "Cannot open the file: " << filename << endl;
        return -1;
    }

    Field* f = loadMap(ifs);
    if (f == NULL) {
        cout << "Failed to load map!" << endl;
        return -1;
    }
    play(*f, cin, cout);
    //system("PAUSE");
    delete f;
    ifs.close();
    return 0;
}