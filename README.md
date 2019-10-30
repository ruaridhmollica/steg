# steg
This repository contains a PPM image steganography program, written in C

# Running
To compile this program into an exe in a command prompt do:
* $ gcc -o steg.exe steg.c

## To run this program(encode):
* $ ./steg.exe e readin.ppm > decodeMe.ppm
* steg prompts for a message and a secret and shows file1.ppm with the message encoded within it. Here, the output is redirected to decodeMe.ppm.
## To run this program(Decode):
* $ ./steg.exe d decodeMe.ppm
* steg prompts for the secret, then shows the message hidden in file.ppm if the secret is the right one (or a wrong message, extracted from the image using the wrong secret).

