#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// ============================================================================
typedef void(*pf)(const short*, char*);

typedef struct
{
    uint16_t        opcode;
    uint16_t        mask;
    pf              pDecode;
} ts_instr;

// ============================================================================
char* decode_ea(short ea, const short* table, int dec)
{
    static char result[64];

    switch(ea & 0b111000)
    {
        case 0b000000: // Dn
            sprintf(result, "D%d", ea & 0b000111);
            break;
        case 0b001000: // An
            if ((ea & 0b000111) == 0b111)
                sprintf(result, "SP");
            else        
                sprintf(result, "A%d", ea & 0b000111);
            break;
        case 0b010000: // (An)
            sprintf(result, "(A%d)", ea & 0b000111);
            break;
        case 0b011000: // (An)+
            sprintf(result, "(A%d)+", ea & 0b000111);
            break;
        case 0b100000: // -(An)
            sprintf(result, "-(A%d)", ea & 0b000111);
            break;
        case 0b101000: // (d16, An)
            sprintf(result, "%s%X(A%d)", (table[dec+1] & 0xFFFF) < 10 ? "" : "$", table[dec + 1] & 0xFFFF, ea & 0b000111);
            break;
        case 0b110000: // (d8, An, Xn)
            if ((table[dec+1]&0xFF) != 0)
                sprintf(result, "$%X(A%d,%c%d.%c)", (int8_t)(table[dec+1] & 0xFF), ea & 0b000111,
                    table[dec+1]&0x8000?'A':'D', (table[dec+1]&0x7000)>>12, "WL"[(table[dec+1]%0b100000000000)>>11]);
            else
                sprintf(result, "(A%d,%c%d.%c)", ea & 0b000111,
                    table[dec+1]&0x8000?'A':'D', (table[dec+1]&0x7000)>>12, "WL"[(table[dec+1]%0b100000000000)>>11]);
            break;
        case 0b111000: // Other
            switch(ea & 0b000111)
            {
                case 0b000: // (xxx).W
                    sprintf(result, "($%8.8X).W", (int32_t)(table[dec + 1]));
                    break;
                case 0b001: // (xxx).L
                    sprintf(result, "($%8.8X).L", (int32_t)(((int32_t)(table[dec + 1]) << 16) + (table[dec + 2] & 0xFFFF)));
                    break;
                case 0b010: // d16(PC)
                    sprintf(result, "$%8.8X(PC)", (int32_t)(table[dec + 1]));
                    break;
                case 0b011: // d8(PC, Xn)
                    // il faut maintenant coder cette partie
                    if ((table[dec+1]&0xFF) != 0)
                        sprintf(result, "$%X(PC, %c%d.%c)", table[dec+1]&0xFF, table[dec+1]&0x8000?"A":"D",
                            (table[dec+1]&0x7000)>>12, "WL"[(table[dec+1]%0b100000000000)>>11]);
                    else
                        sprintf(result, "(PC, %c%d.%c)", table[dec+1]&0x8000?"A":"D",
                            (table[dec+1]&0x7000)>>12, "WL"[(table[dec+1]%0b100000000000)>>11]);
                    break;
                case 0b100: // #<data>
                    sprintf(result, "#$%X", (int32_t)(table[dec + 1]));
                    break;
                default:
                    sprintf(result, "todo");
                    break;
            }
            break;
    }
    return result;
}

//                      |5|4|3|2|1|0
//                      |1|1|1|1|1|1|9|8|7|6|5|4|3|2|1|0|
//                      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// ABCD                 |1 1 0 0|  Rx |1 0 0 0 0|R|  Ry |
void decode_ABCD(const short* table, char* result)
{
    short reg1 = (table[0] & 0b000111000000000) >> 9;
    short reg2 = (table[0] & 0b000000000000111) >> 0;
    if (table[0] & 0b1000)
    {
        sprintf(result, "ABCD -(A%d), -(A%d)", reg2, reg1);
    }
    else
    {
        sprintf(result, "ABCD D%d, D%d", reg2, reg1);
    }
}

