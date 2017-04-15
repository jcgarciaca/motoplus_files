#include "motoPlus.h"


STATUS GetBVar(UINT16 index, long *value) {
	MP_VAR_INFO info;

	info.usType = MP_RESTYPE_VAR_B;
	info.usIndex = index;

	return mpGetVarData(&info, value, 1);
}