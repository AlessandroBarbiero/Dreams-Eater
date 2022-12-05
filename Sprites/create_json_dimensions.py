import json
import os




d = {}

for _,_,filenames in os.walk("Characters"):
    for filename in filenames:
            if filename.endswith(".json"):
                l = {}
                for dir in os.listdir(f"Characters//{filename[:-11]}"):
                    if "." not in dir:
                        # l.append({dir: len(os.listdir(f"Characters//{filename[:-11]}//{dir}"))})
                        l[dir] = len(os.listdir(f"Characters//{filename[:-11]}//{dir}"))
                d[filename[:-11]] = l   

with open("animationSizes.json", "w") as f:
    json.dump(d,f)