//                      |5|4|3|2|1|0
//                      |1|1|1|1|1|1|9|8|7|6|5|4|3|2|1|0|
//                      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// ADD                  |1 1 0 1| Reg | OpM | EaM | EaR |
// ADDA                 |1 1 0 1| Reg | OpM | EaM | EaR |
// ADDX                 |1 1 0 1|  Rx |1|Siz|0 0|R|  Ry |
void decode_ADD_ADDA_ADDX(const short* table, char* result)
{
    short opmode = (table[0] & 0b000000111000000) >> 6;
    short reg = (table[0] & 0b000111000000000) >> 9;

    switch (opmode)
    {
        case 0b000: // ADD.B
        case 0b001: // ADD.W
        case 0b010: // ADD.L
            sprintf(result, "ADD.%c %s, D%d", "BWL"[opmode & 0b11], decode_ea(table[0] & 0b111111, table, 0), reg);
            break;
        case 0b100: // ADD.B
        case 0b101: // ADD.W
        case 0b110: // ADD.L
            if (table[0] & 0b110000)
                sprintf(result, "ADD.%c D%d, %s", "BWL"[opmode & 0b11], reg, decode_ea(table[0] & 0b111111, table, 0));
            else if (table[0] & 0b1000)
                sprintf(result, "ADDX.%c -(A%d), -(A%d)", "BWL"[opmode & 0b11], table[0] & 0b111, reg);
            else
                sprintf(result, "ADDX.%c D%d, D%d", "BWL"[opmode & 0b11], table[0] & 0b111, reg);
            break;
        case 0b011: // ADDA.W
        case 0b111: // ADDA.L
            sprintf(result, "ADDA.%c %s, A%d", "WL"[(opmode & 0b100)>>2], decode_ea(table[0] & 0b111111, table, 0), reg);
            break;
        default:
            sprintf(result, "UNKNOWN ADD/ADDA/ADDX");
            break;
    }
}

//                      |5|4|3|2|1|0
//                      |1|1|1|1|1|1|9|8|7|6|5|4|3|2|1|0|
//                      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// ADDI                 |0 0 0 0 0 1 1 0|Siz| EaM | EaR |
void decode_ADDI(const short* table, char* result)
{
    short size = (table[0] & 0b0000000011000000) >> 6;
    switch (size)
    {
        case 0b00: // Byte
            sprintf(result, "ADDI.%c #$%X, %s", "BWL"[size], table[1] & 0xFF, decode_ea(table[0] & 0b111111, table, 1));
            break;
        case 0b01: // Word
            sprintf(result, "ADDI.%c #$%X, %s", "BWL"[size], table[1] & 0xFFFF, decode_ea(table[0] & 0b111111, table, 1));
            break;
        case 0b10: // Long
            sprintf(result, "ADDI.%c #$%X, %s", "BWL"[size], (table[1] << 16) + (table[2] & 0xFFFF), decode_ea(table[0] & 0b111111, table, 2));
            break;
        default:
            sprintf(result, "ADDI Error Size");
            break;
    }
}

//                      |5|4|3|2|1|0
//                      |1|1|1|1|1|1|9|8|7|6|5|4|3|2|1|0|
//                      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// ADDQ                 |0 1 0 1| Dat |0|Siz| EaM | EaR |
void decode_ADDQ(const short* table, char* result)
{
    short data = (table[0] & 0b0000111000000000) >> 9;
    short size = (table[0] & 0b0000000011000000) >> 6;
    if (data == 0)
        data = 8;
    if (size != 0b11)
        sprintf(result, "ADDQ.%c #%d, %s", "BWL"[size], data, decode_ea(table[0] & 0b111111, table, 0));
    else
        sprintf(result, "ADDQ Error Size");
}

//                      |5|4|3|2|1|0
//                      |1|1|1|1|1|1|9|8|7|6|5|4|3|2|1|0|
//                      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// AND                  |1 1 0 0| Reg | OpM | EaM | EaR |
// EXG                  |1 1 0 0| Reg |1| OpMode  | Reg |
// MULS                 |1 1 0 0| Reg |1 1 1| EaM | EaR |
// MULU                 |1 1 0 0| Reg |0 1 1| EaM | EaR |
void decode_AND_EXG_MUL(const short* table, char* result)
{
    short exgmode = (table[0] & 0b111111000) >> 3;
    short opmode = (table[0] & 0b000000111000000) >> 6;
    short reg = (table[0] & 0b000111000000000) >> 9;

    if (exgmode == 0b101000) // EXG Dn, Dm
    {
        short reg2 = (table[0] & 0b111);
        sprintf(result, "EXG D%d, D%d", reg, reg2);
        return;
    }
    else if (exgmode == 0b101001) // EXG An, Am
    {
        short reg2 = (table[0] & 0b111);
        sprintf(result, "EXG A%d, A%d", reg, reg2);
        return;
    }
    else if (exgmode == 0b110001) // EXG Dn, Am
    {
        short reg2 = (table[0] & 0b111);
        sprintf(result, "EXG D%d, A%d", reg, reg2);
        return;
    }
    else
    {
        switch (opmode)
        {
            case 0b000: // AND.B
            case 0b001: // AND.W
            case 0b010: // AND.L
                sprintf(result, "AND.%c %s, D%d", "BWL"[opmode & 0b11], decode_ea(table[0] & 0b111111, table, 0), reg);
                break;
            case 0b100: // AND.B
            case 0b101: // AND.W
            case 0b110: // AND.L
                sprintf(result, "AND.%c D%d, %s", "BWL"[opmode & 0b11], reg, decode_ea(table[0] & 0b111111, table, 0));
                break;
            case 0b011: // MULU
                sprintf(result, "MULU.W %s, D%d", decode_ea(table[0] & 0b111111, table, 0), reg);
                break;
            case 0b111: // MULS
                sprintf(result, "MULS.W %s, D%d", decode_ea(table[0] & 0b111111, table, 0), reg);
                break;
            default:
                sprintf(result, "UNKNOWN AND/EXG/MUL");
                break;
        }
    }
}

