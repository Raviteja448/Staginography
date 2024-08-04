#ifndef DECODE_H
#define DECODE_H
#include "types.h" // Contains user defined types
#include<stdio.h>
#define EXTN_LENGTH 4
#define MAGIC_STRING_LENGTH 50
#define SECRET_FILE_LEN (SECRET_FILE_LEN * 8)
	
typedef struct DecodeInfo_
{
    /* Encoded stego image info */
    char *stego_image_fname;
    FILE *fptr_stego_image;
    /* Decoded output file info */
    char output_file[100];
    FILE *fptr_output_file;
	uint len_magic_string;
    char magic_string[40];
	uint len_extn;
    char output_file_extn[EXTN_LENGTH];
	uint len_secret_file;
	char secret_file_data[200];

    /* Passcode */
    char password[20];
    uint password_size;

}DecodeInfo;

/*Read and validate Decode args from argv*/
Status decode_read_and_validate(char **argv, DecodeInfo *decInfo);

/*decode open files*/
Status decode_open_files(DecodeInfo *decInfo);

/*decoding function*/
Status do_decoding(char **argv,DecodeInfo *decInfo);

/*decode magic string length:*/
Status decode_length_magic_string(DecodeInfo *decInfo);

/*Decode the magic string*/
Status decode_magic_string(DecodeInfo *decInfo);

//Decode the size of the secret file extension from the source image//
Status decode_output_file_extn_size(FILE *fptr,DecodeInfo *decInfo);

//Decode the secret file extension from the source image //
Status decode_output_file_extn( uint size, DecodeInfo *decInfo,FILE *fptr);

//Decode the size of the secret file data from the source image//
Status decode_file_size(DecodeInfo *decInfo,FILE *fptr);

//Decode the secret file data from the source image //
Status decode_file_data(DecodeInfo *decInfo);

//Decode the data from the source image //
Status decode_byte_to_lsb(char *magic_string,char *decode_data);

//Decode the size from the least significant bits(LSB)//
Status decode_size_to_lsb(uint *magic_string_len, char *decode_data);
  //Status decode_lsb_to_size(char *decode_data, long *size);
Status checking_password(char *password,DecodeInfo *decInfo);

#endif
