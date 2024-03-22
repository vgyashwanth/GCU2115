/*
 * J1939Database.h
 *
 *  Created on: Apr 05, 2022
 *      Author: shubham.wader
 */

#ifndef J1939DATABASE_H_
#define J1939DATABASE_H_

#include "../mw/J1939_DRIVER/J1939DRIVER.h"
#include "HMI/CONST_UI/CONST_UI.h"

#define NO_OF_SPNS_IN_DM            (15)
#define PGN_NAME_MAX_LEN            (30)
#define SPN_NAME_MAX_LEN            (18)


#define NO_OF_SPN_AT1T1I_PGN_65110_1            (2U)
#define NO_OF_SPN_AT1T1I_PGN_65110_2            (2U)
#define NO_OF_SPN_FOR_PGN_EEC1_61444       (1)
#define NO_OF_SPN_FOR_PGN_EEC2_61443       (1)
#define NO_OF_SPN_FOR_PGN_EOI_64914        (1)
#define NO_OF_SPN_FOR_PGN_HOURS_65253      (1)
#define NO_OF_SPN_FOR_PGN_LFC1_65257       (2)
#define NO_OF_SPN_FOR_PGN_ET1_65262        (2)
#define NO_OF_SPN_FOR_PGN_EFL_P1_65263     (1)
#define NO_OF_SPN_FOR_PGN_AMB_65269        (3)
#define NO_OF_SPN_FOR_PGN_VEP1_65271       (1)
#define NO_OF_SPN_FOR_PGN_WFI_OI_65279     (2)
#define NO_OF_SPN_FOR_PGN_DEFA_65383       (2)
#define NO_OF_SPN_FOR_PGN_DM11_65235       (1)
#define NO_OF_SPN_FOR_PGN_DM03_65228       (1)
#define NO_OF_SPN_FOR_PGN_IC1_65270        (2)
#define NO_OF_SPN_FOR_PGN_SHUTDN_65252     (1)
#define NO_OF_SPN_FOR_PGN_CSA_64966        (1)
#define NO_OF_SPN_FOR_PGN_LFE1_65266       (3)

#define TOTAL_NO_OF_SPNS     (                                     \
                                NO_OF_SPN_AT1T1I_PGN_65110_1    + \
                                NO_OF_SPN_AT1T1I_PGN_65110_2    + \
                                NO_OF_SPN_FOR_PGN_EEC1_61444    + \
                                NO_OF_SPN_FOR_PGN_EEC2_61443    + \
                                NO_OF_SPN_FOR_PGN_EOI_64914     + \
                                NO_OF_SPN_FOR_PGN_HOURS_65253   + \
                                NO_OF_SPN_FOR_PGN_LFC1_65257    + \
                                NO_OF_SPN_FOR_PGN_ET1_65262     + \
                                NO_OF_SPN_FOR_PGN_EFL_P1_65263  + \
                                NO_OF_SPN_FOR_PGN_AMB_65269     + \
                                NO_OF_SPN_FOR_PGN_VEP1_65271    + \
                                NO_OF_SPN_FOR_PGN_WFI_OI_65279  + \
                                NO_OF_SPN_FOR_PGN_DEFA_65383    + \
                                NO_OF_SPN_FOR_PGN_DM11_65235    + \
                                NO_OF_SPN_FOR_PGN_DM03_65228    + \
                                NO_OF_SPN_FOR_PGN_IC1_65270     + \
                                NO_OF_SPN_FOR_PGN_CSA_64966     + \
                                NO_OF_SPN_FOR_PGN_SHUTDN_65252  + \
                                NO_OF_SPN_FOR_PGN_LFE1_65266      \
                             )


typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[3];
} Tag_PGN_DPFC1_64892;



typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
}Tag_PGNRQST_59904;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[1];
} Tag_PGN_EEC1_61444     ;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[1];
} Tag_PGN_EEC2_61443     ;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[1];
} Tag_PGN_EOI_64914     ;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[4];
} Tag_PGN_AT1T1I_65110;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[1];
} Tag_PGN_HOURS_65253     ;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[2];
} Tag_PGN_LFC1_65257     ;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[2];
} Tag_PGN_ET1_65262     ;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[1];
} Tag_PGN_EFL_P1_65263     ;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[3];
} Tag_PGN_AMB_65269     ;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[1];
} Tag_PGN_VEP1_65271     ;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[2];
} Tag_PGN_WFI_OI_65279     ;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[2];
} Tag_PGN_DEFA_65383     ;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[1];
}Tag_PGN_SHUTDN_65252;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[1];
} Tag_PGN_CSA_64966;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
}Tag_PGN_DM11_65235;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
}Tag_PGN_DM03_65228;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[2];
}Tag_PGN_IC1_65270;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[3];
}Tag_PGN_LFE1_65266;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[4];
} PGN_65014_t ;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[4];
} PGN_65011_t ;


typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[4];
} PGN_65030_t ;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[4];
} PGN_65027_t ;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[2];
} PGN_65026_t ;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[3];
} PGN_65025_t ;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[4];
} PGN_65024_t ;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[2];
} PGN_65023_t ;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[3];
} PGN_65022_t ;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[4];
} PGN_65021_t ;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[2];
} PGN_65020_t ;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[3];
} PGN_65019_t ;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[2];
} PGN_65018_t ;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[2];
} PGN_65029_t ;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[3];
} PGN_65028_t ;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[3];
} PGN_64911_t ;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[17];
} PGN_65280_t ;


typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[4];
} PGN_65008_t ;


typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[32];
} PGN_65289_t ;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[4];
} PGN_65290_t ;
typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[16];
} PGN_65291_t ;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[16];
} PGN_65292_t ;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[16];
} PGN_65293_t ;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[16];
} PGN_65294_t ;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[4];
} PGN_65295_t ;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[2];
} PGN_65296_t ;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[2];
} PGN_65297_t ;

typedef struct __attribute__((packed))
{
    J1939DRIVER::J1939_PGN_DB_t PGNinfo;
    J1939DRIVER::J1939_SPN_DB_t arrSPNinfo[3];
} PGN_65300_t;

typedef struct
{
    Tag_PGN_DPFC1_64892        PGN_DPFC1_64892    ;
    Tag_PGN_AT1T1I_65110       PGN_AT1T1I_65110   ;
    Tag_PGN_EEC1_61444        PGN_EEC1_61444 ;
    Tag_PGN_EEC2_61443        PGN_EEC2_61443 ;
    Tag_PGN_EOI_64914         PGN_EOI_64914 ;
    Tag_PGN_HOURS_65253       PGN_HOURS_65253 ;
    Tag_PGN_LFC1_65257        PGN_LFC1_65257 ;
    Tag_PGN_ET1_65262         PGN_ET1_65262 ;
    Tag_PGN_EFL_P1_65263      PGN_EFL_P1_65263 ;
    Tag_PGN_AMB_65269         PGN_AMB_65269 ;
    Tag_PGN_VEP1_65271        PGN_VEP1_65271 ;
    Tag_PGN_WFI_OI_65279      PGN_WFI_OI_65279 ;
    Tag_PGN_DEFA_65383        PGN_DEFA_65383 ;
    Tag_PGN_SHUTDN_65252      PGN_SHUTDN_65252;
    Tag_PGN_CSA_64966         PGN_CSA_64966;
    Tag_PGN_DM11_65235        PGN_DM11_65235 ;
    Tag_PGN_DM03_65228        PGN_DM03_65228 ;
    Tag_PGNRQST_59904         PGN_RQST_59904 ;
    Tag_PGN_IC1_65270         PGN_IC1_65270 ;
    Tag_PGN_LFE1_65266        PGN_LFE1_65266;

    PGN_65014_t PGNUtilityPhABasicACQty;      //  65014
    PGN_65011_t PGNUtilityPhBBasicACQty;      //  65011
    PGN_65008_t PGNUtilityPhCACBasicQty;      //  65008
    PGN_65030_t PGNGenAvgBasicACQty;          //  65030
    PGN_65027_t PGNGenPhABasicQty;            //  65027
    PGN_65026_t PGNGenPhAACPower;             //  65026
    PGN_65025_t PGNGenPhAACReactPow;          //  65025
    PGN_65024_t PGNGenPhBBasicQty;            //  65024
    PGN_65023_t PGNGenPhBACPow;               //  65023
    PGN_65022_t PGNGenPhBACReactPow;          //  65022
    PGN_65021_t PGNGenPhCBasicQty;            //  65021
    PGN_65020_t PGNGenPhCACPow;               //  65020
    PGN_65019_t PGNGenPhCACReactPow;          //  65019
    PGN_65018_t PGNGenTotalACActiveEnergy;    //  65018
    PGN_65029_t PGNGenTotalACPow;             //  65029
    PGN_65028_t PGNGenTotalACReactPow;        //  65028
    PGN_64911_t PGNGenTotlaACPercentPow;      //  64911
    PGN_65280_t PGNSedemacProprietary;        //  65280
    PGN_65289_t PGNPropStatusCnts;            //  65289
    PGN_65290_t PGNRunHrs;                    //  65290
    PGN_65291_t PGNAlarms1_4;                 //  65291
    PGN_65292_t PGNAlarms5_8;                 //  65292
    PGN_65293_t PGNAlarms9_12;                //  65293
    PGN_65294_t PGNAlarms13_16;               //  65294
    PGN_65295_t PGNPrcntgLdFuelCA;            //  65295
    PGN_65296_t PGNCumltvDGKwhKvah;           //  65296
    PGN_65297_t PGNCumltvDGKvarh;             //  65297
}PGNS_t;

