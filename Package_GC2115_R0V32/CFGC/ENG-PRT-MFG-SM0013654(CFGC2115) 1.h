#ifndef	ENG-PRT-MFG-SM0013654(CFGC2115) 1_H
#define	ENG-PRT-MFG-SM0013654(CFGC2115) 1_H

typedef struct
{
	uint32_t u32PCBPID;
	uint16_t u16ProductID;
	uint8_t u8UseLanguage;
	uint8_t u8UseExtendedEventLog;
	uint8_t u8PIN1PasswordChar0;
	uint8_t u8PIN1PasswordChar1;
	uint8_t u8PIN1PasswordChar2;
	uint8_t u8PIN1PasswordChar3;
	uint8_t u8PIN2PasswordChar0;
	uint8_t u8PIN2PasswordChar1;
	uint8_t u8PIN2PasswordChar2;
	uint8_t u8PIN2PasswordChar3;
	uint8_t u8CFGZxxxRevision;
	uint8_t u8CFGCxxxRevision;
	uint8_t u8UseMultipleProfiles;
	uint8_t u8PIN3PasswordChar1;
	uint8_t u8PIN3PasswordChar2;
	uint8_t u8PIN3PasswordChar3;
	uint8_t u8PIN3PasswordChar4;
	uint8_t u8ParameterSetRevision;
	uint8_t u8ProductRevision;
	uint8_t u8PCBPIDRevision;
	uint8_t u8ProductVersion;
	uint8_t u8ReservedParameter1;
	uint8_t u8ReservedParameter2;
	uint8_t u8EngineSrNoChar01;
	uint8_t u8EngineSrNoChar02;
	uint8_t u8EngineSrNoChar03;
	uint8_t u8EngineSrNoChar04;
	uint8_t u8EngineSrNoChar05;
	uint8_t u8EngineSrNoChar06;
	uint8_t u8EngineSrNoChar07;
	uint8_t u8EngineSrNoChar08;
	uint8_t u8EngineSrNoChar09;
	uint8_t u8EngineSrNoChar10;
	uint8_t u8EngineSrNoChar11;
	uint8_t u8EngineSrNoChar12;
	uint8_t u8EngineSrNoChar13;
	uint8_t u8EngineSrNoChar14;
	uint8_t u8EngineSrNoChar15;
	uint8_t u8EngineSrNoChar16;
	uint8_t u8EngineSrNoChar17;
	uint8_t u8EngineSrNoChar18;
	uint8_t u8EngineSrNoChar19;
	uint8_t u8EngineSrNoChar20;
	uint8_t u8EngineSrNoChar21;
	uint8_t u8EngineSrNoChar22;
	uint8_t u8EngineSrNoChar23;
	uint8_t u8EngineSrNoChar24;
	uint8_t u8EngineSrNoChar25;
	uint8_t u8CustomerCode;
	char strSeriesProductID[3];
} CFGC_t;

/*
Copy the following line(s) into some C file:
const CFGC_t * const ptrCfgc = (const CFGC_t *) 0x40100100;
*/
extern const CFGC_t * const ptrCfgc;

#endif
