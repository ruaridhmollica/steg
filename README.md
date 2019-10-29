# steg
This repository contains a PPM image steganography program, written in C

## Running
To run this program:
For the call:
* $ steg e file1.ppm > file2.ppm
* steg prompts for a message and a secret and shows file1.ppm with the message encoded within it. Here, the output is redirected to file2.ppm.
- For the call:
* $ steg d file.ppm
* steg prompts for the secret, then shows the message hidden in file.ppm if the secret is the right one (or a wrong message, extracted from the image using the wrong secret).

