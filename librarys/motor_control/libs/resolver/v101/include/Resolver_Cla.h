//----------------------------------------------------------------------------------
//	FILE:			Resolver_CLA.h
//
//	Description:	shared variables and constants
//
//   Revision/ Version: See Resolver-Main.c
//----------------------------------------------------------------------------------
#ifndef Resolver_CLA_H
#define Resolver_CLA_H


#ifndef DSP28_DATA_TYPES
#define DSP28_DATA_TYPES
typedef short           int16;
typedef long            int32;
typedef unsigned char   Uint8;
typedef unsigned short  Uint16;
typedef unsigned long   Uint32;
typedef float           float32;
typedef long double     float64;
#endif

#ifndef GLOBAL_Q
#define GLOBAL_Q 20

extern float CLAatan2Table[];

#endif

//*****************************************************************************
#define    ANGLE_WRAP(angle)            \
             if(angle > 1.0)            \
               angle -= 2.0;            \
             else if(angle < -1.0)      \
               angle += 2.0;
//*****************************************************************************
#define    CLAMP2LIMITS(in, max, min)   \
             if (in > max)              \
               in = max;                \
             else if (in < min)         \
               in = min;
// ****************************************************************************
// Macros to convert numbers between floating point and IQ formats
#define  IQn2float(X, Qn)  ((float)X * (1/(1<<Qn)))
#define  float2IQn(f, Qn)  (f * (float)((long)1<<Qn))
// ****************************************************************************

// ============================================
// Structure of variables between CPU and CLA
// ============================================
// Input variables to CLA
typedef struct  {
	// variables to set up basic functions
    Uint16 firLag,       // lag between sine index and FIR index
           FIR32,        // select-1 / deselect-0 FIR32 function
           TUNING,       // select-1 / deselect-0 TUNING function
           TABLE_LENGTH; // set up FIR filter length

    float  offsetS,       //  dc offset of sine fbk analog channel
       	   offsetC,       //  dc offset of cosine fbk analog channel
           testAngle,     // test Angle used for tuning the PI coefficients
           SAMPLING_TIME, // loop decimation sampling time (carrier cycle time)

	// control loop parameters (can be replaced with MACROs)
		   errorWfT,    // error filter constant, internally computed based on filter coefficients
		   picon_K0,    // PI controller constant, internally computed based on PI coefficients
		   picon_K1,    // PI controller constant, internally computed based on PI coefficients
		   rpsMax;      // max motor speed in eqvt elec freq
} RESOLVER_INPUT;

//--------------------------------------------------------------------------------
// Output variables from CLA
typedef struct  {
	// variables for outputting results in float
	float angleRaw,      // raw angle estimate from arctan
          angleObs,      // observer angle estimate w/o FIR delay compensation
          angleOut,      // final angle estimate after FIR delay compensation
          rpsObs,        // shaft speed estimate by the observer
          errorNew,      // new angle error estimated by the observer
          resMag,        // resolver fbk mag

          // debug variables - can be commented out
	      sinFIRout,     // FIR bandpass filter output of sine feedback signal
	      cosFIRout;     // FIR bandpass filter output of cosine feedback signal

#ifndef FLOAT_CPU_RESOLVER_CLA_LIB
	// variables for outputting results in Q20
	int32 angleRaw20,    // arctan angle estimate in pu
	      angleObs20,    // observer angle estimate in pu
	      angleOut20,    // final estimated angle in pu
	      rpsObs20,      // shaft speed estimate
	      errorNew20;    // PLL loop error in pu

	// variables for data analysis
	int32 resMag20;                     // resolver magnitude in Q20
#endif

	// variables used within library
	float sin_input,    // sine input from resolver
	      cos_input;    // cosine input from resolver
	Uint16 sineIndex;   // index to element within sine table

	// variables for cycle counting the execution times
	Uint16 uxCycleCount,      // counter to store cycle count of selected code - debug purpose
	       uxCycleCount1;     // counter to store cycle count of selected code - debug purpose
} RESOLVER_OUTPUT;

//*****************************************************************************
// ============================================
// Functions defined in library
// ============================================
void init_resolver_CLA(void);
int16 resolver_algo_CLA(void);

// ============================================
// variables used between CPU and CLA
// ============================================
extern int32 *sineTable;            // pointer to SINE_TABLE (CPU to CLA space)
extern float *firCoeff;             // pointer to FIR_COEFF_TABLE (CPU to CLA space)
extern RESOLVER_INPUT rslvrIn;      // variables in CPU to CLA space
extern RESOLVER_OUTPUT rslvrOut;    // variables in CLA to CPU space

#endif
