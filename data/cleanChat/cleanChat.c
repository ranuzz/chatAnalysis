#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define HANGOUT 1
#define WHATSAPP1 2
#define WHATSAPP2 3

struct outcsv {
	int year;
	int month;
	int day;
	int hour;
	int min;
	char author[512];
	char message[20480];
	char source[256];
};

int getMonth(char *month)
{
	if (!strcmp(month, "Jan")) return 1;
	if (!strcmp(month, "Feb")) return 2;
	if (!strcmp(month, "Mar")) return 3;
	if (!strcmp(month, "Apr")) return 4;
	if (!strcmp(month, "May")) return 5;
	if (!strcmp(month, "Jun")) return 6;
	if (!strcmp(month, "Jul")) return 7;
	if (!strcmp(month, "Aug")) return 8;
	if (!strcmp(month, "Sep")) return 9;
	if (!strcmp(month, "Oct")) return 10;
	if (!strcmp(month, "Nov")) return 11;
	if (!strcmp(month, "Dec")) return 12;
}

struct outcsv *cleanhangout(struct outcsv *out, char *buff)
{
	/*
	2014-02-18 16:44:22: <1author-1> message
	2014-02-18 16:44:33: <2author-2> message
	*/
	buff[4] = '\0';
	buff[7] = '\0';
	buff[10] = '\0';
	buff[13] = '\0';
	buff[16] = '\0';
	char *year = &buff[0],
	     *month = &buff[5],
	     *day = &buff[8],
	     *hour = &buff[11],
	     *min = &buff[14],
	     *msg = &buff[22]; 

	out->year = (int)strtol(year, NULL, 10);
	out->month = (int)strtol(month, NULL, 10);
	out->day = (int)strtol(day, NULL, 10);
	out->hour = (int)strtol(hour, NULL, 10);
	out->min = (int)strtol(min, NULL, 10);

	if (msg[0] == '1') {
		sprintf(out->author, "%s", "author-1");
	} else {
		sprintf(out->author, "%s", "author-2");
	}

	while (*msg != '>') msg++;
	if (*msg == '>') {
		msg++; msg++;
	}

	int msglen = strlen(msg);
	// to handle windows end line formating ... use -1 for UNIX files
	msg[msglen-2] = '\0';

	sprintf(out->message, "%s", msg);
	sprintf(out->source, "%s", "hangout");

	return out;
}

struct outcsv *cleanwp1(struct outcsv *out, char *buff)
{
	/*
	09:53, 21 Jan - 1author-1: message
	09:57, 24 Jan - 2author-2: message
	*/
	buff[2] = '\0';
	buff[5] = '\0';
	buff[9] = '\0';
	buff[12] = '\0';
	char *month = &buff[10],
	     *day = &buff[7],
	     *hour = &buff[0],
	     *min = &buff[3],
	     *msg = &buff[16]; 

	out->year = 2014;
	out->month = getMonth(month);
	out->day = (int)strtol(day, NULL, 10);
	out->hour = (int)strtol(hour, NULL, 10);
	out->min = (int)strtol(min, NULL, 10);

	if (msg[0] == '1') {
		sprintf(out->author, "%s", "author-1");
	} else {
		sprintf(out->author, "%s", "author-2");
	}

	while (*msg != ':') msg++;
	if (*msg == ':') {
		msg++; msg++;
	}

	int msglen = strlen(msg);
	// to handle windows end line formating ... use -1 for UNIX files
	msg[msglen-2] = '\0';

	sprintf(out->message, "%s", msg);
	sprintf(out->source, "%s", "whatsapp");

	return out;
}

struct outcsv *cleanwp2(struct outcsv *out, char *buff)
{
	/*
	20/05/2015, 1:18 PM - 1author-1: message
	20/05/2015, 1:18 PM - 2author-2: message
	*/
	buff[2] = '\0';
	buff[5] = '\0';
	buff[10] = '\0';
	char *year = &buff[6],
	     *month = &buff[3],
	     *day = &buff[0],
	     *hour = &buff[12],
	     *min = NULL,
	     *msg = NULL; 

	out->year = (int)strtol(year, NULL, 10);
	out->month = (int)strtol(month, NULL, 10);
	out->day = (int)strtol(day, NULL, 10);

	char *tmp = hour;
	while (*tmp != ':') tmp++;
	*tmp = '\0';
	out->hour = (int)strtol(hour, NULL, 10);

	min = tmp++;
	while (*tmp != ' ') tmp++;
	*tmp = '\0';
	out->min = (int)strtol(min, NULL, 10);

	tmp++;
	if (*tmp == 'P') {
		out->hour += 12;
		if (out->hour == 24) out->hour = 0;
	}
	
	while (*tmp != '-') tmp++;
	tmp++;
	tmp++;
	msg = tmp;
	if (msg[0] == '1') {
		sprintf(out->author, "%s", "author-1");
	} else {
		sprintf(out->author, "%s", "author-2");
	}

	while (*msg != ':') msg++;
	if (*msg == ':') {
		msg++; msg++;
	}

	int msglen = strlen(msg);
	// to handle windows end line formating ... use -1 for UNIX files
	msg[msglen-2] = '\0';

	sprintf(out->message, "%s", msg);
	sprintf(out->source, "%s", "whatsapp");

	return out;
}

// cleanChat filename format outfile
int main(int argc, char **argv)
{
	if (argc < 4) {
		printf("not enough arguments\n");
		printf("./a.out filename format outfile\n");
		printf("\tformat\n\t\tHangout - 1\n\t\tWhatsapp 2014 - 2\n\t\tWhatsapp 2015 - 3\n");
		exit(0);	
	}

	char *filename = argv[1];
	char *outfile = argv[3];
	int format = (int)strtol(argv[2], NULL, 10);
	struct outcsv line, *out;
	char buffer[65536];
	FILE *fin, *fout;
	int bflag = 0;
	
	out = &line;

	fin = fopen(filename, "r");
	fout = fopen(outfile, "w");

	// add header
	fprintf(fout, "year,month,day,hour,minute,author,message,source\n");
	while(fgets(buffer, 65536, fin)) {
		bflag = 0;
		switch(format) {
			case HANGOUT:
				out = cleanhangout(out, buffer);
				break;
			case WHATSAPP1:
				out = cleanwp1(out, buffer);
				break;
			case WHATSAPP2:
				if (!isdigit((int)buffer[0])) {
					bflag = 1;
					break;
				}
				out = cleanwp2(out, buffer);
				break;
			default:
				printf("invalid format\n");
				exit(0);
				break;
		}
		if (out && (!bflag)) {
			fprintf(fout, "%d,%d,%d,%d,%d,%s,%s,%s\n", out->year, out->month, out->day,
					out->hour, out->min, out->author,
					out->message, out->source);
		}
	}

	return 0;
}
