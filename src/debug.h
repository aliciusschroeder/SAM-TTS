#ifndef DEBUG_H
#define DEBUG_H

// -------------------------------------------------------------------------
// pysamtts stage dumps
//
// These are distinct from -debug. -debug emits verbose, human-oriented,
// interleaved rule traces AND (in the libsdl build) plays the full sentence
// on the speaker, blocking until playback finishes. That is noisy and slow
// to harvest from.
//
// The Dump* functions below instead emit one cleanly delimited,
// machine-parseable section per pipeline stage, and the -dump-* CLI flags
// that select them suppress audio entirely (see main.c / render.c). They
// exist so tools/gen_gold.py can capture byte-exact per-stage golden
// fixtures for the Python port to validate against.
//
// Each section is framed as:
//     @@STAGE <name>
//     # <column header>
//     <rows of space-separated integers, or the raw reciter string>
//     @@END <name>
//
// dumpstages is a bitmask so a single run can emit several sections.
#define DUMP_RECITER 0x01  // phonetic string produced by the reciter
#define DUMP_PARSER1 0x02  // phoneme list after Parser1
#define DUMP_REWRITE 0x04  // phoneme list after Parser2/CopyStress/SetPhonemeLength
#define DUMP_ADJUST  0x08  // phoneme list after AdjustLengths
#define DUMP_FINAL   0x10  // phoneme list after InsertBreath (input to PrepareOutput)
#define DUMP_PREPARE 0x20  // *Output[] arrays after PrepareOutput (input to Render)
#define DUMP_FRAMES  0x40  // freq/ampl/pitch frames after CreateTransitions
#define DUMP_ALL     0x7F

void DumpReciter(const char *phonemes);
void DumpPhonemeList(
    const char *stage,
    unsigned char *phonemeindex,
    unsigned char *phonemeLength,
    unsigned char *stress,
    int maxlen);
void DumpFrames(
    const char *stage,
    unsigned char *flag,
    unsigned char *f1,
    unsigned char *f2,
    unsigned char *f3,
    unsigned char *a1,
    unsigned char *a2,
    unsigned char *a3,
    unsigned char *p);

void PrintPhonemes(unsigned char *phonemeindex, unsigned char *phonemeLength, unsigned char *stress);
void PrintOutput(
    unsigned char *flag,
    unsigned char *f1,
    unsigned char *f2,
    unsigned char *f3,
    unsigned char *a1,
    unsigned char *a2,
    unsigned char *a3,
    unsigned char *p);

void PrintRule(int offset);

#endif
