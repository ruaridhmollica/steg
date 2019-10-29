#include<stdio.h>
#include<stdlib.h>
#include <string.h>
/*#################################################### CREATE THE STRUCTURES USED TO HOLD PPM IMAGE ########################################################################################################*/


struct data {
  int r,g,b;
};

struct PPM {
  char P3[4];
  char comments;
  int width;
  int height;
  int max;
  struct data *Data;//creates an array Data that is a pointer to the RGB structre created above
};

/*#################################################### GET THE PPM FILE CONTENTS AND PUT THEM INTO THE STRUCTURE ########################################################################################################*/


struct PPM* GetPPM(FILE * fp){

  int c;
  int r,g,b;
  int size;
  int i;
  i = 0;
  r = 0;
  g = 0;
  b = 0;
  char readChars[256] = {0};
  struct PPM *im = (struct PPM *)calloc(1, sizeof(struct PPM));//allocates memory for the image based on the structure
  fgets(im->P3, sizeof(im->P3), fp);

//goes through the PPM image using the readchars "buffer" inserting everything into the correct variables in the structure
  if((im->P3[0] != 'P') && (im->P3[1] != '3')){
      return NULL;
     }

  im->P3[2] = '\0';
  fgets(readChars, sizeof(readChars), fp);

  while(readChars[0] == '#'){ //where there is a comment it attempts to read it and if not skips it
        im->comments = readChars[0];
       fgets(readChars, sizeof(readChars), fp);

     }

  sscanf(readChars, "%d %d", &im->width, &im->height);
  fgets(readChars, sizeof(readChars), fp);
  sscanf(readChars, "%d", &im->max);

  im->Data = (struct data*)malloc(im->width * im->height * sizeof(struct data));//allocates memory for the RGB pixels that the image holds, simple calculation
  size = im->width * im->height;

  for (i=0;i<size;i++){//loops through all the pixels reading each rgb value into the structure
        fscanf(fp, "%d %d %d ", &r, &g, &b);
        im -> Data[i].r = (int) r;
        im -> Data[i].g = (int) g;
        im -> Data[i].b = (int) b;
      }

  fclose(fp);
  return im;

}

/*#################################################### FREE UP MEMORY ########################################################################################################*/

//De-Allocates space in memory
void FreePPM(struct PPM *im){
  free(im->Data);
  free(im);
}

/*#################################################### SHOW PPM IMAGE ############################################################################################################*/


void showPPM(struct PPM *im){

  int i = 0;
  int k;
  int totalpixels = im->width * im->height;

//basic print functions
  printf("P3%s\n", im->P3);

  printf("#Comments\n");

  printf("%d %d\n", im->width, im->height);
  printf("%d\n", im->max);

//loops to the total number of pixels so that all rgb values are printed
  for(i = 0; i < totalpixels; ++i){
      printf("%d %d %d  ", im->Data[i].r, im->Data[i].g, im->Data[i].b);
      if((i+1)%im->width == 0 && i > 0){//generates a new line in the correct position so that formatting is correct
        printf("\n");
      }

  }
  //printf("\n%d\n", key);

}

/*#################################################### CONVERT INTEGER TO BINARY ########################################################################################################*/

long long toBinary(int ascii){

  long long binaryNumber = 0;
  int remainder, i = 1, step = 1;
//takes the decimal value, uses modulus to get the remainder and uses a multiple of 10 in conjuction with this to create each binary character then adds puts it in a variabl
  while(ascii != 0){
        remainder = ascii%2;
        ascii/=2;
        binaryNumber += remainder*i;
        i *= 10;
  }
return binaryNumber;
}

/*#################################################### CONVERT BINARY TO INTEGER ########################################################################################################*/

int binaryToDecimal(long long binary){

  int number;
  int decimalNum = 0;
  int base = 1;
  int rmndr;

  number = binary;
  while(number > 0){//does a standard calculation using modulo and base to go through the binary changing it to decimal and adding it to the current decimal value. used for encoding and decoding
        rmndr = number % 10;
        decimalNum = decimalNum + rmndr * base;
        number = number / 10;
        base = base * 2;
  }
  return decimalNum;
}


/*######################################################## ENCODE IMAGE ####################################################################################################################*/

struct PPM * encode(struct PPM * im, char * message, unsigned int mSize, unsigned int secret){
  int k,i,j;
  int m = -1;
  int ascii[20];
  long long binaryVal[mSize];
  int imageSize = im->width * im->height;//creates a variable that knows the number of pixels
  long long pixelBinaryR;
  long long pixelBinaryG;
  long long pixelBinaryB;
  int binarySize = mSize*7;
  int amountOfRand = 0;
  int binArr[binarySize];
  int finalBinaryArray[binarySize];
  srand(secret);//sets the seed of the random function to be the secret key that the user enters(used for allocating pixels and decoding later on)
  char *end = "!";
  int asciiEnd;

