
## PiFaceCam
### Facial Recognition for Raspberry Pi
#### Overview
PiFaceCam is a facial recognition API for Raspberry Pi4 (Tested on Pi4 Model B-4GB. Pi4 2GB should be able to run as the RAM usage was estimated to peak around 860MB). 


**Main Features:**
1. Run efficiently on Raspberry Pi4.
2. Ease of use. Cameras management, face ids creation, facial-recognition and video creation/streaming are all handled automatically by pifacecam.
3. Flexible to implement. Important facial-recognition information can be retrieve via a callback-function for any post recognition actions.
4. Powerful. On top of basic facial-recognition (faces are identified against pre-stored facial ids.), pifacecam can run in "verification server" mode where a client can upload photos for verification against the person infront of camera.
5. Stereo cameras setup option for defense against attack of placing a photo infront of camera.
6. Able to recognised faces that are partially covered by face-masks.


**PiFaceCam's LFW benchmark:**
[Category: Unrestricted, Labeled Outside Data](http://vis-www.cs.umass.edu/lfw/results.html#UnrestrictedLb)

|Mode|Accuracy(%)|
|:--|:--:|
|High Precision|99.50 +/-0.38|
|Standard|99.13 +/-0.56|

*[Human performance](http://vis-www.cs.umass.edu/lfw/results.html#Human) = 97.53 %.<br/>


**Prerequisites (Software):**
1. Raspbian Buster OS with desktop (Tested on kernel version: 4.19 & 5.4).
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
1. Raspberry Pi4 (Tested on Pi4 Model B-4GB. Pi4 2GB should be able to run as the RAM usage was estimated to peak around 860MB). 
2. Picamera or USB camera.(Note: For stereo camera setup, you will need 1 Picamera + 1 USB camera. PiFaceCam does not support 2 USB cameras)
3. A LED connected to GPIO19 (pin number can be changed later) via a resistor as system status indicator.The LED will blinks during system loading and ON continuously when system is ready. LED blinks indefinitely signify error has occurred.
4. GPIO26 (pin number can be changed later) connected to ground via a resistor. It will trigger program exit when connect to high.
5. As facial-recognition is computationally heavy, proper heat management is required. Standard cooling fan with heat sink on CPU was tested to be sufficient.


**Quick Run:**
- In terminal, run the following lines.<br/>
```
$ sudo python3
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
(Note: In some version of OpenCV, you may see bunch of warning messages, but so far we haven't encounter any problem during operation despite of these warning messages.)<br/>
The status LED will blinks during system loading and turn ON continuously when system is ready.<br/> Once system is ready, open a browser and goto URL `http://[ipaddress of RPI]:9090/video` you should able to view the camera feeds with some facial recognition information.
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
|detect_conf_percent|Int/Float|95.0|Minimum confident percentage required to identify a person.|
|status_pin_num|Int|19|GPIO pin number to use as output for status LED. Value has to be between 2 and 27 and different from shutdown_pin_num.|
|shutdown_pin_num|Int|26|GPIO pin number to use for trigger program exit. Value has to be between 2 and 27 and different from status_pin_num.|
|full_face_only|Boolean|False|[3]When set to true, will force pifacecam to use whole face for recognition.|
|high_precision_mode|Boolean|False|When set to true, will use a more accurate model for recogniton.|
|max_num_for_hp_mode|Int|4|The maximum number of faces to detect each time in high precision mode. Setting a lower number will reduce lagging when there are too many faces to process. Setting a value N will mean only the N most prominent faces will be recognised. This value has to be between 1 and 4. Note: Max face detected is 1 in stereo camera setup.|
|max_num_for_std_mode|Int|6|The maximum number of faces to detect each time in standard mode. This value has to be between 1 and 6. Note: Max face detected is 1 in stereo camera setup.|
|show_bbox|Boolean|True|Whether to show bounding boxes around detected faces during video streaming.|
|show_faceid|Boolean|True|Display the person's id below each identified face during video streaming.|
|show_camid|Boolean|True|Display the device's id during video streaming.|
|show_fps|Boolean|True|Whether to display the frames per second during video streaming.|
|show_measurements|Boolean|True|For stereo cameras setting only. Display the delta face angle, delta bbox dimensions between left and right cameras during video streaming.|
|show_positioning_guides|Boolean|False|Display the window borders and min/max detectable face size during video streaming.|
|show_conf_percentage|Boolean|True|Display the recognition confidence percentage above each identified face during video streaming.|
|show_precision_mode|Boolean|True|Display the word "HP" beside the device id if high precision mode is ON or "STD" if OFF.|
|stereo_max_delta_bbox_w_percent|Int/Float|50|[4]For stereo cameras setting only. Max delta percentage between left and right camera bbox width. Value has to be between 0 and 100.|
|stereo_max_delta_bbox_h_percent|Int/Float|50|[4]For stereo cameras setting only. Max delta percentage between left and right camera bbox height. Value has to be between 0 and 100.|
|stereo_min_delta_face_angle|Int/Float|20|[5]For stereo cameras setting only. Min delta percentage between left and right face angle. Value has to be between 0 and 100, and smaller than stereo_max_delta_face_angle.|
|stereo_max_delta_face_angle|Int/Float|60|[5]For stereo cameras setting only. Max delta percentage between left and right face angle. Value has to be between 0 and 100, and larger than stereo_min_delta_face_angle.|
|in_verification_server_mode|Boolean|False|[6]Run as facial verification server.|
|verification_server_port_no|Int|9990|[6]Port number of verification server.|
|verification_server_token|String|None|[6]If set, will use to validate client's request.|
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
**[2] faceids_folder_pathname:** This is the path to the folder where you store images for each person. Pifacecam will scan this folder for any new images and generate the face ids for facial recognition. In the example below, faceids_folder_pathname will be path to "imagefolder" and pifacecam will create face ids for Adam, Lisa and James.<br/>
This process may takes several minutes depending on how many ids and images are available. This process will only run once for each id when there is any change to the id folder.<br/>
```
imagefolder
|-- Adam
|   |-- image01.jpg
|   |-- image02.jpg
|   |-- image03.jpg
|
|-- Lisa
|   |-- image01.jpg
|   |-- image02.jpg
|
|-- James
    |-- image01.jpg
```
**[3] full_face_only:** By default, this parameter is set to false, meaning PiFaceCam will automatically use the whole face for recognition if no facemask is detected. If facemask is detected, it will only use the eyes area for recognition. In situation where accuracy is critical and you do not want recognition to be based only on eyes area, you can force PiFaceCam to use the whole face for recognition all the time by setting parameter full_face_only to true.<br/><br/>
**[4] stereo_max_delta_bbox_w_percent/ stereo_max_delta_bbox_h_percent:** In stereo cameras setting the size of face will varies as the person move towards the left or right camera. You can limit the acceptable difference for facial recognition.<br/><br/>
**[5] stereo_min_delta_face_angle/ stereo_min_delta_face_angle:** 

![Stereo cameras layout](https://raw.githubusercontent.com/tensorfactory/PiFaceCam/master/images/stereo_cameras_layout.JPG)

To defense against attack using photo, you can use stereo cameras setup. It works by detecting the face angles from the left and right camera, -alpha & beta. The difference of these angles (beta -(-alpha) = beta + alpha) should be about the same as the angle between the left and right camera.
For this example, the left and right cameras were placed 40deg apart, beta - (-alpha) will be close to 40deg. However, if a photo (instead of a 3D face) was placed in front of both cameras, both cameras will measure the same face angle and beta - (-alpha) will be close to 0deg. You can set a minimum and maximum acceptable face angle (beta + alpha). Both values should be between 0deg and 100deg.<br/><br/>
**[6] in_verification_server_mode/ verification_server_port_no/ verification_server_token:** In verification mode, you need to provide the port number for the server and if a verification token is also provides, it will be used by the client during request for validation.
Once setup, the raspberry pi will act as a verification server. Any client can send to it a reference image for verification. The verification server will return if the person currently in front of the camera matches the person in the reference image. <br/><br/>
***Information to server:***<br/>
To request for verification, the client needs to send the reference image, the token, and a boolean indicating if a returned image is required, in JSON format to server.

|Element|Key|Value|
|:--|:--:|:--:|
|Token|"token"|String|
|Image|"image"|Reference image (base64 string)|
|If need to return image|"need_return_image"|Boolean|

The image bytes have to be converted to base64 string (see below example). The length of this JSON bytes needs to be included at the beginning of the packet when send to verification server. The length value is encoded in a big-endian ordering 4 bytes.
```
# --------Load image bytes from file-------- 
img_filestream = open(image_file_pathname, "rb") 
ori_image_bytes = img_filestream.read() 
img_filestream.close() 

# --------Convert to base64 string-------- 
ori_image_base64 = base64.b64encode(ori_image_bytes) 
ori_image_base64_str = ori_image_base64.decode('utf-8') 

# --------Compose JSON packet----------- 
JSON_dict = {"token": server_token, 
             "image": ori_image_base64_str, 
             "need_return_image": need_return_image} 
JSON_str = json.dumps(JSON_dict) 

# --------Convert to binary string and add length of packet------------ 
JSON_bytes = JSON_str.encode("utf-8") 
len_of_packet = len(JSON_bytes) 
len_of_packet_bytes = pack('>I', len_of_packet)

# ------------Send packet to verification server---------------- 
server_address = (server_ipaddress, server_port) 
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 
sock.connect(server_address) 
sock.sendall(total_bytes)
```

<br/>

***Information returned from server:***<br/>
Returned JSON object from verification server to client.

|Key|Value|
|:--|:--|
|"isSuccessful"|True / False|
|"errMessage"|Error message if isSuccessful is False|
|"noOfFaces"|Number of faces detected. Only faces that passed the dimensional constrain check.|
|"imageWidth"|Width of the image from verification server.|
|"imageHeight"|Height of the image from verification server.|
|"faces"|List of faces detected. Only faces that passed the dimensional constrain check.|
|"returnImage"|Returned image (base64 string)|

<br/>

***Faces list:***

|Key|Value|
|:--|:--|
|"top"|Top y-coordinate of face bounding box.|
|"left"|Left x-coordinate of face bounding box.|
|"width"|Width of face bounding box.|
|"height"|Height of face bounding box.|
|"confPercentage"|Confidence level (%) of face matches image sent.|
<br/>
Similarly, the first 4 bytes of the returned packet indicates the length of packet.

```
# -------------Receive reply from server----------------- 
len_of_packet_bytes = sock.recv(4) 
len_of_packet_int = unpack('>I', len_of_packet_bytes)[0] 
received_data = b'' 
while len(received_data) < len_of_packet_int: 
    packet = sock.recv(len_of_packet_int - len(received_data)) 
	if not packet: 
	    break 
	received_data += packet sock.close() 

#------------Decode received binary string to JSON object--------- 
received_data_string = received_data.decode("utf-8")
received_data_JSON_obj = json.loads(received_data_string) 

# -------------Retrieve verification results--------------- 
isSuccessful = received_data_JSON_obj.get("isSuccessful") 
if isSuccessful: 
    noOfFaces = received_data_JSON_obj.get("noOfFaces") 
	imageWidth = received_data_JSON_obj.get("imageWidth") 
	imageHeight = received_data_JSON_obj.get("imageHeight") 
	
	if noOfFaces > 0: 
	    faces = received_data_JSON_obj.get("faces") 
		for face_idx, face in enumerate(faces): 
		    face_top = face["top"] 
			face_left = face["left"] 
			face_width = face["width"] 
			face_height = face["height"] 
			confPercentage = face["confPercentage"] 
			
			print("Face {}: confident {:.2f}%".format(face_idx, confPercentage)) 
	else: 
	    print("No face detected.") 
		
	if need_return_image: 
	    returned_image_base64_str = received_data_JSON_obj.get("returnImage")
		returned_image_base64 = returned_image_base64_str.encode("utf-8")
		returned_image_bytes = base64.b64decode(returned_image_base64)
		returned_image_array = np.frombuffer(returned_image_bytes, np.uint8)
		returned_image_np = cv2.imdecode(returned_image_array, cv2.IMREAD_COLOR) 
		
		cv2.imshow("Returned image", returned_image_np) cv2.waitKey(0)
		cv2.destroyAllWindows() else: print("Server return error message :" +
		received_data_JSON_obj.get("errMessage"))

else: 
	print("Server return error message :" + received_data_JSON_obj.get("errMessage"))
```

<br/>

***Guidelines for preparing reference image:***

![Face guidelines](https://raw.githubusercontent.com/tensorfactory/PiFaceCam/master/images/face_guidelines.JPG)

The reference image used for verification has to meet the following guidelines, failing which may affect the accuracy or getting rejected by the verification server.
1) The face (green box) has to be at the center of the image.
2) The image has to be square (W = H).
3) The margin (m) between face and border of image has to be within 10~25 % of the
image size (H). (Example, if the image size is 500pixels x 500pixels, then m has to be
within the range of 50 to 125pixels.)
4) Image size (W and H) has to be within the range of 320 to 512 pixels.

