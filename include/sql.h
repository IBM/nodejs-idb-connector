/* begin_generated_IBM_copyright_prolog                              */
/* This is an automatically generated copyright prolog.              */
/* After initializing,  DO NOT MODIFY OR MOVE                        */
/* ----------------------------------------------------------------- */
/*                                                                   */
/* Product(s):                                                       */
/*     5770-SS1                                                      */
/*                                                                   */
/* (C)Copyright IBM Corp.  1987, 2009                                */
/*                                                                   */
/* All rights reserved.                                              */
/* US Government Users Restricted Rights -                           */
/* Use, duplication or disclosure restricted                         */
/* by GSA ADP Schedule Contract with IBM Corp.                       */
/*                                                                   */
/* Licensed Materials-Property of IBM                                */
/*                                                                   */
/*  ---------------------------------------------------------------  */
/*                                                                   */
/* end_generated_IBM_copyright_prolog                                */
/* ----------------------------------------------------------------- */

/**********************************************************************
 *
 * Source File Name = SQL.H
 *
 * Function = Include File defining:
 *              System Constants
 *              National Language Support Information
 *              SQLCA / SQLDA Constants
 *
 * Operating System = IBM i
 *
 * Product Name = IBM DB2 for i
 *
 *  Change Activity:
 *
 *  CFD List:
 *
 *  FLAG REASON       LEVEL DATE   PGMR     CHANGE DESCRIPTION
 *  ---- ------------ ----- ------ -------- ------------------------
 *  $A0= D99237       5D30  030226 ROCH     New Include
 *  $A1= P9A51843     5Q30  040102 ROCH     Misc Fixes
 *  $B1= D93088       5D40  050313 ROCH     Change pack use
 *  $B2= P9B00837     5Q40  050630 ROCH     Fix SQL_MAX_IDENT
 *  $C1= P9B25902     5D50  060509 ROCH     DECFLOAT changes
 *  $D1= D92300.3     7D10  081211 ROCH     XML additions
 *
 *  End CFD List.
 *
 *   Additional notes about the Change Activity
 *
 *  End Change Activity:
 *
 *********************************************************************/
#ifndef SQL_H_SQL
#define SQL_H_SQL

#if (__OS400_TGTVRM__>=510)
#pragma datamodel(P128)
#endif

