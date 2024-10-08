# SCRIPT TO GZIP CRITICAL FILES FOR ACCELERATED WEBSERVING
# see also https://community.platformio.org/t/question-esp32-compress-files-in-data-to-gzip-before-upload-possible-to-spiffs/6274/10
#
# Ampliamente modificado  Ver original en el link citado anteriormente
Import( 'env', 'projenv' )
import os
import shutil
import glob
import json
from re import sub
# Comprime los archivos definidos de 'data_src/' en 'data/'
def renameReactAssets( source, target, env ):
    # Build the react web application
    # react_proj_dir = "react-websocket-client"
    react_proj_dir = "local_webapp"
    os.chdir(react_proj_dir)
    print( '\nReact build: Starting the build process for the React webapp...\n' )
    
    print('\nRunning `npm install`')
    if os.system("npm install") != 0:
        print(f"Error installing dependencies for react application in ./{react_proj_dir}")
        return    
    print('\nRunning `npm run build`')
    if os.system("npm run build") != 0:
        print(f"Error building react application in ./{react_proj_dir}")
        return    
    os.chdir('..')    
    print( '\nRename: Starting the renaming process for the SPIFFS image...\n' )
    data_src_dir_path = os.path.join(env.get('PROJECT_DIR'), f"{react_proj_dir}/dist")
    data_dir_path = os.path.join(env.get('PROJECT_DIR'), 'data_src')
    # rename dest to source if needed
    if(os.path.exists(data_dir_path) and not os.path.exists(data_src_dir_path) ):
        print('Rename: The directory "'+data_dir_path+'" exists, "'+data_src_dir_path+'" is not found.')
        print('Rename: Renaming "' + data_dir_path + '" to "' + data_src_dir_path + '"')
        os.rename(data_dir_path, data_src_dir_path)
    # delete existing folder
    if(os.path.exists(data_dir_path)):
        print('Rename: Deleting the "data" directory ' + data_dir_path)
        shutil.rmtree(data_dir_path)
    print('Rename: Re-creating an empty data directory ' + data_dir_path)
    os.mkdir(data_dir_path)
    files_to_copy = ["digital.ttf", "light.ttf", "regular.ttf" , "semibold.ttf"]
    for file in files_to_copy:
        print('Rename: Copying file: ' + file + ' to the data directory' + data_dir_path)
        shutil.copy(data_src_dir_path + "/" + file, data_dir_path)
    replacedNames = ["index.html", "index.html"]
    with open(data_src_dir_path + "/assets/asset-manifest.json", 'r',encoding="utf-8") as f:
        jsonData = json.load(f)
        css = jsonData["index.html"]["css"][0]
        javascript = jsonData["index.html"]["file"]
        data  = { "css": css, "javascript": javascript}
        # edit files
        for key in data:
            if key in replacedNames: continue
            src_name = data[key]
            
            if key == "css":
                dst_name = "main.css"
            elif key == "javascript":
                dst_name = "main.js"
            # dst_name = sub(r'/([^/]+/)*([^.]+.[^.]+)',r'/\2',src_name)
            # replacedNames += (src_name[:], dst_name[:].replace("assets/",""))
            replacedNames += (src_name, dst_name)
        print(f"names: {replacedNames}")
    # move each file in manifest
    for i in range(0, len(replacedNames), 2):
        ext = replacedNames[i].split(".")[-1]
        if ext == "js" or ext == "css" or ext == "html" or ext == "htm" or ext == "txt":
            print("Renaming references in " + replacedNames[i] + " and move to " + replacedNames[i+1])
            with open(data_src_dir_path + "/" + replacedNames[i], 'r',encoding="utf-8") as inFile:
                with open(data_dir_path + "/" + replacedNames[i+1], 'w',encoding="utf-8") as outFile:
                    for line in inFile:
                        outLine = line
                        for i in range(0, len(replacedNames), 2):
                            outLine = outLine.replace(replacedNames[i], replacedNames[i+1])
                        outFile.write(outLine)
        elif ext == "map":
            continue
        else:
            print("Copying " + replacedNames[i] + " to " + replacedNames[i+1])
            shutil.copy(data_src_dir_path + "/" + replacedNames[i], data_dir_path + "/" + replacedNames[i+1])

env.AddPreAction( '$BUILD_DIR/littlefs.bin', renameReactAssets )