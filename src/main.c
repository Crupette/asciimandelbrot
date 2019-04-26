#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

float iterations = 100;
int startzoom = 1;
float zoomfactor = 1;

int msdelay = 100;

int termWidth = 80;
int termHeight = 25;

float panx = -0.745;
float pany = 0.1;

float autoincrease = 0;

char *colors[] = {
	"\033[30m.",
	"\033[30m*",
	"\033[31m*",
	"\033[31m*",
	"\033[33m%",
	"\033[33m%",
	"\033[93m%",
	"\033[93m@",
	"\033[91m@",
	"\033[91m@",
	"\033[91m#",
	"\033[97m#",
};

int mandel(float x, float y){
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

void parseargs(int argc, char **argv){
	for(size_t i = 1; i < argc; i++){
		if(argv[i][0] == '-'){
			if(argv[i][1] == 'w'){
				i++;
				if(i >= argc) return;
				termWidth = atol(argv[i]);
			}
			if(argv[i][1] == 'h'){
				i++;
				if(i >= argc) return;
				termHeight = atol(argv[i]);
			}

			if(argv[i][1] == 'd'){
				i++;
				if(i >= argc) return;
				msdelay = atol(argv[i]);
			}
			if(argv[i][1] == 'a'){
				autoincrease = 0.5;
			}	
			if(argv[i][1] == 'i'){
				i++;
				if(i >= argc) return;
				iterations = atol(argv[i]);
			}
			if(argv[i][1] == 'z'){
				i++;
				if(i >= argc) return;
				zoomfactor = atol(argv[i]);
			}
		}
	}
}

int main(int argc, char **argv){
	parseargs(argc, argv);
	printf("\033[H\033[J");

	while(1){
	for(int y = -termHeight / 2; y < termHeight / 2; y++){
		for(int x = -termHeight / 2; x < termWidth / 2; x++){
			float ix = (float)(x) / (float)startzoom;
			float iy = (float)(y) / (float)startzoom;
			
			ix += panx;
			iy += pany;

			int res = mandel(ix, iy);
			printf("%s", colors[res]);
		}
		printf("\n");
	}
	printf("\033[97mZoom: %i, Iterations: %f", startzoom, iterations);
	startzoom += zoomfactor;
	iterations += autoincrease;
	usleep(msdelay);
	printf("\033[%iA\033[%iD", termWidth, termHeight);
	}
}
