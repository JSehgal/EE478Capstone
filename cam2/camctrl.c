/****************************************
 * Program for Omnivision OV9655
 * CMOS camera
 ****************************************/

#include <stdio.h> 
#include <string.h>
#include <math.h>
#include "i2cfunc.h"
#include <stdio.h> 
#include "linux/i2c.h"
#include "linux/i2c-dev.h"
#include "sys/ioctl.h"
#include "fcntl.h"
#include "errno.h"
#include <time.h>


#include <sys/types.h>

#include "camctrl.h"


// we will use I2C2 which is enumerated as 1 on the BBB
#define I2CBUS 1
#define CAM_ADDR 0x30

#define DBG_PRINT 0

// array of data
const unsigned char params_qvga[]={
  0x00, 0x00, 0x01, 0x80,	0x02, 0x80, 0x03, 0x02,
  0x04, 0x03, 0x09, 0x03, 0x0b, 0x57, 0x0e, 0x61,
  0x0f, 0x40, 0x11, 0x01, 0x12, 0x62, 0x13, 0xc7,
  0x14, 0x3a, 0x15, 0x08, 0x16, 0x24, 0x17, 0x18,
  0x18, 0x04, 0x19, 0x01, 0x1a, 0x81, 0x1e, 0x20,
  0x24, 0x3c, 0x25, 0x36, 0x26, 0x72, 0x27, 0x08,
  0x28, 0x08, 0x29, 0x15, 0x2a, 0x00, 0x2b, 0x00,
  0x2c, 0x08, 0x32, 0xa4, 0x33, 0x00, 0x34, 0x3f,
  0x35, 0x00, 0x36, 0x3a, 0x38, 0x72, 0x39, 0x57,
  0x3a, 0xcc, 0x3b, 0x04, 0x3d, 0x99, 0x3e, 0x0e,
  0x3f, 0xc1, 0x40, 0xc0, 0x41, 0x41, 0x42, 0xc0,
  0x43, 0x0a, 0x44, 0xf0, 0x45, 0x46, 0x46, 0x62,
  0x47, 0x2a, 0x48, 0x3c, 0x4a, 0xfc, 0x4b, 0xfc,
  0x4c, 0x7f, 0x4d, 0x7f, 0x4e, 0x7f, 0x4f, 0x98,
  0x50, 0x98, 0x51, 0x00, 0x52, 0x28, 0x53, 0x70,
  0x54, 0x98, 0x58, 0x1a, 0x59, 0x85, 0x5a, 0xa9,
  0x5b, 0x64, 0x5c, 0x84, 0x5d, 0x53, 0x5e, 0x0e,
  0x5f, 0xf0, 0x60, 0xf0, 0x61, 0xf0, 0x62, 0x00,
  0x63, 0x00, 0x64, 0x02, 0x65, 0x20, 0x66, 0x00,
  0x69, 0x0a, 0x6b, 0x1a, 0x6c, 0x04, 0x6d, 0x55,
  0x6e, 0x00, 0x6f, 0x9d, 0x70, 0x21, 0x71, 0x78,
  0x72, 0x22, 0x73, 0x02, 0x74, 0x10, 0x75, 0x10,
  0x76, 0x01, 0x77, 0x02, 0x7A, 0x12, 0x7B, 0x08,
  0x7C, 0x16, 0x7D, 0x30, 0x7E, 0x5e, 0x7F, 0x72,
  0x80, 0x82, 0x81, 0x8e, 0x82, 0x9a, 0x83, 0xa4,
  0x84, 0xac, 0x85, 0xb8, 0x86, 0xc3, 0x87, 0xd6,
  0x88, 0xe6, 0x89, 0xf2, 0x8a, 0x24, 0x8c, 0x80,
  0x90, 0x7d, 0x91, 0x7b, 0x9d, 0x02, 0x9e, 0x02,
  0x9f, 0x7a, 0xa0, 0x79, 0xa1, 0x40, 0xa4, 0x50,
  0xa5, 0x68, 0xa6, 0x4a, 0xa8, 0xc1, 0xa9, 0xef,
  0xaa, 0x92, 0xab, 0x04, 0xac, 0x80, 0xad, 0x80,
  0xae, 0x80, 0xaf, 0x80, 0xb2, 0xf2, 0xb3, 0x20,
  0xb4, 0x20, 0xb5, 0x00, 0xb6, 0xaf, 0xb6, 0xaf,
  0xbb, 0xae, 0xbc, 0x7f, 0xbd, 0x7f, 0xbe, 0x7f,
  0xbf, 0x7f, 0xbf, 0x7f, 0xc0, 0xaa, 0xc1, 0xc0,
  0xc2, 0x01, 0xc3, 0x4e, 0xc6, 0x05, 0xc7, 0x82,
  0xc9, 0xe0, 0xca, 0xe8, 0xcb, 0xf0, 0xcc, 0xd8,
  0xcd, 0x93,
  0xaa, 0xbb, 0xcc, 0xdd // 0xaabbccdd is the end sequence
  };




