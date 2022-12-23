import json
import os

d = {}



for character_name in os.listdir("Characters"):

    character = {}

    for action in os.listdir(f"Characters//{character_name}//Right"):
        if os.path.isdir(f"Characters//{character_name}//Right//{action}"):

            character[action] = len(os.listdir(f"Characters//{character_name}//Right//{action}"))
        
    d[character_name] = character

# for _,_,filenames in os.walk("Characters"):
#     for filename in filenames:
#             if filename.endswith(".json"):
#                 l = {}
#                 for dir in os.listdir(f"Characters//{filename[:-11]}"):
#                     if "." not in dir:
#                         l[dir] = len(os.listdir(f"Characters//{filename[:-11]}//{dir}"))
#                 d[filename[:-11]] = l   

with open("animationSizes.json", "w") as f:
    json.dump(d,f)