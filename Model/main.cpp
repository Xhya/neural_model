#include <iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include <map>
#include <random>
#include <stdio.h>
#include <stdlib.h>
#include <time.h> 


using namespace std;


#include "network.hpp"


int main(int argc, char* argv[]) 
{

	// Get parameters values from graphical interface
    double mossy_granule = stod(argv[1]);
    double mossy_nuclear = stod(argv[2]);
    double granule_purkinje = stod(argv[3]);
    double granule_basket = stod(argv[4]);
    double basket_purkinje = stod(argv[5]);
    double purkinje_nuclear = stod(argv[6]);
    double climbing_basket = stod(argv[7]);
    double climbing_purkinje = stod(argv[8]);
    double climbing_nuclear = stod(argv[9]);




	// init variables (in seconds)
	double dt = 0.001;		
	double duration 		= 10.000 ;	
	double starting_tone 	= 0.000 ;
	double ending_tone 		= 0.500 ;
	double starting_airpuff = 0.400 ;
	double ending_airpuff 	= 0.600 ;

	double interval = 2.500;

	double refractory = 0.003 ;
	double noise = 20e-12 ; 
	double delay_duration = 0.010;
	double ww=0.2;
	double gmax = 0.0433e-9;

	unsigned int nb_trials = 1;

	map<double, unsigned int > frequency_first_firing_map; 
	vector<double> frequency_vector;

	//system("rm learning.txt");
	system("rm Results/histogram.csv");


	for(int trials = 0 ; trials < nb_trials ; trials++)
	{

    	cout << "Experiment " << trials << " is running..." << endl;

		// create network
		Network *net = new Network(dt);

		// create populations
		vector<unsigned int> mossy = net->create_pop(10,refractory, noise);
		vector<unsigned int> granule = net->create_pop(1000,refractory, noise);
		vector<unsigned int> basket = net->create_pop(1,refractory, noise);
		vector<unsigned int> purkinje = net->create_pop(1,refractory, noise);
		vector<unsigned int> nuclear = net->create_pop(1,refractory, noise);
		vector<unsigned int> climbing = net->create_pop(1,refractory, noise);



		// parameters : 
		//		pre synaptic neurone population
		//		post synaptic neuron population
		//		weight 
		// 		type of synapse (true for excitatory, false for inhibitory)
		//		ww (V)
		//		delay duration
		//		type of learning
		// 		max number of input for one output

		net->connect_pop(mossy, 	granule, 	mossy_granule, 		true, 	0. , gmax, delay_duration, 	0, 	mossy.size() );
		net->connect_pop(mossy, 	nuclear, 	mossy_nuclear , 	true, 	0.,  gmax, delay_duration, 	0 , 1 );
		net->connect_pop(granule, 	purkinje, 	granule_purkinje , 	true, 	0. , gmax, delay_duration, 	-1, granule.size() );
		net->connect_pop(granule, 	basket, 	granule_basket , 	true, 	0. , gmax, delay_duration, 	1, 	granule.size() );
		net->connect_pop(basket, 	purkinje, 	basket_purkinje , 	false, 	0. , gmax, delay_duration, 	0, 	basket.size() );
		net->connect_pop(purkinje, 	nuclear, 	purkinje_nuclear , 	false, 	0. , gmax, delay_duration, 	0, 	purkinje.size() );
		net->connect_pop(climbing, 	basket, 	climbing_basket , 	true, 	0. , gmax, delay_duration, 	0, 	climbing.size() );
		net->connect_pop(climbing, 	purkinje, 	climbing_purkinje , true, 	0. , gmax, delay_duration, 	0, 	climbing.size() );
		net->connect_pop(climbing, 	nuclear, 	climbing_nuclear , 	true, 	0. , gmax, delay_duration, 	0, 	climbing.size() );


		// run experiment
		while(net->get_t() <= duration)
		{

			if (net->get_t() == 0 or (net->get_t() > interval-0.001 and net->get_t() < interval+0.001) or (net->get_t() > 2*interval-0.001 and net->get_t() < 2*interval+0.001) or (net->get_t() > 3*interval-0.001 and net->get_t() < 3*interval+0.001) )
				net->set_ff_neuron(nuclear[0], false);


			// Injects current into mossy fibers for 4 trials
			if ( starting_tone <= net->get_t() and net->get_t() <= ending_tone )
			{
				for (int i = mossy[0] ; i <= mossy.back() ; i++)
					net->set_I_neuron(i , 0.100025e-9) ;
			}

			if ( starting_tone + interval <= net->get_t() and net->get_t() <= ending_tone + interval )
			{
				for (int i = mossy[0] ; i <= mossy.back() ; i++)
					net->set_I_neuron(i , 0.100025e-9);
			}

			if ( starting_tone + 2*interval <= net->get_t() and net->get_t() <= ending_tone + 2 * interval )
			{
				for (int i = mossy[0] ; i <= mossy.back() ; i++)
					net->set_I_neuron(i , 0.100025e-9);
			}
			if ( starting_tone + 3*interval <= net->get_t() and net->get_t() <= ending_tone + 3 * interval )
			{
				for (int i = mossy[0] ; i <= mossy.back() ; i++)
					net->set_I_neuron(i , 0.100025e-9);
			}



			// Injects current into climbing fibers for 4 trials
			if ( starting_airpuff <= net->get_t() and net->get_t() <= ending_airpuff )
			{
				for (int i = climbing[0] ; i <= climbing.back() ; i++)
					net->set_I_neuron(i , 0.100025e-9);
			}

			if ( starting_airpuff + interval <= net->get_t() and net->get_t() <= ending_airpuff + interval )
			{
				for (int i = climbing[0] ; i <= climbing.back() ; i++)
					net->set_I_neuron(i , 0.100025e-9);
			}

			if ( starting_airpuff + 2 * interval <= net->get_t() and net->get_t() <= ending_airpuff + 2*interval )
			{
				for (int i = climbing[0] ; i <= climbing.back() ; i++)
					net->set_I_neuron(i , 0.100025e-9);
			}
			if ( starting_airpuff + 3 * interval <= net->get_t() and net->get_t() <= ending_airpuff + 3*interval )
			{
				for (int i = climbing[0] ; i <= climbing.back() ; i++)
					net->set_I_neuron(i , 0.100025e-9);
			}


			net->step(frequency_vector, purkinje[0], nuclear[0], climbing[0]);

		}

		cout << "Experiment done" << endl;

		//if (trials == 0)
			//save("/home/alexia/dossiers_partages/Documents_LINUX/Stage/Stage_neuro/Code/V3/Interface/Interface/Results/learning.csv", net->get_time_L(), net->get_recVM_neuron(nuclear[0])) ; 

		delete net;


	}

	cout << "Saving..." << endl;

	save("Results/histogram.csv", frequency_vector);

	cout << "Saving done" << endl;

	return 0;
	

}