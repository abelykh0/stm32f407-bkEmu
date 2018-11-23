#include "bkSnapshot.h"

// First 2 bytes : start address
// Next 2 bytes : length
// Data follows

bool LoadSnapshot(FIL* file)
{
	FRESULT readResult = f_read(file, buffer1, bytesToRead, &bytesRead);
	if (readResult != FR_OK || bytesRead != bytesToRead)
	{
		return false;
	}

	return false;
}

bool SaveSnapshot(FIL* file)
{
	return false;
}
