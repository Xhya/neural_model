
/*
 * File:   network.hpp
 * Author: Alexia Souvane
 *
 * Created on 06/08/2016
 */

#ifndef BASICS
#define BASICS

#include <iostream>
#include <string.h>
#include <fstream>
#include <map>
#include <vector>
#include <random>
#include <stdio.h>
#include <stdlib.h>
#include <time.h> 


using namespace std;

#endif



#ifndef NETWORK_H
#define NETWORK_H



/****************************************************************/
/* CLASS NEURON 	 											*/
/****************************************************************/


/**  Neuron accounts for a unit of the network */
class Neuron {

protected:
	// Attributes


	/**  Neuron ID*/
	unsigned int nb;

	/**  cell resistance (ohm.m2) */
	double R; 

	/**  cell capacitance (F.m-2)*/
	double C;

	/**  resting potential (V)*/
	double V_max;

	/**  tresholding potential (V)*/
	double V_hyp;	

	/**  voltage value (V) reached by an action potential (at the peak)*/
	double V_rest;	

	/** voltage value (V) during hyperpolarization (after action potential) */
	double V_tresh;	

	/**  current value (A) at time t*/
	double I;

	/**  membrane potential value (V) at time t*/
	double v;

	/** noise value applied to current I */ 
	double noise;

	/**  indicates if the neuron has fired already or not*/
	bool first_firing ;

	/** refractory delay in seconds (remaining time for a neuron to 
	be ready to fire again after an action potential)*/
	double refractory_delay;	

	/** refractory duration in seconds (fixed duration for a neuron to 
	be ready to fire again after an action potential)*/
	double refractory_duration;

	/** recordings list of membrane potentials*/
	vector<double> rec_Vm;

	/**  number of neurons in the network*/
	static unsigned int NB_NEURONS;




public:

	// Constructor
	Neuron(	double ref, 
			double _noise ); 


	// Destructor
	~Neuron();



	// Methods
	
	/**  return value of voltage */
	const double f_v();
	
	/**  update membrane potential value of the Neuron*/
	bool step(double dt);

	// GETTERS & SETTERS

	/** Set the current(A) of the neuron*/
	void set_I(double i);

	/** Get the current value (A) of the neuron*/
	double get_I();

	/** Set the first firing to state */
	void set_first_firing(bool state);

	/** get the value of first firing variable*/
	bool get_first_firing();

	/** Get the vector containing voltage values*/
	vector<double> get_rec_Vm();

	/** Get the number of the neuron*/
	unsigned int get_nb();

	/** Get the current voltage value (V) of the neuron*/
	double get_v();

	/** Get the refractory delay of the neuron*/
	double get_refractory_delay();

	/** Set the refractory delay of the neuron*/
	void set_refractory_delay(double rd);

	/** Get the refractory duration of the neuron*/
	double get_refractory_duration();

	/** Get the maximal value of the voltage (V)*/ 
	double get_V_max();

	/** Get the number of neurons created*/
	static unsigned int get_NB_NEURONS();

	/** Set the number of neuron created*/
	static void set_NB_NEURONS(unsigned int nb);
	

};
















/****************************************************************/
/* CLASS SYNAPSE 												*/
/****************************************************************/


/**  Synapse accounts for the link between two neurons (axon + synapse) */
class Synapse {

protected:
//public:
	// Attributes

	/** synapse ID*/
	unsigned int nb;

	/** a synapse can be excitatory (+1 or true) or inhibitory (-1 or false) */
	int type;

	/**  synaptic weight*/
	double weight;

	/** true if an action potential is running through the axon. Else, false*/
	bool state;

	/** pointer to pre-synaptic Neuron*/
	Neuron *Ppre_syn;

	/** pointer to post-synpatic Neuron*/
	Neuron *Ppost_syn;

	/** determine remaining time for an AP to reach the synapse */
	double delay;

	/** determine a fixed duration for an AP to reach the synapse */
	double delay_duration;

	/** adjust to scale the value of weight*/
	double ww;

	/** current synaptic conductance (S) */
	double g ;

	/** Maximal synaptic conductance (S) */
	double gMax ;

	/** accounts for plasticity of the synapse, that is weight adjustements. 
	If type_of_learning = 0, no plasticity.
	If type_of_learning = 1, hebbian learning.
	If type_of_learning = -1, anti-hebbian learning */
	int type_of_learning;

 	/** is the last time at which the synapse was activated */
	double t_last_AP;


	/** accounts for the number of synapses in the network*/
	static unsigned int NB_SYN;
	







public:
	// Constructor
	Synapse( double _weight, 
			 Neuron *_pre_syn, 
			 Neuron *_post_syn, 
			 bool _type, 
			 double _ww,
			 double _gMax,
			 double _delay_duration,
			 int type_of_learning );