#ifdef __cplusplus
extern "C" {
#endif


#include <stddef.h>
#include "sqlsystm.h"                  /* System dependent defines  */
#include "sqludf.h"                    /* User Define Function defines  */

/* #ifndef SQLCODE */
/* #include "sqlca.h"                   Required include file           */
/* #endif */

#ifdef __OS400__
#pragma pack(4)
#else
#pragma options align=packed
#endif /* __OS400__ */

/* System Constants                                                           */

#ifndef   SQL_RC_OK
#define SQL_RC_OK              0       /* successful execution                */
#endif


#define SQL_KEYLMAX            32767   /* Maximum key length                  */
#define SQL_MAXRECL            32766   /* Maximum record length               */
#define SQL_MAXTABLES          256     /* Maximum nbr of tables in a SELECT   */
#define SQL_MAXVARS_STMT       4096    /* Maximum nbr of Host Vars per stmt   */
#define SQL_MAXCOLS            8000    /* Internal max nbr of columns in a    */
                                       /* table                               */
#define SQL_MAXCOLS_EXT        8000    /* External max nbr of columns in a    */
                                       /* table                               */
#define SQL_MAXSEL_ITEMS       8000    /* Maximum nbr of items in a SELECT    */
#define SQL_MAXPARMS           90      /* Maximum nbr of parms in a function  */
#define SQL_MAX_STMT_SIZ       2097152 /* Maximum statement size              */

#define SQL_SMALL_LENGTH       2       /* Size of a SMALLINT                  */
#define SQL_MAXSMALLVAL        32767   /* Maximum value of a SMALLINT         */
#define SQL_MINSMALLVAL (-(SQL_MAXSMALLVAL)-1) /* Minimum value of a SMALLINT */
#define SQL_INT_LENGTH         4       /* Size of an INTEGER                  */
#define SQL_MAXINTVAL          2147483647 /* Maximum value of an INTEGER      */
#define SQL_MININTVAL (-(SQL_MAXINTVAL)-1) /* Minimum value of an INTEGER     */

#define SQL_BIGINT_LENGTH      8       /*           Size of a BIGINT          */
#ifndef SQL_NO_NATIVE_BIGINT_SUPPORT
#ifdef SQL_BIGINT_CONST_SUFFIX_L
#define SQL_MAXBIGINTVAL (9223372036854775807L)  /* Maximum value of a BIGINT */
#else
#define SQL_MAXBIGINTVAL (9223372036854775807LL) /* Maximum value of a BIGINT */
#endif
#define SQL_MINBIGINTVAL (-(SQL_MAXBIGINTVAL)-1) /* Minimum value of a BIGINT */
#endif

#define SQL_FLOAT_LENGTH       8       /* Size of a FLOAT                     */
#define SQL_FLOAT4_LENGTH      4       /* Size of a 4-byte FLOAT              */
#define SQL_MAXSFLTPREC        24      /* Maximum prec for small float        */
#define SQL_MINSFLTPREC        1       /* Minimum prec for small float        */
#define SQL_MAXFLOATPREC       53      /* Minimum prec for any float          */
#define SQL_DEFDEC_PRECISION   5       /* Default precision for DECIMAL       */
#define SQL_DEFDEC_SCALE       0       /* Default scale for DECIMAL           */
#define SQL_MAXDECIMAL         63      /* Maximum scale/prec. for DECIMAL     */
#define SQL_DEFCHAR            1       /* Default length for a CHAR           */
#define SQL_DEFWCHAR           1       /* Default length for a graphic        */
#define SQL_MAXCHAR            32765   /* Maximum length of a CHAR            */
#define SQL_MAXVCHAR           32739   /* Maximum length of a VARCHAR         */
#define SQL_MAXVGRAPH          SQL_MAXVCHAR/2 /* Maximum length of a          */
                                       /* VARGRAPHIC                          */
#define SQL_MAXBLOB            2147483647 /* Max. length of a BLOB host var   */
#define SQL_MAXCLOB            2147483647 /* Max. length of a CLOB host var   */
#define SQL_MAXDBCLOB          1073741823 /* Max. length of an DBCLOB host    */
                                       /* var                                 */
#define SQL_LOBLOCATOR_LEN     4       /* Length of a LOB locator host var    */
#define SQL_LOBFILE_LEN        267     /* Length of a LOB file host var       */
#define SQL_VCHAROH            4       /* Overhead for VARCHAR in record      */
#define SQL_LONGMAX            65511   /* Maximum length of a LONG VARCHAR    */
#define SQL_LONGGRMAX          SQL_LONGMAX/2 /* Max. length of a              */
                                       /* LONG VARGRAPHIC                     */
#define SQL_BLOB_MAXLEN        2147483647 /* BLOB maximum length, in bytes    */
#define SQL_CLOB_MAXLEN        2147483647 /* CLOB maximum length, in chars    */
#define SQL_DBCLOB_MAXLEN      1073741823 /* maxlen for dbcs lobs             */
#define SQL_TIME_LENGTH        3       /* Size of a TIME field                */
#define SQL_TIME_STRLEN        8       /* Size of a TIME field output         */
#define SQL_TIME_MINSTRLEN     5       /* Size of a non-USA TIME field        */
                                       /* output without seconds              */
#define SQL_DATE_LENGTH        4       /* Size of a DATE field                */
#define SQL_DATE_STRLEN        10      /* Size of a DATE field output         */
#define SQL_STAMP_LENGTH       10      /* Size of a TIMESTAMP field           */
#define SQL_STAMP_STRLEN       26      /* Size of a TIMESTAMP field output    */
#define SQL_STAMP_MINSTRLEN    19      /* Size of a TIMESTAMP field output    */
                                       /* without microseconds                */
#define SQL_BOOLEAN_LENGTH     1       /* Size of a BOOLEAN field             */
#define SQL_DATALINK_LENGTH    254     /* Size of a DATALINK field            */
#define SQL_IND_LENGTH         2       /* Size of an indicator value          */

#define SQL_MAX_PNAME_LENGTH   128     /* Max size of Stored Proc Name        */
#define SQL_MAX_IDENT          128     /* Maximum length of Identifer         */
#define SQL_LG_IDENT           128     /* Maximum length of Long Identifer    */
#define SQL_SH_IDENT           10      /* Maximum length of Short Identifer   */
#define SQL_MN_IDENT           1       /* Minimum length of Identifiers       */
#define SQL_MAX_VAR_NAME       128     /* Max size of Host Variable Name      */

#define SQL_KILO_VALUE         1024    /* # of bytes in a kilobyte            */
#define SQL_MEGA_VALUE         1048576 /* # of bytes in a megabyte            */
#define SQL_GIGA_VALUE         1073741824 /* # of bytes in a gigabyte         */

#define SQLB_MAX_CONTAIN_NAME_SZ 256   /* size of container name for api's    */
                                       /* (includes 1 byte for C NULL         */
                                       /* terminator)                         */

/* Codepages                                                                  */
#define SQL_CP_367             367     /* Codepage 367 - EUC single byte      */
#define SQL_CP_437             437     /* Codepage 437 - US, Europe           */
#define SQL_CP_737             737     /* Codepage 737 - WIN Greece           */
#define SQL_CP_806             806     /* Codepage 806 - ISCII, India         */
#define SQL_CP_813             813     /* Codepage 813 - AIX Greece           */
#define SQL_CP_819             819     /* Codepage 819 - ISO 8859-1           */
#define SQL_CP_850             850     /* Codepage 850 - International PC     */
#define SQL_CP_855             855     /* Codepage 855 - OS2 Cyrillic         */
#define SQL_CP_852             852     /* Codepage 852 - OS2 Latin2           */
#define SQL_CP_856             856     /* Codepage 856 - Hebrew               */
#define SQL_CP_857             857     /* Codepage 857 - OS2 Turkey           */
#define SQL_CP_860             860     /* Codepage 860 - Portuguese           */
#define SQL_CP_862             862     /* Codepage 862 - OS2 Hebrew           */
#define SQL_CP_863             863     /* Codepage 863 - Canadian-French      */
#define SQL_CP_864             864     /* Codepage 864 - OS2 Arabic           */
#define SQL_CP_865             865     /* Codepage 865 - Norway, Denmark      */
#define SQL_CP_866             866     /* Codepage 866 - Russia               */
#define SQL_CP_869             869     /* Codepage 869 - OS2 Greece           */
#define SQL_CP_874             874     /* Codepage 874 - OS2/AIX Thailand     */
#define SQL_CP_891             891     /* Codepage 891 - Korean               */
#define SQL_CP_897             897     /* Codepage 897 - Japanese             */
#define SQL_CP_903             903     /* Codepage 903 - Chinese              */
#define SQL_CP_904             904     /* Codepage 904 - Taiwan               */
#define SQL_CP_912             912     /* Codepage 912 - AIX Latin2           */
#define SQL_CP_915             915     /* Codepage 915 - AIX Cyrillic         */
#define SQL_CP_916             916     /* Codepage 916 - AIX Hebrew           */
#define SQL_CP_920             920     /* Codepage 920 - AIX Turkey           */
#define SQL_CP_921             921     /* Codepage 921 - Latvia, Lithuania    */
#define SQL_CP_922             922     /* Codepage 922 - Estonia              */
#define SQL_CP_1004            1004    /* Codepage 1004 - MS-WINDOWS          */
#define SQL_CP_1040            1040    /* Codepage 1040 - Extended Korean     */
#define SQL_CP_1041            1041    /* Codepage 1041 - Extended Japanese   */
#define SQL_CP_1042            1042    /* Codepage 1042 - Extended Chinese    */
#define SQL_CP_1043            1043    /* Codepage 1043 - Extended Taiwan     */
#define SQL_CP_1046            1046    /* Codepage 1046 - AIX Arabic          */
#define SQL_CP_1051            1051    /* Codepage 1051 - HP Roman8           */
#define SQL_CP_1088            1088    /* Codepage 1088 - Korea Std           */
#define SQL_CP_1089            1089    /* Codepage 1089 - AIX Arabic          */
#define SQL_CP_1114            1114    /* Codepage 1114 - Big-5 & GBK         */
#define SQL_CP_1115            1115    /* Codepage 1115 - China GB            */
#define SQL_CP_1124            1124    /* Codepage 1124 - AIX Ukraine         */
#define SQL_CP_1125            1125    /* Codepage 1125 - OS/2 Ukraine        */
#define SQL_CP_1126            1126    /* Codepage 1126 - Windows Korean Std  */
#define SQL_CP_1131            1131    /* Codepage 1131 - OS/2 Belarus        */
#define SQL_CP_1163            1163    /* Codepage 1163 - Vietnamese          */
#define SQL_CP_1250            1250    /* Codepage 1250 - Windows Latin-2     */
#define SQL_CP_1251            1251    /* Codepage 1251 - Windows Cyrillic    */
#define SQL_CP_1252            1252    /* Codepage 1252 - Windows Latin-1     */
#define SQL_CP_1253            1253    /* Codepage 1253 - Windows Greek       */
#define SQL_CP_1254            1254    /* Codepage 1254 - Windows Turkish     */
#define SQL_CP_1255            1255    /* Codepage 1255 - Windows Hebrew      */
#define SQL_CP_1256            1256    /* Codepage 1256 - Windows Arabic      */
#define SQL_CP_1257            1257    /* Codepage 1257 - Windows Baltic      */
#define SQL_CP_1258            1258    /* Codepage 1258 - Windows Vietnamese  */
#define SQL_CP_1275            1275    /* Codepage 1275 - Mac Latin-1         */
#define SQL_CP_1280            1280    /* Codepage 1280 - Mac Greek           */
#define SQL_CP_1281            1281    /* Codepage 1281 - Mac Turkish         */
#define SQL_CP_1282            1282    /* Codepage 1282 - Mac Latin-2         */
#define SQL_CP_1283            1283    /* Codepage 1283 - Mac Cyrillic        */
#define SQL_CP_62208           62208   /* CCSID xF300, (CP856, ST4)           */
#define SQL_CP_62209           62209   /* CCSID xF301, (CP862, ST4)           */
#define SQL_CP_62210           62210   /* CCSID xF302, (CP916, ST4)           */
#define SQL_CP_62213           62213   /* CCSID xF305, (CP862, ST5)           */
#define SQL_CP_62220           62220   /* CCSID xF30C, (CP856, ST6)           */
#define SQL_CP_62221           62221   /* CCSID xF30D, (CP862, ST6)           */
#define SQL_CP_62222           62222   /* CCSID xF30E, (CP916, ST6)           */
#define SQL_CP_62223           62223   /* CCSID xF30F, (CP1255, ST6)          */
#define SQL_CP_62225           62225   /* CCSID xF311, (CP864, ST6)           */
#define SQL_CP_62226           62226   /* CCSID xF312, (CP1046, ST6)          */
#define SQL_CP_62227           62227   /* CCSID xF313, (CP1089, ST6)          */
#define SQL_CP_62228           62228   /* CCSID xF314, (CP1256, ST6)          */
#define SQL_CP_62230           62230   /* CCSID xF316, (CP856, ST8)           */
#define SQL_CP_62231           62231   /* CCSID xF317, (CP862, ST8)           */
#define SQL_CP_62232           62232   /* CCSID xF318, (CP916, ST8)           */
#define SQL_CP_62236           62236   /* CCSID xF31C, (CP856, ST10)          */
#define SQL_CP_62238           62238   /* CCSID xF31E, (CP916, ST10)          */
#define SQL_CP_62239           62239   /* CCSID xF31F, (CP1255, ST10)         */
#define SQL_CP_62241           62241   /* CCSID xF321, (CP856, ST11)          */
#define SQL_CP_62242           62242   /* CCSID xF322, (CP862, ST11)          */
#define SQL_CP_62243           62243   /* CCSID xF323, (CP916, ST11)          */
#define SQL_CP_62244           62244   /* CCSID xF324, (CP1255, ST11)         */

/* DBCS Codepages                                                             */
#define SQL_CP_926             926     /* Codepage 926 - Korean               */
#define SQL_CP_951             951     /* Codepage 951 - New Korean           */
#define SQL_CP_301             301     /* Codepage 301 - Japanese             */
#define SQL_CP_928             928     /* Codepage 928 - Chinese              */
#define SQL_CP_927             927     /* Codepage 927 - Taiwan               */
#define SQL_CP_941             941     /* Codepage 941 - Japanese             */
#define SQL_CP_947             947     /* Codepage 947 - Taiwan Big-5         */
#define SQL_CP_971             971     /* Codepage 971 - Korean 5601          */
#define SQL_CP_1351            1351    /* Codepage 1351 - Japanese            */
#define SQL_CP_1362            1362    /* Codepage 1362 - Korean Windows      */
#define SQL_CP_1380            1380    /* Codepage1380 - China GB             */
#define SQL_CP_1382            1382    /* Codepage1382 - Simp Chinese GB      */
#define SQL_CP_1385            1385    /* Codepage1385 - Simp Chinese GBK     */
#define SQL_CP_13488           13488   /* Codepage13488 - UCS-2               */
#define SQL_CP_1200            1200    /* Codepage1200 - UCS-2                */

/* Combined Codepages                                                         */
#define SQL_CP_934             934     /* Codepage 891 + 926 - Korean         */
#define SQL_CP_949             949     /* CP 1088 + 951 - Korean Std          */
#define SQL_CP_932             932     /* Codepage 897 + 301 - Japanese       */
#define SQL_CP_936             936     /* Codepage 903 + 928 - Chinese        */
#define SQL_CP_938             938     /* Codepage 904 + 927 - Taiwan         */
#define SQL_CP_944             944     /* Codepage 1040 + 926 - Ext.Korean    */
#define SQL_CP_942             942     /* Codepage 1041 + 301 - Ext.Japanese  */
#define SQL_CP_943             943     /* Codepage  897 + 941 - Ext.Japanese  */
#define SQL_CP_946             946     /* Codepage 1042 + 928 - Ext.Chinese   */
#define SQL_CP_948             948     /* Codepage 1043 + 927 - Ext.Taiwan    */
#define SQL_CP_950             950     /* Codepage 1114 + 947 - Taiwan Big5   */
#define SQL_CP_954             954     /* Codepage 954 + 13488 - euc Japan    */
#define SQL_CP_964             964     /* Codepage 964 + 13488 - euc Taiwan   */
#define SQL_CP_970             970     /* Codepage  367 + 971 - Korean 5601   */
#define SQL_CP_1363            1363    /* Codepage 1363 - Korean Windows      */
#define SQL_CP_1381            1381    /* Codepage 1115 +1380 - China GB      */
#define SQL_CP_1383            1383    /* Codepage  367 +1382 - Chinese GB    */
#define SQL_CP_1386            1386    /* Codepage  1114 +1385 - Chinese GBK  */
#define SQL_CP_5039            5039    /* Codepage  1041 + 1351 - Japanese    */
#define SQL_CP_1208            1208    /* Codepage  1208 + 1200 - UTF-8       */
/* Datastream Types                                                           */
#define SQL_SBC_PC             0       /* Single byte PC                      */
#define SQL_JPN_PC             1       /* Japanese-PC                         */
#define SQL_CHN_PC             2       /* Chinese-PC                          */
#define SQL_KOR_PC             3       /* Korean-PC                           */
#define SQL_KSC_PC             4       /* New Korean-PC                       */
#define SQL_KR5_PC             5       /* Korean 5601                         */
#define SQL_TWN_PC             6       /* Taiwan Big-5                        */
#define SQL_CGB_PC             7       /* China GB                            */
#define SQL_CGBA_PC            8       /* China GB on AIX                     */
#define SQL_EUCJP_PC           9       /* Japan euc                           */
#define SQL_EUCTW_PC           10      /* Taiwan euc                          */
#define SQL_UCS2_PC            11      /* UCS-2                               */
#define SQL_KSC2_PC            12      /* Korean Windows                      */
#define SQL_CGBK_PC            13      /* China GBK                           */
#define SQL_UTF8_PC            14      /* UTF-8                               */
#define SQL_UNKN_PC            255     /* Unknown                             */


/* SQLCA Constants                                                            */
#ifndef SQL_RC_INVALID_SQLCA
#endif

/* Size of SQLCA                                                              */
#define SQLCA_SIZE             sizeof(struct sqlca)

/* SQL Error message token delimiter                                          */
#define SQL_ERRMC_PRES ((char) 0xFF) /* delimiter for string entry */

/* Offset in SQLERRD - Diagnostic information                                 */
#define SQL_ERRD_RC            0       /* return code                         */
#define SQL_ERRD_REAS          1       /* reason code                         */
#define SQL_ERRD_CNT           2       /* nbr rows inserted/updated/deleted   */
#define SQL_ERRD_OPT_CARD      2       /* optimizer estimate of # rows        */
#define SQL_ERRD_OPTM          3       /* obsolete -- do not use --           */
#define SQL_ERRD_OPT_TOTCOST   3       /* optimzer estimate of total cost     */
#define SQL_ERRD_DCNT          4       /* nbr of cascaded deletes/updates     */
#define SQL_ERRD_LINE          4       /* line number for recompile error     */
#define SQL_ERRD_AUTHTYPE      4       /* authentication type returned for    */
                                       /* CONNECT/ATTACH                      */
#define SQL_ERRD_DIAG          5       /* diagnostics                         */

/* Indexes in SQLWARN - Warning flags                                         */
#define SQL_WARN_ANY           0       /* composite - set if any warnings     */
#define SQL_WARN_TRUNC         1       /* string column truncated             */
#define SQL_WARN_NUL           2       /* null values eliminated              */
#define SQL_WARN_MISM          3       /* nbr of columns/host vars mismatch   */
#define SQL_WARN_ALLR          4       /* no WHERE clause in update/delete    */
#define SQL_WARN_DATE          6       /* date has been truncated             */
#define SQL_WARN_SUB           8       /* character conversion substitution   */
#define SQL_WARN_NUL2          9       /* arithmetic error nulls eliminated   */
#define SQL_WARN_SQLCA         10      /* SQLCA conversion error              */

/* Values for Warning flags in SQLWARN                                        */
#define SQL_WARNING            'W'     /* warning indicator                   */
#define SQL_NULL_TRN           'N'     /* null terminator truncated warning   */
#define SQL_TRN_APP_LEN        'X'     /* truncation warning with             */
                                       /* application context length          */
                                       /* returned in sqlind                  */
#define SQL_NO_WARN            ' '     /* no warning indicator                */

/* SQLDA Constants                                                            */

/* Increment for type with null indicator                                     */
#define SQL_TYP_NULINC         1

/* Variable Types                                                             */
#define SQL_TYP_DATE           384     /* DATE                                */
#define SQL_TYP_NDATE          (SQL_TYP_DATE+SQL_TYP_NULINC)

#define SQL_TYP_TIME           388     /* TIME                                */
#define SQL_TYP_NTIME          (SQL_TYP_TIME+SQL_TYP_NULINC)

#define SQL_TYP_STAMP          392     /* TIMESTAMP                           */
#define SQL_TYP_NSTAMP         (SQL_TYP_STAMP+SQL_TYP_NULINC)

#define SQL_TYP_DATALINK       396     /* DATALINK                            */
#define SQL_TYP_NDATALINK      (SQL_TYP_DATALINK+SQL_TYP_NULINC)

#define SQL_TYP_CGSTR          400     /* C NUL-terminated graphic str        */
#define SQL_TYP_NCGSTR         (SQL_TYP_CGSTR+SQL_TYP_NULINC)

#define SQL_TYP_BLOB           404     /* BLOB - varying length string        */
#define SQL_TYP_NBLOB          (SQL_TYP_BLOB+SQL_TYP_NULINC)

#define SQL_TYP_CLOB           408     /* CLOB - varying length string        */
#define SQL_TYP_NCLOB          (SQL_TYP_CLOB+SQL_TYP_NULINC)

#define SQL_TYP_DBCLOB         412     /* DBCLOB - varying length string      */
#define SQL_TYP_NDBCLOB        (SQL_TYP_DBCLOB+SQL_TYP_NULINC)

#define SQL_TYP_VARCHAR        448     /* VARCHAR(i) - varying length string  */
                                       /* (2 byte length)                     */
#define SQL_TYP_NVARCHAR       (SQL_TYP_VARCHAR+SQL_TYP_NULINC)

#define SQL_TYP_CHAR           452     /* CHAR(i) - fixed length string       */
#define SQL_TYP_NCHAR          (SQL_TYP_CHAR+SQL_TYP_NULINC)

#define SQL_TYP_LONG           456     /* LONG VARCHAR - varying length       */
                                       /* string                              */
#define SQL_TYP_NLONG          (SQL_TYP_LONG+SQL_TYP_NULINC)

#define SQL_TYP_CSTR           460     /* varying length string for C (null   */
                                       /* terminated)                         */
#define SQL_TYP_NCSTR          (SQL_TYP_CSTR+SQL_TYP_NULINC)

#define SQL_TYP_VARGRAPH       464     /* VARGRAPHIC(i) - varying length      */
                                       /* graphic string (2 byte length)      */
#define SQL_TYP_NVARGRAPH      (SQL_TYP_VARGRAPH+SQL_TYP_NULINC)

#define SQL_TYP_GRAPHIC        468     /* GRAPHIC(i) - fixed length graphic   */
                                       /* string                              */
#define SQL_TYP_NGRAPHIC       (SQL_TYP_GRAPHIC+SQL_TYP_NULINC)

#define SQL_TYP_LONGRAPH       472     /* LONG VARGRAPHIC(i) - varying        */
                                       /* length graphic string               */
#define SQL_TYP_NLONGRAPH      (SQL_TYP_LONGRAPH+SQL_TYP_NULINC)

#define SQL_TYP_LSTR           476     /* varying length string for Pascal    */
                                       /* (1-byte length)                     */
#define SQL_TYP_NLSTR          (SQL_TYP_LSTR+SQL_TYP_NULINC)

#define SQL_TYP_FLOAT          480     /* FLOAT - 4 or 8 byte floating point  */
#define SQL_TYP_NFLOAT         (SQL_TYP_FLOAT+SQL_TYP_NULINC)

#define SQL_TYP_DECIMAL        484     /* DECIMAL (m,n)                       */
#define SQL_TYP_NDECIMAL       (SQL_TYP_DECIMAL+SQL_TYP_NULINC)

#define SQL_TYP_ZONED          488     /* Zoned Decimal -> DECIMAL (m,n)      */
#define SQL_TYP_NZONED         (SQL_TYP_ZONED+SQL_TYP_NULINC)

#define SQL_TYP_BIGINT         492     /* BIGINT - 8-byte signed integer      */
#define SQL_TYP_NBIGINT        (SQL_TYP_BIGINT+SQL_TYP_NULINC)

#define SQL_TYP_INTEGER        496     /* INTEGER - 4-byte signed integer     */
#define SQL_TYP_NINTEGER       (SQL_TYP_INTEGER+SQL_TYP_NULINC)

#define SQL_TYP_SMALL          500     /* SMALLINT - 2-byte signed integer    */
#define SQL_TYP_NSMALL         (SQL_TYP_SMALL+SQL_TYP_NULINC)

#define SQL_TYP_NUMERIC        504     /* NUMERIC -> DECIMAL (m,n)            */
#define SQL_TYP_NNUMERIC       (SQL_TYP_NUMERIC+SQL_TYP_NULINC)

#define SQL_TYP_BLOB_FILE_OBSOLETE 804 /* Obsolete Value                      */
#define SQL_TYP_NBLOB_FILE_OBSOLETE (SQL_TYP_BLOB_FILE_OBSOLETE+SQL_TYP_NULINC)

#define SQL_TYP_CLOB_FILE_OBSOLETE 808 /* Obsolete Value                      */
#define SQL_TYP_NCLOB_FILE_OBSOLETE (SQL_TYP_CLOB_FILE_OBSOLETE+SQL_TYP_NULINC)

#define SQL_TYP_DBCLOB_FILE_OBSOLETE 812 /* Obsolete Value                    */
#define SQL_TYP_NDBCLOB_FILE_OBSOLETE \
                               (SQL_TYP_DBCLOB_FILE_OBSOLETE+SQL_TYP_NULINC)
                                       /*                                 @A1C*/
#define SQL_TYP_BLOB_FILE      916     /* BLOB File - Binary Large Object     */
                                       /* File                                */
#define SQL_TYP_NBLOB_FILE     (SQL_TYP_BLOB_FILE+SQL_TYP_NULINC)

#define SQL_TYP_CLOB_FILE      920     /* CLOB File - Char Large Object File  */
#define SQL_TYP_NCLOB_FILE     (SQL_TYP_CLOB_FILE+SQL_TYP_NULINC)

#define SQL_TYP_DBCLOB_FILE    924     /* DBCLOB File - Double Byte Char      */
                                       /* Large Object File                   */
#define SQL_TYP_NDBCLOB_FILE   (SQL_TYP_DBCLOB_FILE+SQL_TYP_NULINC)

#define SQL_TYP_BLOB_LOCATOR   960     /* BLOB locator                        */
#define SQL_TYP_NBLOB_LOCATOR  (SQL_TYP_BLOB_LOCATOR+SQL_TYP_NULINC)

#define SQL_TYP_CLOB_LOCATOR   964     /* CLOB locator                        */
#define SQL_TYP_NCLOB_LOCATOR  (SQL_TYP_CLOB_LOCATOR+SQL_TYP_NULINC)

#define SQL_TYP_DBCLOB_LOCATOR 968     /* DBCLOB locator                      */
#define SQL_TYP_NDBCLOB_LOCATOR (SQL_TYP_DBCLOB_LOCATOR+SQL_TYP_NULINC)

#define SQL_TYP_XML            988    /* XML                              @D1A*/
#define SQL_TYP_NXML           (SQL_TYP_XML+SQL_TYP_NULINC)            /* @D1A*/

#define SQL_TYP_DECFLOAT       996     /* Decimal floating point          @C1A*/
#define SQL_TYP_NDECFLOAT      (SQL_TYP_DECFLOAT+SQL_TYP_NULINC)       /* @C1A*/

#define SQL_TYP_XML_LOCATOR    2452   /* XML locator                      @D1A*/
#define SQL_TYPE_NXML_LOCATOR  (SQL_TYP_XML_LOCATOR+SQL_TYP_NULINC)    /* @D1A*/

#define SQL_LOBTOKEN_LEN       SQL_LOBLOCATOR_LEN
#define SQL_TYP_BLOB_TOKEN     SQL_TYP_BLOB_LOCATOR
#define SQL_TYP_NBLOB_TOKEN    SQL_TYP_NBLOB_LOCATOR
#define SQL_TYP_CLOB_TOKEN     SQL_TYP_CLOB_LOCATOR
#define SQL_TYP_NCLOB_TOKEN    SQL_TYP_NCLOB_LOCATOR
#define SQL_TYP_DBCLOB_TOKEN   SQL_TYP_DBCLOB_LOCATOR
#define SQL_TYP_NDBCLOB_TOKEN  SQL_TYP_NDBCLOB_LOCATOR
#define SQL_NCLOB_MAXLEN       SQL_DBCLOB_MAXLEN
#define SQL_LOBHANDLE_LEN      SQL_LOBTOKEN_LEN
#define SQL_TYP_BLOB_HANDLE    SQL_TYP_BLOB_TOKEN
#define SQL_TYP_NBLOB_HANDLE   SQL_TYP_NBLOB_TOKEN
#define SQL_TYP_CLOB_HANDLE    SQL_TYP_CLOB_TOKEN
#define SQL_TYP_NCLOB_HANDLE   SQL_TYP_NCLOB_TOKEN
#define SQL_TYP_DBCLOB_HANDLE  SQL_TYP_DBCLOB_TOKEN
#define SQL_TYP_NDBCLOB_HANDLE SQL_TYP_NDBCLOB_TOKEN

/* Values for 30th byte of sqlname                                            */
#define SQL_SQLNAME_SYSGEN     ((char) 0xFF) /* sqlname is system generated   */
#define SQL_SQLNAME_NOT_SYSGEN ((char) 0x00) /* sqlname is directly derived   */
                                             /* from a single column or       */
                                             /* specified in the AS clause    */


/* The size of a date/time format buffer                                      */
#define SQL_FMT_LENGTH         3

/* Structures used system wide                                                */

#ifndef SQL_SQLDBCHAR_DEFN
#define SQL_SQLDBCHAR_DEFN
  typedef unsigned short sqldbchar;
#endif

SQL_STRUCTURE sqlchar                  /* General-purpose VARCHAR for         */
                                       /* casting                             */
{
        short                  length;
        _SQLOLDCHAR            data[1];
};

SQL_STRUCTURE sqlgraphic               /* General-purpose VARGRAPHIC for      */
                                       /* casting                             */
{
        short                  length;
#ifdef SQL_WCHART_CONVERT
        wchar_t                data[1];
#else
        sqldbchar              data[1];
#endif
};

SQL_STRUCTURE sqllob                   /* General-purpose LOB for casting     */
{
        sqluint32              length;
        char                   data[1];
};

SQL_STRUCTURE sqldbclob                /* General-purpose DBCLOB for casting  */
{
        sqluint32              length;
#ifdef SQL_WCHART_CONVERT
        wchar_t                data[1];
#else
        sqldbchar              data[1];
#endif
};

SQL_STRUCTURE sqlfile                  /* File reference structure for LOBs   */
{
        sqluint32              name_length;
        sqluint32              data_length;
        sqluint32              file_options;
        char                   name[255];
};

/* Values used for file_options in the sqlfile structure                      */
#define SQL_FILE_READ          2       /* Input file to read from             */
#define SQL_FILE_CREATE        8       /* Output file - new file to be        */
                                       /* created                             */
#define SQL_FILE_OVERWRITE     16      /* Output file - overwrite existing    */
                                       /* file or create a new file if it     */
                                       /* doesn't exist                       */
#define SQL_FILE_APPEND        32      /* Output file - append to an          */
                                       /* existing file or create a new file  */
                                       /* if it doesn't exist                 */


#ifdef __OS400__
#pragma pack(pop)
#else
#pragma options align=reset
#endif /* __OS400__ */

#ifdef __cplusplus
}
#endif

#if (__OS400_TGTVRM__>=510)
#pragma datamodel(pop)
#endif

#endif /* SQL_H_SQL */
