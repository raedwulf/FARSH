#pragma once

#include <stdint.h>
#if defined(_MSC_VER)
# include <intrin.h>
#endif

enum CPUIDInfoType
{
    FeatureSupport = 1,  BrandNameFirst = 0x80000002,  BrandNameLast = 0x80000004

};

struct CpuidFeatures
{
    union
    {
        uint32_t CPUInfo[4*4];
        struct
        {
            char IDString[3*4*4];
            // First ID string
            unsigned SteppingID     :4;//4      0-3
            unsigned Model          :4;//8      4-7
            unsigned Family         :4;//12     8-11
            unsigned TypeItl        :2;//14     12-13
            unsigned Reserved11     :2;//16     14-15
            unsigned ExtendedModel  :4;//20     16-19
            unsigned ExtendedFamily :8;//28     20-27
            unsigned Reserved12     :3;//32     28-31
            // Second ID string
            unsigned BrandIndex     :8;//8      0-7
            unsigned QwordCFLUSH    :8;//16     8-15
            unsigned LogicProcCount :8;//24     16-23
            unsigned ApicID         :8;//32     24-31
            // Third ID string
            unsigned SSE3           :1;//1      0   
            unsigned Reserved31     :2;//3      1-2
            unsigned MWAIT          :1;//4      3
            unsigned CPLDebug       :1;//5      4
            unsigned VMExt          :1;//6      5
            unsigned SafeModeExt    :1;//7      6
            unsigned IntelSpeedStep :1;//8      7
            unsigned ThermalMonitor :1;//9      8
            unsigned SupplSSE3      :1;//10     9
            unsigned L1CtxID        :1;//11     10
            unsigned Reserved32     :1;//12     11
            unsigned FMA256         :1;//13     12
            unsigned CMPXCHG16B     :1;//14     13
            unsigned xTPR           :1;//15     14
            unsigned MSRDebug       :1;//16     15
            unsigned Reserver32     :2;//18     16-17
            unsigned DirectCacheAcc :1;//19     18
            unsigned SSE41          :1;//20     19
            unsigned SSE42          :1;//21     20
            unsigned x2APIC         :1;//22     21
            unsigned MOVBE          :1;//23     22
            unsigned POPCNT         :1;//24     23
            unsigned Reserved33     :1;//25     24
            unsigned AESInput       :1;//26     25
            unsigned XSAVE          :1;//27     26
            unsigned OSXSAVE        :1;//28     27
            unsigned AVX            :1;//29     28
            unsigned Reserved34     :3;//31     29-31
            // Fourth ID String
            unsigned FPU            :1;//1      0   
            unsigned VME            :1;//2      1
            unsigned DE             :1;//3      2
            unsigned PSE            :1;//4      3
            unsigned TSC            :1;//5      4
            unsigned MSR            :1;//6      5
            unsigned PAE            :1;//7      6
            unsigned MCE            :1;//8      7
            unsigned Cx8            :1;//9      8
            unsigned APIC           :1;//10     9
            unsigned Reserved41     :1;//11     10
            unsigned SEP            :1;//12     11
            unsigned MTTR           :1;//13     12
            unsigned PGE            :1;//14     13
            unsigned MCA            :1;//15     14
            unsigned CMOV           :1;//16     15
            unsigned PAT            :1;//17     16
            unsigned PSE36          :1;//18     17
            unsigned PSN            :1;//19     18
            unsigned CFLUSH         :1;//20     19
            unsigned Reserved42     :1;//21     20
            unsigned DS             :1;//22     21
            unsigned ACPI           :1;//23     22
            unsigned MMX            :1;//24     23
            unsigned FXSR           :1;//25     24
            unsigned SSE            :1;//26     25
            unsigned SSE2           :1;//27     26
            unsigned SS             :1;//28     27
            unsigned HTT            :1;//29     28
            unsigned TM             :1;//30     29
            unsigned Reserved43     :1;//31     30
            unsigned PBE            :1;//32     31
        };
    };
};


void run_cpuid(uint32_t eax, uint32_t ecx, uint32_t* abcd)
{
#if defined(_MSC_VER)
    __cpuidex(abcd, eax, ecx);
#else
    uint32_t ebx, edx;
# if defined( __i386__ ) && defined ( __PIC__ )
     /* in case of PIC under 32-bit EBX cannot be clobbered */
    __asm__ ( "movl %%ebx, %%edi \n\t cpuid \n\t xchgl %%ebx, %%edi" : "=D" (ebx),
# else
    __asm__ ( "cpuid" : "+b" (ebx),
# endif
              "+a" (eax), "+c" (ecx), "=d" (edx) );
    abcd[0] = eax; abcd[1] = ebx; abcd[2] = ecx; abcd[3] = edx;
#endif
}     

inline void GetCpuidFeatures (CpuidFeatures *featureStruct)
{
    for (unsigned i=BrandNameFirst; i<=BrandNameLast; i++)
        run_cpuid (i, 0, featureStruct->CPUInfo + 4*(i-BrandNameFirst));

    run_cpuid (FeatureSupport, 0, featureStruct->CPUInfo+3*4);
}