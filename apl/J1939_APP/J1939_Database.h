/*
 * J1939_Database.h
 *  This is only data base file for J1939 PGN's and related SPN's
 *  Created on: 13-Oct-2021
 *      Author: madhuri.abhang
 */

#ifndef APL_J1939APP_J1939_DATABASE_H_
#define APL_J1939APP_J1939_DATABASE_H_

#include "../mw/J1939_DRIVER/J1939DRIVER.h"

#define NO_OF_SPNS_IN_DM        (162U)
#define NO_OF_FMIS              (22U)
#define NO_TEXT_FMI             (31U) // string("")  is No Text FMI

#define STRING_LENGTH           (32U)
#define NUMBER_OF_PGN_SCREENS    (79U)
#define PGN_NAME_MAX_LEN        (30U)
#define SPN_NAME_MAX_LEN        (18U)
#define TOTAL_PCD_ALARMS        (3U)
#define TOTAL_NCD_ALARMS        (2U)


#define NO_OF_SPN_IVECO_ENG_STATUS_65282        (3U)
#define NO_OF_SPN_LFE_PGN_65266                 (2U)
#define NO_OF_SPN_EEC2_PGN_61443                (3U)
#define NO_OF_SPN_AMB_PGN_65269                 (3U)
#define NO_OF_SPN_HOURS_PGN_65253               (1U)
#define NO_OF_SPN_VEP1_PGN_65271                (2U)
#define NO_OF_SPN_DD_PGN_65276                  (2U)
#define NO_OF_SPN_WFI_PGN_65279                 (1U)
#define NO_OF_SPN_LFC_PGN_65257                 (2U)
#define NO_OF_SPN_EEC4_PGN_65214                (2U)
#define NO_OF_SPN_LFI_PGN_65203                 (1U)
#define NO_OF_SPN_IC2_PGN_64976                 (2U)
#define NO_OF_SPN_EOI_PGN_64914                 (3U)
#define NO_OF_SPN_ET2_PGN_65188                 (1U)
#define NO_OF_SPN_EEC3_PGN_65247_1                (3U)
#define NO_OF_SPN_EEC3_PGN_65247_2                (2U)
#define NO_OF_SPN_S2_PGN_65166                  (1U)
#define NO_OF_SPN_EFLP2_PGN_65243               (1U)
#define NO_OF_SPN_SD_PGN_65252                  (3U)
#define NO_OF_SPN_EFG1_PGN_61450                (1U)
#define NO_OF_SPN_DPF1S_PGN_64796               (1U)
#define NO_OF_SPN_ET1_PGN_65262_1               (3U)
#define NO_OF_SPN_ET1_PGN_65262_2               (2U)
#define NO_OF_SPN_AT1IG1_PGN_61454              (2U)
#define NO_OF_SPN_A1DOC_PGN_64800               (1U)
#define NO_OF_SPN_AT1IG2_PGN_64948              (1U)
#define NO_OF_SPN_AT1IMG_PGN_64946              (1U)
#define NO_OF_SPN_AT1OG1_PGN_61455              (2U)
#define NO_OF_SPN_A1SCRDSI1_PGN_61475           (2U)
#define NO_OF_SPN_A1SCRDSI2_PGN_64833           (1U)
#define NO_OF_SPN_A1SCRDSR1_PGN_61476           (1U)
#define NO_OF_SPN_A1SCREGT1_PGN_64830           (2U)
#define NO_OF_SPN_AT2IG1_PGN_61456              (1U)
#define NO_OF_SPN_AT2OG1_PGN_61457              (1U)
#define NO_OF_SPN_A2SCRDSI2_PGN_64827           (1U)
#define NO_OF_SPN_A2SCRDSI1_PGN_61478           (2U)
#define NO_OF_SPN_A2SCRDSR1_PGN_61479           (1U)
#define NO_OF_SPN_A2SCREGT1_PGN_64824           (2U)
#define NO_OF_SPN_AT1OG2_PGN_64947              (1U)
#define NO_OF_SPN_AT1S2_PGN_64697               (1U)
#define NO_OF_SPN_AT1S_PGN_64891                (2U)
#define NO_OF_SPN_EEC1_PGN_61444_1              (4U)
#define NO_OF_SPN_EEC1_PGN_61444_2              (3U)
#define NO_OF_SPN_EFL_P1_PGN_65263_1            (3U)
#define NO_OF_SPN_EFL_P1_PGN_65263_2            (3U)
#define NO_OF_SPN_IC1_PGN_65270_1               (4U)
#define NO_OF_SPN_IC1_PGN_65270_2               (3U)
#define NO_OF_SPN_AT1T1I_PGN_65110_1            (2U)
#define NO_OF_SPN_AT1T1I_PGN_65110_2            (2U)
#define NO_OF_SPN_PROPB5E_PGN_65373_1           (2U)
#define NO_OF_SPN_PROPB5E_PGN_65373_2           (2U)
#define NO_OF_SPN_PROPB5E_PGN_65373_3           (2U)
#define NO_OF_SPN_PROPB5E_PGN_65373_4           (2U)
#define NO_OF_SPN_PROPB5E_PGN_65373_5           (2U)
#define NO_OF_SPN_PROPB5E_PGN_65373_6           (2U)
#define NO_OF_SPN_PROSTOUT_PGN_65364_1          (4U)
#define NO_OF_SPN_PROSTOUT_PGN_65364_2          (4U)
#define NO_OF_SPN_HATZ_PROPB_PHYS_PGN_65280     (2U)
#define NO_OF_SPN_AI_PGN_65237                  (1U)
#define NO_OF_SPN_ETC5_PGN_65219                (1U)
#define NO_OF_SPN_OII_PGN_64554                 (2U)
#define NO_OF_HATZ_CCVS_PGN_65265               (3U)
#define NO_OF_SPN_IT1_PGN_65154                  (1U)
#define NO_OF_SPN_GFP_PGN_65163                  (1U)
#define NO_OF_SPN_IMI1_PGN_65190                 (2U)
#define NO_OF_SPN_FANDRV_PGN_65213               (3U)
#define NO_OF_SPN_DLCC_PGN_64775                  (2U)
#define NO_OF_SPN_GFC_PGN_65199                  (2U)
#define NO_OF_SPN_EPT1                            (4U)
#define NO_OF_SPN_EPT2                            (4U)
#define NO_OF_SPN_EPT3                            (4U)
#define NO_OF_SPN_EPT4                            (4U)
#define NO_OF_SPN_EPT5                            (4U)
#define NO_OF_SPN_ET4                              (3U)
#define NO_OF_SPN_TI4                              (2U)
#define NO_OF_EFLP12                              (1U)
#define TOTAL_NO_OF_SPNS                        (   NO_OF_SPN_IVECO_ENG_STATUS_65282    +   \
                                                    NO_OF_SPN_LFE_PGN_65266             +   \
                                                    NO_OF_SPN_EEC2_PGN_61443            +   \
                                                    NO_OF_SPN_AMB_PGN_65269             +   \
                                                    NO_OF_SPN_HOURS_PGN_65253           +   \
                                                    NO_OF_SPN_VEP1_PGN_65271            +   \
                                                    NO_OF_SPN_DD_PGN_65276              +   \
                                                    NO_OF_SPN_WFI_PGN_65279             +   \
                                                    NO_OF_SPN_LFC_PGN_65257             +   \
                                                    NO_OF_SPN_EEC4_PGN_65214            +   \
                                                    NO_OF_SPN_LFI_PGN_65203             +   \
                                                    NO_OF_SPN_IC2_PGN_64976             +   \
                                                    NO_OF_SPN_EOI_PGN_64914             +   \
                                                    NO_OF_SPN_ET2_PGN_65188             +   \
                                                    NO_OF_SPN_ET1_PGN_65262_1           +   \
                                                    NO_OF_SPN_ET1_PGN_65262_2           +   \
                                                    NO_OF_SPN_AT1IG1_PGN_61454          +   \
                                                    NO_OF_SPN_AT1OG1_PGN_61455          +   \
                                                    NO_OF_SPN_A1SCRDSI1_PGN_61475       +   \
                                                    NO_OF_SPN_A1SCRDSI2_PGN_64833       +   \
                                                    NO_OF_SPN_A1SCRDSR1_PGN_61476       +   \
                                                    NO_OF_SPN_A1SCREGT1_PGN_64830       +   \
                                                    NO_OF_SPN_AT2IG1_PGN_61456          +   \
                                                    NO_OF_SPN_AT2OG1_PGN_61457          +   \
                                                    NO_OF_SPN_A2SCRDSI2_PGN_64827       +   \
                                                    NO_OF_SPN_A2SCRDSI1_PGN_61478       +   \
                                                    NO_OF_SPN_A2SCRDSR1_PGN_61479       +   \
                                                    NO_OF_SPN_A2SCREGT1_PGN_64824       +   \
                                                    NO_OF_SPN_AT1OG2_PGN_64947          +   \
                                                    NO_OF_SPN_AT1S2_PGN_64697           +   \
                                                    NO_OF_SPN_AT1S_PGN_64891            +   \
                                                    NO_OF_SPN_EEC1_PGN_61444_1          +   \
                                                    NO_OF_SPN_EEC1_PGN_61444_2          +   \
                                                    NO_OF_SPN_EFL_P1_PGN_65263_1        +   \
                                                    NO_OF_SPN_EFL_P1_PGN_65263_2        +   \
                                                    NO_OF_SPN_IC1_PGN_65270_1           +   \
                                                    NO_OF_SPN_IC1_PGN_65270_2           +   \
                                                    NO_OF_SPN_AT1T1I_PGN_65110_1        +   \
													NO_OF_SPN_AT1T1I_PGN_65110_2        +   \
                                                    NO_OF_SPN_PROSTOUT_PGN_65364_1      +   \
                                                    NO_OF_SPN_PROSTOUT_PGN_65364_2      +   \
                                                    NO_OF_SPN_HATZ_PROPB_PHYS_PGN_65280 +   \
                                                    NO_OF_SPN_AI_PGN_65237              +   \
                                                    NO_OF_SPN_ETC5_PGN_65219            +   \
                                                    NO_OF_SPN_OII_PGN_64554             +   \
                                                    NO_OF_HATZ_CCVS_PGN_65265               \
                                                )
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[7];
}Tag_PGNGC1_64915;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[9];
}Tag_PGNPROPB80_65408;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[13];
}Tag_PGNPROPB46_65350;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[11];
}Tag_PGNTSC1_0;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[7];
}Tag_PGNPROPB55_65365;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[9];
}Tag_PGNPROPB01_65281;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[6];
}Tag_PGNPROPB16_65302;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[5];
}Tag_PGNPROPB00_65280;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[11];
}Tag_PGNPROPBF7_65527;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[4];
}Tag_PGNPROPB69_65385;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[7];
}Tag_PGNCM1_57344;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[4];
}Tag_PGNPROPB_08_65288;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
}Tag_PGNRQST_59904;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[3];
} Tag_PGN_DPFC1_64892;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[2];
}Tag_PGN_VOLVO_PREHEAT_65351;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[7];
} Tag_PGN_EEC1_61444;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[2];
} Tag_PGN_LFE_65266;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[3];
} Tag_PGN_EEC2_61443;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[6];
} Tag_PGN_EFL_P1_65263    ;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[7];
} Tag_PGN_IC1_65270       ;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[5];
} Tag_PGN_ET1_65262       ;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[3];
} Tag_PGN_AMB_65269       ;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[1];
} Tag_PGN_HOURS_65253     ;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[2];
} Tag_PGN_VEP1_65271      ;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[2];
} Tag_PGN_DD_65276        ;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[1];
} Tag_PGN_WFI_65279       ;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[2];
} Tag_PGN_LFC_65257       ;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[2];
} Tag_PGN_EEC4_65214      ;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[1];
} Tag_PGN_LFI_65203       ;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[2];
} Tag_PGN_IC2_64976;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[3];
} Tag_PGN_EOI_64914;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[1];
} Tag_PGN_ET2_65188;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[4];
} Tag_PGN_AT1T1I_65110;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[2];
}Tag_PGN_EMR_PREHEAT_65284;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[3];
}Tag_PGN_IVECO_ENG_STATUS_65282;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[2];
} Tag_PGN_AT1IG1_61454;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[2];
} Tag_PGN_AT1OG1_61455    ;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[2];
} Tag_PGN_A1SCRDSI1_61475 ;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[1];
} Tag_PGN_A1SCRDSI2_64833 ;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[1];
} Tag_PGN_A1SCRDSR1_61476 ;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[2];
} Tag_PGN_A1SCREGT1_64830 ;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[1];
} Tag_PGN_AT2IG1_61456    ;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[1];
} Tag_PGN_AT2OG1_61457    ;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[1];
} Tag_PGN_A2SCRDSI2_64827 ;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[2];
} Tag_PGN_A2SCRDSI1_61478 ;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[1];
} Tag_PGN_A2SCRDSR1_61479 ;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[2];
} Tag_PGN_A2SCREGT1_64824 ;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[1];
} Tag_PGN_AT1OG2_64947    ;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[1];
} Tag_PGN_AT1S2_64697;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[2];
} Tag_PGN_AT1S_64891;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[7];
}Tag_PGNETC5_65219;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[3];
}Tag_PGNPROPB53_65363;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[7];
}Tag_PGNCCVS_65265;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[5];
}Tag_PGN_EEC3_65247;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[1];
}Tag_PGN_S2_65166;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[1];
}Tag_PGN_EFL_P2_65243;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[3];
}Tag_PGN_SHUTDOWN_65252;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[1];
}Tag_PGN_EFG1_61450;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[1];
}Tag_PGN_DPF1S_64796;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[1];
}Tag_PGN_PROPB32_65330;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[5];
}Tag_PGN_PROPB51_65361;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[1];
}Tag_PGN_PROPB62_65378;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[12];
}Tag_PGN_PROPB5E_65374;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[1];
}Tag_PGN_PROPB57_65367;


typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[1];
}Tag_PGN_A1DOC_64800;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[1];
}Tag_PGN_AT1IG2_64948;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[1];
}Tag_PGN_AT1IMG_64946;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t  PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t  arrSPNinfo[8];
} Tag_PGN_PROSTOUT_65364       ;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
}Tag_PGN_DM11_65235;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[9];
}Tag_PGNPROPB73_65395;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[4];
}Tag_PGNPROPB7E_65406;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[2];
} Tag_PGN_HATZ_PROPB_PHYS_65280       ;
typedef struct __attribute__((packed))
{
   J1939DRIVER::J1939_PGN_DB_t PGNinfo;
   J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[1];
} Tag_PGN_AI_65237       ;
typedef struct __attribute__((packed))
{
   J1939DRIVER::J1939_PGN_DB_t PGNinfo;
   J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[1];
} Tag_PGN_ETC5_65219       ;
typedef struct __attribute__((packed))
{
   J1939DRIVER::J1939_PGN_DB_t PGNinfo;
   J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[2];
} Tag_PGN_OII_64554       ;
typedef struct __attribute__((packed))
{
   J1939DRIVER::J1939_PGN_DB_t PGNinfo;
   J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[3];
} Tag_PGN_CCVS_HATZ_65265       ;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t  arrSPNinfo[1];
} Tag_PGN_IT1_65154       ;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t  arrSPNinfo[1];
} Tag_PGN_GFP_65163       ;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t  arrSPNinfo[2];
} Tag_PGN_IMI1_65190     ;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[3];
} Tag_PGN_FD1_65213    ;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[2];
} Tag_PGN_DLCC1_64775    ;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[2];
} Tag_PGN_GFC_65199       ;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t  arrSPNinfo[7];
}Tag_PGNEBC1_61441;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[7];
}Tag_PGNETC3_65223;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[4];
}Tag_PGN_EPT1_65187;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[4];
}Tag_PGN_EPT2_65186;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[4];
}Tag_PGN_EPT3_65185;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[4];
}Tag_PGN_EPT4_65184;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[4];
}Tag_PGN_EPT5_65183;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[3];
}Tag_PGN_ET4_64870;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[2];
}Tag_PGN_TCI4_65176;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[1];
}Tag_PGN_EFL_P12_64735;
//typedef struct __attribute__((packed))
//{
//    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
//    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[4];
//}Tag_PGN_EGOV_PID_GAINS_65285;
//typedef struct __attribute__((packed))
//{
//    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
//    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[4];
//}Tag_PGN_EGOV_PARAMS_65286;
//typedef struct __attribute__((packed))
//{
//    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
//    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[2];
//}Tag_PGN_EGOV_APPL_MODE_65287;
typedef enum
{
    PGN_GC1 = 0,    //0
    PGN_PROPB_80,
    PGN_PROPB_46,
    PGN_TSC1,
    PGN_PROPB_55,
    PGN_PROPB_01,
    PGN_PROPB_16,
    PGN_PROPB_00,
    PGN_PROPB_F7,
    PGN_PROPB_69,
    PGN_PROPB_73,
    PGN_PROPB_7E,
    PGN_CM1,
    PGN_ETC5,
    PGN_PROPB_53_KBT,
    PGN_CCVS,
    PGN_PROPB_08_ENG_SPEED,
    PGN_ETC3,
    PGN_EBC1,
    PGN_TX_RQST,
    TX_LAST_PGN
}DATABASE_TX_PGN_LIST_t;

