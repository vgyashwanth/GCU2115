#ifndef _AC_IP_H_
#define _AC_IP_H_

#include "stdint.h"


class AC_IP
{
public:

	/** Data structure used to transact the ADC counts of AC parameters.*/
	typedef struct
	{
	    /*Genset instantaneous voltages ADC counts*/
	    uint16_t u16GensetRCnt;
	    uint16_t u16GensetYCnt;
	    uint16_t u16GensetBCnt;
	    uint16_t u16GensetNCnt;
	    /*Mains instantaneous voltages ADC counts*/
	    uint16_t u16MainsRCnt;
	    uint16_t u16MainsYCnt;
	    uint16_t u16MainsBCnt;
	    uint16_t u16MainsNCnt;
	    /*Instantaneous current ADC counts as seen at the i/p terminals*/
	    uint16_t u16RCurrentCnt;
	    uint16_t u16YCurrentCnt;
	    uint16_t u16BCurrentCnt;

	} AC_PARAMS_t;
	
	typedef void (*pAC_PARAM_CB_t)(AC_PARAMS_t *pACParams);

    /**
     * Stub function.
     */
    void RegisterACParamsCB(pAC_PARAM_CB_t pCB);
};

#endif
