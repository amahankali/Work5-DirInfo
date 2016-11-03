#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>
#include <sys/types.h>

char dirs[10000];
char regFiles[10000];

int printinfo(DIR* dirstream, char* dirname)
{
	int totalsize = 0;
	struct dirent* currentEntry = readdir(dirstream);
	while(currentEntry)
	{
		if(currentEntry->d_type == DT_DIR)
		{
			strcat(dirs, currentEntry->d_name);
			strcat(dirs, "\n");
		}
		else if(currentEntry->d_type == DT_REG)
		{
			strcat(regFiles, currentEntry->d_name);
			strcat(regFiles, "\n");

			/////create path name/////
			char pathname[50];
			pathname[0] = 0;
			strcat(pathname, dirname);
			strcat(pathname, "/");
			strcat(pathname, currentEntry->d_name);
			printf("%s\n", pathname);
			//////////////////////////

			///////get file size//////
			struct stat* filestat = (struct stat*) malloc(sizeof(struct stat));
			//int ret = stat(currentEntry->d_name, filestat);
			int ret = stat(pathname, filestat);
			if(ret) printf("Error on getting filestats: %s\n", strerror(errno));
			totalsize += filestat->st_size;
			printf("Size of file %s: %lld bytes\n", currentEntry->d_name, filestat->st_size);
			free(filestat);
			//////////////////////////
		}
		currentEntry = readdir(dirstream);
	}

	return totalsize;
}

int main () {

	//char* dirname = ".";
	char* dirname = "./testDir";

	DIR* dirstream = opendir(dirname);
	if(dirstream == NULL) printf("Error in Opening Directory: %s\n", strerror(errno));

	printf("\n======================================================\n");
	printf("Statistics for directory: %s\n", dirname);

	//null-terminate
	dirs[0] = 0;
	regFiles[0] = 0;

	int totalsize = printinfo(dirstream, dirname);
	closedir(dirstream);

	printf("\n");
	printf("Total Regular File Size: %d Bytes\n", totalsize);
	printf("Directories:\n");
	printf("%s\n", dirs);
	printf("Regular Files:\n");
	printf("%s\n", regFiles);
}