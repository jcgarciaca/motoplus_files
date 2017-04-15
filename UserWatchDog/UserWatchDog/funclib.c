#include "motoPlus.h"

STATUS WriteIO(UINT32 addr, UINT16 value) {
	MP_IO_DATA sData;
	sData.ulAddr = addr;
	sData.ulValue = value;

	return mpWriteIO(&sData, 1);
}

STATUS mpGetMultiDVar(UINT16 startIdx, long *values, long nbr) {
	MP_VAR_INFO info[64];
	int i;

	nbr = (nbr > 64) ? 64 : nbr;

	for (i = 0; i < 5; i++) {
		info[i].usType = MP_RESTYPE_VAR_D;
		info[i].usIndex = startIdx + i;
	}
	return mpGetVarData(info, values, nbr);
}