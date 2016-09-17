#include "network.hpp"





/****************************************************************/
/* CLASS NEURON 												*/
/****************************************************************/


unsigned int Neuron::NB_NEURONS = 0;

Neuron::Neuron(double ref, double _noise) 
{
	nb = NB_NEURONS;
	NB_NEURONS++;

	R 	= 1.e8;
	C 	= 1.e-10;

	V_rest  = -0.065;
	V_tresh = -0.055;
	V_max 	=  0.04;
	V_hyp 	= -0.08;

	noise = _noise;

	refractory_delay = 0. ; 

	refractory_duration = ref;

	I 	= 0;
	v 	= V_rest;

	first_firing = false;

	//cout << "NEURON " << nb << " CREATED" << endl;
}

Neuron::~Neuron() {}


const double Neuron::f_v()
{
	double value;

	if (noise > 0)
		value = get_normal(0,1);
	else
		value = 0;

	double N = value * noise;

	return ( (I+N) / C ) - ( v-V_rest) / ( R*C )  ;		
}


bool Neuron::step(double dt)
{
	if (v == V_max or refractory_delay > 0)
	{
		v = V_hyp;
		return false;
	}

	v += dt * f_v();

	if (v >= V_tresh)
	{
		v = V_max;
		return true;
	}

	return false;
}

// GETTERS & SETTERS

void Neuron::set_I(double i)
{
	I = i;
}


void Neuron::set_first_firing(bool state)
{
	first_firing = state;
}

vector<double> Neuron::get_rec_Vm()
{
	return rec_Vm;
}

unsigned int Neuron::get_nb()
{
	return nb;
}


double Neuron::get_I()
{
	return I;
}


double Neuron::get_v()
{
	return v;
}

double Neuron::get_refractory_delay()
{
	return refractory_delay;
}

void Neuron::set_refractory_delay(double rd)
{
	refractory_delay = rd;
}

bool Neuron::get_first_firing()
{
	return first_firing;
}

double Neuron::get_refractory_duration()
{
	return refractory_duration;
}

double Neuron::get_V_max()
{
	return V_max;
}

unsigned int Neuron::get_NB_NEURONS()
{
	return NB_NEURONS;
}

void Neuron::set_NB_NEURONS(unsigned int nb)
{
	NB_NEURONS = nb;
}












/****************************************************************/
/* CLASS SYNAPSE 												*/
/****************************************************************/



unsigned int Synapse::NB_SYN = 0;

default_random_engine generator(time(0));



Synapse::Synapse(double _weight, Neuron *_pre_syn, Neuron *_post_syn, bool _type, double _ww, double _gMax, double _delay_duration, int _type_of_learning)
{

	nb = NB_SYN;
	NB_SYN++;

	weight	=  _weight;

	Ppre_syn  = _pre_syn;
	Ppost_syn = _post_syn;

	delay = 0. ;

	if (_type)
		type = +1;
	else
		type = -1;

	ww = _ww;

	g = 0;

	gMax = _gMax;

	delay_duration = _delay_duration;

	state = false;

	type_of_learning = _type_of_learning;

	t_last_AP = -1.;

	//cout << "SYNAPSE " << nb << " CREATED		" << 
	//Ppre_syn->nb << "  ----->  " << Ppost_syn->nb << endl;

}


Synapse::~Synapse() {}


const bool Synapse::check_triggering()
{
	if (delay <= 0 and state )
		return true;
	else
		return false;
}


void Synapse::trigger_AP()
{	
 	delay = delay_duration; 
 	state = true;
}


void Synapse::activate_synapse()
{		
	if (type == 1)
		g += gMax;
	else if (type == -1)
		g -= gMax;
	
	state = false;
}
    

void Synapse::step(double dt, double t)
{
	g *= (1.- dt / 0.4);
	
	if ( state )
		delay -= dt;	

	if ( check_triggering() )
	{
		activate_synapse();
		t_last_AP = t;		
	}
}



double Synapse::get_PSP()
{

	if (weight < 0)
		weight = 0;
	if(weight > 1)
		weight = 1;
	
	return g * weight ;
}



void Synapse::update_weight(double t)
{

	if (type_of_learning != 0)
	{

 		double delta_timing = t_last_AP - t;

		double delta_w = calculate_delta_w(delta_timing);

		weight = weight + delta_w;

		if ( weight < 0)
			weight = 0;
		if ( weight > 1)
			weight = 1;

	}

}




