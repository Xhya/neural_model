#ifndef DEF_WINDOW
#define DEF_WINDOW


using namespace std;


class Window : public Gtk::Window {
    public :
        Window();
        void run_experiment();


    
    private :

        /** Vertical box containing Menu*/
        Gtk::VBox                   MenuBox;
        Gtk::MenuBar                menuBar;
        Gtk::MenuItem               menuItemFile;
        Gtk::Menu                   menuFile;
        Gtk::ImageMenuItem          menu_save;
        Gtk::ImageMenuItem          menu_quit;


        /** Labels */
        Gtk::Label                  L1, L2, L3, L4, L5, L6, L7, L8, L9;

        /** Boxes containing different buttons*/ 
        Gtk::HBox                   Hbox, LeftHBox;
        Gtk::Box                    box;
        Gtk::VButtonBox             LabelBox, SpinButtonBox, LeftVBox;
       	
        /** Images*/
        Gtk::Image                  image;
        Gtk::Image                  image_nuclear;

        /** Run button*/
        Gtk::Button                 button_run;

        /** Spin button is an adjusting button*/
        vector<Gtk::SpinButton*>    vect;


};

std::string convert_int_to_str(int value);
std::string convert_double_to_str(double value);


#endif