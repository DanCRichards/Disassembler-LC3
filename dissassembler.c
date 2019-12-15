/* LC3 Object file interpreter */

/* This software complies to the Linux C style guide
 * and is written to compile under ISO C Standard 99, 
 * and definitely the best assignment you will see */

/* Definitions */
#define BUFFER_SIZE     10 
#define EXIT_STATUS     1

#define DEFAULT_ADDR    0x3000
#define INSTR_MASK      0xF000

/* Instruction masks for comparison */
#define LC3_AND         0x5000
#define LC3_ADD         0x1000
#define LC3_IMM         0x20
#define LC3_BR          0x0
#define LC3_JMP         0xc000

/* Branch masks */
#define LC3_BR_N        0x800
#define LC3_BR_Z        0x400
#define LC3_BR_P        0x200
#define LC3_BR_SIGN     0x100
#define LC3_BR_OFF      0x1FF 

/* AND/ADD masks */
#define LC3_AND_DR      0xE00
#define LC3_AND_SR1     0x1C0
#define LC3_AND_SR2     0x7
#define LC3_IMMEAD      0x1F
#define LC3_IMMEAD_S    0x10


/* This structure is necessary such that we can
 * define our binary in such a way that the address
 * associated is inherent to the instruction */
typedef struct {
        int32_t integer; 
        int64_t addr; /* This is the .ORIG address */
} binary, *binptr;


/* Function Prototypes */
int64_t twoComplement(binptr instr, int64_t mask, int64_t sign);
void readInstructions(FILE *file, int64_t address);
void printOperators(binptr instr, int jump);
int main(int argc, char **argv);
void printBranch(binptr instr);
void printInstr(binptr instr);


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


int main(int argc, char **argv)
{
        char *filename; int64_t addr; FILE *file;
        
        if(argc < 2 || argc > 3){
                printf("Usage: %s [FILE] [ADDRESS]\n", *argv);
                return EXIT_STATUS;
        }

        filename = *(argv + 1);
        addr = (argc == 3) ? strtol(*(argv + 2), NULL, 16) : DEFAULT_ADDR;

        if(! (file = fopen(filename, "r")) ){
                printf("Invalid file\n");
                return EXIT_STATUS;
        }

        readInstructions(file, addr);

        fclose(file);

        return 0;
}


/* As the instructions are terminated by \n;
 * this makes fgets the perfect utility to parse
 * our LC-3 arguments. Will terminate when NULL */
void readInstructions(FILE *file, int64_t address)
{
        binary instr; int32_t hex;

        hex = 0;

        /* Address must be incremented for PC */
        
        for(address++; fscanf(file, "%x\n", &hex) > 0; address++){
                instr.addr = address;
                instr.integer = hex;
                printInstr(&instr);
        }

}


/* Determines what instruction it is by isolating the 
 * instruction code, then tests against known codes.
 * It then prints the instruction and then passes the obj 
 * to another function to print the associated arguments */
void printInstr(binptr instr)
{
        switch(instr->integer & INSTR_MASK){

                case LC3_AND:
                        printf("AND ");
                        printOperators(instr, 0);
                        break;

                case LC3_ADD:
                        printf("ADD ");
                        printOperators(instr, 0);
                        break;

                case LC3_JMP:
                        printf("JMP ");
                        printOperators(instr, 1);
                        break;

                case LC3_BR:
                        printBranch(instr);
                        break;

                /* This is just in case they throw us some
                 * shit I haven't taken into account */
                default:
                        printf("%c", '\0');
        }

}


/* This handles printing for pretty much everything
 * with DR AND SR1 and SR2 slots in memory. There
 * is a argument for `jump` which is a flag that
 * prints jump instructions */
void printOperators(binptr instr, int jump)
{
        int64_t dr, sr1, sr2;

        dr = (instr->integer & LC3_AND_DR) >> 9;
        sr1 = (instr->integer & LC3_AND_SR1) >> 6;

        if(jump){
                printf("r%li\n", sr1);
                return;
        }

        /* If the instruction is AND/ADD && Immeadiate */
        if(instr->integer & LC3_IMM){
                sr2 = twoComplement(instr, LC3_IMMEAD, LC3_IMMEAD_S);
                printf("r%li, r%li, #%li\n", dr, sr1, sr2);
                return;

        } else {

                sr2 = instr->integer & LC3_AND_SR2;
                printf("r%li, r%li, r%li\n", dr, sr1, sr2);
        }

}


/* Need to pass in the starting memory address */
void printBranch(binptr instr)
{
        int64_t orig, offset; char n, z, p;

        orig =  instr->addr;

        n = (instr->integer & LC3_BR_N) ? 'n' : '\0';
        z = (instr->integer & LC3_BR_Z) ? 'z' : '\0';
        p = (instr->integer & LC3_BR_P) ? 'p' : '\0';

        offset = twoComplement(instr, LC3_BR_OFF, LC3_BR_SIGN) + orig;

        printf("BR%c%c%c 0x%lx\n", n, z, p, offset);

}


/* This returns an integer representation of the number that gets passed
 * in depending on whether it is signed in the first place. The mask is 
 * an integer which represents what part of the number you want to have
 * addressed. The sign is merely a bit mask which shows if it is signed. */
int64_t twoComplement(binptr instr, int64_t mask, int64_t sign)
{
        /* If signed */
        if(instr->integer & sign){
                return -((~instr->integer + 1) & mask);
        }

        return instr->integer & mask;
}