//                      |5|4|3|2|1|0
//                      |1|1|1|1|1|1|9|8|7|6|5|4|3|2|1|0|
//                      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// ANDI                 |0 0 0 0 0 0 1 0|Siz| EaM | EaR |
void decode_ANDI(const short* table, char* result)
{
    short size = (table[0] & 0b0000000011000000) >> 6;
    if ((table[0] & 0b111111) == 0b111100) // ANDItoSR/ANDItoCCR
    {
        if (size == 0b00)       { sprintf(result, "ANDI #$%X,CCR", table[1] & 0xFF); }
        else if (size == 0b01)  { sprintf(result, "ANDI #$%X,SR", table[1] & 0xFFFF); }
    }
    else
    {
        switch (size)
        {
            case 0b00: // Byte
                sprintf(result, "ANDI.%c #$%X, %s", "BWL"[size], table[1] & 0xFF, decode_ea(table[0] & 0b111111, table, 1));
                break;
            case 0b01: // Word
                sprintf(result, "ANDI.%c #$%X, %s", "BWL"[size], table[1] & 0xFFFF, decode_ea(table[0] & 0b111111, table, 1));
                break;
            case 0b10: // Long
                sprintf(result, "ANDI.%c #$%X, %s", "BWL"[size], (table[1] << 16) + (table[2] & 0xFFFF), decode_ea(table[0] & 0b111111, table, 2));
                break;
            default:
                sprintf(result, "ANDI Error Size");
                break;
        }
    }
}


//                      |5|4|3|2|1|0
//                      |1|1|1|1|1|1|9|8|7|6|5|4|3|2|1|0|
//                      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// ASL/ASR              |1 1 1 0|Count|D|Siz|I|0 0| Reg |
// ASL/ASR mem          |1 1 1 0|0 0 0|D|1 1| EaM | EaR |
// Bcc                  |0 1 1 0| Cond  | 8 bit depl    |
// BCHG                 |0 0 0 0| Reg |1 0 1| EaM | EaR |
// BCHG                 |0 0 0 0 1 0 0 0 0 1| EaM | EaR |   |0 0 0 0 0 0 0 0|x x x x x x x x|
// BCLR                 |0 0 0 0| Reg |1 1 0| EaM | EaR |
// BCLR                 |0 0 0 0 1 0 0 0 1 0| EaM | EaR |   |0 0 0 0 0 0 0 0|x x x x x x x x|
// BFCHG                |1 1 1 0 1 0 1 0 1 1| EaM | EaR |
// BFCLR                |1 1 1 0 1 1 0 0 1 1| EaM | EaR |
// BFEXTS               |1 1 1 0 1 0 1 1 1 1| EaM | EaR |
// BFEXTU               |1 1 1 0 1 0 0 1 1 1| EaM | EaR |
// BFFFO                |1 1 1 0 1 1 0 1 1 1| EaM | EaR | <= Version corrigée
// BFINS                |1 1 1 0 1 1 1 1 1 1| EaM | EaR |
// BFSET                |1 1 1 0 1 1 1 0 1 1| EaM | EaR |
// BFTST                |1 1 1 0 1 0 0 0 1 1| EaM | EaR |
// BKPT                 |0 1 0 0 1 0 0 0 0 1 0 0 1|Vect |
// BRA                  |0 1 1 0 0 0 0 0| 8 bit depl    | <= VOIR Bcc
// BSET                 |0 0 0 0| Reg |1 1 1| EaM | EaR |
// BSET                 |0 0 0 0 1 0 0 0 1 1| EaM | EaR |   |0 0 0 0 0 0 0 0|x x x x x x x x|
// BSR                  |0 1 1 0 0 0 0 1| 8 bit depl    | <= VOIR Bcc
// BTST                 |0 0 0 0| Reg |1 0 0| EaM | EaR |
// BTST                 |0 0 0 0 1 0 0 0 0 0| EaM | EaR |   |0 0 0 0 0 0 0 0|x x x x x x x x|
// CALLM                |0 0 0 0 0 1 1 0 1 1| EaM | EaR |
// CAS/CAS2             |0 0 0 0 1|Siz|0 1 1| EaM | EaR |
// CHK                  |0 1 0 0| Reg |Siz|0| EaM | EaR |
// CHK2                 |0 0 0 0 0|Siz|0 1 1| EaM | EaR |
// CLR                  |0 1 0 0 0 0 1 0|Siz| EaM | EaR |
// CMP/CMPA/CMPM/EOR    |1 0 1 1| Reg | OpM | EaM | EaR |
// CMPA                 |1 0 1 1| Reg | OpM | EaM | EaR |
// CMPI                 |0 0 0 0 1 1 0 0|Siz| EaM | EaR |
// CMPM                 |1 0 1 1| Reg |1|Siz|0 0 1| Reg |
// CMP2                 |0 0 0 0 0|Siz|0 1 1| EaM | EaR |
// cpBcc                |1 1 1 1| CId |0 1|S| Coop Cond |
// cpDBcc               |1 1 1 1| CId |0 0 1 0 0 1| Reg |
// cpGEN                |1 1 1 1| CId |0 0 0| EaM | EaR |
// cpScc                |1 1 1 1| CId |0 0 1| EaM | EaR |
// cpTRAPcc             |1 1 1 1| CId |0 0 1 1 1 1| Mod |
// DBcc                 |0 1 0 1| Cond  |1 1 0 0 1| Reg |
// DIVS                 |1 0 0 0| Reg |1 1 1| EaM | EaR |
// DIVSL/DIVUL          |0 1 0 0 1 1 0 0 0 1| EaM | EaR |
// DIVU                 |1 0 0 0| Reg |0 1 1| EaM | EaR |
// EORI                 |0 0 0 0 1 0 1 0|Siz| EaM | EaR |
// EORItoCCR            |0 0 0 0 1 0 1 0 0 0 1 1 1 1 0 0|   |0 0 0 0 0 0 0 0|x x x x x x x x| 
// EXT/EXTB             |0 1 0 1 1 0 0| OpM |0 0 0| Reg |
// ILLEGAL              |0 1 0 0 1 0 1 0 1 1 1 1 1 1 0 0|
// JMP                  |0 1 0 0 1 1 1 0 1 1| EaM | EaR |

