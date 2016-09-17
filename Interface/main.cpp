//g++ *.cpp -o Fenetre `pkg-config gtkmm-3.0 --cflags --libs` -std=c++14
#include <gtkmm.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>


#include "window.hpp"

using namespace std;


int main(int argc, char* argv[]) {

	vector<double> a;
    Gtk::Main app(argc, argv);
    Window window;

    Gtk::Main::run(window);

    return 0;
}
