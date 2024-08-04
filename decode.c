#include<stdio.h>
#include "types.h"
#include "decode.h"
#include <stdlib.h>
#include<string.h>

//function defination foe decode_lsb_byte:
Status decode_byte_to_lsb(char *data, char *decode_data)
{
	int i;
	*data=0;//assigning value defalut as 0
	for (i=0;i<8;i++)
	{
		*data = *data | (decode_data[i] & 1) << (7 - i);//taking lsb bit and storing into Data
	}
}
//function defination for Decode_size_to_byte:
Status decode_size_to_lsb(uint *magic_string_len, char *decode_data)
{
	int i;
	*magic_string_len=0;
	for (i=0;i<32;i++)
	{
		*magic_string_len = *magic_string_len | (decode_data[i] & 1) << (31 - i);//taking lsb and storing into Magic_string
	}
}
//function defination for do_decoding
Status do_decoding(char **argv,DecodeInfo *decInfo)
{
	if (decode_read_and_validate(argv,decInfo)==e_success)
	{
		printf("INFO : ## Decoding Procedure Started ##\n");
		if (decode_open_files(decInfo)==e_success)
		{
			fseek(decInfo->fptr_stego_image,54,SEEK_SET);
			if ((decode_length_magic_string(decInfo))==e_success)
			{
				printf("INFO : Decoding Magic_string size:\nDone\n");
				if ((decode_magic_string(decInfo))==e_success)
				{
					printf("INFO : Decoding Magic string \nDone\n");
					printf("Enter the Your Password:");
					scanf("%s",decInfo->password);
					if (checking_password(decInfo->password,decInfo)==e_success)
					{
						if (decode_output_file_extn_size(decInfo->fptr_stego_image,decInfo)==e_success)
						{
							printf("INFO : Decoding Extension Size:\nDone\n");
							if (decode_output_file_extn( decInfo->len_extn,decInfo,decInfo->fptr_stego_image)==e_success)
							{
								printf("INFO : Decoding Extension:\nDone\n");

								if ( decode_file_size(decInfo,decInfo->fptr_stego_image)==e_success)
								{
									printf("INFO : Decoding  stego_imag.file Size:\nDone\n");
										if (argv[3]==NULL)
											printf("INFO : Output file is Not created Creating output.txt default\n");
										if (decode_file_data(decInfo)==e_success)
											printf("INFO : Decoding  stego_imag.file Data :\nDone\n");
										else
											printf("INFO : Decoding  stego_imag.file Data :\nDone\n");
								}
								else
									printf("INFO : Decoding  stego_imag.file Size:\n Not Done\n");
							}
							else
								printf("INFO : Decoding Extension Size:\n Not Done\n");

						}
						else
							printf("INFO : Decoding Extension Size:\nNot Done\n");
					}
					else
						printf("INFO : Decoding Your Password is Locked:\n");
				}
				else
					printf("INFO : Decoding Magic_string Decoded\nNot Done\n");
			}
			else
				printf("INFO : Decoding Magic_string:\nNot Done\n");
		}
		else
			printf("Decoding open files:\nNot Done\n");
	}
	else
		printf("Decoding Read and validate:\nNot Done\n");

}

Status decode_read_and_validate(char **argv,DecodeInfo *decInfo)
{
	char str[20];
	if (argv[2]!=NULL)
	{
		strcpy(str,strstr(argv[2],"."));
		if (strcmp(str,".bmp")==0)//comparing .bmp
		{
			decInfo->stego_image_fname=argv[2];//storing input file name
			printf("%s\n",decInfo->stego_image_fname);
			if (argv[3]!=NULL)
			{
				strcpy(decInfo->output_file,argv[3]);//storing output file name
				return e_success;
			}
			else
			{
				strcpy(decInfo->output_file,"default");//assigning default output file name
				return e_success;
			}
		}
		else
			printf("Please Pass: Input file With '.bmp'\n");
	}

}

