#include <stdio.h>
#include <BBBiolib.h>

int getData(void);

int main(void)
{
	FILE *fp;
	int data;

	iolib_init();
	iolib_setdir(8, 27, DIR_IN); //VSYNC
	iolib_setdir(8, 28, DIR_IN); //HREF
	iolib_setdir(8, 30, DIR_OUT); //buffers
	
	iolib_setdir(8, 45, DIR_IN); //data
	iolib_setdir(8, 46, DIR_IN); //data
	iolib_setdir(8, 43, DIR_IN); //data
	iolib_setdir(8, 44, DIR_IN); //data
	iolib_setdir(8, 41, DIR_IN); //data
	iolib_setdir(8, 42, DIR_IN); //data
	iolib_setdir(8, 39, DIR_IN); //data
	iolib_setdir(8, 40, DIR_IN); //data

	pin_low(8, 30); //buffers off

	//while(1)
	//{
		fp = fopen("picture.png", "w");

		while(is_low(8, 27)); //VSYNC
		{
		}
		for(int i = 0; i < 1024; i++) //1024 may be wrong value
		{
			while(is_low(8, 28)) //HREF
			{
			}
			pin_high(8, 30); //enable buffers
			data = getData();
			fprintf(fp, "%x ", data);
			pin_low(8,30);
		}
		fclose(fp);
	//}


	iolib_free();

}

int getData()
{
	int data;
	data = 0;

	if(is_high(8, 40))
	{
		data = data + 128;	
	} 
	if(is_high(8, 39))
	{
		data = data + 64; 
	}
	if(is_high(8, 42))
	{
		data = data + 32;	
	}
	if(is_high(8, 41))
	{
		data = data + 16;
	}
	if(is_high(8, 44))
	{
		data = data + 8; 
	}
	if(is_high(8, 43))
	{
		data = data + 4;	
	}
	if(is_high(8, 46))
	{
		data = data + 2;
	}
	if(is_high(8, 45))
	{
		data = data + 1;
	}

	return data;
}