<br/>

***Advanced discussions:***<br/>
- Events happened during the current run can be retrieved from info_log.txt file stored in the module folder. This file will be overided each time pifacecam startd.
- Any parameters set in the run function will be verified before accepted. If rejected, the error message will be print out and logged in info_log.txt file. If accepted, they will overide the default values. These values (except for "faceids_folder_pathname"),  will persist even if you subsequently remove(not set) the parameter in the "run" function. 
- To reset all settings to the default values, you need to delete the settings.dat file in the module folder.
- During face id processing [2], images with face not facing front, belongs to different person, partially obstructed or too blur will be rejected. These rejected images will be moved to a "rejected" folder created within the original folder.
- Once face ids are created, they are stored as encrypted files (faceids_image_cache.dat & data.dat).You can subsequently choose to remove all images from "faceids_folder_pathname" TOGETHER with the id folders. 
- To removing a saved face id, you create an empty folder for the face id. PiFaceCam will remove the id when it detect an empty folder for the id during start. To remove all saved ids at once, just delete both faceids_image_cache.dat & data.dat files.<br/><br/>



**Terms and conditions:**<br/>
This is the PiFaceCam License Agreement<br/>
1) Introduction
* 1.1 The PiFaceCam (referred to in the License Agreement as the "PFC") is licensed to you subject to the terms of the License Agreement. The License Agreement forms a legally binding contract between you and TensorFactory in relation to your use of the PFC.
* 1.2 "TensorFactory" means TensorFactory Enterprise and its owner.

