#include <stdio.h>
#include "encode.h"
#include "types.h"
#include<string.h>
#include<stdlib.h>
#include"common.h"
/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
#if 1
uint get_image_size_for_bmp(FILE *fptr_image)
{
	uint width, height;
	// Seek to 18th byte
	fseek(fptr_image, 18, SEEK_SET);

	// Read the width (an int)
	fread(&width, sizeof(int), 1, fptr_image);
	printf("width = %u\n", width);

	// Read the height (an int)
	fread(&height, sizeof(int), 1, fptr_image);
	printf("height = %u\n", height);

	// Return image capacity
	return width * height * 3;
}
#endif
/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
#if 1
Status open_files(EncodeInfo *encInfo)
{
	printf("INFO : Opening required files\n");
	// Src Image file
	encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
	// Do Error handling
	if (encInfo->fptr_src_image == NULL)
	{
		perror("fopen");
		fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

		return e_failure;
	}

	// Secret file
	encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
	// Do Error handling
	if (encInfo->fptr_secret == NULL)
	{
		perror("fopen");
		fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

		return e_failure;
	}

	// Stego Image file
	encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w+");
	// Do Error handling
	if (encInfo->fptr_stego_image == NULL)
	{
		perror("fopen");
		fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

		return e_failure;
	}

	// No failure return e_success
	return e_success;
}
#endif
//check opration function defination:
OperationType check_operation_type(char **argv)
{
	if (strcmp(argv[1],"-e")==0)//checking Encoding 
		return e_encode;//if true Encoding
	else if (strcmp(argv[1],"-d")==0)//checking Decoding 
		return e_decode;//if true return Decoding
	else
		return e_unsupported;//returning Unsupported
}
//secret_file size:
uint get_file_size(FILE *fptr)
{
	char ch;int count=0;
	while (ch=fgetc(fptr)!=EOF)
		count++;
	printf("secret file size = %d\n",count);
	return count;//returning length of secret file
}
//function defination for Capacity of Input file:
Status check_capacity(EncodeInfo *encInfo)
{
	printf("INFO : ##Encoding Procedure Started ##\n");
	encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);//length of source.bmp file
	strcpy(encInfo->extn_secret_file,strstr(encInfo->secret_fname,"."));//copying extension
	int size = strlen(encInfo->extn_secret_file);//length of extension 
	int size_magic_string=strlen(MAGIC_STRING);//length of magic string
	encInfo->size_secret_file=get_file_size(encInfo->fptr_secret);//secret file size
	uint img_size;
	img_size = get_image_size_for_bmp(encInfo->fptr_src_image);//length of source.bmp
	printf("INFO : Image size = %u\n",img_size);
	if(encInfo->image_capacity > 54+32+(size_magic_string*8)+32+(size*8)+32+(encInfo->size_secret_file*8))//checking image size >our whole Data
	{
		printf("INFO : Checking for secret.txt size\n");
		printf("INFO : Done.Not Empty");
		return e_success;
	}
	else
		return e_failure;
}

//functon defination for the arguments validating:
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
	char str[15];
	if (argv[4]==NULL)
		printf("INFO: Output File not mentioned.Creating default.bmp as default\n");

	if ( strcmp(argv[1],"-e")==0 || strcmp(argv[1],"-d")==0)
	{
		strcpy(str,strstr(argv[2],"."));
		if (strcmp(str,".bmp")==0)
		{
			encInfo->src_image_fname=argv[2];
			strcpy(str,strstr(argv[3],"."));
			if ( strcmp(str,".txt")==0 || strcmp(str,".sh")==0 || strcmp(str,".c")==0)
			{
				encInfo->secret_fname=argv[3];
				if (argv[4]!=NULL)
				{
					strcpy(str,strstr(argv[4],"."));
					if ( strcmp(str,".bmp")==0)
					{	
						encInfo->stego_image_fname=argv[4];
						return e_success;
					}
				}
				else
				{
					encInfo->stego_image_fname="default.bmp";	
					return e_success;
				}
			}
			else
			{
				printf("Please Pass: '.txt' Extension");
				return e_failure;
			}
		}
		else
		{
			printf("Please Pass: '.bmp' Extension");
			return e_failure;
		}
	}
	else
		return e_failure;
}

