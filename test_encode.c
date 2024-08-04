#include <stdio.h>
#include "encode.h"
#include "types.h"
#include<string.h>
#include<stdlib.h>
#include "decode.h"
int main(int argc,char *argv[])
{
	EncodeInfo encInfo;
	uint img_size;
	DecodeInfo decInfo;

	if (argc==1 || argc==2)
	{
		printf("Encoding:./a.out -e <.bmp file> <.txt file> [output file]\n");
		printf("Decoding:./a.out -d <.bmp file> [output file]\n");
	}
	if (argc>2)
	{
		 int op=check_operation_type(argv);
		 if (op==e_encode)
		 {
			//printf("Selected Encoding: \n");
			  if (argc>3)
			  {
			  if(do_encoding(argv,&encInfo)==e_success)
				printf("##Encoding Done Successfulyy##\n");
			  }
			  else
				printf("Encoding: ./a.out -e <.bmp file > <.txt file> [output file]\n");

		 }
		 else if (op==e_decode)
		 {
			 printf("Selected Decoding: \n");
			 if (do_decoding(argv,&decInfo)==e_success)
		 		printf("Decoding Complited:\n");

		 }
		 else
			 printf("Invalid Operation :\n");

	}

		
	return 0;
}
