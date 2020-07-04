
## PiFaceCam
### Facial Recognition for Raspberry Pi
#### Overview
PiFaceCam is a facial recognition API for Raspberry Pi (Tested on Pi3 Model B+ and Pi4). 

**Main Features:**
1. Run efficiently on Raspberry Pi.
2. Highly accurate even when faces are partially covered by face-masks.
3. Ease of use. Cameras management, face ids creation, facial-recognition and video creation/streaming are all handled automatically by pifacecam.
4. Flexible to implement. Important facial-recognition information can be retrieve via a callback-function for any post recognition actions.
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
<br/><br/>

**Parameters:**

The run() function can receive many parameters. Description of each parameter are listed below.<br/>

|Parameter|Type|Default Value|Description|
|:--:|:--|:--|:--|
|callback_function|Function|None|[1]Callback function to retrieve facial recognition data and perform post recognition activities.| 
|faceids_folder_pathname|String|None|[2]Path to where images of each id are stored.| 
|device_id|String|"CAM001"|Name of device. Will be displayed at the bottom left of video screen.Maximum length allowed is 10 characters.| 
|cam_setup|String|"MONO_PICAM"|Defining the camera setup to use. Available options are  "MONO_PICAM"=Single Picamera; "MONO_USB"=Single USB camera; "STEREO_USB+PICAM"=1 USB + 1 Pi cameras; "NOCAM"=No camera.|
|stereo_left_cam_type|String|"USB"|Left camera type for stereo setup. Available options are  "PICAM" and "USB".|
|stereo_right_cam_type|String|"PICAM"|Right camera type for stereo setup. Available options are  "PICAM" and "USB".|
|detect_conf_percent|Int/Float|99.9|Minimum confident percentage required to identify a person.|
|status_pin_num|Int|19|GPIO pin number to use as output for status LED. Value has to be between 2 and 27 and different from shutdown_pin_num|
|shutdown_pin_num|Int|26|GPIO pin number to use for trigger program exit. Value has to be between 2 and 27 and different from status_pin_num|
|full_face_only|Boolean|False|[3]When set to true, will force pifacecam to use whole face for recognition.|
|eyes_only|Boolean|False|[3]When set to true, will force pifacecam to focus on the eyes area for recognition.|
|high_precision_mode|Boolean|False|When set to true, will use a more accurate model for recogniton. This will however limit the number of face for recognition to one (instead of six in the standard mode). Only the most prominent face will be recognised.|
|show_bbox|Boolean|True|Whether to show bounding boxes around detected faces during video streaming.|
|show_faceid|Boolean|True|Display the person's id below each identified face during video streaming.|
|show_camid|Boolean|True|Display the device's id during video streaming.|
|show_fps|Boolean|True|Whether to display the frames per second during video streaming.|
|show_measurements|Boolean|True|For stereo cameras setting only. Display the delta face angle, delta bbox dimensions between left and right cameras during video streaming.|
|show_positioning_guides|Boolean|False|Display the window borders and min/max detectable face size during video streaming.|
|show_conf_percentage|Boolean|True|Display the recognition confidence percentage above each identified face during video streaming.|
|show_precision_mode|Boolean|True|Display the word "HP" beside the device id if high precision mode is ON or "STD" if OFF.|
|stereo_max_delta_bbox_w_percent|Int/Float|50|[4]For stereo cameras setting only. Max delta percentage between left and right camera bbox width. Value has to be between 0 and 100|
|stereo_max_delta_bbox_h_percent|Int/Float|50|[4]For stereo cameras setting only. Max delta percentage between left and right camera bbox height. Value has to be between 0 and 100|
|stereo_min_delta_face_angle|Int/Float|20|[5]For stereo cameras setting only. Min delta percentage between left and right face angle. Value has to be between 0 and 100, and smaller than stereo_max_delta_face_angle|
|stereo_max_delta_face_angle|Int/Float|60|[5]For stereo cameras setting only. Max delta percentage between left and right face angle. Value has to be between 0 and 100, and larger than stereo_min_delta_face_angle|
|in_verification_server_mode|Boolean|False|Run as facial verification server.|
|verification_server_port_no|Int|9990|Port number of verification server.|
|verification_server_token|String|None|If set, will use to validate client's request.|
|usb_cam_zoom_ratio|Int/Float|1.0|Zoom ratio of image from USB camera.|
|picam_cam_zoom_ratio|Int/Float|1.0|Zoom ratio of image from Picamera.|
<br/>