//function defination for Encoding:
Status do_encoding(char **argv,EncodeInfo *encInfo)
{
	//printf("Encoding Started:\n");
	if (read_and_validate_encode_args(argv,encInfo)==e_success)
	{
		//printf("Read And Validate:\nDone\n");
		if (open_files(encInfo)==e_success)
		{	
			printf("INFO : Opended secret.txt\n");
			printf("INFO : Opended stego_imag.bmp\nDone\n");
			if (check_capacity(encInfo)==e_success)
			{
				printf("INFO : Checking for beautiful.bmp Capacity to handle secret.txt\n");
				printf("INFO : Done ok\n");
				//functioncall for Header copy
				if (copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
					printf("INFO :Image Header\nDone\n");
				else 
					printf("INFO : Image Header:\nNot Done\n");
				//function call for Encode Length_of_magic_string:
				if (encode_length_magic_string(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
					printf("INFO : Encoding Magic string Size:\nDone\n");
				else
					printf("INFO : Encoding Magic string Size:\nNot Done\n");
				//function_call_forcEncode Magic_string:
				if (encode_magic_string(MAGIC_STRING,encInfo)==e_success)
					printf("INFO : Encoding Magic string :\nDone\n");
				else
					printf("INFO : Encoding Magic string :\nNot Done\n");
				//function call for Encode length of extension of secret.txt
				if (encode_length_of_secret_file_extn(encInfo->extn_secret_file,encInfo)==e_success)
					printf("INFO : Encoding secret_file  Extension Size:\nDone\n");
				else
					printf("INFO : Encoding secret_file  Extension Size:\nNot Done\n");
				//function call for Encode extension of secret.txt
				if (encode_secret_file_extn(encInfo->extn_secret_file,encInfo)==e_success)
					printf("INFO : Encoding Secret_file Extension:\nDone\n");
				else
					printf("INFO : Encoding Secret_file Extension:\nNot Done\n");
				//function call for Encode length of secret.txt
				if (encode_secret_file_size(encInfo)==e_success)
					printf("INFO : Encoding secret_file size\nDone\n");
				else
					printf("INFO : Encoding secret_file size\nDone\n");
				//function call for Encode the Data of secret.txt
				if (encode_secret_file_data(encInfo)==e_success)
					printf("INFO : Encoding secret_file\nDone\n");
				else
					printf("INFO : Encoding secret_file\nNot Done\n");
				//function call for remaning data 
				if (copy_remaining_img_data(encInfo)==e_success)
					printf("INFO : Encoding Remaining_data \nDone\n");
				else
					printf("INFO : Encoding Remaining_data \n Not Done\n");

				return e_success;
			}

		}
		else
			printf("Failed Open_files:\n");
	}
	else
		printf("Failed Read And validate args:\n");


}
//function defination for the Header copy :
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
	rewind(fptr_src_image);//poiting first char in file 
	char header[54];//storing header into the string :
	fread(header,54,1,fptr_src_image);//taking source file 54 bytes into str1:
	fwrite(header,54,1,fptr_dest_image);//copying str1 into destination file 54 bytes
	return e_success;//returning succces enum member:
}
//function defination for byte_to_lsb:
Status encode_byte_to_lsb(char data, char *image_buffer)
{
	int i,j;
	for (i=7;i>=0;i--)
	{
		//logic 1
		//image_buffer[7 - i] = (((data & (1 << i)) >> i) | image_buffer[7-i] & (~1));
#if 0
		//logic 2
		   if (data & (1<<i))//checking data bit
		   image_buffer[7-i]=image_buffer[7-i]| 1;//adding last bit with 1
		   else
		   image_buffer[7-i]=image_buffer[7-i] & ~1;//adding last char 0
#endif
		   //logic 3
		   image_buffer[7-i]=image_buffer[7-i] & ~1 | ((data>>i) & 1);
	}
}
//function defination size_to_lsb:
Status encode_size_to_lsb(int data, char *image_buffer)
{
	int index = 0;
	for(int i = 31 ; i >= 0 ; i--)
	{
		//logic 1
#if 0
		   if (data & (1<<i))//checking bit is 0 or 1
		   image_buffer[31-i]=image_buffer[31-i]| 1;//add lsb 1
		   else
		   image_buffer[31-i]=image_buffer[31-i] & ~1;// add lsb 0 
		//image_buffer[31 - i] = (((data & (1 << i)) >> i) | image_buffer[31-i] & (~1));
#endif
		image_buffer[31 - i] = ((data >> i) &1 | image_buffer[31-i] & ~1);
	}
}

//function defination for the Encode length of Magic string:
Status encode_length_magic_string(FILE *fptr_src_image,FILE *fptr_dest_image)
{
	char length[32];
	fread(length,32,1,fptr_src_image);//taking 32 bytes into source file
	uint len=strlen(MAGIC_STRING);//length if Magic_string
	encode_size_to_lsb(len,length);//function call
	fwrite(length,32,1,fptr_dest_image);//writing into output file
	return e_success;//returning succces enum member
}

//function definatiom for the Encode_magic_string:
Status encode_magic_string(const char *magic_string,EncodeInfo *encInfo)
{
	uint len=strlen(MAGIC_STRING);
	char arr[8];//storing  into the string :
	for (int i=0;i<len;i++)
	{
		fread(arr,8,1,encInfo->fptr_src_image);//redaing 8 bytes
		encode_byte_to_lsb(magic_string[i],arr);//function call
		fwrite(arr,8,1,encInfo->fptr_stego_image);	//writing into file 	
	}
	return e_success;//returning succces enum member:
}
//function defination for the encod length of secret.txt extension
Status encode_length_of_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
	uint len_extn=strlen(file_extn);//length of extn
	char length[32];
	fread(length,32,1,encInfo->fptr_src_image);//taking source file 32 bytes into str1:
	uint len=strlen(MAGIC_STRING);
	encode_size_to_lsb(len_extn,length);//function call
	fwrite(length,32,1,encInfo->fptr_stego_image);//writing output file	
	return e_success;//returning succces enum member:
}

//function defination for encode secret.txt file extension
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
	uint len_extn=strlen(file_extn);
	char arr[8];
	for (int i=0;file_extn[i]!='\0';i++)
	{
		fread(arr,8,1,encInfo->fptr_src_image);//read
		encode_byte_to_lsb(file_extn[i],arr);//function call
		fwrite(arr,8,1,encInfo->fptr_stego_image);	//write	
	}
	return e_success;//returning succces enum member:
}

