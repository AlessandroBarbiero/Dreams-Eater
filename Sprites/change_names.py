import os

# Look in all subdirectories of rootDir, there will be the Characters, 
# for each one of the subfolders of that folders change the names with increasing numbers -> they will be the animations
# example of folder organization ./StillNoAtlas/BigTroll/Attack1/0.png
def changeAllFileNamesInSubSubFolders(rootDir = './StillNoAtlas'):
    count = 0

    for dirName, subdirList, _ in os.walk(rootDir):
        count+=1
        if count == 1:
            continue
        i = 0
        for subdir in subdirList:
            print("-- subdir \t" +subdir)
            subdirName = os.path.join(dirName, subdir)
            files = os.listdir(subdirName)
            for i in range(len(files)):
                print(files[i])
                old_fname = os.path.join(subdirName, files[i])
                new_fname = os.path.join(subdirName, str(i)+".png")
                os.rename(old_fname, new_fname)


def changeNames(folder = "Characters/Robot", subfolder = "Walk"):
    os.chdir(f"{folder}//{subfolder}")

    filenames = [x[2] for x in os.walk(".")][0]

    for i in range(len(filenames)):
        os.rename(filenames[i], str(i)+".png")
