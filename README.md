
## PiFaceCam
### Facial Recognition for Raspberry Pi
#### Overview
PiFaceCam is a facial recognition API for Raspberry Pi (Tested on Pi3 Model B+ and Pi4). 

**Main Features:**
1. Run efficiently on Raspberry Pi.
2. Highly accurate even when faces are partially covered by face-masks.
3. Ease of use. Cameras management, face ids creation, facial-recognition and video creation/streaming are all handled automatically by pifacecam.
4. Flexible to implement. Important facial-recognition information can be retrieve via a callback-function for further implementation.
5. Powerful. On top of basic facial-recognition (faces are identified against pre-stored facial ids.), pifacecam can run in "verification server" mode where a client can upload photos for verification against the person infront of camera.
6. Stereo cameras setup option for defense against attack of placing a photo infront of camera.


**Prerequisites (Software):**
1. Raspbian Buster OS with desktop (Kernel version:4.19).
2. Python 3.5 (and above)
3. Numpy
4. Tensorflow 1.15
5. OpenCV for python
6. Scikit-learn
7. Pycryptodomex


**Installation:**
1. Python 3.5 (and above): Raspberry Pi OS should have Python3.7 preinstalled.
2. Numpy: Raspberry Pi OS should have Numpy preinstalled.
3. Tensorflow 1.15: Follow Katsuya Hyodo's procedures at <https://github.com/PINTO0309/Tensorflow-bin>(Choose option "tensorflow-1.15.0-cp37-cp37m-linux_armv7l.whl")
4. OpenCV: Follow Emmet's procedures at <https://pimylifeup.com/raspberry-pi-opencv/> (Note: If you are using Pi4 4GB, you can skip the "increase the swap space" portion.)
5. Scikit-learn: Run `sudo apt-get install python3-sklearn`
6. Pycryptodomex: Run `sudo pip3 install pycryptodomex`
7. PiFaceCam: Run `sudo pip3 install pifacecam`


**Hardware requirement / setup:**
1. Raspberry Pi3 Model B+ and Pi4
2. Picamera or USB camera.(Note: For stereo camera setup, you will need 1 Picamera + 1 USB camera. PiFaceCam does not support 2 USB cameras)
3. A LED connected to GPIO19 (pin number can be changed later) via a resistor as system status indicator.The LED will blinks during system loading and on continuously when system is ready. LED blinks indefinitely signify error has occurred.
4. GPIO26 (pin number can be changed later) connected to ground via a resistor. It will trigger program exit when connect to high.
5. As facial-recognition is computationally heavy, proper heat management is required. Standard cooling fan with heat sink on CPU was tested to be sufficient.


**Quick Run:**
- In terminal, run the following lines.<br/>
```
$ python3
>>>from pifacecam import pifacecam
>>>pifacecam.run()
```
You should see something like the following print out.<br/>
```
1) Checking for attached Picamera.
2) Picamera found.
3) Checking for attached USB camera.
4) USB camera found at index[1].
```

The status LED will blinks during system loading and turn ON continuously when system is ready. Once system is ready, open a browser and goto URL `http://[ipaddress of RPI]:9090/video` you should able to view the camera feeds with some facial recognition information.


**Parameters:**
The run() function can receive many parameters. Details of each parameter are listed below.<br/>

|Parameter|Type|Default Value|Description|
|:--:|:--|:--|:--|
|faceids_images_root_folder_pathname|String|None|path to where images of each id is stored.|







