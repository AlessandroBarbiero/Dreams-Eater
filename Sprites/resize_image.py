import cv2
import os



path = "Room//Inside//Walls"

path_floor = "Room//Inside//Floor"

d = {}

# for image in os.listdir(path_floor):

#     im = cv2.imread(f"{path_floor}//{image}", cv2.IMREAD_UNCHANGED)
#     print(image, im.shape)


# for image in os.listdir(f"{path}//Wood"):
#     im = cv2.imread(f"{path}//Wood//{image}",cv2.IMREAD_COLOR)
#     d[image] = [im.shape[1], im.shape[0]]

for folder in os.listdir(path):
    if folder =="Stone":
        for image in os.listdir(f"{path}//{folder}"):
            
            im = cv2.imread(f"{path}//{folder}//{image}",cv2.IMREAD_UNCHANGED)

            # if image in d:
            #     im = cv2.resize(im, d[image])
            # if (im.shape[0] == im.shape[1]):
            #     im = cv2.resize(im,[341, 341])
            #     cv2.imwrite(f"{path}//{folder}//{image}",im)
            
            if image == "Top.png":
                im = cv2.resize(im,[341,99])
            print(image,im.shape)

            # cv2.imwrite(f"{path}//{folder}//{image}",im)


# print(d)
        
    # for image in os.listdir(f"{path}//{folder}"):

    #     im = cv2.imread(f"{path}//{folder}//{image}",cv2.IMREAD_COLOR)
    #     if image == "Left.png":
    #         print(image, im.shape)