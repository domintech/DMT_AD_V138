/*
 * @file include/linux/dmt030818.h
 * @brief DMT g-sensor Linux device driver
 * @author Domintech Technology Co., Ltd (http://www.domintech.com.tw)
 * @version 1.35
 * @date 2012/10/03
 *
 * @section LICENSE
 *
 *  Copyright 2012 Domintech Technology Co., Ltd
 *
 * 	This software is licensed under the terms of the GNU General Public
 * 	License version 2, as published by the Free Software Foundation, and
 * 	may be copied, distributed, and modified under those terms.
 *
 * 	This program is distributed in the hope that it will be useful,
 * 	but WITHOUT ANY WARRANTY; without even the implied warranty of
 * 	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * 	GNU General Public License for more details.
 *
 *
 */
#ifndef DMT030818_H
#define DMT030818_H
#include <linux/types.h>
#include <linux/ioctl.h>
#include <linux/cdev.h>
#include <linux/mutex.h>
#include <linux/syscalls.h>
#include <linux/wait.h>
#include <linux/workqueue.h>
#include <linux/delay.h>
#define CHIP_ENABLE    137 // panda GPIO
#define AUTO_CALIBRATION	0

#ifdef DMT_DEBUG_DATA
#define dmtprintk(x...) printk(x)
#define INFUN printk(KERN_DEBUG "@DMT@ In %s: %s: %i\n", __FILE__, __func__, __LINE__)
#define PRINT_X_Y_Z(x, y, z) printk(KERN_INFO "@DMT@ X/Y/Z axis: %04d , %04d , %04d\n", (x), (y), (z))
#define PRINT_OFFSET(x, y, z) printk(KERN_INFO "@offset@  X/Y/Z axis: %04d , %04d , %04d\n",offset.x,offset.y,offset.z)
#define DMT_DATA(dev, ...) dev_dbg((dev), ##__VA_ARGS__)
#else
#define dmtprintk(x...)
#define INFUN
#define PRINT_X_Y_Z(x, y, z)
#define PRINT_OFFSET(x, y, z)
#define DMT_DATA(dev, ...)
#endif

#define INPUT_NAME_ACC				"DMT_accel" /* Input Device Name  */
#define DEVICE_I2C_NAME 			"dmt"		/* Device name for DMT misc. device */

#define CHIP_ID_REG					0x0a
#define CHIP_ID_VALUE				0x88
#define BANDWIDTH_REG				0x08
//#define BANDWIDTH_REG_VALUE			0x07	// enable Tms AverageOrder(1)No Filter
//#define BANDWIDTH_REG_VALUE			0x06	// enable Tms AverageOrder(2)
//#define BANDWIDTH_REG_VALUE			0x05	// enable Tms AverageOrder(4)
//#define BANDWIDTH_REG_VALUE			0x04	// enable Tms AverageOrder(8)
#define BANDWIDTH_REG_VALUE			0x03	// disable Tms AverageOrder(1)No Filter(default)
//#define BANDWIDTH_REG_VALUE			0x02	// disable Tms AverageOrder(2)
//#define BANDWIDTH_REG_VALUE			0x01	// disable Tms Tms AverageOrder(4)
//#define BANDWIDTH_REG_VALUE			0x00	// disable Tms Tms AverageOrder(8)
#define ACC_REG						0x02

#define CALIBRATION_MAX		256
#define CALIBRATION_MIN		-256

//g-senor layout configuration, choose one of the following configuration
#define CONFIG_GSEN_LAYOUT_PAT_1	1
#define CONFIG_GSEN_LAYOUT_PAT_2	0
#define CONFIG_GSEN_LAYOUT_PAT_3	0
#define CONFIG_GSEN_LAYOUT_PAT_4	0
#define CONFIG_GSEN_LAYOUT_PAT_5	0
#define CONFIG_GSEN_LAYOUT_PAT_6	0
#define CONFIG_GSEN_LAYOUT_PAT_7	0
#define CONFIG_GSEN_LAYOUT_PAT_8	0

