import json
import os

sprites = ["Wizard","IceWizard" , "PurpleWizard" ,"Guy", "Robot", "Wraith", "PurpleWraith", "BrownWraith","Zombie"]

sprite_pivot = {
    # "Wizard" : {"x":0.5,"y":0.6}, 
    # "IceWizard" : {"x":0.5,"y":0.6}, 
    # "FireWizard" : {"x":0.5,"y":0.6}, 
    # "Guy" : {"x":0.5,"y":0.7}, 
    # "Robot": {"x":0.5,"y":0.7}, 
    # "Wraith": {"x":0.5,"y":0.6},
    # "Zombie": {"x":0.5,"y":0.7},
    # "PurpleWraith": {"x":0.5,"y":0.6},
    # "BrownWraith": {"x":0.5,"y":0.6},

}


def add_pivot(image,sprite):
    image["pivot"] = sprite_pivot[sprite]
    return image

# Pivots has been changed by hand, do not use this function, left there just for reference
def applyPivotTo(sprites):
    for sprite in sprites:
        for _,_,filenames in os.walk(f"Characters//{sprite}"):
            for filename in filenames:
                if filename.endswith(".json"):
                    with open(f"Characters/{sprite}//{filename}", "r") as f:
                        data = json.load(f)["frames"]
                        for i in range(len(data)):
                            data[i] = add_pivot(data[i], sprite)
                    with open(f"Characters/{sprite}//{filename}", "w") as f:
                        json.dump({"frames" : data},f)

