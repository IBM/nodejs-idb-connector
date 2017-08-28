/* begin_generated_IBM_copyright_prolog                              */
/* This is an automatically generated copyright prolog.              */
/* After initializing,  DO NOT MODIFY OR MOVE                        */
/* ----------------------------------------------------------------- */
/*                                                                   */
/* Product(s):                                                       */
/*     5722-SS1                                                      */
/*                                                                   */
/* (C)Copyright IBM Corp.  1999, 2007                                */
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
/**********************************************************************
 *
 * Source File Name = SQLSYSTM.H
 *
 * Function = Include File defining:
 *              Operating System Specific Information
 *
 * Operating System = OS/400
 *
 * Product Name = DB2 Universal Database for AS/400
 *
 *  Change Activity:
 *
 *  CFD List:
 *
 *  FLAG REASON       LEVEL DATE   PGMR     CHANGE DESCRIPTION
 *  ---- ------------ ----- ------ -------- ------------------------
 *  $A0= D97030       4D40  990124 ROCH     New Include
 *  $A1= D97473       4D50  990930 ROCH     BIGINT support
 *  $A2= D97473       4D50  990930 ROCH     LOB parameters
 *  $A3= D99237       5D30  030226 ROCH     Support LOB APIs
 *  $A4= D99237       5D30  030610 ROCH     Add definition of sqldbchar
 *                                          typedef
 *  $A5= P9B44168     5D50  070206 ROCH     Add definition of sqluint64
 *                                          typedef
 *
 *  End CFD List.
 *
 *   Additional notes about the Change Activity
 *
 *  End Change Activity:
 *
 *********************************************************************/

#ifndef  SQL_H_SQLSYSTM
   #define SQL_H_SQLSYSTM          /* Permit duplicate Includes */


#ifndef DB2400                     /* This is DB2 for AS/400    */
  #define DB2400    1
#endif

/* Datalinks are not supported as parameters on DB2 UDB for AS/400   */
#ifndef NO_DATALINK
  #define NO_DATALINK 1
#endif



/* Operating System Control Parameters */

#ifndef SQL_API_RC
 #define SQL_API_RC      int
 #define SQL_STRUCTURE   struct
 #define PSQL_API_FN     *
 #define SQL_API_FN
 #define SQL_POINTER
 #define SQL_API_INTR
#endif

#ifndef SQL_WCHART_CONVERT
  #define SQL_WCHART_CONVERT
#endif
#include <wcstr.h>

#ifndef NO_BIGINT
/**********************************************************************
**
** Define the 64 bit integer type.
**
**********************************************************************/

#ifndef SQL_BIGINT_TYPE
   #define SQL_BIGINT_TYPE long long
#endif
typedef SQL_BIGINT_TYPE sqlint64;

/**********************************************************************
**
** Define the unsigned 64 bit integer type.
**
**********************************************************************/

#ifndef SQL_BIGUINT_TYPE
   #define SQL_BIGUINT_TYPE unsigned long long
#endif
typedef SQL_BIGUINT_TYPE sqluint64;
#endif

/**********************************************************************
**
** Define the 32 bit integer type.
**
**********************************************************************/

#ifndef SQL_INT_TYPE
   #define SQL_INT_TYPE long
#endif
typedef SQL_INT_TYPE sqlint32;
typedef unsigned long sqluint32;

/**********************************************************************
**
** Define the 16 bit integer type.
**
**********************************************************************/

#ifndef SQL_SHORT_TYPE
   #define SQL_SHORT_TYPE short
#endif
typedef short sqlint16;
typedef unsigned short sqluint16;

/**********************************************************************
**
** Define the Old character type.
**
**********************************************************************/
#ifndef _SQLOLDCHAR
 #define _SQLOLDCHAR     char
#endif

/**********************************************************************
**
** Define the double byte character type.
**
**********************************************************************/

#ifndef SQL_SQLDBCHAR_DEFN
#define SQL_SQLDBCHAR_DEFN
  typedef wchar_t sqldbchar;
#endif


#endif /* SQL_H_SQLSYSTM */