// ============================================================================
int decode(const short* table, char* result)
{
    int res = 0;

    const ts_instr  w_instrList[] = 
    {
        { 0b1100000100000000, 0b1111000111110000, &decode_ABCD },
        { 0b1101000000000000, 0b1111000000000000, &decode_ADD_ADDA_ADDX },
        { 0b0000011000000000, 0b1111111100000000, &decode_ADDI},
        { 0b0101000000000000, 0b1111000111000000, &decode_ADDQ },
        { 0b0101000001000000, 0b1111000111000000, &decode_ADDQ },
        { 0b0101000010000000, 0b1111000111000000, &decode_ADDQ },
        { 0b1100000000000000, 0b1111000000000000, &decode_AND_EXG_MUL },
        { 0b0000001000000000, 0b1111111100000000, &decode_ANDI },
    };
    size_t w_nbInstr = sizeof(w_instrList)/sizeof(w_instrList[0]);

    int i = 0;
    for (i = 0; i < w_nbInstr; i++)
    {
        if ((table[0] & w_instrList[i].mask) == w_instrList[i].opcode)
        {
            break;
        }
    }
    if (i < w_nbInstr)
    {
        w_instrList[i].pDecode(table, result);
        res = 1;
    }
    return res;
}

// ============================================================================
int main(int argc, char** argv)
{
    if (argc > 1)
    {
        int i = 1;
        int j = 0;
        unsigned short table[16] = { 0 };

        for (int i = 1; i < argc; i++)
        {
            if (strlen(argv[i]) == 2)
            {
                long l = strtol(argv[i], NULL, 16);
                if (i % 2 == 1)
                {
                    table[j] += ((short)l & 0xFF) << 8;
                    j++;
                }
                else
                {
                    table[j-1] += (short)l & 0xFF;
                }
            }
            else if (strlen(argv[i]) == 4)
            {
                long l = strtol(argv[i], NULL, 16);
                table[j] += l;
                j++;
            }
            else
            {
                // Probleme
            }
        }
        printf("Entree:");
        for (i = 0; i < j; i++)
        {
            printf(" %04.4X", table[i]);
        }
        printf("\n");

        char result[1024];
        printf("Resultat:");
        if (decode(table, result))
        {
            printf(" %s", result);
        }
        else
        {
            printf(" inconnu");
        }
        printf("\n");
    }
    else
    {
        fprintf(stderr, "Usage: instr <hexa>+\n");
        return -1;
    }




    return 0;
}