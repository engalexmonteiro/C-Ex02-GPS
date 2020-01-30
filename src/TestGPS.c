/*
 * testegps.c
 *
 *  Created on: Apr 25, 2014
 *      Author: Alex Monterio
 *      e-mail: alex.monteiro@icomp.ufam.edu.br
 *      #sudo apt-get install libgps-dev
 */


#include <unistd.h>
#include "position.h"

static int print = 0;
static int file = 0;

int main(int argc, char *argv[]){

	int i;
	int option, samples = -1;

	static struct gps_data_t gpsdata;

	while( (option=getopt(argc, argv, "n:fp")) != -1 ){
		switch(option){
			case 'n' : samples = atoi(optarg);
				   	   break;
			case 'f' : file = 1;
						break;
			case 'p' : print = 1;
					    break;
			default: samples = -1;
				 	 break;
		}
	}

	for(i=0;i != samples;i++){

		get_location(&gpsdata);

		if(print)
			print_all_position(i,&gpsdata);

		if(file)
			fprint_all_position(i,&gpsdata,"position.dat");

		sleep(1);


	}

	return 0;
}
