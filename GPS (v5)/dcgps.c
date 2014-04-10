/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: dcgps.c
--
-- PROGRAM: dcgps
--
-- FUNCTIONS:
-- bool closeGPS(struct gps_data_t *gps_data_ptr)
-- void errorCheck(int n)
-- 
-- DATE: November 7, 2013
--
-- REVISIONS: November 8, 2013 - Modified the struct gps_dat_t and malloc'd the memory space to allow for portability &
-- added the error check for the device being null.
--
-- DESIGNER: Mat Siwoski & Robin Hsieh
--
-- PROGRAMMER: Mat Siwoski & Robin Hsieh
--
-- NOTES:
-- The function of the program is to pick accept a GPS dongle and to read information from the GPS dongle.
-- The information needed is the time, Azimuth, SNR, PRN, if the satellite is used, the Latitude,
-- and Longitude and if there is enough satellite signals, the Altitude as well. As well, the total of amount of 
-- satellites currently used.
----------------------------------------------------------------------------------------------------------------------*/

#include <gps.h>
#include <stdio.h>
#include <stdlib.h>
#include "gpsdata.h"
#include <ncurses.h>
#include <errno.h>

bool closeGPS(struct gps_data_t *gps_data_ptr);
void errorCheck(int n);
void read(struct gps_data_t *gps_data_ptr);

int main(void)
{
    unsigned int flags = WATCH_ENABLE;
    Source source;
    static struct gps_data_t *gps_data_ptr;
    
    source.server   = LOCAL_HOST;
    source.port     = DEFAULT_PORT;

    gps_data_ptr = malloc(sizeof(struct gps_data_t));
   
    if(gps_open(source.server, source.port, gps_data_ptr)!= 0)
    {
        fprintf(stderr,
              "GPSD is not running: %d, %s\n",
              errno, gps_errstr(errno));
        return -1;
    }
    
    if (source.device == NULL)
    {
        errorCheck(GPS_DEVICE_MISSING);
    }
    
    gps_stream(gps_data_ptr, flags, source.device);
    
    read(gps_data_ptr);

    errorCheck(GPS_EXIT);
    if(!closeGPS(gps_data_ptr)){
        printf("Stream closed\n");
        exit(EXIT_SUCCESS);
    } 
    return 0;
}

/*----------------------------------------------------------------------------------------------------------------
-- FUNCTION: closeGPS
--
-- DATE: October 23, 2013
--
-- REVISIONS: -
--
-- DESIGNER: Robin Hsieh
--
-- PROGRAMMER: Robin Hsieh
--
-- INTERFACE: bool closeGPS(struct gps_data_t *gps_data_ptr)
--             struct gps_data_t *gps_data_ptr - pointer to the struct 
--
-- RETURNS: Returns a boolean value depending on if the program has been closed or not.
--
-- NOTES:
-- This function disables the GPS stream, closes the gps and free the malloc'd memory
-----------------------------------------------------------------------------------------------------------------*/
bool closeGPS(struct gps_data_t *gps_data_ptr)
{    
    if(!gps_stream(gps_data_ptr, WATCH_DISABLE, NULL))
    {
        return false;
    }
    if(!gps_close(gps_data_ptr)){
        return false;
    }
    free(gps_data_ptr);
    return true;
}