	// Destructor
	~Synapse();


	// Methods
	
	/**  launch action potential in axon (set up delay)*/
	void trigger_AP();

	/**  activate a synapse by increasing synaptic conductance */
	void activate_synapse();

	/**  activate a synapse if delay == 0 (that is, if the AP reached the synapse)*/
	const bool check_triggering();

	/** adjust synaptic conductance value and check if synapse should activate*/
	void step(double dt, double t);

	/** returns value of PSP (post-synaptic potential) */
	double get_PSP();

	/** adjust the weight of the synapse depending on its plasticity */
	void update_weight(double t);

	/** return the weight delta value */
	double calculate_delta_w(double delta_timing);

	// GETTERS & SETTERS

	/** Set the number of synapses created*/
	static void set_NB_SYN(unsigned int nb);

	/** Get the number of the synapse*/
	unsigned int get_nb();

	/** Set the current value of post synaptic neuron*/
	void set_I_Ppost_syn(double i);

	/** Get the current value of post synaptic neuron*/
	double get_I_Ppost_syn();

	/** Set the type of learning of the synapse*/
	void set_type_of_learning(int type_of_learning);

};













/****************************************************************/
/* CLASS NETWORK 												*/
/****************************************************************/


/**  Network accounts for the neural network */
class Network{

protected:
//public:
	// Attributes

	/**  time step (s)*/
	double dt; 

	/**  time t (s)*/
	double t;

	/**  time list (s)*/
	vector<double> time_L;

	/**  neurons list of the network*/
	vector<Neuron> neuron_L;	

	/**  synapses list of the network*/
	vector<Synapse> synapse_L;

	vector<double> first_firing_L;

	/** contain pre synaptic neuron IDs and associates their synapses IDs*/
	map<unsigned int, vector< unsigned int > > connection_pre; 

	/** contain post synaptic neuron IDs and associates their synapses IDs*/
	map<unsigned int, vector< unsigned int > > connection_post; 



public:
	// Constructor
	Network(double dt);


	// Destructor
	~Network();


	// Methods

	/**  add a Neuron to the Network*/
	void add_neuron(	double refractory_duration, 
						double noise  );

	/**  create a Synapse between two Neuron*/
	void connect_neurons(	unsigned int pre_syn_nb, 
							unsigned int post_syn_nb, 
							double weight,
							bool type, 
							double ww,
							double gmax,
							double delay_duration,
							int type_of_learning );
	
	/**  update the network ; that is, update all the Synapse and Neuron*/
	void step(	vector<double> &frequency_first_firing,
				unsigned int nb_purkinje, 
				unsigned int nb_nuclear, 
				unsigned int nb_climbing );

	/** reset the current value of every neurons to 0*/
	void resetI();


	/** create a population of Neurons*/
	vector<unsigned int> create_pop(	unsigned int nb_neurons, 
										double ref,
										double noise );

	/** connect two populations of Neuron*/
	void connect_pop( 	const vector<unsigned int> & V_pre_syn, 
						const vector<unsigned int> & V_post_syn, 
						double weight,
						bool type, 
						double ww,
						double gmax, 
						double delay_duration,
						unsigned int type_of_learning,
						unsigned int nb_pre_max_for_one_post );


	/** create a site of plasticity between two population of neurons*/
	void hebbian_learning(vector<unsigned int> a, vector<unsigned int> b, int type_of_learning);

	// GETTERS & SETTERS

	/** Get the current time*/
	double get_t() { return t ; };
	
	/** Get the neuron number nb*/
	Neuron get_Neuron(unsigned int nb) { return neuron_L[nb]; };

	/** Set the current value to i of neuron #nb */
	void set_I_neuron(unsigned int neuron_nb , double i);

	/** Set the first firing to state of neuron #nb*/
	void set_ff_neuron(unsigned int neuron_nb, bool state);

	/** Get the vector of time*/
	vector<double> get_time_L();

	/** Get the voltage values vector*/
	vector<double> get_recVM_neuron(unsigned int neuron_nb);




};













/****************************************************************/
/* DIVERSE FUNCTIONS											*/
/****************************************************************/

void save(string file_name, vector<double> x, vector<double> y);

void save(string file_name, vector<double> x);

void save(string file_name, map<double, unsigned int> map);

void save_app(string file_name, vector<double> x, vector<double> y);


/** write membrane potentials on file*/
const void save_Vm(string, Network & );

const double get_normal(double,double);

const int get_uniform(int, int);




#endif