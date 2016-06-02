#include <stdio.h>
#include <string.h>

char * reply (char * text)
{
	char *pos;
	char *ret;
	int i = 1;
	pos = strtok(text, "DUUHNAA=");
	while (pos != NULL) {
		if (i == 2) {
			ret = pos;
		}
		pos = strtok (NULL, "DUUHNAA=");
		i++;
	}
	return ret;
}

int main(int argc, char **argv)
{
	char* tmp;
	tmp = reply(argv[1]);
	printf(tmp);
	return 0;
}