#define CONFIG_GSEN_CALIBRATION_GRAVITY_ON_Z_NEGATIVE 1
#define CONFIG_GSEN_CALIBRATION_GRAVITY_ON_Z_POSITIVE 2
#define CONFIG_GSEN_CALIBRATION_GRAVITY_ON_Y_NEGATIVE 3
#define CONFIG_GSEN_CALIBRATION_GRAVITY_ON_Y_POSITIVE 4
#define CONFIG_GSEN_CALIBRATION_GRAVITY_ON_X_NEGATIVE 5
#define CONFIG_GSEN_CALIBRATION_GRAVITY_ON_X_POSITIVE 6

#define AVG_NUM 				16
#define SENSOR_DATA_AVG			3
#define SENSOR_DATA_SIZE 		3 
#define DEFAULT_SENSITIVITY 	1024

#define IOCTL_MAGIC  0x09
#define SENSOR_RESET    		_IO(IOCTL_MAGIC, 0)
#define SENSOR_CALIBRATION   	_IOWR(IOCTL_MAGIC,  1, int[SENSOR_DATA_SIZE])
#define SENSOR_GET_OFFSET  		_IOR(IOCTL_MAGIC,  2, int[SENSOR_DATA_SIZE])
#define SENSOR_SET_OFFSET  		_IOWR(IOCTL_MAGIC,  3, int[SENSOR_DATA_SIZE])
#define SENSOR_READ_ACCEL_XYZ  	_IOR(IOCTL_MAGIC,  4, int[SENSOR_DATA_SIZE])
#define SENSOR_SETYPR  			_IOW(IOCTL_MAGIC,  5, int[SENSOR_DATA_SIZE])
#define SENSOR_GET_OPEN_STATUS	_IO(IOCTL_MAGIC,  6)
#define SENSOR_GET_CLOSE_STATUS	_IO(IOCTL_MAGIC,  7)
#define SENSOR_GET_DELAY		_IOR(IOCTL_MAGIC,  8, unsigned int*)

#define SENSOR_MAXNR 8

s16 sensorlayout[3][3] = {
#if CONFIG_GSEN_LAYOUT_PAT_1
    { 1, 0, 0},	{ 0, 1,	0}, { 0, 0, 1},
#elif CONFIG_GSEN_LAYOUT_PAT_2
    { 0, 1, 0}, {-1, 0,	0}, { 0, 0, 1},
#elif CONFIG_GSEN_LAYOUT_PAT_3
    {-1, 0, 0},	{ 0,-1,	0}, { 0, 0, 1},
#elif CONFIG_GSEN_LAYOUT_PAT_4
    { 0,-1, 0},	{ 1, 0,	0}, { 0, 0, 1},
#elif CONFIG_GSEN_LAYOUT_PAT_5
    {-1, 0, 0},	{ 0, 1,	0}, { 0, 0,-1},
#elif CONFIG_GSEN_LAYOUT_PAT_6
    { 0,-1, 0}, {-1, 0,	0}, { 0, 0,-1},
#elif CONFIG_GSEN_LAYOUT_PAT_7
    { 1, 0, 0},	{ 0,-1,	0}, { 0, 0,-1},
#elif CONFIG_GSEN_LAYOUT_PAT_8
    { 0, 1, 0},	{ 1, 0,	0}, { 0, 0,-1},
#endif
};

typedef union {
	struct {
		s16	x;
		s16	y;
		s16	z;
	} u;
	s16	v[SENSOR_DATA_SIZE];
} raw_data;

struct dev_data {
	dev_t 					devno;
	struct cdev 			cdev;
  	struct class 			*class;
  	struct input_dev 		*input;
	struct i2c_client 		*client;
	struct delayed_work 	delaywork;	//work;
	struct work_struct 		work;	//irq_work;
	struct mutex 			DMT_mutex;
	wait_queue_head_t		open_wq;
	atomic_t				active;
	atomic_t 				delay;
	atomic_t 				enable;
};

#endif               
