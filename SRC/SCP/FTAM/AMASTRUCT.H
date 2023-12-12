#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
typedef struct {
		int no;
		char name[200];
		char value[200];
	}Config;

struct BOR_Type {      /*      09036  */
	char         BDW[4];
	char         RDW[4];    /*    4 digit number  */
	char         HID[2];    /* 2 - char code  */
	char         SID[5];    /*  5-digitnuber */
	char         CT[3];     /*  090(sensor),098(Collector),042(Time) */
	char         ROT[3];    /*  008  */
	char         ROID[6];   /*  6-alphanumber char */
	char         DTRS[5];   /*  Date YMMDD  */
	char         TTRS[7];   /*  Time HHmmsss */
	char         CPGN[5];   /*  5-digit generic issue Number */
	char         TofT[3];   /*  008,027,050,051,052,053 */
	char         TSN[3];    /*  3-digit Sequence Number  */
	char         TTN;       /*  1-digit Number  */
	};

struct EOR_Type {     /*   09037    */
	char         RDW[4];    /*    4 digit number  */
	char         HID[2];    /* 2 - char code  */
	char         SID[5];    /*  5-digitnuber */
	char         CT[3];     /*  090(sensor),098(Collector),042(Time) */
	char         ROT[3];    /*  008  */
	char         ROID[6];   /*  6-alphanumber char */
	char         DTRS[5];   /*  Date YMMDD  */
	char         TTRS[7];   /*  Time HHmmsss */
	char         CPGN[5];   /*  5-digit generic issue Number */
	char         TofT[3];   /*  008,027,050,051,052,053 */
	char         TSN[3];    /*  3-digit Sequence Number  */
	char         TTN;       /*  1-digit Number  */
	char         CofR[7];   /*  t15 7-digit Number  */
	char         TCC[7];    /*  t34 7-digit Number  */
	char         CofB[5];   /* t16  5-digit Number  */
	char         R[5];
	};
struct FSHT_Type {     /*   09050    */
	char         BDW[4];
	char         RDW[4];    /*  T19 4 digit number  */
	char         HID[2];    /*  T20 2 - char code  */
	char         SIC[5];    /*  T21 5-digitnuber */
	char         FSN[13];    /*  T18 13-char code  */
	char         CT[3];     /*  T1  090(sensor),098(Collector),042(Time) */
	char         ST[3];     /*  T2  008 5SEE  */
	char         SID[7];    /*  T3  0,1,6    */
	char         ROT[3];    /*  T4  008  */
	char         ROID[6];   /*  T5  6-alphanumber char */
	char         DHR[5];    /*  T6  Date YMMDD  */   
	char         THR[7];    /*  T7  Time HHmmsss  */
	char         GSN[5];    /*  T33 5-digit Number  */
	char         CPGN[5];   /*  T8  5-digit generic issue Number */
	char         TofT[3];   /*  T9  008,027,050,051,052,053 */
	char         HT;        /*  T10 0,2  */
	char         SUN[3];    /*  T11 Sending unit  */
	char         FBSN[6];   /*  T12 6-digit Number  */
	char         DFBF[5];   /*  T6  Date  */
	char         TFBF[7];   /*  T7  Time  */
	char         DDT[5];    /*  T6  Date  */
	char         TDT[7];    /*  T7  Time  */
	};

struct FSTT_Type {
	unsigned char RDW[4];    /*  T19 4 digit number  */
	unsigned char HID[2];    /*  T20 2 - char code  */
	unsigned char SIC[5];    /*  T21 5-digitnuber */
	unsigned char FSN[13];    /*  T18 13-char code  */
	unsigned char CT[3];     /*  T1 090(sensor),098(Collector),042(Time) */
	unsigned char ST[3];     /*  T2  008 5SEE  */
	unsigned char SID[7];    /*  T3  0,1,6    */
	unsigned char ROT[3];    /*  T4  008  */
	unsigned char ROID[6];   /*  T5  6-alphanumber char */
	unsigned char DHR[5];    /*  T6  Date YMMDD  */   
	unsigned char THR[7];    /*  T7  Time HHmmsss  */
	unsigned char CPGN[5];   /*  T8  5-digit generic issue Number */
	unsigned char TofT[3];   /*  T9  008,027,050,051,052,053 */
	unsigned char HT;        /*  T10 0,2  */
	unsigned char LBSN[6];   /*  T12 6-digit Number  */
	unsigned char DLBF[5];   /*  T6  Date  */
	unsigned char TLBF[7];   /*  T7  Time  */
	unsigned char FRC[7];     /*  T15  7-digit Number  */
	unsigned char FBC[5];     /*  T16  5- digit Number  */
	};

struct FH_Type {
	char         RDW[4];    /*  T19 4 digit number  */
	char         HID[2];    /*  T20 2 - char code  */
	char         SIC[5];    /*  T21 5-digitnuber */
	char         FND[13];    /*  T18 13-char code  */
	char         DFD[5];   /*  T6  Date  */
	char         TFD[7];   /*  T7  Time  */
	};


