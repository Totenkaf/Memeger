import os 
import subprocess
os.chdir(r'./build') 
print(os.getcwd())
os.system("cmake ..")
os.system("make")
command ="./async.http.server localhost 8080 3"
subprocess.call(command, shell = True)
os.system("./async.http.client localhost 8080 /delete/")