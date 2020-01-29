import PiFaceCam_Client_1_3_6
import multiprocessing as mp
import socket
from struct import unpack
import pickle
import cv2
import numpy as np
import os

def UDP_server(port):
    _server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    _server_socket.bind(("", port))

    while True:
        # --------------Handling Receiving of packet---------------------
        bytes_AddressPort_Pair = _server_socket.recvfrom(65000)
        client_data = bytes_AddressPort_Pair[0]
        client_addressPort = bytes_AddressPort_Pair[1]

        data_length_bytes = client_data[:8]
        (data_length,) = unpack('>Q', data_length_bytes)

        data_bytes = client_data[8: 8 + data_length]
        (image_jpeg_np, bbox_minXminYmaxXmaxY_np, faceID_list) = pickle.loads(data_bytes)

        image_np = cv2.imdecode(image_jpeg_np, cv2.IMREAD_UNCHANGED)
        print("Packet length:", data_length)
        print("Shape of received image is ", np.shape(image_np))
        if (len(faceID_list) >0):
            for bbox_idx, face_id in enumerate(faceID_list):
                print("Bounding box coordinates for " +
                      face_id + ": minX={:.0f}, minY={:.0f}, maxX={:.0f}, maxY={:.0f}".
                      format(bbox_minXminYmaxXmaxY_np[bbox_idx, 0],
                             bbox_minXminYmaxXmaxY_np[bbox_idx, 1],
                             bbox_minXminYmaxXmaxY_np[bbox_idx, 2],
                             bbox_minXminYmaxXmaxY_np[bbox_idx, 3]))


if __name__ == "__main__":
    port = 60000
    UDP_server_process = mp.Process(target=UDP_server, args=(60000,))

    UDP_server_process.start()
    PiFaceCam_Client_1_3_6.run_client("localhost", port)
    os.system("sudo shutdown -h now")