//function defination for encode length of the secret file.
Status encode_secret_file_size(EncodeInfo *encInfo)
{
	char length[32];
	fread(length,32,1,encInfo->fptr_src_image);//taking source file 32 bytes into str1:
	encode_size_to_lsb(encInfo->size_secret_file,length);//function call
	fwrite(length,32,1,encInfo->fptr_stego_image);	//write into output file
	return e_success;//returning succces enum member:
}

//function defination for the encode the secret.txt 
Status encode_secret_file_data(EncodeInfo *encInfo)
{
	//printf("fseek=%ld\n",ftell(encInfo->fptr_src_image));
	char source[(encInfo->size_secret_file) + 1];//storing  into the string :
											 //fseek(encInfo->fptr_src_image,total_len,SEEK_SET);
	rewind(encInfo->fptr_secret);
	fread(source,encInfo->size_secret_file,1,encInfo->fptr_secret);//taking source file len*8 bytes into magic array:
	source[encInfo-> size_secret_file] = '\0';
	char arr[8];
	for (int i=0;source[i]!='\0';i++)
	{
		fread(arr,8,1,encInfo->fptr_src_image);
		encode_byte_to_lsb(source[i],arr);
		fwrite(arr,8,1,encInfo->fptr_stego_image);		
	}
	return e_success;//returning succces enum member):
}
//remailning data copying into dest.file
Status copy_remaining_img_data(EncodeInfo *encInfo)
{
	char ch;
	while(fread(&ch,1,1,encInfo -> fptr_src_image))//copying into ch
		fwrite(&ch,1,1,encInfo -> fptr_stego_image);//writing into output file
	fclose(encInfo->fptr_secret);
	fclose(encInfo->fptr_stego_image);
	fclose(encInfo->fptr_src_image);
	return e_success;//returning succces enum member:
}