**Details:**

**[1] callback_function:** This function will receive a dictionary containing facial recognition information "data_dict". This function will be called once every frame. Ideally, any processing activities in this function should be able to complete before the next frame arrives or else the next frame will be ignored. Listed below are the available dictionary keys. <br/>
|Key|Description|
|:--:|:--|
|"image"|BGR image in uint8 numpy array format (HeightxWidthxDepth).|
|"face_id_list"|List of face ids. For faces that not meeting the "detect_conf_percent" will be identified as "UNREGISTERED".|
|"face_confidence_percentage_list"|List of confidence percentage for all detected faces.|
|"face_is_dimensional_check_passed_list"|List of booleans indicating if the face passed dimensional check.|
|"face_is_masked_list"|List of booleans indicating if mouth is covered (face-mask for example).|
|"face_bbox_minXminYmaxXmaxY_list"|List of bounding box "left, top, right, bottom" coordinates for each detected face.|

Below is an example of callback function.
```
def example_callback_function(data_dict):
    returned_image_np  = data_dict["image"]
    returned_faceID_list = data_dict["face_id_list"]
    returned_face_confidence_percentage_list= data_dict["face_confidence_percentage_list"]
    returned_face_is_dimensional_check_passed_list = data_dict["face_is_dimensional_check_passed_list"]
    returned_face_is_masked_list = data_dict["face_is_masked_list"]
    returned_face_abs_minXminYmaxXmaxY_list = data_dict["face_bbox_minXminYmaxXmaxY_list"]

    no_of_faces_passed_dimensional_check = 0
    for this_face_is_dimensional_check_passed in returned_face_is_dimensional_check_passed_list:
        if this_face_is_dimensional_check_passed:
            no_of_faces_passed_dimensional_check += 1

    print("Returned image shape :", returned_image_np.shape)
    print("No of face detected :", len(returned_faceID_list))
    print("No of faces passed dimensional check :", no_of_faces_passed_dimensional_check)

    for face_idx in range(len(returned_faceID_list)):
        print("This face id :", returned_faceID_list[face_idx])
        print("This face confidence percentage :", returned_face_confidence_percentage_list[face_idx])
        print("This face's mouth is covered :", returned_face_is_masked_list[face_idx])
        print("This face's bbox minXminYmaxXmaxY :", returned_face_abs_minXminYmaxXmaxY_list[face_idx])

    # Perform post recognition task here....

```
**[2] faceids_folder_pathname:** This is the path to the folder where you store images for each person. Pifacecam will scan this folder for any new images and generate the face ids for facial recognition. In the example below, faceids_folder_pathname will be path to "imagefolder" and pifacecam will create face ids Adam, Lisa and James.<br/>
```
imagefolder
├── Adam
│   ├── image01.jpg
│   ├── image02.jpg
│   └── image03.jpg
├── Lisa
│   ├── image01.jpg
│   └── image02.jpg
├── James
    └── image01.jpg
```
**[3] full_face_only/eyes_only:** When these two parameters were set to false(default), pifacecam will decide to use the whole face or only eyes area for recognition based on whether the mouth is covered. However, if it is known upfront that mouths will or will not be covered, we can use these parameters to force pifacecam to use full face or eyes only area for recognition. Doing this will improve the speed of recognition, especially in verification server mode. (Note: We can't set both full_face_only and eyes_only to true at the same time.)<br/>
**[4] stereo_max_delta_bbox_w_percent/stereo_max_delta_bbox_h_percent:** In stereo cameras setting the size of face will varies as the person move towards the left or right camera. We can limit the acceptable difference for facial recognition.<br/>
**[5] stereo_min_delta_face_angle/stereo_min_delta_face_angle:** In 
[Stereo cameras layout](https://github.com/tensorfactory/PiFaceCam/blob/master/images/stereo_cameras_layout.JPG)
