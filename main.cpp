#include <stdio.h>
#include <libusb-1.0/libusb.h>
#define VENDOR  0x1038
#define SENSEI_RAW_MLG  0x136b


int main (int argc, char *argv[])
{

    int result;
    unsigned char// zerodata[1025],
        data[1025],/*configdataset*/
        profile_1[8]= {0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00}; /*sysprofile e 1 */
data[0]=0x05;
  int i=1;
    while(i<=1025)
    {
        data[i]=0x00;
        i++;
    }
    ssize_t counter;
    libusb_device **devs;
    libusb_device_handle *dev_handle;
    libusb_context *seasson=NULL;

    result = libusb_init(&seasson);
    if(result)
    {
        printf("USB Init Error(): %s\n",libusb_error_name (result));
        return 1;
    }
    libusb_set_debug(seasson, 3);
    counter = libusb_get_device_list(seasson,&devs);
    if (counter<0)
    {
        printf("try again (no device found)!!!\n");
        return 1;
    }
    dev_handle = libusb_open_device_with_vid_pid(seasson, VENDOR, SENSEI_RAW_MLG);
    if(dev_handle == NULL)
    {
        printf("cannot open Device.(device connected?/you are root?)\n");
        return 1;
    }
    if(libusb_kernel_driver_active(dev_handle, 1) == 1)
    {
        if(libusb_detach_kernel_driver(dev_handle, 1) == 0)
        {
            libusb_detach_kernel_driver(dev_handle, 1);
        }
    }
    libusb_free_device_list(devs, 1);
    result=0;
    result =libusb_claim_interface(dev_handle, 1);

    if(result < 0)
    {
        printf("%s Cannot Claim Interface\n",libusb_error_name (result));
        return 1;
    }
    i=0;
    while(i<=1025)
    {
        data[i]=0x00;
        i++;
    }



    libusb_control_transfer(dev_handle,LIBUSB_ENDPOINT_OUT | 0x21 | 0x01, 0x09,0x0303,0x00001,profile_1,sizeof(profile_1),0);
    libusb_control_transfer(dev_handle,LIBUSB_ENDPOINT_IN | 0x21 | 0x01, 0x01,0x0305,0x00001,data,sizeof(data),0);
    printf("    profile[1-5]: "), printf("%c\n",data[10]);
    printf("    pooling[0-x]: "), printf("%i\n",data[20]);
    printf(" wheelled[00-ff]: "), printf("R:%x G:%x B:%x I:%x\n",data[24],data[25],data[26],data[27]);
    printf("  Logoled[00-ff]: "),  printf("R:%x G:%x B:%x I:%x\n",data[28],data[29],data[30],data[31]);//wip correct values needed
    printf("  contrast[0-10]: "), printf("%i\n",data[32]);
    printf(" brigtness[0-10]: "), printf("%i\n",data[33]);
    printf("  exactsense1xxx: "), printf("%i %i\n",data[53],data[54]);
    printf("  exactsense2xxx: "), printf("%i %i\n",data[64],data[65]);
    printf("  cpiled1[00-ff]: "), printf("R:%x G:%x B:%x I:%x\n",data[57],data[58],data[59],data[60]);
    printf("  cpiled2[00-ff]: "), printf("R:%x G:%x B:%x I:%x\n",data[66],data[67],data[68],data[69]);
    printf(" exactlift[0-10]: "), printf("%i\n",data[71]);
    printf("  freemove[0-10]: "), printf("%i\n",data[73]);
    printf("  exactaim[0-10]: "), printf("%i\n",data[75]);

    //TODO Data I/O
    result=0;

    libusb_release_interface(dev_handle, 1);
    result = libusb_attach_kernel_driver (dev_handle, 1);
    if (result){
            printf("couldn't reattach kernel driver: %s\n",libusb_error_name (result));
            return 1;
            }




    libusb_close(dev_handle);
    libusb_exit(seasson);


    return 0;
}
