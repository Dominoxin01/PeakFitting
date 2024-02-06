#include <stdio.h>
#include <stdlib.h>


/* Structure of chn file 
0  2 Must be -1
2  2 MCA number or Detector number
4  2 Segment number (set to 1 in UMCBI)
6  2 ASCII seconds of start time
8  4 Real Time (increments of 20 ms) (4-byte integer)
12 4 Live Time (increments of 20 ms) (4-byte integer)
16 8 Start date as ASCII DDMMMYY* or binary zeros, if not known. The *
     character should be ignored if it is not a "1". If it is a "1", it 
     indicates the data is after the year 2000.
24 4 Start time as ASCII HHMM or binary zeros, if not known (see Byte 6 above)
28 2 Channel offset of data
30 2 Number of channels (length of data)

/************************************************************/
/* Sample program compatible with Microsoft and Borland C   */
/* to read header and channel data from a .CHN data file.   */
/* This is a modified version of the code for reading in a  */
/* .CHN in C taken from the MAESTRO softwear manual.	    */
/************************************************************/
std::vector<int> DataReadIn_CHN(const char *filename){
	std::vector<int> data;
	int n;
	#define CHN -1
	char 	acq_time[8], /* buffer for time, date */
		month[4]; /* buffer for month string */
	short	f_type;	/* .CHN file type */
	unsigned short chan_offset, /* beginning channel number */
		count, /* loop counter */
		mca_num,/* MCA number */
		num_chans,/* no. of data channels */
		num_writ, /* no. of bytes written out */
		segment, /* segment number */
		year,	/* acquisition year */
		day, /* acquisition day */
		hour, /* acquisition hour */
		minute, /* acquisition minute */
		second; /* acquisition second */
	int livetime, /* 20ms tics of livetime */
		real_time, /* 20ms tics of realtime */
		chan_data; /* stores channel data */
	FILE *f_pointer;
	
	f_pointer = fopen( filename, "rb" );

/********************************************************/
/*			Header Data			*/
/*		Output header info from .CHN file	*/
/********************************************************/

/* Read fileype -1 (.CHN) */

	fread( &f_type, sizeof(f_type), 1, f_pointer );
	if ( f_type != CHN)
	{
		printf("Not a valid file\n");
		exit(1);
	}
	
	fread(&mca_num, sizeof(mca_num), 1, f_pointer); /* MCA # */
	fread(&segment, sizeof(segment), 1, f_pointer); /* seg # */
	fread(acq_time, sizeof(char), 2, f_pointer);	/* start time */
	
	acq_time[2] = 0;
	second = (short)atoi( acq_time );
	
	fread(&real_time, sizeof(real_time), 1, f_pointer); /* 20 ms tics */
	fread(&livetime, sizeof(livetime), 1, f_pointer); /* 20 ms tics */
	fread(acq_time, sizeof(char), 2, f_pointer);	/* start day */
	
	acq_time[2] = 0;
	day = (short)atoi( acq_time );
	
	fread(month, sizeof(char), 2, f_pointer); /* start month */
	 
	month[3] = 0;
	fread(acq_time, sizeof(char), 2, f_pointer); /* start year */
	
	acq_time[2] = 0;
	year = 1900 + (short)atoi(acq_time);
	
	fread(acq_time, sizeof(char), 1, f_pointer); /* century */
	if ( acq_time[0] >= '0' )
	{
		year = year + (acq_time[0] - '0') * 100;
	}
	
	fread(acq_time, sizeof(char), 3, f_pointer);	/* hour */
	
	acq_time[2] = 0;
	hour = (short)atoi(acq_time);
	
	fread(acq_time, sizeof(char), 2, f_pointer); /* minute */
	
	acq_time[2] = 0;
	minute = (short)atoi(acq_time);
	
	fread(&chan_offset, sizeof(chan_offset), 1, f_pointer); /* offset */

	fread(&num_chans, sizeof(num_chans), 1, f_pointer); /* # chans */



	/********************************************************/
	/*			Channel Data			*/
	/*		Output channel data from .CHN file	*/
	/********************************************************/

	printf("CHANNEL DATA:\n");
	
	for (count = 0; count < num_chans; count++ )
	{
		if ( (count % 6 == 0) ) /* 6 channels per line */
		printf("\n%7d", count); /* with channel number */
		
		fread(&chan_data, sizeof(chan_data), 1, f_pointer );
		printf("%11d", chan_data);
		data.push_back (chan_data );
	}
	fcloseall();
	return data;
}


std::vector<int> DataReadIn_SPE(const char *filename){
//This function reads in the data from a .spe file. It recognizes the begining of data by finding a line with a "0 " string and saying that every line until a line containing a "$" contains data.
	std::string line;
	std::ifstream inFile;
	std::vector<int> data;
	bool Data_line = false;
	inFile.open(filename);
	int n;
	//Looping over all lines (line) in a file (inFile):
	while (getline(inFile, line)){
		//loading in a line as a string (iss(line)):
		std::istringstream iss(line);
		//std::cout << iss.str() << std::endl;
		//if the line containes $ charachter stop reading in the data:
		if (Data_line && iss.str().find("$")!= std::string::npos){
			Data_line = false;
		};
		
		if (Data_line == true){
			iss >> n;
			data.push_back (n);
		};
		
		//if the line contains "0 " charachters make sure that the next line is treated as data:
		if ((iss.str().find("0 ")!=std::string::npos)){
			Data_line = true;
		};
	};
	
	//used for checking if output array has the required data:
	//for (int i = 0; i < 1000; i++){
	//std::cout << data[i] << std::endl;
	//};
	return data;
}
