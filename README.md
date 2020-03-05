**Downloads:**
* Complete user guide:[PiFaceCam User Guide V2_1.pdf](https://github.com/tensorfactory/PiFaceCam/tree/master/downloads)
* PiFaceCam-App Community (Free from Google Play):[PiFaceCam Application](https://play.google.com/store/apps/details?id=com.tensorfactory.pifacecamcommunity)
* Image of SD Card (Raspbian Buster desktop + necessary libraries + PiFaceCam-IDServer + PiFaceCam-Client):[2019-09-26 buster with base libraries and pifacecam 1.3.11.img.7z](https://drive.google.com/drive/folders/17RT4cYmYMQs3T3JijVTyo2ctESoDOa-v?usp=sharing)

![alt text](images/Overview V02.png)
## PiFaceCam
### Facial Recognition for Raspberry Pi

#### Overview
PiFaceCam system uses deep neural network for accurate facial recognition. It is designed from ground-up to run efficiently in Raspberry Pi*.
PiFaceCam currently consists of 3 components (which collectively known as PiFaceCam system). 
 
1. PiFaceCam_Edge (Client): Runs in Raspberry Pi* with camera(s) attached. It performs all facial recognition processes and can be programmed to carry out instructions using simple scripts. It can also be programmed to send facial data to other devices.
2. PiFaceCam_Edge (Verification server):  Runs in Raspberry Pi* with camera(s) attached.  It acts as verification server where client send images for verifications.  It compares the face in received image with the one detected in front of the camera(s) and return the verification results to clients.
3. PiFaceCam_App: Runs in Android devices (currently only support android devices). It is for managing and controls all PiFaceCam devices connected through local network.

**Features:**
* Facial recognition based on assigned face ids and carry instruction per scripts.
* Support stereo camera setting where 2 cameras are used to capture images of a person at different angles as an added level of security against simply attack using photo.
* Comprehensive tools for collecting, editing and organizing face ids.
* Simple scripting concept for automation.
* User-friendly tools to create, edit and manage scripts/programs.
* Remote monitor, manage and control multiple devices.
* Built-in functions to control GPIOs, communicate with JSON-server and sending emails.


Note: Please read user guide: [PiFaceCam User Guide V2_1.pdf](https://github.com/tensorfactory/PiFaceCam/tree/master/downloads) for more information.


**Install:**

**(PiFaceCam-Edge)**

This PiFaceCam-Client was developed for Raspbian Buster with desktop (Kernel version: 4.19).  You will need the following supporting libraries.
1.	Pyhton3 (developed using version 3.7.3)
2.	Tensorflow 1.X  (developed using version 1.15)
3.	OpenCV for python (developed using version 4.2.0)
4.	Scikit-learn (developed using version 0.22)
5.	Pycryptodomex  (developed using version 3.9.6)

After you have installed all the necessary libraries, copy all the files from ["pifacecam_edge" folder at github](https://github.com/tensorfactory/PiFaceCam/tree/master/pifacecam_edge) .

*If you don't want to go through all the trouble of installing everything yourself, we are sharing out SD Card's image file at github as well. Please take note that we are using 16GB SD Card, therefore you will need a large card to flash the image.
After flashing your SD Card, remember to reclaim the missing disk space by running raspi-config -> Advanced Options -> Expand Filesystem Ensures that all of the SD card storage is available. PiFaceCam_Client's files are placed in "home/pi" folder.*

**Run:**

1) Connect GPIO 19 to LED via a resistor and GPIO 26 to ground via a resistor.

2) $ python3 wrapper.py

The LED will start to blink indicating system loading. When successfully loaded and ready, the LED will continuously ON. Connecting GPIO 26 to HIGH will trigger system shutdown.

Note: Follow step 1 in section 4.1 of "userguide" to setup PiFaceCam_edge(Client) to auto-start when power ON.


**(PiFaceCam-App)**

Requires Android OS Marshmallow and above. The device will also require network connection to communicate with PiFaceCam_Edge and camera if you want to create face ids directly from image captured. It allows you to control and manage all connected PiFaceCam_Edge devices.

(Versions)

There are 2 versions of PiFaceCam_App, PiFaceCam (Community) and PiFaceCam (Professional). Community version is free and professional version is paid. Both can be download from Google Play store.

**(Features)**

| Features                                | Community     | Professional |
| --------------------------------------- | ------------- | ------------ |
| Create and manage scripts / programs.   | Yes           | Yes          |
| Create and manage face ids.             | Limited to 20 | Not limited  |
| Setting and manage of attached cameras. | Yes           | Yes          |
| Setting facial recognition criteria.    | Yes           | Yes          |
| Enabling face verification server mode. | No            | Yes          |
