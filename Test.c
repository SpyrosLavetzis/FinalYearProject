#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
char buffer[2333999];

int main(void) {
	printf("Hi");

	int size;
	FILE* vfile;
	FILE* test;

	vfile = fopen("C:\\Users\\spyro\\Desktop\\Ant.mp4", "rb");
	test = fopen("C:\\Users\\spyro\\Desktop\\Test.mp4", "wb");
	printf("%d\n", sizeof(buffer));
	size = fread(buffer, 1, 2333999, vfile);
	printf("%d", size);
	fwrite(buffer, 1, 2333999, test);
	fclose(vfile);
	fclose(test);
	printf("done");
	return 0;
}