2) Accepting this License Agreement
* 2.1 By downloading, accessing or using the PFC, you agree to be bound by this License Agreement. If you do not accept the License Agreement, you must immediately discontinue your use of PFC. Continued use of the PFC will constitute acceptance of the License Agreement.
* 2.2 If you are agreeing to be bound by the License Agreement on behalf of your employer or other entity, you represent and warrant that you have full legal authority to bind your employer or such entity to the License Agreement. If you do not have the requisite authority, you may not accept the License Agreement or use the PFC on behalf of your employer or other entity.

3) PFC License from TensorFactory
* 3.1 Subject to the terms of the License Agreement, TensorFactory grants you a limited, worldwide, royalty-free, non-assignable, non-exclusive, and non-sublicensable license to use the PFC.
* 3.2 You agree that TensorFactory own all legal right, title and interest in and to the PFC, including any Intellectual Property Rights that subsist in the PFC. "Intellectual Property Rights" means any and all rights under patent law, copyright law, trade secret law, trademark law, and any and all other proprietary rights. TensorFactory reserves all rights not expressly granted to you.
* 3.3 You may not use the PFC for any purpose not expressly permitted by the License Agreement. You may not modify, decompile, reverse engineer or disassemble any part of the PFC.
* 3.4 You agree that the form and nature of the PFC that TensorFactory provides may change without prior notice to you and that future versions of the PFC may be incompatible with files generated on previous versions of the PFC.
* 3.5	Nothing in the License Agreement gives you a right to use any of TensorFactory's trade names, trademarks, service marks, logos, domain names, or other distinctive brand features.