typedef enum
{
    RX_PGN_PROPB62_65378 = 0,
    RX_PGN_PROPB57_65367 ,
    RX_PGN_EMR_PREHEAT_65284,
    RX_PGN_IVECO_ENG_STATUS_65282,
    RX_PGN_LFE_65266     ,
    RX_PGN_EEC2_61443    ,
    RX_PGN_AMB_65269     ,
    RX_PGN_HOURS_65253   ,
    RX_PGN_VEP1_65271    ,
    RX_PGN_DD_65276      ,
    RX_PGN_WFI_65279     ,
    RX_PGN_LFC_65257     ,
    RX_PGN_EEC4_65214    ,
    RX_PGN_LFI_65203     ,
    RX_PGN_IC2_64976     ,
    RX_PGN_EOI_64914     ,
    RX_PGN_ET2_65188     ,
    RX_PGN_EEC3_65247    ,
    RX_PGN_S2_65166      ,
    RX_PGN_EFL_P2_65243  ,
    RX_PGN_SHUTDOWN_65252,
    RX_PGN_EFG1_61450    ,
    RX_PGN_DPF1S_64796   ,
    RX_PGN_ET1_65262     ,
    RX_PGN_EEC1_61444    ,
    RX_PGN_EFL_P1_65263  ,
    RX_PGN_IC1_65270     ,
    RX_PGN_AT1IG1_61454  ,
    RX_PGN_A1DOC_64800   ,
    RX_PGN_AT1IG2_64948  ,
    RX_PGN_AT1IMG_64946  ,
    RX_PGN_AT1OG1_61455   ,
    RX_PGN_A1SCRDSI1_61475,
    RX_PGN_A1SCRDSI2_64833,
    RX_PGN_A1SCRDSR1_61476,
    RX_PGN_A1SCREGT1_64830,
    RX_PGN_AT2IG1_61456  ,
    RX_PGN_AT2OG1_61457   ,
    RX_PGN_A2SCRDSI2_64827,
    RX_PGN_A2SCRDSI1_61478,
    RX_PGN_A2SCRDSR1_61479,
    RX_PGN_A2SCREGT1_64824,
    RX_PGN_AT1OG2_64947   ,
    RX_PGN_AT1S2_64697    ,
    RX_PGN_AT1S_64891     ,
    RX_PGN_AT1T1I_65110   ,
    RX_PGN_PROPB_32_KUBOTA_65330,
    RX_PGN_PROPB5E_65374 ,
    RX_PGN_DPFC1_64892   ,
    RX_PGN_PROPB51_65361 ,
    RX_PGN_IVECO_PREHEAT_65281,
    RX_PGN_VOLVO_PREHEAT_65351,
    RX_PGN_PROSTOUT_65364,
    RX_PGN_HATZ_PROPB_PHYS_65280,
    RX_PGN_AI_65237,
    RX_PGN_ETC5_65219,
    RX_PGN_OII_64554,
    RX_PGN_IT1_65154    ,
    RX_PGN_GFP_65163   ,
    RX_PGN_IMI1_65190   ,
    RX_PGN_FD1_65213,
    RX_PGN_DLCC1_64775,
    RX_PGN_GFC_65199     ,
    RX_PGN_EPT1_65187,
    RX_PGN_EPT2_65186,
    RX_PGN_EPT3_65185,
    RX_PGN_EPT4_65184,
    RX_PGN_EPT5_65183,
    RX_PGN_ET4_64870,
    RX_PGN_TCI4_65176,
    RX_PGN_EFL_P12_64735,
//    RX_PGN_EGOV_PID_GAINS_65285,
//    RX_PGN_EGOV_PARAMS_65286,
//    RX_PGN_EGOV_APPL_MODE_65287,
    RX_PGN_HATZ_CCVS_65265,
    RX_PGN_DM11_65235,
    RX_PGN_LAST
}DATABASE_RX_PGN_LIST_t;



