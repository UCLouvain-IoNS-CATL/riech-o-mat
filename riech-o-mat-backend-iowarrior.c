#include "iowkit.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdbool.h>


#define version "Riech-O-Mat Linux Backend 1.0"


IOWKIT_HANDLE devHandle,iowHandle;








int WriteSimple (IOWKIT_HANDLE devHandle, DWORD value)
{
	IOWKIT_REPORT rep;
	IOWKIT56_IO_REPORT rep56;

	/* Init report */
	switch (IowKitGetProductId(devHandle))
	{
		/* Write simple value to IOW40*/
		case IOWKIT_PRODUCT_ID_IOW40:
			memset(&rep, 0xff, sizeof(rep));
			rep.ReportID = 0;
			rep.Bytes[3] = (UCHAR) value;
			return IowKitWrite(devHandle, IOW_PIPE_IO_PINS,
					(PCHAR) &rep, IOWKIT40_IO_REPORT_SIZE) == IOWKIT40_IO_REPORT_SIZE;
			/* Write simple value to IOW24*/
		case IOWKIT_PRODUCT_ID_IOW24:
			memset(&rep, 0xff, sizeof(rep));
			rep.ReportID = 0;
			rep.Bytes[0] = (UCHAR) value;
			return IowKitWrite(devHandle, IOW_PIPE_IO_PINS,
					(PCHAR) &rep, IOWKIT24_IO_REPORT_SIZE) == IOWKIT24_IO_REPORT_SIZE;
			/* Write simple value to IOW56*/
		case IOWKIT_PRODUCT_ID_IOW56:
			memset(&rep56, 0xff, sizeof(rep56));
			rep56.ReportID = 0;
			rep56.Bytes[6] = (UCHAR) value;
			return IowKitWrite(devHandle, IOW_PIPE_IO_PINS,
					(PCHAR) &rep56, IOWKIT56_IO_REPORT_SIZE) == IOWKIT56_IO_REPORT_SIZE;
		default:
			return 0;
	}
}





/**
 * return 1 if this is a character that means "turn valve on", 0 otherwise.
 */
int isValveOnChar(char c) {
	switch (c) {
		case '1':
			return 1;
		default:
			return 0;
	}
}

/* Returns true if OK. */
bool
checkValvesPositions (const char *valves_positions)
{
	bool basic_error = false;
	int i;

	if (strlen (valves_positions) != 5)
	{
		basic_error = true;
		goto handle_basic_error;
	}

	for (i = 0; i < 5; i++)
	{
		if (valves_positions[i] != '0' &&
		    valves_positions[i] != '1')
		{
			basic_error = true;
			goto handle_basic_error;
		}
	}

handle_basic_error:
	if (basic_error)
	{
		fprintf (stderr,
			 "\"%s\" is an invalid valves_positions, it must contain five 0's and 1's.\n",
			 valves_positions);
		return false;
	}

	if (strcmp (valves_positions, "10000") == 0)
	{
		fprintf (stderr,
			 "\"10000\" is an invalid valves_positions, because it opens *only* the airflow valve, "
			 "which can destroy the olfactometer because the pressure builds up.\n"
			 "So \"10000\" has not been executed.\n"
			 "See section 2. \"Understanding your Riech-O-Mat\" in the manual for more details.\n");
		return false;
	}

	return true;
}

/**
 * main
 */

int main ( int argc, char *argv[] ){

    if ( argc != 2 ) /* argc should be 2 for correct execution */
    {
        fprintf(stderr,"usage: %s valve_position\ne.g. \"./riech-o-mat 11000\"\n", argv[0] );
	return 1;
    }

    if (!checkValvesPositions (argv[1]))
    {
        return 1;
    }

    /* Open device*/
    devHandle = IowKitOpenDevice();
    if (devHandle == NULL) {
	fprintf(stderr, "Error opening device \"iowarior\". make sure it's connected.\n");
	exit(1);
    }

    /* Convert command line arg to dword*/
    DWORD valvepos = 0xFFFF;
    int bitpos = 0;
    char* cp;
    for (cp = argv[1]; *cp != 0; ++cp, ++bitpos) {
	DWORD bit = (isValveOnChar(*cp) ? 1 : 0);
	valvepos = valvepos ^ (bit << bitpos);
    }

    iowHandle = IowKitGetDeviceHandle(1);
    IowKitSetWriteTimeout(iowHandle, 10);
    WriteSimple(iowHandle,valvepos);
    IowKitCloseDevice(devHandle);

    return 0;
}
