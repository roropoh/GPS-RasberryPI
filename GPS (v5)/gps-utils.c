/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: gps-utils.c
--
-- PROGRAM: dcgps
--
-- FUNCTIONS:
-- void read(struct gps_data_t *gps_data_ptr)
-- void errorCheck(int n)
--
-- DATE: November 7, 2013
--
-- REVISIONS: 
--
-- DESIGNER: Robin Hsieh
--
-- PROGRAMMER: Robin Hsieh
--
-- NOTES:
-- This file will do a loop to read all the gps information and if there is data to be read, will call updateGPS to print
-- the information out.
--
----------------------------------------------------------------------------------------------------------------------*/
#include "gpsdata.h"
#include <ncurses.h>
#include <gps.h>
#include <errno.h>

void read(struct gps_data_t *gps_data_ptr);
void errorCheck(int n);
void updateGPS(struct gps_data_t *gps_data_ptr);

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: read
--
-- DATE: November 7, 2013
--
-- REVISIONS: -
--
-- DESIGNER: Robin Hsieh
--
-- PROGRAMMER: Robin Hsieh
--
-- INTERFACE: void read(struct gps_data_t *gps_data_ptr)
--
-- RETURNS: -
--
-- NOTES:
-- This function will take the pointer to the struct and read all the information from the gps.
-- It first must wait to read a signal from the GPS unit and then if the information is valid, it will
-- call the updateGPS function to print out the information.
------------------------------------------------------------------------------------------------------------------*/
void read(struct gps_data_t *gps_data_ptr){
    for(;;)
    {
        if(gps_waiting(gps_data_ptr, 5000000))
        {
            errno = 0;
            if(gps_read(gps_data_ptr)==-1)
            {
                fprintf(stderr, "agps:socket error4\n");
                errorCheck(errno == 0 ? GPS_GONE:GPS_ERROR);
            }
            else{
                updateGPS(gps_data_ptr);
            }            
        }
        else
        {
            errorCheck(GPS_TIMEOUT);
        }
    }
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: errorCheck
--
-- DATE: November 7, 2013
--
-- REVISIONS: 
-- November 8, 2013 - Added the case GPS_DEVICE_MISSING to deal with if the source.device value is NULL
-- November 9, 2013 - Fixed some logic errors.
--
-- DESIGNER: Robin Hsieh
--
-- PROGRAMMER: Robin Hsieh
--
-- INTERFACE: void errorCheck(int n)
--                int n - A set integer value set in the gpsdata.h
--
-- RETURNS: Returns the input stream 
--
-- NOTES:
-- This function is used for error checking. Depending on the error encountered, a different message
-- will be printed off.
------------------------------------------------------------------------------------------------------------------*/
void errorCheck(int n){

    switch(n){
        case GPS_TIMEOUT:
            mvprintw(22, 1, "GPS: timed out\n");
            break;
        case GPS_EXIT:
            break;
        case GPS_ERROR:
            mvprintw(22, 1, "GPS: read error\n");
            break;
        case GPS_GONE:
            mvprintw(22, 1, "GPS: lost\n");
            break;
        case GPS_DEVICE_MISSING:
            mvprintw(22, 1, "GPS: device missing\n");
            break;
        default:
            mvprintw(22, 1, "GPS: Caught signal %d\n", n);
    }
    refresh();
}