typedef struct
{
    // J1939 PGNs
        Tag_PGNGC1_64915           PGNGC1_64915       ;
        Tag_PGNPROPB80_65408       PGNPROPB80_65408   ;
        Tag_PGNPROPB46_65350       PGNPROPB46_65350   ;
        Tag_PGNTSC1_0              PGNTSC1_0          ;
        Tag_PGNPROPB55_65365       PGNPROPB55_65365   ;
        Tag_PGNPROPB01_65281       PGNPROPB01_65281   ;
        Tag_PGNPROPB16_65302       PGNPROPB16_65302   ;
        Tag_PGNPROPB00_65280       PGNPROPB00_65280   ;
        Tag_PGNPROPBF7_65527       PGNPROPBF7_65527   ;
        Tag_PGNPROPB69_65385       PGNPROPB69_65385   ;
        Tag_PGNPROPB73_65395       PGNPROPB73_65395   ;
        Tag_PGNPROPB7E_65406       PGNPROPB7E_65406   ;
        Tag_PGNCM1_57344           PGNCM1_57344       ;
        Tag_PGNPROPB_08_65288      PGNPROPB08_65288   ;
        Tag_PGNETC3_65223          PGNETC3_65223      ;
        Tag_PGNEBC1_61441          PGNEBC1_61441      ;
        Tag_PGNRQST_59904          PGNRQST_59904      ;

        Tag_PGN_EMR_PREHEAT_65284  PGN_EMR_PREHEAT_65284;
        Tag_PGN_IVECO_ENG_STATUS_65282 PGN_IVECO_ENG_STATUS_65282;
        Tag_PGN_LFE_65266          PGN_LFE_65266      ;
        Tag_PGN_EEC2_61443         PGN_EEC2_61443     ;
        Tag_PGN_AMB_65269          PGN_AMB_65269      ;
        Tag_PGN_HOURS_65253        PGN_HOURS_65253    ;
        Tag_PGN_VEP1_65271         PGN_VEP1_65271     ;
        Tag_PGN_DD_65276           PGN_DD_65276       ;
        Tag_PGN_WFI_65279          PGN_WFI_65279      ;
        Tag_PGN_LFC_65257          PGN_LFC_65257      ;
        Tag_PGN_EEC4_65214         PGN_EEC4_65214     ;
        Tag_PGN_LFI_65203          PGN_LFI_65203      ;
        Tag_PGN_IC2_64976          PGN_IC2_64976      ;
        Tag_PGN_EOI_64914          PGN_EOI_64914      ;
        Tag_PGN_ET2_65188          PGN_ET2_65188      ;
        Tag_PGN_ET1_65262          PGN_ET1_65262      ;
        Tag_PGN_EEC1_61444         PGN_EEC1_61444     ;
        Tag_PGN_EFL_P1_65263       PGN_EFL_P1_65263   ;
        Tag_PGN_IC1_65270          PGN_IC1_65270      ;
        Tag_PGN_AT1IG1_61454       PGN_AT1IG1_61454   ;
        Tag_PGN_AT1OG1_61455       PGN_AT1OG1_61455   ;
        Tag_PGN_A1SCRDSI1_61475    PGN_A1SCRDSI1_61475;
        Tag_PGN_A1SCRDSI2_64833    PGN_A1SCRDSI2_64833;
        Tag_PGN_A1SCRDSR1_61476    PGN_A1SCRDSR1_61476;
        Tag_PGN_A1SCREGT1_64830    PGN_A1SCREGT1_64830;
        Tag_PGN_AT2IG1_61456       PGN_AT2IG1_61456   ;
        Tag_PGN_AT2OG1_61457       PGN_AT2OG1_61457   ;
        Tag_PGN_A2SCRDSI2_64827    PGN_A2SCRDSI2_64827;
        Tag_PGN_A2SCRDSI1_61478    PGN_A2SCRDSI1_61478;
        Tag_PGN_A2SCRDSR1_61479    PGN_A2SCRDSR1_61479;
        Tag_PGN_A2SCREGT1_64824    PGN_A2SCREGT1_64824;
        Tag_PGN_AT1OG2_64947       PGN_AT1OG2_64947   ;
        Tag_PGN_AT1S2_64697        PGN_AT1S2_64697    ;
        Tag_PGN_AT1S_64891         PGN_AT1S_64891     ;
        Tag_PGN_AT1T1I_65110       PGN_AT1T1I_65110   ;
        Tag_PGN_DPFC1_64892        PGN_DPFC1_64892    ;
        Tag_PGN_VOLVO_PREHEAT_65351 PGN_VOLVO_PREHEAT_65351;
        Tag_PGNETC5_65219          PGNETC5_65219      ;
        Tag_PGNPROPB53_65363       PGNPROPB53_65363   ;
        Tag_PGNCCVS_65265          PGNCCVS_65265      ;
        Tag_PGN_PROPB62_65378      PGN_PROPB62_65378  ;
        Tag_PGN_PROPB57_65367      PGN_PROPB57_65367  ;
        Tag_PGN_EEC3_65247         PGN_EEC3_65247     ;
        Tag_PGN_S2_65166           PGN_S2_65166       ;
        Tag_PGN_EFL_P2_65243       PGN_EFL_P2_65243   ;
        Tag_PGN_SHUTDOWN_65252     PGN_SHUTDOWN_65252 ;
        Tag_PGN_EFG1_61450         PGN_EFG1_61450     ;
        Tag_PGN_DPF1S_64796        PGN_DPF1S_64796    ;
        Tag_PGN_A1DOC_64800        PGN_A1DOC_64800    ;
        Tag_PGN_AT1IG2_64948       PGN_AT1IG2_64948   ;
        Tag_PGN_AT1IMG_64946       PGN_AT1IMG_64946   ;
        Tag_PGN_PROPB32_65330      PGN_PROPB_32_65330 ;
        Tag_PGN_PROPB5E_65374      PGN_PROPB5E_65374  ;
        Tag_PGN_PROPB51_65361      PGN_PROPB51_65361  ;
        Tag_PGN_PROSTOUT_65364     PGN_PROSTOUT_65364      ;
        Tag_PGN_HATZ_PROPB_PHYS_65280     PGN_HATZ_PROPB_PHYS_65280      ;
        Tag_PGN_AI_65237           PGN_AI_65237     ;
        Tag_PGN_ETC5_65219         PGN_ETC5_65219     ;
        Tag_PGN_OII_64554          PGN_OII_64554     ;
        Tag_PGN_IT1_65154          PGN_IT1_65154      ;
        Tag_PGN_GFP_65163          PGN_GFP_65163      ;
        Tag_PGN_IMI1_65190         PGN_IMI1_65190      ;
        Tag_PGN_FD1_65213          PGN_FD1_65213;
        Tag_PGN_DLCC1_64775        PGN_DLCC1_64775;
        Tag_PGN_GFC_65199          PGN_GFC_65199;
        Tag_PGN_EPT1_65187             PGN_EPT1_65187;
        Tag_PGN_EPT2_65186             PGN_EPT2_65186;
        Tag_PGN_EPT3_65185             PGN_EPT3_65185;
        Tag_PGN_EPT4_65184             PGN_EPT4_65184;
        Tag_PGN_EPT5_65183             PGN_EPT5_65183;
        Tag_PGN_ET4_64870              PGN_ET4_64870;
        Tag_PGN_TCI4_65176             PGN_TCI4_65176;
        Tag_PGN_EFL_P12_64735          PGN_EFL_P12_64735;
//        Tag_PGN_EGOV_PID_GAINS_65285   PGN_EGOV_PID_GAINS_65285;
//        Tag_PGN_EGOV_PARAMS_65286       PGN_EGOV_PARAMS_65286;
//        Tag_PGN_EGOV_APPL_MODE_65287    PGN_EGOV_APPL_MODE_65287;
        Tag_PGN_CCVS_HATZ_65265    PGN_CCVS_HATZ_65265     ;
        Tag_PGN_DM11_65235         PGN_DM11_65235     ;
}PGNS_t;

