import os


folder = "Characters/Robot"
subfolder = "Walk"

os.chdir(f"{folder}//{subfolder}")

filenames = [x[2] for x in os.walk(".")][0]

for i in range(len(filenames)):
    os.rename(filenames[i], str(i)+".png")