double Synapse::calculate_delta_w(double delta_timing)
{

	if (type_of_learning == 1)
	{
		if (delta_timing < -0.010 or delta_timing > 0.010)
			return 0;
		
		if(delta_timing <= 0)
			return ( 0.04 * delta_timing + 0.2 ) * weight ;
		else
			return ( -0.04 * delta_timing + 0.2 )* weight ;
	}


	if (type_of_learning == -1)
	{

		if (delta_timing < -0.010 or delta_timing > 0.010)
			return 0;

		if(delta_timing <= 0)
			return ( - 0.04 * delta_timing - 0.2 ) * weight ;
		else
			return ( 0.04 * delta_timing - 0.2 )* weight ;
	}

}

// GETTERS & SETTERS


void Synapse::set_NB_SYN(unsigned int nb)
{
	NB_SYN = nb;
}


unsigned int Synapse::get_nb()
{
	return nb;
}

void Synapse::set_I_Ppost_syn(double i)
{
	Ppost_syn->set_I(i);
}

double Synapse::get_I_Ppost_syn()
{
	return Ppost_syn->get_I();
}

void Synapse::set_type_of_learning(int tol)
{
	type_of_learning = tol;
}







/****************************************************************/
/* CLASS NETWORK												*/
/****************************************************************/


Network::Network(double _dt)
{
	dt 		   = _dt; 		// time step
	t 		   = 0;

	// vectors & map already initialized empty.
}


Network::~Network() 
{
	Neuron::set_NB_NEURONS(0);
	Synapse::set_NB_SYN(0);
}


void Network::add_neuron(double ref, double noise)
{
	// create a Neuron object and add it to the neurons list of the network.
	Neuron new_neuron(ref, noise);
	neuron_L.push_back(new_neuron);

	// add to connection_pre map
	vector<unsigned int> empty;
	connection_pre[ new_neuron.get_nb() ] = empty;
	connection_post[ new_neuron.get_nb() ] = empty;

}

void Network::connect_neurons(unsigned int pre_syn_nb, unsigned int post_syn_nb, double weight, bool type, double ww, double gmax, double delay_duration, int type_of_learning)
{

	// create a new synapse
	Synapse new_synapse( weight, &neuron_L[pre_syn_nb], &neuron_L[post_syn_nb], type, ww, gmax, delay_duration, type_of_learning );

	// add the synapse to synapses list
	synapse_L.push_back(new_synapse);

	// add to connection_pre map
	connection_pre[ pre_syn_nb ].push_back( new_synapse.get_nb() );
	connection_post[ post_syn_nb ].push_back( new_synapse.get_nb() );

}


void Network::step(	vector<double> &frequency_vector,
					unsigned int nb_purkinje, 
					unsigned int nb_nuclear, 
					unsigned int nb_climbing )
{

	time_L.push_back(t);
	t += dt;

	// update synapses
	for (int i = 0 ; i < synapse_L.size() ; i++ )
	{
		synapse_L[i].step(dt,t);
		synapse_L[i].set_I_Ppost_syn(synapse_L[i].get_I_Ppost_syn() + synapse_L[i].get_PSP());
	}





	// update neurons
	for (int i = 0 ; i < neuron_L.size() ; i++)
	{

		// record membrane potential
		neuron_L[i].get_rec_Vm().push_back( neuron_L[i].get_v() );


		if ( neuron_L[i].get_refractory_delay() > 0 )
			neuron_L[i].set_refractory_delay(neuron_L[i].get_refractory_delay() - dt);

		// if there's an AP in a neuron:
		// 		- refractory period starts
		//		- spiking time is recorded
		// 		- AP propagated through the axon to the synapse
		if (neuron_L[i].step(dt))
		{

			if(neuron_L[i].get_nb() == nb_nuclear and neuron_L[i].get_first_firing() == false)
			{
				frequency_vector.push_back(t);
				neuron_L[i].set_first_firing(true);
			}

			// neuron gets in refractory period
			neuron_L[i].set_refractory_delay(neuron_L[i].get_refractory_duration()) ; // est ce que la période réfractaire est la mm pour tous les neurones ?


			// record spiking time
			//neuron_L[i].rec_ST.push_back(t);

	
			// AP triggered (delay set up)
			for (int j = 0 ; j < connection_pre[i].size() ; j++)
				synapse_L[ connection_pre[i][j] ].trigger_AP();

		}

	}

	if (neuron_L[nb_climbing].get_v() == neuron_L[nb_climbing].get_V_max())
	{
		for (int i = 0 ; i < connection_post[ nb_purkinje ].size() ; i++)
			synapse_L[ connection_post[nb_purkinje][i] ].update_weight(t);
	}


	resetI();

}


