/* 
 * Author:- Roshan Raj Kanakaiprath
 *
 * Email ID:- rkanakai@asu.edu 
 * 
 */
#include <logging/log.h>
LOG_MODULE_REGISTER(croxel_assignment, LOG_LEVEL_DBG);

#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>
#include <drivers/sensor.h>
#include <sys/util.h>
#include <sys/printk.h>
#include <shell/shell.h>
#include <shell/shell_uart.h>
#include <version.h>
#include <stdlib.h>
#include <fsl_iomuxc.h>
#include <errno.h>

//Device structure to for device binding
const static struct device *hcsr_04;

//Global variable to hold last measured distance value
static struct sensor_value measured_distance_val;

//Flag to control the main execution, when false main terminates
static bool measure_flag = true;

//Shell handler for command "get_value"
static int command_get_val(const struct shell *shell, size_t argc, char **argv)
{
    shell_print(shell, "Measure distance %d.%02d inches\n", measured_distance_val.val1, (measured_distance_val.val2/10000));
    return 0;
}

static int exit_handler(const struct shell *shell, size_t argc, char *argv[])
{   
    //Set the flag to false to terminate the main while loop
    measure_flag = false;

    LOG_DBG("No further measurement is possible\n");
    return 0;
}

SHELL_CMD_REGISTER(get_value, NULL, "Commands to get the last measured distance", command_get_val);
SHELL_CMD_REGISTER(exit, NULL, "Commands to exit the execution", exit_handler);

//Main Routine
void main()
{
    int ret;

    LOG_DBG("Start main\n");
    
    //Configure the Multiplexer for Trigger and echo signals of the sensor

    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_08_GPIO1_IO24, 0);

    IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_08_GPIO1_IO24,
			    IOMUXC_SW_PAD_CTL_PAD_PUE(1) |
			    IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
			    IOMUXC_SW_PAD_CTL_PAD_SPEED(2) |
			    IOMUXC_SW_PAD_CTL_PAD_DSE(6));

	IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_02_GPIO1_IO18, 0);

	IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_02_GPIO1_IO18,
			    IOMUXC_SW_PAD_CTL_PAD_PUE(1) |
			    IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
			    IOMUXC_SW_PAD_CTL_PAD_SPEED(2) |
			    IOMUXC_SW_PAD_CTL_PAD_DSE(6));
    
    //Get Device bindings
	hcsr_04 = device_get_binding("HC-SR04_0");
	if (!hcsr_04) {
		LOG_DBG("error binding sensor 0\n");
		return;
	}

    //Measure the distance sensor value every 500 ms
    while(measure_flag)
    {   
    //Get sensor_sample
        ret = sensor_sample_fetch_chan(hcsr_04,SENSOR_CHAN_ALL);
        if(ret != 0U)
        {
            LOG_ERR("Error when reading device: %s", hcsr_04->name);
            return;
        }

        ret = sensor_channel_get(hcsr_04, SENSOR_CHAN_DISTANCE, &measured_distance_val);
        if(ret)
        {   
            printk("error gettimg chan\n");
            LOG_ERR("sensor_channel_get failed ret %d", ret);
            return;            
        }
        k_msleep(500);
    }
    LOG_DBG("Exited Main\n");
}//Exit main