  message = strcat(message, "*");//adds on my end pointer to the message so i can decode successfully
  mSize = strlen(message);

  for(i=0; i<mSize;++i){
    ascii[i] = (int) message[i];//converts the whole message char by char into its ascii value and stores it in an array, used for binary conversion
  }

  for(j=0;j<mSize;++j){//loops to the size of the word that the user enters and then calls the binary function passing in the value of each letter in turn giving us an array that holds the 8 bit binary for each letter of the word
      binaryVal[j] = toBinary(ascii[j]);
  }

  /* ###################################### Puts 8 bit binary strings into one array  ready for indexing ######################################*/

  j = 0;
  for(i=(mSize-1); i>=0; --i){//this flips the binary as it was back to front
      while(binaryVal[i] > 0){
            binArr[j] = binaryVal[i] % 10;
            ++j;
            binaryVal[i] /= 10;
      }
  }

  k=0;
  int count = binarySize;
  for(k=0; k<binarySize; ++k){//this reverses the array as the whole array was backwards
      finalBinaryArray[k] = binArr[count-1];
      count--;
  }

  for(i=0; i<binarySize; ++i){
  }

/*###################################### Allocates how many random numbers need to be generated ######################################*/

  amountOfRand = binarySize/3;//divides the binary by 3 to figure out how many 3 bit pixels need to have random numbers generated
  if(binarySize % 3 != 0){
    ++amountOfRand;
  }

/*###################################### Populates the random number array with numbers #############################################*/

  int r[amountOfRand];//creates an array of the size we need -- found above
  for(j=0; j<amountOfRand; j++){
      r[j] = rand() % (imageSize);
  }

  asciiEnd = (int)end[0];//converts the end char * into an ascii character this is actually not used anymore as i have another function for it but leaving it in as a test example

/*-###################################### Converts the Random image Pixel into binary and changes the LSB according to the binary bit of the currrent position in the "word" ######################################*/

  i = 0;
  for(k=0; k<binarySize; k++){//loops to the length of the binary that needs to be inserted
      pixelBinaryR = toBinary(im->Data[r[i]].r);
      pixelBinaryG = toBinary(im->Data[r[i]].g);//converts each r, g, b value at the index of the random number into binary ready to change
      pixelBinaryB = toBinary(im->Data[r[i]].b);

      if(finalBinaryArray[k] == 1){//performs basic calculation so that if the binary value we want to add to the LSB is 1 then we change the LSB to 0 then add 1, if its 0 then we change it to 0
        pixelBinaryR = pixelBinaryR/10*10 +1;
      }else{
        pixelBinaryR = pixelBinaryR/10*10;
      }
      im->Data[r[i]].r = binaryToDecimal(pixelBinaryR);//converts the new encoded binary into decimal and puts it back in place in the image
//the above process is repeated until all binary bits are encoded
      if(finalBinaryArray[k+1] == 1){
        pixelBinaryG = pixelBinaryG/10*10 +1;
      }else{
        pixelBinaryG = pixelBinaryG/10*10;
      }
      im->Data[r[i]].g = binaryToDecimal(pixelBinaryG);

      if(finalBinaryArray[k+2] == 1){
        pixelBinaryB = pixelBinaryB/10*10 +1;
      }else{
        pixelBinaryB = pixelBinaryB/10*10;
      }
      im->Data[r[i]].b = binaryToDecimal(pixelBinaryB);

      ++i;//increments to the next random number in the array
      k = k+2;//increments by 2 to get the next 3 r g b
  }
  showPPM(im);//calls show PPM which will output the new image in turn writing  it to the decode file that the user specifies
}

/*#################################################### DECODE IMAGE #############################################################################*/