extern const PGNS_t  gstPGNs;


#define PGN_SUB_HEADING_STRING_LENGTH   (35U)
#define MAX_NUM_OF_PGN_SUB_HEADINGS     (19U)
typedef struct
{
    char SubHeadings[PGN_SUB_HEADING_STRING_LENGTH];
}PGN_SUB_HEADINGS_t;

#define ENG_OPER_STATE_STRING_LENGTH    (20U)
#define MAX_NUM_OF_ENG_OPER_STATES      (16)
typedef struct
{
    char OperatingState[20];
}ENG_OPERATION_STATES_t;

#define FMI_STRING_LENGTH       (30U)
#define MAX_NUM_OF_FMIS         (22U)
typedef struct
{
    char FmiStrings[30];
}DM01_FMI_STRINGS_t;

#define PCD_NCD_ALARMS_STRING_LENGHT    (65U)
#define MAX_NUM_OF_PCD_NCD_ALARMS       (5U)
typedef struct
{
    char PcdNcdAlarms[PCD_NCD_ALARMS_STRING_LENGHT];
}PCD_NCD_ALARMS_DATA_t;

#define NCD_INDUCE_STRAT_STRING_LENGTH  (60U)
#define MAX_NUM_OF_NCD_INDUCE_STRAT     (8U)
typedef struct
{
    char NcdInduceStrategy[NCD_INDUCE_STRAT_STRING_LENGTH];
}NCD_INDUCE_STRATEGTY_t;

