#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

#define VERSION_MAJOR 1
#define VERSION_MINOR 0
#define VERSION_BUGFIX 1

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

float granularity = 2;
char smoothing = 1;

int helpFlag = 0, versionFlag = 0;

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

static struct option long_options[] = {
	{"help", no_argument, &helpFlag, 1},
	{"version", no_argument, &versionFlag, 1},
	{"width", required_argument, 0, 'w'},
	{"height", required_argument, 0, 'h'},
	{"panx", required_argument, 0, 'x'},
	{"pany", required_argument, 0, 'y'},
	{"iterations", required_argument, 0, 'i'},
	{"zoom", required_argument, 0, 'z'},
	{"delay", required_argument, 0, 'd'},
	{"autoincrement", required_argument, 0, 'a'},
	{"autozoom", required_argument, 0, 'A'},
	{"granularity", required_argument, 0, 'g'},
	{"nosmooth", no_argument, (int*)&smoothing, 1},
	{NULL, 0, NULL, 0}
};

void help(void){
	printf("Usage: mandel [OPTION]\n");
	printf("Prints a mandel brot zoom to the terminal with variable height and optional zooming\n\n");
	printf("Options:\n");
	printf("  -w --width:      Width to render the set.\n");
	printf("  -h --height:     Height to render the set.\n");
	printf("  -x --panx:       X coordinate to zoom into. (float)\n");
	printf("  -y --pany:       Y coordinate to zoom into. (float)\n");
	printf("  -i --iterations: Number of iterations (detail)\n");
	printf("  -z --zoom:       Ammount of zoom to start with.\n");
	printf("  -d --delay:      Number of milliseconds to delay each frame.\n");
	printf("  -a --autoincrement: Ammount of detail to add to the iterations each frame.\n");
	printf("  -A --autozoom:   Ammount of zoom to add to the zoom factor each frame.\n");
	printf("  -g --granularity:   Inverse ammount of smoothing to preform.\n");
	printf("  -s --nosmooth:   Do not smooth the frame.\n");
	printf("     --help:       Shows this imformation.\n");
	printf("     --version:    Shows the version number.\n");
}

int mandel(double x, double y){
	double realComp = x;
	double imagComp = y;
	for(size_t i = 0; i < iterations; i++){
		double tmpReal = realComp * realComp - imagComp * imagComp + x;
		double tmpImag = 2 * realComp * imagComp + y;

		realComp = tmpReal;
		imagComp = tmpImag;

		if(realComp * imagComp > 5){
			return i;
		}
	}
	return 0;
}

int parseargs(int argc, char **argv){
	int c;
	int option_index = 0;
	opterr = 0;
	while((c = getopt_long(argc, argv, "w:h:x:y:i:z:d:a:A:g:s:", long_options, &option_index)) != -1){
		switch(c){
			case '0':

			break;
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
			case 'g':
				granularity = atof(optarg);
			break;
			case 's':
				smoothing = 0;
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
		}
	}
	if(helpFlag){
		help();
		return 1;
	}
	if(versionFlag){
		printf("asciimandelbrot v. %i.%i.%i\n", VERSION_MAJOR, VERSION_MINOR, VERSION_BUGFIX);
		return 1;
	}
	return 0;
}

int main(int argc, char **argv){
	if(parseargs(argc, argv)) exit(1);
	printf("\033[H\033[J");

	while(1){
	for(int y = -termHeight / 2; y < termHeight / 2; y++){
		for(int x = -termWidth / 2; x < termWidth / 2; x++){
			double avgpart = 0;
				
			double ix = (float)(x) / (float)startzoom;
			double iy = (float)(y) / (float)startzoom;
			ix += panx;
			iy += pany;

			if(smoothing){
				for(int y2 = -1; y2 <= 1; y2++){
					for(int x2 = -1; x2 <= 1; x2++){
						double ix2 = (float)(x2) / ((float)startzoom * granularity);
						double iy2 = (float)(y2) / ((float)startzoom * granularity);

						ix2 += ix;
						iy2 += iy;
						
						avgpart += mandel(ix2, iy2);
					}
				}
				avgpart /= (iterations * 9);
				avgpart *= 12;
				if(avgpart > 11) avgpart = 11;
			}else{
				avgpart = ((mandel(ix, iy) / iterations) * 12);
				if(avgpart > 11) avgpart = 11;
			}

			printf("%s", colors[(int)avgpart]);
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