int i2ch; // i2c handle


unsigned char cam_reg_read(unsigned char addr, unsigned char reg)
{
	int ret;
	unsigned char buf[2];
	// To read from the camera, we first write the register value
	buf[0]=reg;
	i2c_write_ignore_nack(i2ch, CAM_ADDR, buf, 1);
	i2c_close(i2ch);
	i2ch=i2c_open(I2CBUS, CAM_ADDR);
	ret=i2c_read_no_ack(i2ch, CAM_ADDR, buf, 1);
	if (ret<0)
		printf("cam_reg_read error!\n");
	
	return(buf[0]);
}

void cam_id_dump(void)
{
	unsigned char val;
	val=cam_reg_read(CAM_ADDR, OV9655_MIDH);
	printf("Mnfr ID1= 0x%02x\n", val);
	val=cam_reg_read(CAM_ADDR, OV9655_MIDL);
	printf("Mnfr ID2= 0x%02x\n", val);
	val=cam_reg_read(CAM_ADDR, OV9655_VER);
	printf("Ver     = 0x%02x\n", val);
	val=cam_reg_read(CAM_ADDR, OV9655_PID);
	printf("PID     = 0x%02x\n", val);
}

int cam_init(void)
{
	int i=0;
	unsigned char pair[2];
	int not_finished=1;
	
	i2ch=i2c_open(I2CBUS, CAM_ADDR);
	
	// do a reset
  pair[0]=0x12; pair[1]=0x80;
  i2c_write_ignore_nack(i2ch, CAM_ADDR, pair, 2);
  delay_ms(999);
	
	
	do
	{
		// obtain address and value to program
		pair[0]=params_qvga[i];
		pair[1]=params_qvga[i+1];
		// reached end?
		if ((pair[0]==0xaa) && (pair[1]==0xbb))
		{
			if ((params_qvga[i+2]==0xcc) && (params_qvga[i+3]==0xdd))
			{
				// end sequence seen.
				not_finished=0;
				break;
			}
		}
		//i2c_write(i2ch, pair, 2);
		i2c_write_ignore_nack(i2ch, CAM_ADDR, pair, 2);
		delay_ms(10);
		i=i+2;
	}while (not_finished);

  // Set the RGB565 mode
  pair[0]=OV9655_COM7; pair[1]=0x63;
  i2c_write_ignore_nack(i2ch, CAM_ADDR, pair, 2);
  delay_ms(10);
  pair[0]=OV9655_COM15; pair[1]=0x10;
  i2c_write_ignore_nack(i2ch, CAM_ADDR, pair, 2);
  delay_ms(10);
  
  // test
  //pair[0]=OV9655_COM3; pair[1]=0x80;
  //i2c_write_ignore_nack(i2ch, CAM_ADDR, pair, 2);
  //delay_ms(10);
  
  
  // Invert the HRef signal
  pair[0]=OV9655_COM10; pair[1]=0x08;
  i2c_write_ignore_nack(i2ch, CAM_ADDR, pair, 2);
  delay_ms(10);

	cam_id_dump();
	i2c_close(i2ch);
	

	printf("Camera should be working\n");
	
	return(0);
}