extern const PGNS_t gstPGNs;


typedef enum
{
    RX_PGN_DPFC1_64892   ,
    RX_PGN_AT1T1I_65110   ,
    RX_PGN_EEC1_61444 ,
    RX_PGN_EEC2_61443 ,
    RX_PGN_EOI_64914 ,
    RX_PGN_HOURS_65253 ,
    RX_PGN_LFC1_65257 ,
    RX_PGN_ET1_65262 ,
    RX_PGN_EFL_P1_65263 ,
    RX_PGN_AMB_65269 ,
    RX_PGN_VEP1_65271 ,
    RX_PGN_WFI_OI_65279 ,
    RX_PGN_DEFA_65383 ,
    RX_PGN_SHUTDN_65252,
    RX_PGN_CSA_64966,
    RX_PGN_DM11_65235,
    RX_PGN_DM03_65228,
    RX_PGN_IC1_65270,
    RX_PGN_LFE1_65266,
    RX_PGN_EGR_RUNHRS_65290,
    RX_PGN_LAST
}DATABASE_RX_PGN_LIST_t;

enum
{
    UPAAC = 0,
    UPBAC ,
    UPCAC,
    GAAC,
    GPAAC,
    GPAACP,
    GPAACRP,
    GPBAC,
    GPBACP,
    GPBACRP,
    GPCAC,
    GPCACP,
    GPCACRP,
    GTACE,
    GTACP,
    GTACRP,
    GTACPP,
    SEDEMAC_PROP,
    PROP_STATUS_N_CNTS,
    PROP_RUN_HRS,
    PROP_ALARMS1_4,
    PROP_ALARMS5_8,
    PROP_ALARMS9_12,
    PROP_ALARMS13_16,
    PROP_PERCNTGLD_FUEL_CA,
    PROP_CUML_GEN_KWH_KVAH,
    PROP_CUML_GEN_KVARH,
    PGN_TX_RQST,
    TX_LAST_PGN
};

typedef struct
{
    char OperatingState[16];
}ENG_OPERATION_STATES_t;

enum ID_SPN_STATUS
{
    VALID_DATA = 0,
    ERROR,
    NOT_AVAILABLE
};

#endif /* J1939DATABASE_H_ */
