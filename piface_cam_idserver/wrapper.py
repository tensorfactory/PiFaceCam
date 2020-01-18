import PiFaceCam_IDServer_1_3_0
import os

if __name__ == "__main__":
 
    PiFaceCam_IDServer_1_3_0.run_server()
    os.system("sudo shutdown -h now")
