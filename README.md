## PiFaceCam
### Facial Recognition for Raspberry Pi

#### Overview
PiFaceCam system uses deep neural network for accurate facial recognition. It is designed to run efficiently in Raspberry Pi (tested in Pi3 Model B+ and Pi4 Model B 4GB).
PiFaceCam currently consists of 3 components (which collectively known as PiFaceCam system). 
1. PiFaceCam-Client(Camera): runs in Raspberry Pi with camera attached. It performs facial recognition and carrys out instructions as per the uploaded program.
2. PiFaceCam-App(Controller): runs in Android devices (currently only support android devices). It is for managing all PiFaceCam-Clients connected through local network. It is responsible for;
	* 	  setting and monitor PiFaceCam-Clients.
	* 	  create and manage face ids.
	* 	  create/edit scripts/programs for PiFaceCam-Clients.
3. PiFaceCam-IdServer (Optional): runs in Raspberry Pi. It allows synchronization for face ids among multiple PiFaceCam-Clients (More will be explained at the implementation section).

**Features:**
* Facial recognition based on known face ids and carry instruction per scripts.
* Auto tracking and assigning temporary ids for unknown faces (which user can later review and assign proper ids).
* Comprehensive tools for collecting, editing and organising face ids.
* Simple scripting concept for automation.
* User friendly tools to create, edit and manage scripts/programs.
* Remote monitor, manage and control multiple devices via PiFaceCam-App.
* Built-in functions to control GPIOs, communicate with JSON-server and sending emails.
* Provide callback function that expose raw information (image, detected face-ids and face bounding boxes) for unrestricted capability extension using python programing.


