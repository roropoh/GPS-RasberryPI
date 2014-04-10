/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: gpsprint.c
--
-- PROGRAM: dcgps
--
-- FUNCTIONS:
-- void updateGPS(struct gps_data_t *gps_data_ptr)
--
-- DATE: November 7, 2013
--
-- REVISIONS: 
--
-- DESIGNER: Mat Siwoski & Robin Hsieh
--
-- PROGRAMMER: Mat Siwoski & Robin Hsieh
--
-- NOTES:
--    This function will print out the gps_data_ptr struct to ncurses 
--    The information required in the print will be:
--                            -Satellites Visible: The number of satellites visible. 
--                            -PRN: The ID of the satellite.
--                            -SNR: The signal to noise ratio.
--                            -Elev: The elevation of the satellites.
--                            -Azimuth: The azimuth of the Satellite in correlation to the GPS
--                            -Satellites Used: Whether the visible satellite is used or not. 
--                            -Latitude: The latitude of the GPS 
--                            -Longitude: The longitude of the GPS.
--                            -Altitude: The altitude of the GPS
--                            -Timestamp: Current date and time.
--
----------------------------------------------------------------------------------------------------------------------*/
#include "gpsdata.h"
#include <gps.h>
#include <errno.h>
#include <math.h>
#include "gpsdclient.h"
#include <ncurses.h>
#include <time.h>

bool usedflags[MAXCHANNELS];

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: updateGPS
--
-- DATE: November 7, 2013
--
-- REVISIONS: 
-- November 8, 2013 - Added the Speed of the GPS in kilometers per hour, and corrected the Altitude to 
-- show in meters. Changed the formatting for the Latitude, Longitude to show whether it is N, S, E or W. 
-- Added the type of fix of the GPS (whether the fix is 2D, 3D or there is no fix)
-- November 10, 2013 - Added elevation to print out and corrected the formatting
-- November 12, 2013 - Changed the display to ncurses (Robin Hsieh)
--
-- DESIGNER: Mat Siwoski
--
-- PROGRAMMER: Robin Hsieh
--
-- INTERFACE: void updateGPS(struct gps_data_t *gps_data_ptr)
--
-- RETURNS: -
--
-- NOTES:
-- This function prints out the information taken from the struct.
------------------------------------------------------------------------------------------------------------------*/
void updateGPS(struct gps_data_t *gps_data_ptr){
    size_t i;
    char timestamp[128] = "";
    static enum deg_str_type deg_type = deg_dd;
    static float altfactor = 1;
    static char *altunits = "m";
    static char *speedunits = "Kph";
    static float speedfactor = MPS_TO_KPH;

    initscr();                              // Start curses mode         
    keypad(stdscr, TRUE);                   //I need that nifty F1     

    WINDOW *win = newwin(20,50,1,1);
    box(win, 0, 0);
    touchwin(win);

    WINDOW *win2 = newwin(20, 50, 1, 51);
    box(win2, 0, 0);
    touchwin(win2);

    int window1_y_pos = 1;
    int window1_x_pos = 2;

    int window2_y_pos = 1;

    mvprintw(0,0,"Mateusz Siwoski A00758640  Robin Hsieh A00657820\tSatellites visible: %d", gps_data_ptr->satellites_visible);

    switch (gps_data_ptr->fix.mode) {
        case MODE_2D:
            mvprintw(21, 1, "2D %sFIX",
                         (gps_data_ptr->status ==
                          STATUS_DGPS_FIX) ? "DIFF " : "");
            break;
        case MODE_3D:
            mvprintw(21,1,"3D %sFIX",
                         (gps_data_ptr->status ==
                          STATUS_DGPS_FIX) ? "DIFF " : "");
            break;
        default:
            mvprintw(21,1,"NO FIX");
            break;
    }

    if (isnan(gps_data_ptr->fix.time) != 0)
        mvwprintw(win, window1_y_pos++, window1_x_pos, "Time: n/a");
    else
    {
        unix_to_iso8601(gps_data_ptr->fix.time, timestamp, sizeof(timestamp));
        mvwprintw(win, window1_y_pos++, window1_x_pos, "Time: %s", timestamp);
    }

            mvwprintw(win2, window2_y_pos, 2, "PRN: \t Elev: \t\t Azim: \t SNR: \t Used:");

    for(i = 0; i < gps_data_ptr->satellites_visible; ++i)
    {
        mvwprintw(win2, ++window2_y_pos, 2, "%d \t %f.0 \t %d \t %d \t %c \t", gps_data_ptr->PRN[i], 
            gps_data_ptr->ss[i], gps_data_ptr->elevation[i],gps_data_ptr->azimuth[i], (gps_data_ptr->used[i])? 'Y':'N');
    }

     if (gps_data_ptr->fix.mode >= MODE_2D && isnan(gps_data_ptr->fix.latitude) == 0) {
        mvwprintw(win, window1_y_pos++, window1_x_pos,"LAT: %s %c",
                      deg_to_str(deg_type, fabs(gps_data_ptr->fix.latitude)),
                     (gps_data_ptr->fix.latitude < 0) ? 'S' : 'N');
        refresh();
    }
    if (gps_data_ptr->fix.mode >= MODE_2D && isnan(gps_data_ptr->fix.longitude) == 0) {
        mvwprintw(win, window1_y_pos++, window1_x_pos,"LON: %s %c",
                      deg_to_str(deg_type, fabs(gps_data_ptr->fix.longitude)),
                      (gps_data_ptr->fix.longitude < 0) ? 'W' : 'E');
     }
     if (gps_data_ptr->fix.mode >= MODE_3D && isnan(gps_data_ptr->fix.altitude) == 0){
        mvwprintw(win, window1_y_pos++, window1_x_pos,"Alt: %.1f %s",
                      gps_data_ptr->fix.altitude * altfactor, altunits);
     }
     if (gps_data_ptr->fix.mode >= MODE_2D && isnan(gps_data_ptr->fix.track) == 0){
        mvwprintw(win, window1_y_pos++, window1_x_pos,"Speed: %.1f %s",
                      gps_data_ptr->fix.speed * speedfactor, speedunits);
     }

    wrefresh(win);
    wrefresh(win2);
    refresh();
}