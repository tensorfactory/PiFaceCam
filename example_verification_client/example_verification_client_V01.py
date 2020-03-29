import socket
import time
import json
import base64
from struct import pack, unpack
import numpy as np
import cv2


def request_face_verification(need_return_image, server_ipaddress, server_port, image_file_pathname, server_token):

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
    total_bytes = len_of_packet_bytes + JSON_bytes

    # ------------Send packet to verification server----------------
    server_address = (server_ipaddress, server_port)
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect(server_address)
    sock.sendall(total_bytes)

    # -------------Receive reply from server-----------------
    len_of_packet_bytes = sock.recv(4)
    len_of_packet_int = unpack('>I', len_of_packet_bytes)[0]

    received_data = b''
    while len(received_data) < len_of_packet_int:
        packet = sock.recv(len_of_packet_int - len(received_data))
        if not packet:
            break
        received_data += packet


    sock.close()

    #------------Decode received binary string to JSON object---------
    received_data_string = received_data.decode("utf-8")
    received_data_JSON_obj = json.loads(received_data_string)

    # -------------Retrieve verification results---------------
    isSuccessful = received_data_JSON_obj.get("isSuccessful")
    if isSuccessful:
        noOfFaces = received_data_JSON_obj.get("noOfFaces")
        imageWidth = received_data_JSON_obj.get("imageWidth")
        imageHeight = received_data_JSON_obj.get("imageHeight")
        print("imageWidth:{},  imageHeight:{}".format(
            imageWidth,
            imageHeight))
        if noOfFaces > 0:
            faces = received_data_JSON_obj.get("faces")
            for face_idx, face in enumerate(faces):
                face_top = face["top"]
                face_left = face["left"]
                face_width = face["width"]
                face_height = face["height"]
                confPercentage = face["confPercentage"]

                print("Face {}: confident {:.2f}%".format(face_idx, confPercentage))
                print("Face {}: (top,left,width,height):({},{},{},{})".format(
                    face_idx,
                    face_top,
                    face_left,
                    face_width,
                    face_height))
        else:
            print("No face detected.")

        if need_return_image:
            returned_image_base64_str = received_data_JSON_obj.get("returnImage")
            returned_image_base64 = returned_image_base64_str.encode("utf-8")
            returned_image_bytes = base64.b64decode(returned_image_base64)

            returned_image_array = np.frombuffer(returned_image_bytes, np.uint8)
            returned_image_np = cv2.imdecode(returned_image_array, cv2.IMREAD_COLOR)

            cv2.imshow("Returned image", returned_image_np)
            cv2.waitKey(0)
            cv2.destroyAllWindows()

    else:
        print("Server return error message :" + received_data_JSON_obj.get("errMessage"))



if __name__ == "__main__":
    server_ipaddress = "169.254.121.52"
    server_port = 9990
    image_file_pathname = "ref_protrait.jpg"
    server_token = "1234"
    need_return_image = False
    start_time = time.time()
    request_face_verification(need_return_image, server_ipaddress, server_port, image_file_pathname, server_token)
    print("Latency:{:.3f}s".format(time.time() - start_time))

