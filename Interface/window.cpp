#include <gtkmm.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>

#include "window.hpp"


using namespace std;


Window::Window() { 




    /****** Window features ******/
    set_title("Neural Network");
    resize(600, 600); //Redimensionner la fenêtre : nouvelle largeur = 640px, nouvelle hauteur = 480px.
    set_position(Gtk::WIN_POS_CENTER);
    //fullscreen();

    /****** Menu Bar features ******/
    menuItemFile.set_label("File");
    menuBar.append(menuItemFile);
    menuItemFile.set_submenu(menuFile);


    menu_save.set_label("Save");
    menuFile.append(menu_save);

    menu_quit.set_label("Quit");
    menuFile.append(menu_quit);





    /****** Button feature *****/
    button_run.set_label("Run");
    button_run.signal_clicked().connect([this]() { run_experiment(); }); // when RUN button is clicked, the experiment runs
    button_run.set_margin_top(20);


    L1.set_text("Connection mossy to granule");
    LabelBox.pack_start(L1);
    L2.set_text("Connection mossy to nuclear");
    LabelBox.pack_start(L2);
    L3.set_text("Connection granule to purkinje");
    LabelBox.pack_start(L3);
    L4.set_text("Connection granule to basket");
    LabelBox.pack_start(L4);
    L5.set_text("Connection basket to purkinje");
    LabelBox.pack_start(L5);
    L6.set_text("Connection purkinje to nuclear");
    LabelBox.pack_start(L6);
    L7.set_text("Connection climbing to basket");
    LabelBox.pack_start(L7);
    L8.set_text("Connection climbing to purkinje");
    LabelBox.pack_start(L8);
    L9.set_text("Connection climbing to nuclear");
    LabelBox.pack_start(L9);






    for (int i = 0 ; i < 9 ; i++)
    {
        Gtk::SpinButton *spin_button = new Gtk::SpinButton();
        vect.push_back(spin_button);
    }

    vector<double> values;
    values.push_back(0.5);
    values.push_back(0.3);
    values.push_back(0.8);
    values.push_back(0.5);
    values.push_back(0.5);
    values.push_back(0.9);
    values.push_back(0.5);
    values.push_back(0.5);
    values.push_back(1.0);

    for (int i = 0 ; i < vect.size() ; i++)
    {
        Glib::RefPtr<Gtk::Adjustment> adjustment = Gtk::Adjustment::create(values[i], 0., 1., 0.01); 
        vect[i]->set_adjustment(adjustment);
        vect[i]->set_numeric(); // Allows ONLY numerical values
        vect[i]->configure (adjustment, 0.1, 2);
        SpinButtonBox.pack_start(*vect[i]);
    }




    /****** Image features ******/
    Glib::ustring fImg("Images/Welcome.png") ;
    /* a Pixbuf object is created and then stored in Image object */
    Glib::RefPtr<Gdk::Pixbuf> pixbuf_ptr = Gdk::Pixbuf::create_from_file(fImg, 620, 620) ;
    image.set(pixbuf_ptr);



    /****** Horizontal box features ******/
    LabelBox.set_spacing(10);
    SpinButtonBox.set_spacing(10);


    LeftHBox.pack_start(LabelBox);
    LeftHBox.pack_start(SpinButtonBox);   // Hbox = Horizontal Box (= container)
    
    LeftVBox.set_spacing(20);

    LeftVBox.pack_start(LeftHBox);
    LeftVBox.pack_start(button_run);

    Hbox.pack_start(LeftVBox);
    Hbox.pack_start(image);
    Hbox.pack_start(image_nuclear);

    

    /****** Menu box features ******/
    MenuBox.pack_start(menuBar, Gtk::PACK_SHRINK);
    MenuBox.pack_start(Hbox);



    /***** For display *****/
    add(MenuBox); // add the menu box (=main box) to the window
    show_all(); // Display all widgets

}



void Window::run_experiment()
{

    string cmd = "./modeling " ;



    for(int i = 0 ; i < vect.size() ; i++)
    {
        double nb = vect[i]->get_value() ;
        cmd +=  convert_double_to_str(nb) + " " ;
    }


    system(cmd.c_str());
    //system("python makePicture.py");
    system("python Interface/makeHistogram.py");
    

    /****** Image features ******/
    Glib::ustring fImg("Images/histogram.png") ;
    /* a Pixbuf object is created and then stored in Image object */
    Glib::RefPtr<Gdk::Pixbuf> pixbuf_ptr = Gdk::Pixbuf::create_from_file(fImg, 520, 520) ;
    image.set(pixbuf_ptr);

}


std::string convert_int_to_str(int value)
{
    stringstream s;
    s << value;
    return s.str();
}


std::string convert_double_to_str(double value)
{
    std::ostringstream sstream;
    sstream << value;
    std::string str = sstream.str();

    return str;
}