typedef enum
{
    SPN_COOL_WAT_TEMP,      // DISP_IVECO_ENGINE_STATUS
    SPN_OIL_TEMP,
    SPN_OIL_PRESSURE,
    SPN_FUEL_RATE,          // DISP_LFE_PGN_65266
    SPN_THROTTLE_POS,
    SPN_POS_ACC,            // DISP_EEC2_PGN_61443
    SPN_LOAD_SPEED,
    SPN_ACT_MAX_TRQ,
    SPN_P_ATMOS,            // DISP_AMB_PGN_65269
    SPN_T_AMBIENT,
    SPN_T_AIR_INL,
    SPN_HOURS,              // DISP_HOURS_PGN_65253
    SPN_BATT_POT,           // DISP_VEP1_PGN_65271
    SPN_BATTERY,
    SPN_P_FI_FUEL,          // DISP_DD_PGN_65276
    SPN_P_FI_OIL,
    SPN_WATER_IN_FUEL,      // DISP_WFI_PGN_65279
    SPN_TRIP_FUEL,          // DISP_LFC_PGN_65257
    SPN_TOTAL_FUEL,
    SPN_NOM_POWER,          // DISP_EEC4_PGN_65214
    SPN_RATED_SPEED,
    SPN_MEAN_T_FUEL,        // DISP_LFI_PGN_65203
    SPN_A_FIL_DIFF2,        // DISP_IC2_PGN_64976
    SPN_IN_T_MAN_ABS,
    SPN_ENG_OPERATING_STATE,    // DISP_EOI_PGN_64914
    SPN_ENG_DERATE_REQ,
    SPN_ENG_EMERG_SD,
    SPN_T_ECU,                  // DISP_ET2_PGN_65188
    SPN_NOM_FRICTION,           // DISP_EEC3_PGN_65247
    SPN_DES_ENG_SPEED,          // DISP_EEC3_PGN_65247
    SPN_DES_SPEED_ASYM_ADJ,      // DISP_EEC3_PGN_65247
    SPN_ESTIMATED_TRQ_LOSS,      // DISP_EEC3_PGN_65247
    SPM_A_E_GAS_M_FR,            // DISP_EEC3_PGN_65247
    SPN_LAST_SERVICE_TIME,      // DISP_S2_PGN_65166
    SPN_INJ_METERING_PRES,      // DISP_EFL_P2_PGN_65243
    SPN_WAIT_TO_START,          // DISP_SHUTDN_PGN_65252
    SPN_EPS_SHUTDOEN_ENG,
    SPN_EPS_APPROCHING_SHUT_DN,
    SPN_ENG_INTAKE_AIR_FR,      // DISP_EGF1_PGN_61450
    SPN_SOOT_MASS,              // DISP_DPF1S_PGN_64796
    SPN_T_CCOLANT,              // DISP_ET1_PGN_65262_1
    SPN_T_FUEL,
    SPN_T_OIL,
    SPN_T_TUR_OIL,
    SPN_T_INT_CO,
    SPN_AT1_INT_T_NOX,          // DISP_AT1IG1_PGN_61454
    SPN_AT1_INT_O2,
    SPN_AT1_DOX_INT_GAS_TEMP,   // DISP_A1DOC_PGN_64800
    SPN_AT1_IG2_DPF_GAS_TEMP,   // DISP_AT1IG2_PGN_64948
    SPN_AT1_IMG_DPF_DIFF_PRESS, // DISP_AT1IMG_PGN_64948
    SPN_AT1_OUTL_NOX,           // DISP_AT1OG1_PGN_61455
    SPN_AT1_OUT_O2,
    SPN_AT1_EXH_FA_DQ,          // DISP_A2SCRDSI1_PGN_64827
    SPN_AT1_EXH_FLU_DAB,
    SPN_AT1_EXH_FLU_DT,         // DISP_A1SCRDSI2_PGN_64833
    SPN_AT1_EXH_FLU_DRQ,        // DISP_A1SCRDSR1_PGN_61476
    SPN_AT1_SCR_IN_G,           // DISP_A1SCREGT1_PGN_64830
    SPN_AT1_SCR_OU_G,
    SPN_AT2_INTT_NOX,           // DISP_AT2IG1_PGN_61456
    SPN_AT2_OUTL_NOX,           // DISP_AT2OG1_PGN_61457
    SPN_AT2_EXH_FLU_DT,         // DISP_A2SCRDSI2_PGN_61478
    SPN_AT2_EXH_FA_DQ,          // DISP_A2SCRDSI1_PGN_64827
    SPN_AT2_EXH_FLU_DAB,
    SPN_AT2_EXH_FLU_DRQ,        // DISP_A2SCRDSR1_PGN_61479
    SPN_AT2_SCR_OU_G,           // DISP_A2SCREGT1_PGN_64824
    SPN_AT2_SCR_IN_G,
    SPN_DPF_OUTL_T,             // DISP_AT1OG2_PGN_64947
    SPN_NEXT_REGEN,             // DISP_AT1S2_PGN_64697
    SPN_DPF_SOOT_LOAD,          // DISP_AT1S_PGN_64891
    SPN_DPF_ASH_LOAD,
    SPN_ENG_TORQUE_MODE,             // DISP_EEC1_PGN_61444_1
    SPN_DDE_TORQUE,
    SPN_AC_TORQUE,
    SPN_SPEED,
    SPN_SA_CONT_DEVICE,         // DISP_EEC1_PGN_61444_2
    SPN_START_MODE,
    SPN_EDE_TORQUE,
    SPN_P_FUEL,                 // DISP_EFL_P1_PGN_65263_1
    SPN_L_OIL,
    SPN_P_OIL,
    SPN_P_CRANKC,               // DISP_EFL_P1_PGN_65263_2
    SPN_P_COOLANT,
    SPN_L_COOLANT,
    SPN_TRAP_INLET,             // DISP_IC1_PGN_65270_1
    SPN_P_BOOST,
    SPN_T_INT_MAN,
    SPN_P_IN_AIR,
    SPN_A_FIL_DIFF1,            // DISP_IC1_PGN_65270_2
    SPN_T_EX_GAS,
    SPN_C_FIL_DIFF,
    SPN_DEF_LEVEL,              // DISP_AT1T1I_PGN_65110
    SPN_AT1_EXH_FLU_TANK,
    SPN_SCR_IND_ACT,
    SPN_SCR_IND_SEV,
    SPN_ACC_TIME_DPF,           // DISP_PROPB5E_PGN_65374_1
    SPN_REMOVAL_OF_DPF,
    SPN_OT_DPF_REMOVAL,         // DISP_PROPB5E_PGN_65374_2
    SPN_DPF_REMOVAL_TIME,
    SPN_LOSS_OF_DPF,            // DISP_PROPB5E_PGN_65374_3
    SPN_NO_OF_LOSS_OF_DPF,
    SPN_OC_FOR_DPF_LOSS,        // DISP_PROPB5E_PGN_65374_4
    SPN_DPF_LOSS_TIME,
    SPN_ACC_TIME_PCD_FAIL,      // DISP_PROPB5E_PGN_65374_5
    SPN_NO_OF_PCD_FAIL,
    SPN_OC_TIME_PCD_FAIL,       // DISP_PROPB5E_PGN_65374_6
    SPN_PCD_FAIL_TIME,
    SPN_REM_SRVC_HR,            //DISP_PROSTOUT_PGN_65364_1
    SPN_ENGINE_RUNNING,
    SPN_SERVICE_DELAY,
    SPN_PRE_GLOW_ACTIVE,
    SPN_P_OIL_MISSING,          //DISP_PROSTOUT_PGN_65364_2
    SPN_ENG_DIAGN_LAMP,
    SPN_ENG_OVER_TEMP,
    SPN_AIR_FILT_SWITCH,
    SPN_PHYS_INJ_QTY,           // DISP_HATZ_PROPB_PHYS_PGN_65280
    SPN_PHYS_TORQ,
    SPN_ALT_1_STAT,             // DISP_AI_PGN_65237
    SPN_TX_NEUTRAL_SW,          // DISP_ETC5_PGN_65219
    SPN_TRQ_DERATE_TIME1,       // DISP_OII_PGN_64554
    SPN_TRQ_DERATE_TIME2,
    SPN_CYLENDER1_ING_TIME,     // DISP_PGN_65154
    SPN_INTAKE_ABS_PRESSURE,    // DISP_PGN_65163
    SPN_TURBO1_PRESS_BOOST,     // DISP_PGN_65190
    SPN_TURBO2_PRESS_BOOST,     // DISP_PGN_65190
    SPN_EST_PERCENT_SPEED,
    SPN_FAN_DRIVE_STATE,
    SPN_FAN_SPEED,
    SPN_AMBER_LAMP_CMD,
    SPN_RED_LAMP_CMD,
    SPN_TRIP_FUEL_GAS,
    SPN_FUEL_GAS,
    SPN_GAS_PORT1,
    SPN_GAS_PORT2,
    SPN_GAS_PORT3,
    SPN_GAS_PORT4,
    SPN_GAS_PORT5,
    SPN_GAS_PORT6,
    SPN_GAS_PORT7,
    SPN_GAS_PORT8,
    SPN_GAS_PORT9,
    SPN_GAS_PORT10,
    SPN_GAS_PORT11,
    SPN_GAS_PORT12,
    SPN_GAS_PORT13,
    SPN_GAS_PORT14,
    SPN_GAS_PORT15,
    SPN_GAS_PORT16,
    SPN_GAS_PORT17,
    SPN_GAS_PORT18,
    SPN_GAS_PORT19,
    SPN_GAS_PORT20,
    SPN_COOL_TEMP_2,
    SPN_PUMP_OUT_TEMP,
    SPN_COOL_TEMP_3,
    SPN_TURB_IN_TEMP_1,
    SPN_TUNB_IN_TMEP_2,
    SPN_FILT_FUEL_PRESS,
    SPN_PARKING_BRAKE_SW,       // DISP_HATZ_CCVS_PGN_65265
    SPN_VEHICLE_SPD,
    SPN_BRAKE_SW,
    SPN_LAST
}DISP_RX_SPN_LIST_t;
#endif /* APL_J1939APP_J1939_DATABASE_H_ */
