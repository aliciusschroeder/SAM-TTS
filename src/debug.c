#include<stdio.h>

#include "debug.h"

extern unsigned char signInputTable1[];
extern unsigned char signInputTable2[];

// -------------------------------------------------------------------------
// pysamtts stage dumps (see debug.h for the rationale and section format).

// Emit the reciter's phonetic output verbatim, up to (but not including) the
// 0x9B / 155 end-of-text marker. Leading/trailing spaces are preserved so the
// Python reciter can be compared byte-for-byte.
void DumpReciter(const char *phonemes)
{
    int i;
    printf("@@STAGE reciter\n");
    for (i = 0; phonemes[i] != 0 && (unsigned char)phonemes[i] != 155; i++)
        putchar(phonemes[i]);
    printf("\n@@END reciter\n");
}

// Emit a phoneme list as one row per slot until the 255 end marker (or maxlen).
// Columns are pure integers: array position, phoneme index, length, stress.
// The phoneme *name* is intentionally omitted -- names contain spaces and '*'
// which would break trivial parsing, and the Python side derives the name from
// the index anyway.
void DumpPhonemeList(
    const char *stage,
    unsigned char *phonemeindex,
    unsigned char *phonemeLength,
    unsigned char *stress,
    int maxlen)
{
    int i;
    printf("@@STAGE %s\n", stage);
    printf("# pos phoneme length stress\n");
    for (i = 0; (i < maxlen) && (phonemeindex[i] != 255); i++)
        printf("%d %d %d %d\n", i, phonemeindex[i], phonemeLength[i], stress[i]);
    printf("@@END %s\n", stage);
}

// Emit the full 256-frame formant/amplitude/pitch tables. Column order matches
// the -debug "Final data for speech output" table (flags, a1, f1, a2, f2,
// a3, f3, pitch). All 256 rows are dumped -- the trailing zero frames are
// deterministic and keep the fixture a fixed shape.
void DumpFrames(
    const char *stage,
    unsigned char *flag,
    unsigned char *f1,
    unsigned char *f2,
    unsigned char *f3,
    unsigned char *a1,
    unsigned char *a2,
    unsigned char *a3,
    unsigned char *p)
{
    int i;
    printf("@@STAGE %s\n", stage);
    printf("# pos flags ampl1 freq1 ampl2 freq2 ampl3 freq3 pitch\n");
    for (i = 0; i < 256; i++)
        printf("%d %d %d %d %d %d %d %d %d\n",
            i, flag[i], a1[i], f1[i], a2[i], f2[i], a3[i], f3[i], p[i]);
    printf("@@END %s\n", stage);
}

void PrintPhonemes(unsigned char *phonemeindex, unsigned char *phonemeLength, unsigned char *stress)
{
    int i = 0;
    printf("===========================================\n");

    printf("Internal Phoneme presentation:\n\n");
    printf(" idx    phoneme  length  stress\n");
    printf("------------------------------\n");

    while((phonemeindex[i] != 255) && (i < 255))
    {
        if (phonemeindex[i] < 81)
        {
            printf(" %3i      %c%c      %3i       %i\n",
            phonemeindex[i],
            signInputTable1[phonemeindex[i]],
            signInputTable2[phonemeindex[i]],
            phonemeLength[i],
            stress[i]
            );
        } else
        {
            printf(" %3i      ??      %3i       %i\n", phonemeindex[i], phonemeLength[i], stress[i]);
        }
        i++;
    }
    printf("===========================================\n");
    printf("\n");
}

void PrintOutput(
    unsigned char *flag,
    unsigned char *f1,
    unsigned char *f2,
    unsigned char *f3,
    unsigned char *a1,
    unsigned char *a2,
    unsigned char *a3,
    unsigned char *p)
{
    printf("===========================================\n");
    printf("Final data for speech output:\n\n");
    int i = 0;
    printf(" flags ampl1 freq1 ampl2 freq2 ampl3 freq3 pitch\n");
    printf("------------------------------------------------\n");
    while(i < 255)
    {
        printf("%5i %5i %5i %5i %5i %5i %5i %5i\n", flag[i], a1[i], f1[i], a2[i], f2[i], a3[i], f3[i], p[i]);
        i++;
    }
    printf("===========================================\n");

}

extern unsigned char GetRuleByte(unsigned short mem62, unsigned char Y);

void PrintRule(int offset)
{
    int i = 1;
    unsigned char A = 0;
    printf("Applying rule: ");
    do
    {
        A = GetRuleByte(offset, i);
        if ((A&127) == '=') printf(" -> "); else printf("%c", A&127);
        i++;
    } while ((A&128)==0);
    printf("\n");
}