struct FT_Type {
	unsigned char RDW[4];     /*  T19 4 digit number  */
	unsigned char HID[2];     /*  T20 2 - char code  */
	unsigned char SIC[5];    /*  T21 5-digitnuber */
	unsigned char FND[13];    /*  T18 13-char code */
	unsigned char DLRW[5];    /*  T6  Date  */
	unsigned char TLRW[7];    /*  T7  Time  */
	unsigned char FRC[7];     /*  T15  7-digit Number  */
	unsigned char FBC[5];     /*  T16  5- digit Number  */
	};

struct KRMT_Type {
	unsigned char RDW[2];    /* T 1  Record Discriptor Word  */
	unsigned char HID;    /* T 2  Hexa ID  */
	unsigned char SC[3];  /* T 3 Struct Code */
	unsigned char SN[3];  /* T 4 Sequnce Num */
	unsigned char OPN[9]; /* T 5 Originating Phone Num */
	unsigned char TPN[9]; /* T 6 Terminating Phone Num  */
	unsigned char LCD;    /* T 7 LCD Indicator */
	unsigned char CK[6];
	unsigned char CI[6];
	unsigned char CCE;
	unsigned char CT[3];
	unsigned char TM;
	unsigned char Day;
	unsigned char ITG[2];
	unsigned char OTG[2];
	unsigned char ITST[3];
	unsigned char OTST[3];
	unsigned char CPC;	 
	};
struct KRMT1_Type {
	short         T1;
	unsigned char T2;
	char T3[7];
	char T4[7];
	char T5[19];
	char T6[19];
	unsigned char T7;
	char T8[6];
	char T9[8];
	char T10[6];
	char T11[8];
	unsigned char T12;
	char T13[7];
	unsigned char T14;
	unsigned char T15;
	short         T16;
	short         T17;
	char T18[7];
	char T19[7];
	char T20;
	};

struct KT_Type {
	unsigned char DBH[4];
	struct KRMT_Type KM[35];
	};


/*
+---------------------+---------------------+
|           Block ID       (I4)             |
+-------------------------------------------+
|           Block No       (I4)             |
+-------------------------------------------+
|           CDR Counter    (I4)             |
+-------------------------------------------+
*/
struct Block_Head {
	char ID[4];
	unsigned int NO;
	unsigned int CDR;
	};
/*
+---------------------+---------------------+
| Service Code   (C2) | Serial No (C2)      |
+---------------------+---------------------+
| Data Type      (S2) | CDR Size  (S2)      |
+---------------------+---------------------+
|           Block Size     (I4)             |
+-------------------------------------------+
|           File No        (I4)             |
+-------------------------------------------+
|           Block Start No (I4)             | 
|           Block End No   (I4)             |
+-------------------------------------------+
*/
struct Billing_Head {
	unsigned short Service_Code;
	unsigned short Serial_No;
	unsigned short Data_Type;
	unsigned short CDR_Size;
	unsigned int Block_Size;
	unsigned int File_No;
	unsigned int Start_No;
	unsigned int End_No;
};
struct Billing_Head_ {
	char Service_Code[2];
	char Serial_No[2];
	char Data_Type[2];
	char CDR_Size[2];
	unsigned int Block_Size;
	unsigned int File_No;
	unsigned int Start_No;
	unsigned int End_No;
};

#define KRMT_T1(dd) printf("%4d ",dd)
#define KRMT_T2(dd) (dd == 0xaa ? printf("T(%2x) ",dd) : printf("F(%2x) ",dd))
#define KRMT_T3(dd) printf("%6s ",dd)
#define KRMT_T4(dd) printf("%6s ",dd)
#define KRMT_T5(dd) printf("%18s ",dd)
#define KRMT_T6(dd) printf("%18s ",dd)
#define KRMT_T7(dd) printf("%9s ",LDC_T7[dd])
#define KRMT_T8(dd) printf("%5s ",dd)
#define KRMT_T9(dd) printf("%7s ",dd)
#define KRMT_T10(dd) printf("%5s ",dd)
#define KRMT_T11(dd) printf("%7s ",dd)
#define KRMT_T12(dd) printf("%3d ",dd)
#define KRMT_T13(dd) printf("%6s ",dd)
#define KRMT_T14(dd) printf("%9s ",Trouble_Mark[dd])
#define KRMT_T15(dd) printf("%4s ",Day[dd-1])
#define KRMT_T16(dd) printf("%04d ",dd)
#define KRMT_T17(dd) printf("%04d ",dd)
#define KRMT_T18(dd) printf("%6s ",dd)
#define KRMT_T19(dd) printf("%6s ",dd)
#define KRMT_T20(dd) printf("%2d ",dd)

typedef struct {
	int No;
	char Name[50];
	char value[100];
} CFG_TYPE;

#define	BOTTON 		1
#define	TEXT		2
#define SAFE		3
#define WANING		4
#define TITLE		5
#define LABEL_		6
#define ALARM		7
#define BACKGROUND	8

