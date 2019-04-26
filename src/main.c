#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

float iterations = 100;
int startzoom = 1;
float zoomfactor = 0;

int msdelay = 1;

int termWidth = 80;
int termHeight = 25;

float panx = 0;
float pany = 0;

float autoincrease = 0;
float autozoom = 0;

char *colors[] = {
	"\033[30m.",
	"\033[30m*",
	"\033[33m*",
	"\033[33m*",
	"\033[31m%",
	"\033[31m%",
	"\033[93m%",
	"\033[93m@",
	"\033[91m@",
	"\033[91m@",
	"\033[91m#",
	"\033[97m#",
};

int mandel(double x, double y){
	double realComp = x;
	double imagComp = y;
	for(size_t i = 0; i < iterations; i++){
		double tmpReal = realComp * realComp - imagComp * imagComp + x;
		double tmpImag = 2 * realComp * imagComp + y;

		realComp = tmpReal;
		imagComp = tmpImag;

		if(realComp * imagComp > 5){
			int res = ((float)i / iterations) * 12;
			if(res > 11) res = 11;
			return res;
		}
	}
	return 0;
}

int parseargs(int argc, char **argv){
	int c;
	opterr = 0;
	while((c = getopt(argc, argv, "w:h:x:y:i:z:d:a:A:")) != -1){
		switch(c){
			case 'w':
				termWidth = atoi(optarg);
			break;
			case 'h':
				termHeight = atoi(optarg);
			break;
			case 'x':
				panx = atof(optarg);
			break;
			case 'y':
				pany = atof(optarg);
			break;	
			case 'i':
				iterations = atoi(optarg);
			break;
			case 'z':
				startzoom = atoi(optarg);
			break;
			case 'd':
				msdelay = atof(optarg);
			break;
			case 'a':
				autoincrease = atof(optarg);
			break;
			case 'A':
				autozoom = atof(optarg);
			break;
			case '?':
				if(optopt == 'w' || optopt == 'h' || optopt == 'i' || optopt == 'z'){
					fprintf(stderr, "Option '%c' requires an argument.\n", optopt);
				}else if(isprint(optopt)){
					fprintf(stderr, "Unknown option: '%c'.\n", optopt);
				}else {
					fprintf(stderr, "Unknown charactor for option: '\\x%x'.\n", optopt);
				}
				return 1;
			break;
			default:
				abort();
		}
	}
	return 0;
}

int main(int argc, char **argv){
	if(parseargs(argc, argv)) exit(1);
	printf("\033[H\033[J");

	while(1){
	for(int y = -termHeight / 2; y < termHeight / 2; y++){
		for(int x = -termWidth / 2; x < termWidth / 2; x++){
			double ix = (float)(x) / (float)startzoom;
			double iy = (float)(y) / (float)startzoom;
			
			ix += panx;
			iy += pany;

			int res = mandel(ix, iy);
			printf("%s", colors[res]);
		}
		printf("\n");
	}
	printf("\033[97mZoom: %i, Iterations: %i, Zoom factor %i", startzoom, (int)iterations, (int)zoomfactor);
	startzoom += zoomfactor;
	iterations += autoincrease;
	zoomfactor += autozoom;
	usleep(msdelay);
	printf("\033[H\033[%iD", termHeight);
	}
}