4) Use of the PFC by You
* 4.1 You agree to use the PFC only for purposes that are permitted by (a) the License Agreement and (b) any applicable law, regulation or generally accepted practices or guidelines in the relevant jurisdictions.
* 4.2 You agree that if you use the PFC on general public persons, you will protect the privacy and legal rights of those persons.
* 4.3 You agree that you are solely responsible for (and that TensorFactory has no responsibility to you or to any third party for) any data, content, or resources that you create, transmit or display through PFC, and for the consequences of your actions (including any loss or damage which TensorFactory may suffer) by doing so.
* 4.4 You agree that you are solely responsible for (and that TensorFactory has no responsibility to you or to any third party for) any breach of your obligations under the License Agreement, any applicable third party contract or Terms of Service, or any applicable law or regulation, and for the consequences (including any loss or damage which TensorFactory or any third party may suffer) of any such breach.

5) DISCLAIMER OF WARRANTIES
* 5.1 YOU EXPRESSLY UNDERSTAND AND AGREE THAT YOUR USE OF THE PFC IS AT YOUR SOLE RISK AND THAT THE APPLICATION IS PROVIDED "AS IS" AND "AS AVAILABLE" WITHOUT WARRANTY OF ANY KIND FROM TENSORFACTORY.
* 5.2 YOUR USE OF THE PFC IS AT YOUR OWN DISCRETION AND RISK AND YOU ARE SOLELY RESPONSIBLE FOR ANY DAMAGE TO YOUR COMPUTER SYSTEM OR OTHER DEVICE OR LOSS OF DATA THAT RESULTS FROM SUCH USE.
* 5.3 TENSORFACTORY FURTHER EXPRESSLY DISCLAIMS ALL WARRANTIES AND CONDITIONS OF ANY KIND, WHETHER EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES AND CONDITIONS OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.

6) LIMITATION OF LIABILITY
* 6.1 YOU EXPRESSLY UNDERSTAND AND AGREE THAT TENSORFACTORY SHALL NOT BE LIABLE TO YOU UNDER ANY THEORY OF LIABILITY FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, CONSEQUENTIAL OR EXEMPLARY DAMAGES THAT MAY BE INCURRED BY YOU, INCLUDING ANY LOSS OF DATA, WHETHER OR NOT TENSORFACTORY HAVE BEEN ADVISED OF OR SHOULD HAVE BEEN AWARE OF THE POSSIBILITY OF ANY SUCH LOSSES ARISING.

7) Indemnification
* 7.1 To the maximum extent permitted by law, you agree to defend, indemnify and hold harmless TensorFactory from and against any and all claims, actions, suits or proceedings, as well as any and all losses, liabilities, damages, costs and expenses (including reasonable attorneys fees) arising out of or accruing from (a) your use of the PFC that infringes any copyright, trademark, trade secret, trade dress, patent or other intellectual property right of any person or defames any person or violates their rights of publicity or privacy, and (b) any non-compliance by you with the License Agreement.
