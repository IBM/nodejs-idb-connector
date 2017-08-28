/* begin_generated_IBM_copyright_prolog                              */
/* This is an automatically generated copyright prolog.              */
/* After initializing,  DO NOT MODIFY OR MOVE                        */
/* ----------------------------------------------------------------- */
/*                                                                   */
/* Product(s):                                                       */
/*     5716-CX2                                                      */
/*     5716-CX4                                                      */
/*     5716-CX5                                                      */
/*     5722-SS1                                                      */
/*     5761-SS1                                                      */
/*     5770-SS1                                                      */
/*                                                                   */
/* (C)Copyright IBM Corp.  1991, 2013                                */
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
#ifndef   __math_h
  #define __math_h

#if defined(__cplusplus) && (__OS400_TGTVRM__ >= 710)
  #include <float.h>
  #include <errno.h>
#endif

#ifdef __cplusplus
 extern "C" {
 #pragma info(none)
#else
 #pragma nomargins nosequence
 #pragma checkout(suspend)
#endif

/* ================================================================= */
/* Header File Name: math.h                                          */
/*                                                                   */
/* This header contains the declarations and definitions used by     */
/* the math library functions.                                       */
/* ================================================================= */

  #pragma datamodel(P128)

  extern double _C_huge_val;
  extern float  _C_huge_valf;

#if (__OS400_TGTVRM__ >= 610)
  extern float _C_qnan_valf;
#endif

  typedef float   float_t;
  typedef double  double_t;

#if defined(__STDC_WANT_DEC_FP__) && \
    (__OS400_TGTVRM__ >= 610) && defined(__IBM_DFP__)
  typedef _Decimal32 _Decimal32_t;
  typedef _Decimal64 _Decimal64_t;

  extern _Decimal32 _C_huge_vald32;
  extern _Decimal32 _C_qnan_vald32;
  extern _Decimal64 _C_huge_vald64;
  extern _Decimal128 _C_huge_vald128;
#endif

  /* Language Levels - ANSI, SAA, SAA Level 2 and IBM Extensions      */
  double acos    ( double );
  double asin    ( double );
  double atan    ( double );
  double atan2   ( double, double );
  double ceil    ( double );
  double cos     ( double );
  double cosh    ( double );
  double erf     ( double );
  double erfc    ( double );
  double exp     ( double );
  double floor   ( double );
  double fmod    ( double, double );
  double frexp   ( double, int * );
  double hypot   ( double, double );
  double ldexp   ( double, int );
  double log     ( double );
  double log10   ( double );
  double modf    ( double, double * );
  double pow     ( double, double );
  double sin     ( double );
  double sinh    ( double );
  double sqrt    ( double );
  double tan     ( double );
  double tanh    ( double );

  double nextafter( double, double );
  double nexttoward( double, long double );
  long double nextafterl( long double, long double );
  long double nexttowardl( long double, long double );
  #if (__OS400_TGTVRM__ >= 610)
    #pragma map(nexttowardl,  "nextafterl")
  #else
    #pragma map(nexttoward,  "nextafter")
    #pragma map(nextafterl,  "nextafter")
    #pragma map(nexttowardl, "nextafter")
  #endif

  #ifndef __cplusplus
    double fabs    ( double );
  #endif

  /* Language Levels - SAA, SAA  Level 2 and IBM Extensions           */
  #if defined (__SAA__) || defined (__SAA_L2__) || defined (__EXTENDED__)

    double gamma ( double );
    double jn    ( int, double );
    double j0    ( double );
    double j1    ( double );
    double yn    ( int, double );
    double y0    ( double );
    double y1    ( double );

  #endif /* Language Levels - SAA, SAA Level 2 and IBM Extension      */

#ifdef __cplusplus
  extern "C" {
#else
  #pragma linkage( __fabs, builtin )
#endif
  double  __fabs          ( double );
#ifdef __cplusplus
  }
#endif

#define HUGE_VAL        _C_huge_val
#define HUGE_VALL       ((long double)_C_huge_val)
#define HUGE_VALF       _C_huge_valf

#define INFINITY        _C_huge_valf
#if (__OS400_TGTVRM__ >= 610)
  #define NAN             _C_qnan_valf
#endif

#if defined(__STDC_WANT_DEC_FP__) && \
    (__OS400_TGTVRM__ >= 610) && defined(__IBM_DFP__)
  #define HUGE_VAL_D32    _C_huge_vald32
  #define HUGE_VAL_D64    _C_huge_vald64
  #define HUGE_VAL_D128   _C_huge_vald128

  #define DEC_INFINITY    _C_huge_vald32
  #define DEC_NAN         _C_qnan_vald32
#endif

#ifndef __bool__
  #ifdef __cplusplus
    #define __bool__ bool
  #elif defined(__C99_BOOL)
    #define __bool__ _Bool
  #else
    #define __bool__ unsigned char
  #endif
#endif

#if defined(__STDC_WANT_DEC_FP__) && \
    (__OS400_TGTVRM__ >= 720) && defined(__IBM_DFP__)
  _Decimal32    quantized32(_Decimal32, _Decimal32);
  _Decimal64    quantized64(_Decimal64, _Decimal64);
  _Decimal128   quantized128(_Decimal128, _Decimal128);
  int           quantexpd32(_Decimal32);
  int           quantexpd64(_Decimal64);
  int           quantexpd128(_Decimal128);
  __bool__      samequantumd32(_Decimal32, _Decimal32);
  __bool__      samequantumd64(_Decimal64, _Decimal64);
  __bool__      samequantumd128(_Decimal128, _Decimal128);
#endif

#if defined(__cplusplus) && (__OS400_TGTVRM__ >= 710)
 extern "C++" {
  inline double fabs(double _X) {
    return ( __fabs( _X ) );
  }

  inline double abs(double _X) {
    return ( __fabs( _X ) );
  }

  inline double pow(double _X, int _Y) {
    return pow( _X, (double)_Y );
  }

  inline float abs(float _X) {
    double _Ans = fabs( (double)_X );
    if (_Ans!=0.0 && _Ans<FLT_MIN) {
      errno = ERANGE;
      return (float)0.0;
    }
    return (float)_Ans;
  }

  inline float acos(float _X) {
    return (float)acos( (double)_X );
  }

  inline float asin(float _X) {
    double _Ans = asin( (double)_X );
    if (_Ans!=0.0 && fabs(_Ans)<FLT_MIN) {
      errno = ERANGE;
      return (float)0.0;
    }
    return (float)_Ans;
  }

  inline float atan(float _X) {
    double _Ans = atan( (double)_X );
    if (_Ans!=0.0 && fabs(_Ans)<FLT_MIN) {
      errno = ERANGE;
      return (float)0.0;
    }
    return (float)_Ans;
  }

  inline float atan2(float _X, float _Y) {
    double _Ans = atan2( (double)_X, (double)_Y );
    if (_Ans!=0.0 && fabs(_Ans)<FLT_MIN) {
      errno = ERANGE;
      return (float)0.0;
    }
    return (float)_Ans;
  }

  inline float ceil(float _X) {
    return (float)ceil( (double)_X );
  }

  inline float cos(float _X) {
    double _Ans = cos( (double)_X );
    if (_Ans!=0.0 && fabs(_Ans)<FLT_MIN) {
      errno = ERANGE;
      return (float)0.0;
    }
    return (float)_Ans;
  }

  inline float cosh(float _X) {
    double _Ans = cosh( (double)_X );
    if (_Ans>FLT_MAX) {
      errno = ERANGE;
      return HUGE_VALF;
    }
    return (float)_Ans;
  }

  #if (__OS400_TGTVRM__ >= 720)
  inline float erf(float _X) {
    double _Ans = erf( (double)_X );
    if (_Ans!=0.0 && fabs(_Ans)<FLT_MIN) {
      errno = ERANGE;
      return (float)0.0;
    }
    return (float)_Ans;
  }

  inline float erfc(float _X) {
    double _Ans = erfc( (double)_X );
    if (_Ans!=0.0 && fabs(_Ans)<FLT_MIN) {
      errno = ERANGE;
      return (float)0.0;
    }
    return (float)_Ans;
  }
  #endif

  inline float exp(float _X) {
    double _Ans = exp( (double)_X );
    if (fabs(_Ans)>FLT_MAX) {
      errno = ERANGE;
      return HUGE_VALF;
    }
    else if (fabs(_Ans)<FLT_MIN) {
      errno = ERANGE;
      return (float)0.0;
    }
    return (float)_Ans;
  }

  inline float fabs(float _X) {
    double _Ans = fabs( (double)_X );
    if (_Ans!=0.0 && _Ans<FLT_MIN) {
      errno = ERANGE;
      return (float)0.0;
    }
    return (float)_Ans;
  }

  inline float floor(float _X) {
    return (float)floor( (double)_X );
  }

  inline float fmod(float _X, float _Y) {
    double _Ans = fmod( (double)_X, (double)_Y );
    if (_Ans!=0.0 && fabs(_Ans)<FLT_MIN) {
      errno = ERANGE;
      return (float)0.0;
    }
    return (float)_Ans;
  }

  inline float frexp(float _X, int* _Y) {
    return (float)frexp( (double)_X, _Y );
  }

  #if (__OS400_TGTVRM__ >= 720)
  inline float hypot(float _X, float _Y) {
    double _Ans = hypot( (double)_X, (double)_Y );
    if (_Ans>FLT_MAX) {
      errno = ERANGE;
      return HUGE_VALF;
    }
    else if (_Ans!=0.0 && _Ans<FLT_MIN) {
      errno = ERANGE;
      return (float)0.0;
    }
    return (float)_Ans;
  }
  #endif

  inline float ldexp(float _X, int _Y) {
    double _Ans = ldexp( (double)_X, _Y );
    if (fabs(_Ans)>FLT_MAX) {
      errno = ERANGE;
      if (_Ans>0.0)
        return HUGE_VALF;
      else
        return -HUGE_VALF;
    }
    else if (_Ans!=0.0 && fabs(_Ans)<FLT_MIN) {
      errno = ERANGE;
      return (float)0.0;
    }
    return (float)_Ans;
  }

  inline float log(float _X) {
    return (float)log( (double)_X );
  }

  inline float log10(float _X) {
    return (float)log10( (double)_X );
  }

  inline float modf(float _X, float* _Y) {
    double _Tmp = 0.0;
    double _Ans = modf((double)_X, &_Tmp);
    *_Y = _Tmp;
    if (_Ans!=0.0 && fabs(_Ans)<FLT_MIN) {
      errno = ERANGE;
      return (float)0.0;
    }
    return (float)_Ans;
  }

  inline float pow(float _X, float _Y) {
    double _Ans = pow( (double)_X, (double)_Y );
    if (fabs(_Ans)>FLT_MAX) {
      errno = ERANGE;
      if (_Ans>0.0)
        return HUGE_VALF;
      else
        return -HUGE_VALF;
    }
    else if (_Ans!=0.0 && fabs(_Ans)<FLT_MIN) {
      errno = ERANGE;
      return (float)0.0;
    }
    return (float)_Ans;
  }

  inline float pow(float _X, int _Y) {
    double _Ans = pow( (double)_X, (double)_Y );
    if (fabs(_Ans)>FLT_MAX) {
      errno = ERANGE;
      if (_Ans>0.0)
        return HUGE_VALF;
      else
        return -HUGE_VALF;
    }
    else if (_Ans!=0.0 && fabs(_Ans)<FLT_MIN) {
      errno = ERANGE;
      return (float)0.0;
    }
    return (float)_Ans;
  }

  inline float sin (float _X) {
    double _Ans = sin( (double)_X );
    if (_Ans!=0.0 && fabs(_Ans)<FLT_MIN) {
      errno = ERANGE;
      return (float)0.0;
    }
    return (float)_Ans;
  }

  inline float sinh (float _X) {
    double _Ans = sinh( (double)_X );
    if (fabs(_Ans)>FLT_MAX) {
      errno = ERANGE;
      if (_Ans>0.0)
        return HUGE_VALF;
      else
        return -HUGE_VALF;
    }
    else if (_Ans!=0.0 && fabs(_Ans)<FLT_MIN) {
      errno = ERANGE;
      return (float)0.0;
    }
    return (float)_Ans;
  }

  inline float sqrt(float _X) {
    return (float)sqrt( (double)_X );
  }

  inline float tan(float _X) {
    double _Ans = tan( (double)_X );
    if (fabs(_Ans)>FLT_MAX) {
      errno = ERANGE;
      if (_Ans>0.0)
        return HUGE_VALF;
      else
        return -HUGE_VALF;
    }
    else if (_Ans!=0.0 && fabs(_Ans)<FLT_MIN) {
      errno = ERANGE;
      return (float)0.0;
    }
    return (float)_Ans;
  }

  inline float tanh(float _X) {
    double _Ans = tanh( (double)_X );
    if (_Ans!=0.0 && fabs(_Ans)<FLT_MIN) {
      errno = ERANGE;
      return (float)0.0;
    }
    return (float)_Ans;
  }

  inline long double abs (long double _X)
  {
    return fabs( (double)_X );
  }

  inline long double acos (long double _X)
  {
    return acos( (double)_X );
  }

  inline long double asin (long double _X)
  {
    return asin( (double)_X );
  }

  inline long double atan (long double _X)
  {
    return atan( (double)_X );
  }

  inline long double atan2(long double _X, long double _Y)
  {
    return atan2( (double)_X, (double)_Y );
  }

  inline long double ceil (long double _X)
  {
    return ceil( (double)_X );
  }

  inline long double cos (long double _X)
  {
    return cos( (double)_X );
  }

  inline long double cosh (long double _X)
  {
    return cosh( (double)_X );
  }

  #if (__OS400_TGTVRM__ >= 720)
  inline long double erf (long double _X)
  {
    return erf( (double)_X );
  }

  inline long double erfc (long double _X)
  {
    return erfc( (double)_X );
  }
  #endif

  inline long double exp (long double _X)
  {
    return exp( (double)_X );
  }

  inline long double fabs (long double _X)
  {
    return fabs( (double)_X );
  }

  inline long double floor(long double _X)
  {
    return floor( (double)_X );
  }

  inline long double fmod (long double _X, long double _Y)
  {
    return fmod( (double)_X, (double)_Y );
  }

  inline long double frexp(long double _X, int* _Y)
  {
    return frexp( (double)_X, _Y );
  }

  #if (__OS400_TGTVRM__ >= 720)
  inline long double hypot (long double _X, long double _Y)
  {
    return hypot( (double)_X, (double)_Y );
  }
  #endif

  inline long double ldexp(long double _X, int _Y)
  {
    return ldexp( (double)_X, _Y );
  }

  inline long double log (long double _X)
  {
    return log( (double)_X );
  }

  inline long double log10(long double _X)
  {
    return log10( (double)_X );
  }

  inline long double modf (long double _X, long double* _Y)
  {
    return modf( (double)_X, (double *) _Y);
  }

  inline long double pow (long double _X, long double _Y)
  {
    return pow( (double)_X, (double)_Y );
  }

  inline long double pow (long double _X, int _Y)
  {
    return pow( (double)_X, (double)_Y );
  }

  inline long double sin (long double _X)
  {
    return sin( (double)_X );
  }

  inline long double sinh (long double _X)
  {
    return sinh( (double)_X );
  }

  inline long double sqrt (long double _X)
  {
    return sqrt( (double)_X );
  }

  inline long double tan (long double _X)
  {
    return tan( (double)_X );
  }

  inline long double tanh (long double _X)
  {
    return tanh( (double)_X );
  }
 }
#elif defined(__cplusplus)
  inline double fabs(double _X) {
    return ( __fabs( _X ) );
  }
#else
  #define fabs(__x)  (__fabs((__x)))
#endif

#pragma datamodel(pop)

#ifdef __cplusplus
 #pragma info(restore)
 }
#else
 #pragma checkout(resume)
#endif

#endif /* #ifndef __math_h */