Status decode_open_files(DecodeInfo *decInfo)
{
	printf("INFO : Opening required files\n");
	if ((decInfo->fptr_stego_image=fopen(decInfo->stego_image_fname,"r"))==NULL)//opening input file
		printf("Error: Not opened Output file:\n");
	else
		printf("Opened Stego_imag.bmp\nDone\n");
	return e_success;
}

//Decode length of magic string
Status decode_length_magic_string(DecodeInfo *decInfo)
{
	int i=0;
	char length[32];
	fread(length,32,1,decInfo->fptr_stego_image);//read from input file
	decInfo->len_magic_string=0;//assingning len_magicstring=0
	decode_size_to_lsb(&(decInfo->len_magic_string), length);//function call
//	printf("magic_string length= %u\n",decInfo->len_magic_string);
	return e_success;
}


//decoding Magic_string
Status decode_magic_string(DecodeInfo *decInfo)
{
	char arr[8];
	int i=0;
	for (i=0;i<decInfo->len_magic_string;i++)
	{
		fread(arr,8,1,decInfo->fptr_stego_image);
    	decode_byte_to_lsb(&(decInfo->magic_string[i]),arr);
	}
	return e_success;

}
//Checking password is Correct Or Wrong:
Status checking_password(char *password,DecodeInfo *decInfo)
{
	if (strcmp(password,decInfo->magic_string)==0)//checking password
	{
		printf("Entered Correct Password:\n");
		return e_success;
	}
	else
	{
		for (int i=1;i<3;i++)
		{
			printf("You have %d Chances only:\n",3-i);//giving user only 3 chances
			printf("Enter your password: \n");
			scanf("%s",password);
			if (strcmp(password,decInfo->magic_string)==0)
			{
				printf("Entered Correct Password:\n");
				return e_success;
			}
		}
		return e_failure;
	}

}
//function defination out_file extension Size:
Status decode_output_file_extn_size(FILE *fptr,DecodeInfo *decInfo)
{
	int i=0;
	char length[32];
	fread(length,32,1,fptr);
	decInfo->len_extn=0;
	decode_size_to_lsb(&(decInfo->len_extn),length);
	//printf("extn_len=%d\n",decInfo->len_extn);
	return e_success;
}

//function defination for Output_file extn:
Status decode_output_file_extn( uint size, DecodeInfo *decInfo,FILE *fptr)
{
	char arr[8];
	int i=0;
	for (i=0;i<decInfo->len_extn;i++)
	{
		fread(arr,8,1,fptr);//read from file
    	decode_byte_to_lsb(&(decInfo->output_file_extn[i]),arr);
	}
	//printf("extn= %s\n",decInfo->output_file_extn);
	strcat(decInfo->output_file,decInfo->output_file_extn);//concatinate file name with extension
	return e_success;
}

//function defination for size of secret_file:
Status decode_file_size(DecodeInfo *decInfo,FILE *fptr)
{
	int i=0;
	char length[32];
	fread(length,32,1,fptr);
	decInfo->len_secret_file=0;
	decode_size_to_lsb(&(decInfo->len_secret_file),length);
	//printf("file_len=%d\n",decInfo->len_secret_file);
	return e_success;
}

//decoding secret file data:
Status decode_file_data(DecodeInfo *decInfo)
{
	FILE *fptr=fopen(decInfo->output_file,"w");//opening outut file int write mode (output.txt)
	char arr[8];
	char ch=0;
	for (int i=0;i<decInfo->len_secret_file;i++)
	{
		fread(arr,8,1,decInfo -> fptr_stego_image);//reading 8 bytes from source file
		decode_byte_to_lsb(&ch,arr);//decoding into one character storing into 'ch'
		fwrite(&ch,1,1,fptr);//writing into output.txt character ch
	}

	//fclose(decInfo->fptr_stego_image);
	//fclose(decInfo->fptr_output_file);
	fclose(fptr);//closing file
	return e_success;

}