void Network::resetI()
{
	for (int i = 0 ; i < neuron_L.size() ; i++ )
		neuron_L[i].set_I(0);
}



vector<unsigned int> Network::create_pop(unsigned int nb_neurons, double ref, double noise)
{
	vector<unsigned int> to_return;

	for (int i = 0 ; i < nb_neurons ; i++)
	{
		to_return.push_back(Neuron::get_NB_NEURONS());
		add_neuron(ref, noise);
	}

	return to_return;

}


void Network::connect_pop( 	const vector<unsigned int> & V_pre_syn, 
							const vector<unsigned int> & V_post_syn, 
							double weight,
							bool type, 
							double ww, 
							double gmax,
							double delay_duration,
							unsigned int type_of_learning,
							unsigned int nb_input )
{

	double w = ww ;

	for (int i = 0 ; i < V_post_syn.size() ; i++ )
	{
		unsigned int nb_connection = get_uniform(1, nb_input );

		if ( ww == 0.0)
			w = 1. / nb_connection;

		for (int j = 0 ; j < nb_connection ; j++ )
		{
			double we = get_normal(weight,0.1);
			connect_neurons( V_pre_syn[j], V_post_syn[ i ], we, type, w, gmax, get_normal(delay_duration,0.003), type_of_learning);
		}
		


	}

}



void Network::hebbian_learning(vector<unsigned int> a, vector<unsigned int> b, int type_of_learning)
{

	for (int i = a[0] ; i <= a.back() ; i++)
	{
		for (int j = 0 ; j < connection_pre[i].size() ; j++)
			synapse_L[  connection_pre[i][j]  ].set_type_of_learning(type_of_learning);
	}


}


void Network::set_I_neuron(unsigned int neuron_nb , double i)
{
	neuron_L[neuron_nb].set_I(i);
}


void Network::set_ff_neuron(unsigned int neuron_nb, bool state)
{
	neuron_L[neuron_nb].set_first_firing(state);
}

vector<double> Network::get_time_L()
{
	return time_L;
}

vector<double> Network::get_recVM_neuron(unsigned int neuron_nb)
{
	return neuron_L[neuron_nb].get_rec_Vm();
}









/****************************************************************/
/* DIVERSE FUNCTIONS											*/
/****************************************************************/



void save(string file_name, vector<double> x, vector<double> y)
{

    ofstream file(file_name.c_str(), ios::out | ios::trunc);  // flux declaration and opening file

    if(file)
	{
    	for (int i = 0 ; i < x.size() ; i++)
    	    file << x[i] << ";" << y[i] << "\n" ;

        file.close();  
	}

    else 
            cerr << " Opening file error !" << endl;

}




void save(string file_name, vector<double> x)
{

    ofstream file(file_name.c_str(), ios::out | ios::trunc);  // flux declaration and opening file

    if(file)
	{
    	for (int i = 0 ; i < x.size() ; i++)
    	{
    		if (i != 0)
    			file << ";";
    	    
    	    file << x[i] ;
    	}
       	
       	file << "\n" ;

        file.close();  
	}

    else 
            cerr << " Opening file error !" << endl;

}


void save(string file_name, map<double, unsigned int> map)
{
    ofstream file(file_name.c_str(), ios::out | ios::trunc);  // flux declaration and opening file

    if (file)
    {
    	std::map<double,unsigned int>::iterator it;

    	for(it=map.begin() ; it != map.end() ; it++)
    	    file << it->first << ";" << it->second << "\n" ;

    	file.close();
    }

    else 
        cerr << " Opening file error !" << endl;

}




void save_app(string file_name, vector<double> x, vector<double> y)
{

    ofstream file(file_name.c_str(), ios::out | ios::app);  // flux declaration and opening file

    if(file)
	{
    	
    	for (int i = 0 ; i < x.size() ; i++)
    	{
    		if (i != 0)
    			file << ";";
    	    
    	    file << x[i] ;
    	}
       	
       	file << "\n" ;

      

    	for (int i = 0 ; i < y.size() ; i++)
    	{
    		if (i != 0)
    			file << ";";
    	    
    	    file << y[i] ;
    	}

    	file << "\n" ;

        file.close();  
	}

    else 
            cerr << " Opening file error !" << endl;

}






const double get_normal(double mean,double sd)
{
	normal_distribution <double> nd(mean,sd); 
	return nd(generator);
}



const int get_uniform(int a, int b)
{
	uniform_int_distribution <int> ud{a, b}; // a number between a and b
	return ud(generator);
}


