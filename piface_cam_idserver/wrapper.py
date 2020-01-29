import PiFaceCam_IDServer_1_3_5
import os


if __name__ == "__main__":
 
    PiFaceCam_IDServer_1_3_5.run_server()
    os.system("sudo shutdown -h now")
