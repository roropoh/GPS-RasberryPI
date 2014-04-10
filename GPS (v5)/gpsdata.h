#ifndef GPSDATA_H
#define GPSDATA_H

#define LOCAL_HOST "127.0.0.1"
#define DEFAULT_PORT "2947"
#define GPS_TIMEOUT         -1
#define GPS_EXIT            -2
#define GPS_GONE            -3
#define GPS_ERROR            -4
#define GPS_DEVICE_MISSING     -5
#define MAXCHANNELS         72
#define MAX_POSSIBLE_SAT (MAXCHANNELS-2)

typedef struct fixsource_t{
    char *spec;
    char *server;
    char *port;
    char *device;
}Source;

#endif