/*
 * position.c
 *
 *  Created on: May 9, 2014
 *      Author: alex
 */


#include "position.h"

char *deg_to_str(enum deg_str_type type, double f)
{
    static char str[40];
    int dsec, sec, deg, min;
    long frac_deg;
    double fdsec, fsec, fdeg, fmin;

    if (f < 0 || f > 360) {
	(void)strlcpy(str, "nan", sizeof(str));
	return str;
    }

    fmin = modf(f, &fdeg);
    deg = (int)fdeg;
    frac_deg = (long)(fmin * 1000000);

    if (deg_dd == type) {
	/* DD.dddddd */
	(void)snprintf(str, sizeof(str), "%3d.%06ld", deg, frac_deg);
	return str;
    }
    fsec = modf(fmin * 60, &fmin);
    min = (int)fmin;
    sec = (int)(fsec * 10000.0);

    if (deg_ddmm == type) {
	/* DD MM.mmmm */
	(void)snprintf(str, sizeof(str), "%3d %02d.%04d'", deg, min, sec);
	return str;
    }
    /* else DD MM SS.sss */
    fdsec = modf(fsec * 60, &fsec);
    sec = (int)fsec;
    dsec = (int)(fdsec * 1000.0);
    (void)snprintf(str, sizeof(str), "%3d %02d' %02d.%03d\"", deg, min, sec,
		   dsec);

    return str;
}


int get_location(struct gps_data_t *gpsdata){

		if (gps_open("localhost", DEFAULT_GPSD_PORT, gpsdata) != 0){
			printf("cgps: no gpsd running or network error: %d, %s\n");
			return -1;
	    }

		unsigned int flags = WATCH_ENABLE;

		(void)gps_stream(gpsdata,flags,NULL);

		if (!gps_waiting(gpsdata, 5000000)) {
			gpsdata->status = 0;
			return -1;
		} else {

			errno = 0;

		    if (gps_read(gpsdata) == -1) {
		    	gpsdata->status = 0;
		    	return -1;
		    }

		    gpsdata->status = 1;

		}

		(void)gps_close(gpsdata);

		return 0;

}

void print_position(struct gps_data_t *gpsdata){
    printf("%lf\t%lf\t%lf\t%lf\t%lf\n",
                gpsdata->fix.latitude, gpsdata->fix.longitude, gpsdata->fix.altitude,gpsdata->fix.speed,
                (gpsdata->fix.epx>gpsdata->fix.epy)?gpsdata->fix.epx:gpsdata->fix.epy);
}


void print_all_position(int sample, struct gps_data_t *gpsdata){

	struct timeval time;
	struct tm *temp;

	gettimeofday(&time,NULL);

	temp = localtime(&time.tv_sec);

	if(sample == 0)
		printf("#Date\t\tHora\t\tSample\tLongitude\tLatitude\tAltitude\tSpeedm/s\tAccuracy\n");


	printf("%d/%d/%d\t%d:%d:%d\t%d\t",
			temp->tm_mday,temp->tm_mon,temp->tm_year+1900,
			temp->tm_hour,temp->tm_min,temp->tm_sec,
			sample);

	print_position(gpsdata);
}

void fprint_position(FILE *file , struct gps_data_t *gpsdata){
    fprintf(file,"%lf\t%lf\t%lf\t%lf\t%lf\n",
                gpsdata->fix.latitude, gpsdata->fix.longitude, gpsdata->fix.altitude,gpsdata->fix.speed,
                (gpsdata->fix.epx>gpsdata->fix.epy)?gpsdata->fix.epx:gpsdata->fix.epy);
}

void fprint_all_position(int sample, struct gps_data_t *gpsdata,char* namefile) {

	FILE *file;
	struct timeval time;
	struct tm *temp;

	if (sample == 0){
		file = fopen(namefile,"w");
		fprintf(file,"#Date\t\tHora\t\tSample\tLongitude\tLatitude\tAltitude\tSpeedm/s\tAccuracy\n");
	}else
		file = fopen(namefile,"a");

	gettimeofday(&time,NULL);

	temp = localtime(&time.tv_sec);


	fprintf(file,"%d/%d/%d\t%d:%d:%d\t%d\t",
			temp->tm_mday,temp->tm_mon,temp->tm_year+1900,
			temp->tm_hour,temp->tm_min,temp->tm_sec,
			sample);

	fprint_position(file,gpsdata);

	fclose(file);
}