char * decode(struct PPM * im, unsigned int secret){

  long long pixelBinaryR;
  long long pixelBinaryG;
  long long pixelBinaryB;
  int binaryarry[500];
  int asciiArr[50];
  int imageSize = im->width * im->height;
  int foundEnd = 0;
  int i = 0;
  char word[50];
  int r,k,j,m;
  int lastDigit;
  int binsize = 0;
  srand(secret);//sets the see to be the secret the user enters. obviously if the user enters an incorrect see then it will not work
  //printf("\n%d\n", secret);
  long long asciiWordArr;
  j=0;
  char *ptr;


  while(foundEnd == 0){
      r = rand() % imageSize;//gets a random number between 0 and the image size
      pixelBinaryR = toBinary(im->Data[r].r);//converts the found bit into binary
      lastDigit = pixelBinaryR % 10;//gets the last digit by getting the remainder if you devide by 10 (always going to leave the last digit)
      binaryarry[i] = lastDigit;//adds the last digit to an array and increases the counter that is keeping track of the size of the array
      binsize++;

      if(binsize % 7 == 0){//if the size of the array is 7 it means we have got enough binary to make a letter so it converts the 7 bits into one long binary value and converts the binary into decimal
        char str[7];
        str[0] = sprintf(str, "%d%d%d%d%d%d%d", binaryarry[0], binaryarry[1],binaryarry[2],binaryarry[3],binaryarry[4],binaryarry[5],binaryarry[6]);
        asciiWordArr = strtol(str,&ptr, 10);
        asciiArr[j] = binaryToDecimal(asciiWordArr);
        printf("%d\n", asciiArr[j]);
        if (asciiArr[j] = 42){//if that converted decimal is the ascii value for the end of file pointer then stop
            foundEnd =1;
        }
        j++;//increment j
      }//process repeated until  last val is found

      pixelBinaryG = toBinary(im->Data[r].g);
      lastDigit = pixelBinaryG % 10;
      binaryarry[i+1] = lastDigit;
      binsize++;

      if(binsize % 7 == 0){
        char str[7];
        str[0] = sprintf(str,"%d%d%d%d%d%d%d", binaryarry[0], binaryarry[1],binaryarry[2],binaryarry[3],binaryarry[4],binaryarry[5],binaryarry[6]);
        asciiWordArr = strtol(str,&ptr, 10);
        asciiArr[j] = binaryToDecimal(asciiWordArr);
        printf("%d\n", asciiArr[j]);
        if (asciiArr[j] = 42){
            foundEnd =1;
        }
        j++;
      }

      pixelBinaryB = toBinary(im->Data[r].b);
      lastDigit = pixelBinaryB % 10;
      binaryarry[i+2] = lastDigit;
      binsize++;
      i+3;

      if(binsize % 7 == 0){
        char str[7];
        str[0] = sprintf(str, "%d%d%d%d%d%d%d", binaryarry[0], binaryarry[1],binaryarry[2],binaryarry[3],binaryarry[4],binaryarry[5],binaryarry[6]);
        asciiWordArr = strtol(str,&ptr, 10);
        asciiArr[j] = binaryToDecimal(asciiWordArr);
        printf("%d\n", asciiArr[j]);
        if (asciiArr[j] = 42){
            foundEnd =1;
        }
        j++;
      }
  }

  for(k=0;k<3;++k){
    printf("%d ", asciiArr[k]);//prints the contents of the ascii array
    char word[k] = asciiArr[k];
    printf("%c ", word[k]);
  }

}

/*#################################################### MAIN METHOD ################################################################################*/
 int main(int argc, char * argv[]){

  char fileName[40];
  char message[40];
  int mSize = 0;
  int temp;
  int secret;
  char term;

//checks if the user is wanting to encode, if they are then it tells them so(using print errors so it doesnt write to the fi;e)
  if(!strcmp(argv[1], "e")){
    fprintf(stderr, "\n||------------You Have Selected ENCODE------------||\n");

    FILE* fp = fopen(argv[2], "r");//takes the argument that contains the ppm file

  if(fp==NULL){//catch clause
    fprintf(stderr,"\nError: cannot open file");
    exit(1);
   }

  struct PPM *im = GetPPM(fp);//calls the method to read the file
  FreePPM(im);//frees up the memory allocation

  fprintf(stderr,"Please Enter the Message you would like to encode into the image:\n");//records the secret message the user wants to encode
  scanf("%s", message);
  mSize = strlen(message);
  fprintf(stderr,"\n");


  fprintf(stderr,"Please Enter the secret key(integer) you would like to use to encode the message:\n");//records the key used for the random number seed
  if(scanf("%d%c", &temp, &term) != 2 || term != '\n'){
    fprintf(stderr,"Sorry, invalid input\n");//makes sure it is an integer
    exit(1);
  }else{
    secret = temp;
  }
  //FreePPM(im);
  encode(im, message, mSize, secret);//calls encode function with these parameters
}

  if(!strcmp(argv[1], "d")){//checks if the user is decoding
    fprintf(stderr,"\n||------------You Have Selected DECODE-----------||\n");

    FILE* fp = fopen(argv[2], "r");//takes the argument that contains the ppm file to decode

  if(fp==NULL){//catch clause
    fprintf(stderr,"\nError: cannot open file");
    exit(1);
   }

  struct PPM *im = GetPPM(fp);//calls the method to read the file in
  FreePPM(im);//frees up malloc memory
    fprintf(stderr,"Please Enter the secret key(integer) to decode the message:\n");//records the key used for the random number seed
    scanf("%d", &secret);

    decode(im, secret);//calls the decode function with these parameters
  //  exit(1);
  }

  return 0;
}
