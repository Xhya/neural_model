 <p align="justify"> A computational model in C++ of cerebellar circuitry implied in delay eyeblink classical conditioning. Classical conditioning is a theory dealing with learning results impliying a link between an environmental stimulus and an automatic reaction of the organism. My goal was to obtain the same results of the following experiment: </p>

<img align="center" src="Images/experiment.png" height="400" alt="experiment" />

 <p align="justify">The first step of the experiment is to put a tone from 100ms to 450ms. Then at time 400ms you put an airpuff into the eye of the rabbit; its eye will normally blink. You repeat the experiment several times to "train" the rabbit, then you'll notice that it will blink just before the entry of the air into the eye in order to prevent damage. It suggests that there's a learning of timing between the conditioned stimulus (tone) and the unconditioned stimulus (airpuff). I have implemented the following network, using a LIF model for spiking neurons. Here is the network I implemented:</p>

<img align="center" src="Images/network.png" height="250" alt="experiment" />


 <p align="justify">For the timing learning, I used an classical Hebbian learning between granule cells and basket cells. And an inversed Hebbian learning between granule cells and Purkinje cells.</p>

 <p align="justify">I made up a graphical interface to do tests on the network and adjust the parameters. Here is a picture of the interface:</p>

<img align="center" src="Images/Gui.png" height="400" alt="experiment" />


 <p align="justify">To compile you must have C++14 STL and Gtkmm librairy on your computer. Here are the links:

C++14 : http://scholtyssek.org/blog/2015/06/11/install-gcc-with-c14-support-on-ubuntumint/
<br>
Gtkmm : http://ftp.gnome.org/pub/GNOME/sources/gtkmm/3.22/

To run the program, download the code above, decompress then simply write these following commands: <br>
	> make <br>
	> ./run
	

