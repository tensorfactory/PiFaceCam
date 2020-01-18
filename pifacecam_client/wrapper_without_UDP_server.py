import PiFaceCam_Client_1_3_0
import os

if __name__ == "__main__":

    PiFaceCam_Client_1_3_0.run_client("", -1)
    os.system("sudo shutdown -h now")
