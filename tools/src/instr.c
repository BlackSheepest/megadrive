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
char* decode_ea(short ea, const short* table, int dec, int size)
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
            if ((ea & 0b000111) == 0b111)
                sprintf(result, "(SP)");
            else        
                sprintf(result, "(A%d)", ea & 0b000111);
            break;
        case 0b011000: // (An)+
            if ((ea & 0b000111) == 0b111)
                sprintf(result, "(SP)+");
            else        
                sprintf(result, "(A%d)+", ea & 0b000111);
            break;
        case 0b100000: // -(An)
            if ((ea & 0b000111) == 0b111)
                sprintf(result, "-(SP)");
            else        
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
                    if (size == 0b00)
                        sprintf(result, "#$%X", ((int16_t)(table[dec+1])) & 0xFF);
                    else if (size == 0b01)
                        sprintf(result, "#$%X", ((int16_t)(table[dec+1])) & 0xFFFF);
                    else
                        sprintf(result, "#$%X", (((int32_t)(table[dec+1])) & 0xFFFF) << 16 | ((int32_t)(table[dec+2]) & 0xFFFF));
                    break;
                default:
                    sprintf(result, "todo");
                    break;
            }
            break;
    }
    return result;
}

// ============================================================================
// Généré par IA ;) [Apres plusieurs tentatives ;)]
char* decode_reglist(short reglist, int ea_mode)
{
    static char result[128];
    result[0] = '\0';
    int first = 1;
    int i;
    
    // A7 ------- D0 in normal mode and postincrement
    // D0 ------- A7 in predecrement
    // In predecrement mode (ea_mode == 0b100), bits are reversed
    if (ea_mode == 0b100)
    {
        // Reverse the bits for predecrement mode
        short reversed = 0;
        for (int j = 0; j < 16; j++)
        {
            if (reglist & (1 << j))
            {
                reversed |= (1 << (15 - j));
            }
        }
        reglist = reversed;
        
        i = 0;
        while (i < 16)
        {
            if (reglist & (1 << i))
            {
                // Find the end of consecutive registers
                int start = i;
                while (i < 16 && (reglist & (1 << i)))
                {
                    i++;
                }
                int end = i - 1;
                
                if (!first)
                {
                    strcat(result, "/");
                }
                first = 0;
                
                // Format start register
                char start_buf[8];
                if (start < 8)
                    sprintf(start_buf, "D%d", start);
                else
                    sprintf(start_buf, "A%d", start - 8);
                
                // If range of consecutive registers
                if (end > start)
                {
                    char end_buf[8];
                    if (end < 8)
                        sprintf(end_buf, "D%d", end);
                    else
                        sprintf(end_buf, "A%d", end - 8);
                    
                    sprintf(result + strlen(result), "%s-%s", start_buf, end_buf);
                }
                else
                {
                    strcat(result, start_buf);
                }
            }
            else
            {
                i++;
            }
        }
    }
    else
    {
        // Normal mode and postincrement
        i = 0;
        while (i < 16)
        {
            if (reglist & (1 << i))
            {
                // Find the end of consecutive registers
                int start = i;
                while (i < 16 && (reglist & (1 << i)))
                {
                    i++;
                }
                int end = i - 1;
                
                if (!first)
                {
                    strcat(result, "/");
                }
                first = 0;
                
                // Format start register
                char start_buf[8];
                if (start < 8)
                    sprintf(start_buf, "D%d", start);
                else
                    sprintf(start_buf, "A%d", start - 8);
                
                // If range of consecutive registers
                if (end > start)
                {
                    char end_buf[8];
                    if (end < 8)
                        sprintf(end_buf, "D%d", end);
                    else
                        sprintf(end_buf, "A%d", end - 8);
                    
                    sprintf(result + strlen(result), "%s-%s", start_buf, end_buf);
                }
                else
                {
                    strcat(result, start_buf);
                }
            }
            else
            {
                i++;
            }
        }
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
            sprintf(result, "ADD.%c %s, D%d", "BWL"[opmode & 0b11], decode_ea(table[0] & 0b111111, table, 0, opmode & 0b11), reg);
            break;
        case 0b100: // ADD.B
        case 0b101: // ADD.W
        case 0b110: // ADD.L
            if (table[0] & 0b110000)
                sprintf(result, "ADD.%c D%d, %s", "BWL"[opmode & 0b11], reg, decode_ea(table[0] & 0b111111, table, 0, opmode & 0b11));
            else if (table[0] & 0b1000)
                sprintf(result, "ADDX.%c -(A%d), -(A%d)", "BWL"[opmode & 0b11], table[0] & 0b111, reg);
            else
                sprintf(result, "ADDX.%c D%d, D%d", "BWL"[opmode & 0b11], table[0] & 0b111, reg);
            break;
        case 0b011: // ADDA.W
        case 0b111: // ADDA.L
            sprintf(result, "ADDA.%c %s, A%d", "WL"[(opmode & 0b100)>>2], decode_ea(table[0] & 0b111111, table, 0, ((opmode & 0b100)>>2)+1), reg);
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
            sprintf(result, "ADDI.%c #$%X, %s", "BWL"[size], table[1] & 0xFF, decode_ea(table[0] & 0b111111, table, 1, size));
            break;
        case 0b01: // Word
            sprintf(result, "ADDI.%c #$%X, %s", "BWL"[size], table[1] & 0xFFFF, decode_ea(table[0] & 0b111111, table, 1, size));
            break;
        case 0b10: // Long
            sprintf(result, "ADDI.%c #$%X, %s", "BWL"[size], (table[1] << 16) + (table[2] & 0xFFFF), decode_ea(table[0] & 0b111111, table, 2, size));
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
        sprintf(result, "ADDQ.%c #%d, %s", "BWL"[size], data, decode_ea(table[0] & 0b111111, table, 0, size));
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
                sprintf(result, "AND.%c %s, D%d", "BWL"[opmode & 0b11], decode_ea(table[0] & 0b111111, table, 0, opmode & 0b11), reg);
                break;
            case 0b100: // AND.B
            case 0b101: // AND.W
            case 0b110: // AND.L
                sprintf(result, "AND.%c D%d, %s", "BWL"[opmode & 0b11], reg, decode_ea(table[0] & 0b111111, table, 0, opmode & 0b11));
                break;
            case 0b011: // MULU
                sprintf(result, "MULU.W %s, D%d", decode_ea(table[0] & 0b111111, table, 0, 1), reg);
                break;
            case 0b111: // MULS
                sprintf(result, "MULS.W %s, D%d", decode_ea(table[0] & 0b111111, table, 0, 1), reg);
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
                sprintf(result, "ANDI.%c #$%X, %s", "BWL"[size], table[1] & 0xFF, decode_ea(table[0] & 0b111111, table, 1, size));
                break;
            case 0b01: // Word
                sprintf(result, "ANDI.%c #$%X, %s", "BWL"[size], table[1] & 0xFFFF, decode_ea(table[0] & 0b111111, table, 1, size));
                break;
            case 0b10: // Long
                sprintf(result, "ANDI.%c #$%X, %s", "BWL"[size], (table[1] << 16) + (table[2] & 0xFFFF), decode_ea(table[0] & 0b111111, table, 2, size));
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
// LSL/LSR              |1 1 1 0|Count|D|Siz|I|0 1| Reg |
// ROL/ROR              |1 1 1 0|Count|D|Siz|I|1 1| Reg |
// ASL/ASR mem          |1 1 1 0|0 0 0|D|1 1| EaM | EaR |
// LSL/LSR mem          |1 1 1 0|0 0 1|D|1 1| EaM | EaR |
// ROL/ROR mem          |1 1 1 0|0 1 1|D|1 1| EaM | EaR |
void decode_ASL_ASR_LSL_LSR_ROL_ROR(const short* table, char* result)
{
    short dir = (table[0]  & 0b0000000100000000) >> 8;
    short size = (table[0] & 0b0000000011000000) >> 6;
    short reg = (table[0]  & 0b0000000000000111);
    short i = (table[0]    & 0b0000000000100000) >> 5;
    short type = 0;
    const char instr[][4] = { "ASL", "ASR", "LSL", "LSR", "ROL", "ROR" };

    if (size == 0b11)
    {
        switch ((table[0] & 0b0000111100000000) >> 8)
        {
            case 0b0000: type = 1; break;   // ASR
            case 0b0001: type = 0; break;   // ASL
            case 0b0010: type = 3; break;   // LSR
            case 0b0011: type = 2; break;   // LSL
            case 0b0110: type = 5; break;   // ROR
            case 0b0111: type = 4; break;   // ROL
        }
        switch((table[0] & 0b111111))
        {
            case 0b111000:  sprintf(result, "%s.W %s", instr[type], decode_ea(table[0] & 0b111111, table, 0, 1)); break;
            case 0b111001:  sprintf(result, "%s.L %s", instr[type], decode_ea(table[0] & 0b111111, table, 0, 2)); break;
            default:        sprintf(result, "%s %s", instr[type], decode_ea(table[0] & 0b111111, table, 0, size)); break;
        }
    }
    else
    {
        switch ((dir * 4) + (((table[0] & 0b11000) >> 3) & 0b11))
        {
            case 0b000: type = 1; break;    // ASR
            case 0b001: type = 3; break;    // LSR
            case 0b011: type = 5; break;    // ROR
            case 0b100: type = 0; break;    // ASL
            case 0b101: type = 2; break;    // LSL
            case 0b111: type = 4; break;    // ROL
        }
        if (i == 0) // Immediate
        {
            short count = (table[0] & 0b0000111000000000) >> 9;
            if (count == 0)
                count = 8;
            sprintf(result, "%s.%c #%d, D%d", instr[type], "BWL"[size], count, reg);
        }
        else // Register
        {
            short reg2 = (table[0] & 0b0000111000000000) >> 9;
            sprintf(result, "%s.%c D%d, D%d", instr[type], "BWL"[size], reg2, reg);
        }
    }
}

//                      |5|4|3|2|1|0
//                      |1|1|1|1|1|1|9|8|7|6|5|4|3|2|1|0|
//                      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// Bcc                  |0 1 1 0| Cond  | 8 bit depl    |
void decode_Bcc(const short* table, char* result)
{
    short cond = (table[0] & 0b0000111100000000) >> 8;
    short depl = (short)(table[0] & 0xFF);
    const char* conds[] = {
        "BRA", "BSR", "BHI", "BLS", "BCC", "BCS", "BNE", "BEQ",
        "BVC", "BVS", "BPL", "BMI", "BGE", "BLT", "BGT", "BLE"
    };
    const char* typ[] = { "S", "W"};
    int type = 0;
    if (depl == 0)
    {
        type = 1;
        depl = (int16_t)(table[1]);
    }
    sprintf(result, "%s.%s #$%X", conds[cond], typ[type], (int32_t)(depl));
}

//                      |5|4|3|2|1|0
//                      |1|1|1|1|1|1|9|8|7|6|5|4|3|2|1|0|
//                      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// BTST                 |0 0 0 0| Reg |1 0 0| EaM | EaR |
// BCHG                 |0 0 0 0| Reg |1 0 1| EaM | EaR |
// BCLR                 |0 0 0 0| Reg |1 1 0| EaM | EaR |
// BSET                 |0 0 0 0| Reg |1 1 1| EaM | EaR |
// BTST                 |0 0 0 0 1 0 0 0 0 0| EaM | EaR |   |0 0 0 0 0 0 0 0|x x x x x x x x|
// BCHG                 |0 0 0 0 1 0 0 0 0 1| EaM | EaR |   |0 0 0 0 0 0 0 0|x x x x x x x x|
// BCLR                 |0 0 0 0 1 0 0 0 1 0| EaM | EaR |   |0 0 0 0 0 0 0 0|x x x x x x x x|
// BSET                 |0 0 0 0 1 0 0 0 1 1| EaM | EaR |   |0 0 0 0 0 0 0 0|x x x x x x x x|
// MOVEP                |0 0 0 0| Reg | OpM |0 0 1| Reg |
void decode_BTST_BCHG_BCLR_BSET(const short* table, char* result)
{
    short reg = (table[0] & 0b000111000000000) >> 9;
    short eam = (table[0] & 0b111000) >> 3;
    short type = (table[0] & 0b111000000) >> 6;

    if (eam == 0b001)
    {
        short size = (table[0] & 0b1000000) >> 6;
        short sens = (table[0] & 0b10000000) >> 7;

        if (sens == 0)
            sprintf(result, "MOVEP.%c %X(A%d), D%d", "WL"[size], (int16_t)(table[1]) & 0xFFFF, table[0] & 0b111, reg);
        else
            sprintf(result, "MOVEP.%c D%d, %X(A%d)", "WL"[size], reg, (int16_t)(table[1]) & 0xFFFF, table[0] & 0b111);
    }
    else
    {
        switch (type)
        {
            case 0b000:     sprintf(result, "BTST #%d, %s", table[1] & 0xFF, decode_ea(table[0] & 0b111111, table, 1, 1)); break;
            case 0b001:     sprintf(result, "BCHG #%d, %s", table[1] & 0xFF, decode_ea(table[0] & 0b111111, table, 1, 1)); break;
            case 0b010:     sprintf(result, "BCLR #%d, %s", table[1] & 0xFF, decode_ea(table[0] & 0b111111, table, 1, 1)); break;
            case 0b011:     sprintf(result, "BSET #%d, %s", table[1] & 0xFF, decode_ea(table[0] & 0b111111, table, 1, 1)); break;
            case 0b100:     sprintf(result, "BTST D%d, %s", reg, decode_ea(table[0] & 0b111111, table, 0, 1)); break;
            case 0b101:     sprintf(result, "BCHG D%d, %s", reg, decode_ea(table[0] & 0b111111, table, 0, 1)); break;
            case 0b110:     sprintf(result, "BCLR D%d, %s", reg, decode_ea(table[0] & 0b111111, table, 0, 1)); break;
            case 0b111:     sprintf(result, "BSET D%d, %s", reg, decode_ea(table[0] & 0b111111, table, 0, 1)); break;
        }
    }
}

//                      |5|4|3|2|1|0
//                      |1|1|1|1|1|1|9|8|7|6|5|4|3|2|1|0|
//                      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// CHK                  |0 1 0 0| Reg |1 1 0| EaM | EaR |
void decode_CHK(const short* table, char* result)
{
    short reg = (table[0]  & 0b0000111000000000) >> 9;

    sprintf(result, "CHK %s, D%d", decode_ea(table[0] & 0b111111, table, 0, 1), reg);
}

//                      |5|4|3|2|1|0
//                      |1|1|1|1|1|1|9|8|7|6|5|4|3|2|1|0|
//                      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// CLR                  |0 1 0 0 0 0 1 0|Siz| EaM | EaR |
void decode_CLR(const short* table, char* result)
{
    short size = (table[0] & 0b11000000) >> 6;
    sprintf(result, "CLR.%c %s", "BWL"[size], decode_ea(table[0] & 0b111111, table, 0, size));
}

//                      |5|4|3|2|1|0
//                      |1|1|1|1|1|1|9|8|7|6|5|4|3|2|1|0|
//                      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// CMP/CMPA/CMPM/EOR    |1 0 1 1| Reg | OpM | EaM | EaR |
void decode_CMP_CMPA_CMPM_EOR(const short* table, char* result)
{
    short opmode = (table[0] & 0b000000111000000) >> 6;
    short reg = (table[0] & 0b000111000000000) >> 9;
    short eam = (table[0] & 0b111000) >> 3;

    switch (opmode)
    {
        case 0b000: // CMP.B
        case 0b001: // CMP.W
        case 0b010: // CMP.L
            sprintf(result, "CMP.%c %s, D%d", "BWL"[opmode & 0b11], decode_ea(table[0] & 0b111111, table, 0, opmode & 0b11), reg);
            break;
        case 0b100: // EOR.B ou CMPM.B
        case 0b101: // EOR.W ou CMPM.W
        case 0b110: // EOR.L ou CMPM.L
            if (eam == 0b001)   { sprintf(result, "CMPM.%c (A%d)+, (A%d)+", "BWL"[opmode & 0b11], reg, table[0] & 0b111); }
            else                { sprintf(result, "EOR.%c D%d, %s", "BWL"[opmode & 0b11], reg, decode_ea(table[0] & 0b111111, table, 0, opmode & 0b11)); }
            break;
        case 0b011: // CMPA.W
        case 0b111: // CMPA.L
            sprintf(result, "CMPA.%c %s, A%d", "WL"[(opmode & 0b100)>>2], decode_ea(table[0] & 0b111111, table, 0, ((opmode & 0b100)>>2)+1), reg);
            break;
        default:
            sprintf(result, "UNKNOWN CMP/CMPA/CMPM/EOR");
            break;
    }
}

//                      |5|4|3|2|1|0
//                      |1|1|1|1|1|1|9|8|7|6|5|4|3|2|1|0|
//                      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// CMPI                 |0 0 0 0 1 1 0 0|Siz| EaM | EaR |
void decode_CMPI(const short* table, char* result)
{
    short size = (table[0] & 0b0000000011000000) >> 6;
    switch (size)
    {
        case 0b00: // Byte
            sprintf(result, "CMPI.B #$%X, %s", table[1] & 0xFF, decode_ea(table[0] & 0b111111, table, 1, size));
            break;
        case 0b01: // Word
            sprintf(result, "CMPI.W #$%X, %s", table[1] & 0xFFFF, decode_ea(table[0] & 0b111111, table, 1, size));
            break;
        case 0b10: // Long
            sprintf(result, "CMPI.L #$%X, %s", (table[1] << 16) + (table[2] & 0xFFFF), decode_ea(table[0] & 0b111111, table, 2, size));
            break;
        default:
            sprintf(result, "CMPI Error Size");
            break;
    }
}


//                      |5|4|3|2|1|0
//                      |1|1|1|1|1|1|9|8|7|6|5|4|3|2|1|0|
//                      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// DBcc                 |0 1 0 1| Cond  |1 1 0 0 1| Reg |
// Scc                  |0 1 0 1| Cond  |1 1| EaM | EaR |
void decode_DBcc_Scc(const short* table, char* result)
{
    short cond = (table[0] & 0b0000111100000000) >> 8;
    short eam = (table[0] & 0b111000) >> 3;

    if (eam == 0b001)
    {
        short reg = (table[0] & 0b0000000000000111);
        short depl = (int16_t)(table[1]);
        const char* conds[] = {
            "DBT", "DBF", "DBHI", "DBLS", "DBCC", "DBCS", "DBNE", "DBEQ",
            "DBVC", "DBVS", "DBPL", "DBMI", "DBGE", "DBLT", "DBGT", "DBLE" };
        sprintf(result, "%s D%d, $%X", conds[cond], reg, (int16_t)(depl) & 0xFFFF);
    }
    else
    {
        const char* conds[] = {
            "ST", "SF", "SHI", "SLS", "SCC", "SCS", "SNE", "SEQ",
            "SVC", "SVS", "SPL", "SMI", "SGE", "SLT", "SGT", "SLE" };
        sprintf(result, "%s %s", conds[cond], decode_ea(table[0] & 0b111111, table, 0, 0));
    }
}

//                      |5|4|3|2|1|0
//                      |1|1|1|1|1|1|9|8|7|6|5|4|3|2|1|0|
//                      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// NOP                  |0 1 0 0 1 1 1 0 0 1 1 1 0 0 0 1|
// STOP                 |0 1 0 0 1 1 1 0 0 1 1 1 0 0 1 0|
// RTE                  |0 1 0 0 1 1 1 0 0 1 1 1 0 0 1 1|
// RTD                  |0 1 0 0 1 1 1 0 0 1 1 1 0 1 0 0|
// RTS                  |0 1 0 0 1 1 1 0 0 1 1 1 0 1 0 1|
// TRAPV                |0 1 0 0 1 1 1 0 0 1 1 1 0 1 1 0|
// RTR                  |0 1 0 0 1 1 1 0 0 1 1 1 0 1 1 1|
void decode_RTD_RTS_RTE_RTR_STOP_TRAPV(const short* table, char* result)
{
    switch (table[0] & 0b111)
    {
        case 0b001:   sprintf(result, "NOP"); break;
        case 0b010:   sprintf(result, "STOP #$%X", (int16_t)(table[1]) & 0xFFFF); break;
        case 0b011:   sprintf(result, "RTE"); break;
        case 0b100:   sprintf(result, "RTD #$%X", (int16_t)(table[1]) & 0xFFFF); break;
        case 0b101:   sprintf(result, "RTS"); break;
        case 0b110:   sprintf(result, "TRAPV"); break;
        case 0b111:   sprintf(result, "RTR"); break;
        default:      sprintf(result, "UNKNOWN RTD/RTS/RTE/RTR/STOP/TRAPV"); break;
    }
}


//                      |5|4|3|2|1|0
//                      |1|1|1|1|1|1|9|8|7|6|5|4|3|2|1|0|
//                      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// DIVS                 |1 0 0 0| Reg |1 1 1| EaM | EaR |
void decode_DIVS(const short* table, char* result)
{
    short reg = (table[0] & 0b000111000000000) >> 9;

    sprintf(result, "DIVS.W %s, D%d", decode_ea(table[0] & 0b111111, table, 0, 1), reg);
}

//                      |5|4|3|2|1|0
//                      |1|1|1|1|1|1|9|8|7|6|5|4|3|2|1|0|
//                      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// DIVU                 |1 0 0 0| Reg |0 1 1| EaM | EaR |
void decode_DIVU(const short* table, char* result)
{
    short reg = (table[0] & 0b000111000000000) >> 9;

    sprintf(result, "DIVU.W %s, D%d", decode_ea(table[0] & 0b111111, table, 0, 1), reg);
}

//                      |5|4|3|2|1|0
//                      |1|1|1|1|1|1|9|8|7|6|5|4|3|2|1|0|
//                      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// EOR                  |1 0 1 1| Reg | OpM | EaM | EaR |
void decode_EOR(const short* table, char* result)
{
    short opmode = (table[0] & 0b000000111000000) >> 6;
    short reg = (table[0] & 0b000111000000000) >> 9;

    switch (opmode)
    {
        case 0b100: // EOR.B
        case 0b101: // EOR.W
        case 0b110: // EOR.L
            sprintf(result, "EOR.%c D%d, %s", "BWL"[opmode & 0b11], reg, decode_ea(table[0] & 0b111111, table, 0, opmode & 0b11));
            break;
        default:
            sprintf(result, "UNKNOWN EOR");
            break;
    }
}


//                      |5|4|3|2|1|0
//                      |1|1|1|1|1|1|9|8|7|6|5|4|3|2|1|0|
//                      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// EORI                 |0 0 0 0 1 0 1 0|Siz| EaM | EaR |
// EORItoCCR            |0 0 0 0 1 0 1 0 0 0 1 1 1 1 0 0|   |0 0 0 0 0 0 0 0|x x x x x x x x| 
// EORItoSR             |0 0 0 0 1 0 1 0 0 1 1 1 1 0 0 1|   |x x x x x x x x x x x x x x x x|
void decode_EORI_EORItoCCR_EORItoSR(const short* table, char* result)
{
    if ((table[0] & 0b1111111) == 0b0111100) // EORItoCCR
    {
        sprintf(result, "EORI #$%X, CCR", table[1] & 0xFF);
    }
    else if ((table[0] & 0b1111111) == 0b1111100) // EORItoSR
    {
        sprintf(result, "EORI #$%X, SR", table[1] & 0xFFFF);
    }
    else
    {
        short size = (table[0] & 0b0000000011000000) >> 6;
        switch (size)
        {
            case 0b00: // Byte
                sprintf(result, "EORI.%c #$%X, %s", "BWL"[size], table[1] & 0xFF, decode_ea(table[0] & 0b111111, table, 1, size));
                break;
            case 0b01: // Word
                sprintf(result, "EORI.%c #$%X, %s", "BWL"[size], table[1] & 0xFFFF, decode_ea(table[0] & 0b111111, table, 1, size));
                break;
            case 0b10: // Long
                sprintf(result, "EORI.%c #$%X, %s", "BWL"[size], (table[1] << 16) + (table[2] & 0xFFFF), decode_ea(table[0] & 0b111111, table, 2, size));
                break;
            default:
                sprintf(result, "EORI Error Size");
                break;
        }
    }
}

//                      |5|4|3|2|1|0
//                      |1|1|1|1|1|1|9|8|7|6|5|4|3|2|1|0|
//                      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// EXT                  |0 1 0 0 1 0 0| OpM |0 0 0| Reg |
// SWAP                 |0 1 0 0 1 0 0 0 0 1 0 0 0| Reg |
void decode_EXT_SWAP(const short* table, char* result)
{
    short reg = (table[0] & 0b111);
    short opmode = (table[0] & 0b111000000) >> 6;
    switch (opmode)
    {
        case 0b001: // SWAP
            sprintf(result, "SWAP D%d", reg);
            break;
        case 0b010: // EXT.W
            sprintf(result, "EXT.W D%d", reg);
            break;
        case 0b011: // EXT.L
            sprintf(result, "EXT.L D%d", reg);
            break;
        default:
            sprintf(result, "UNKNOWN EXT/SWAP");
            break;
    }
}

//                      |5|4|3|2|1|0
//                      |1|1|1|1|1|1|9|8|7|6|5|4|3|2|1|0|
//                      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// PEA                  |0 1 0 0 1 0 0 0 0 1| EaM | EaR |
void decode_PEA(const short* table, char* result)
{
    sprintf(result, "PEA %s", decode_ea(table[0] & 0b111111, table, 0, 1));
}

//                      |5|4|3|2|1|0
//                      |1|1|1|1|1|1|9|8|7|6|5|4|3|2|1|0|
//                      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// MOVEM                |0 1 0 0 1|d|0 0 1|S| EaM | EaR |
void decode_MOVEM(const short* table, char* result)
{
    short d = (table[0] & 0b10000000000) >> 10;
    short s = (table[0] & 0b1000000) >> 6;
    short ea_mode = (table[0] & 0b111000) >> 3;
    if (d == 0) // Reg to Mem
    {
        sprintf(result, "MOVEM.%c %s, %s", "WL"[s], decode_reglist(table[1], ea_mode), decode_ea(table[0] & 0b111111, table, 1, 1));
    }
    else // Mem to Reg
    {
        sprintf(result, "MOVEM.%c %s, %s", "WL"[s], decode_ea(table[0] & 0b111111, table, 1, 1), decode_reglist(table[1], ea_mode));
    }
}

//                      |5|4|3|2|1|0
//                      |1|1|1|1|1|1|9|8|7|6|5|4|3|2|1|0|
//                      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// ILLEGAL              |0 1 0 0 1 0 1 0 1 1 1 1 1 1 0 0|
void decode_ILLEGAL(const short* table, char* result)
{
    sprintf(result, "ILLEGAL");
}


//                      |5|4|3|2|1|0
//                      |1|1|1|1|1|1|9|8|7|6|5|4|3|2|1|0|
//                      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+



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
        { 0b1110000000000000, 0b1111000000000000, &decode_ASL_ASR_LSL_LSR_ROL_ROR },
        { 0b0110000000000000, 0b1111000000000000, &decode_Bcc },
        { 0b0000000100000000, 0b1111000100000000, &decode_BTST_BCHG_BCLR_BSET },
        { 0b0000100000000000, 0b1111111100000000, &decode_BTST_BCHG_BCLR_BSET },
        { 0b0100000110000000, 0b1111000111000000, &decode_CHK },
        { 0b0100001000000000, 0b1111111100000000, &decode_CLR },
        { 0b1011000000000000, 0b1111000000000000, &decode_CMP_CMPA_CMPM_EOR },
        { 0b0000110000000000, 0b1111111100000000, &decode_CMPI },
        { 0b0101000011000000, 0b1111000011000000, &decode_DBcc_Scc },
        { 0b0100111001110000, 0b1111111111111000, &decode_RTD_RTS_RTE_RTR_STOP_TRAPV },
        { 0b1000000111000000, 0b1111000111000000, &decode_DIVS },
        { 0b1000000011000000, 0b1111000111000000, &decode_DIVU },
        { 0b1011000000000000, 0b1111000000000000, &decode_EOR },
        { 0b0000101000000000, 0b1111111100000000, &decode_EORI_EORItoCCR_EORItoSR },
        { 0b0100100000000000, 0b1111111000111000, &decode_EXT_SWAP },
        { 0b0100100001000000, 0b1111111111000000, &decode_PEA },
        { 0b0100100010000000, 0b1111101110000000, &decode_MOVEM },
        { 0b0100101011111100, 0b1111111111111111, &decode_ILLEGAL },
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