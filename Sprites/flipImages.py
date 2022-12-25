import cv2
import os
import shutil


path = "Characters"
folder="Left"
new_folder = "Right"

to_exclude = ["BigTroll","BrownWraith"]


for character in os.listdir(path): #for each character in character folder

    # os.mkdir(f"{path}//{character}//{new_folder}") 
    p = f"{path}//{character}" 

    if character not in to_exclude:
        for action in os.listdir(f"{p}//Right"):

            if os.path.isdir(f"{p}//Right//{action}"):

                os.mkdir(f"{p}//Left//{action}")

                for pic in os.listdir(f"{p}//Right//{action}"):

                    image = cv2.imread(f"{p}//Right//{action}//{pic}", cv2.IMREAD_UNCHANGED)
                    
                    flippedimage = cv2.flip(image, 1)

                    cv2.imwrite(f"{p}//Left//{action}//{pic}",flippedimage)
            

    #     for thing in os.listdir(f"{path}//{character}"):
    #         if "Left" not in thing and "Right" not in thing:
    #             src = f"{path}//{character}//{thing}"
    #             dst = f"{path}//{character}//Right"
    #             shutil.move(src = src, dst = dst)
    


        # for name in os.listdir(f"{path}//{character}"): 

            
            

