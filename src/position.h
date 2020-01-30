/*
 * position.h
 *
 *  Created on: May 9, 2014
 *      Author: alex
 */

#ifndef POSITION_H_
#define POSITION_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <gps.h>
#include <math.h>
#include <errno.h>

enum deg_str_type { deg_dd, deg_ddmm, deg_ddmmss };

int get_location(struct gps_data_t *gpsdata);

void print_position(struct gps_data_t *gpsdata);

void print_all_position(int sample, struct gps_data_t *gpsdata);

void fprint_position(FILE *file, struct gps_data_t *gpsdata);

void fprint_all_position(int sample, struct gps_data_t *gpsdata,char* namefile);

#endif /* POSITION_H_ */
