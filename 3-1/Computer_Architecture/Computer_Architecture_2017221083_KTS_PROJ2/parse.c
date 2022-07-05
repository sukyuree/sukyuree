/***************************************************************/
/*                                                             */
/*   MIPS-32 Instruction Level Simulator                       */
/*                                                             */
/*   SCE212 Ajou University                                    */
/*   parse.c                                                   */
/*   Adapted from CS311@KAIST                                  */
/*                                                             */
/***************************************************************/

#include <stdio.h>

#include "util.h"
#include "parse.h"

int text_size;
int data_size;

instruction parsing_instr(const char *buffer, const int index)
{
    //printf("%d\n",index);
    instruction instr;
    uint32_t value = strtol(buffer,NULL,2);
    instr.value = value;
    char opcode[7];
    opcode[6]='\0';
    char rs[6];
    opcode[0]=buffer[0];
    opcode[1]=buffer[1];
    opcode[2]=buffer[2];
    opcode[3]=buffer[3];
    opcode[4]=buffer[4];
    opcode[5]=buffer[5];
    short i = strtol(opcode, NULL, 2);
    instr.opcode=i;
    //printf("%x\n",i);
    rs[5]='\0';
    char rt[6];
    rt[5]='\0';
    char rd[6];
    rd[5]='\0';
    char shmt[6];
    shmt[5]='\0';
    char func[7];
    func[6]='\0';
    char imm[17];
    imm[16]='\0';
    char add[33];
    add[32]='\0';
    if(i==0){
        rs[0]=buffer[6];
        rs[1]=buffer[7];
        rs[2]=buffer[8];
        rs[3]=buffer[9];
        rs[4]=buffer[10];
        rt[0]=buffer[11];
        rt[1]=buffer[12];
        rt[2]=buffer[13];
        rt[3]=buffer[14];
        rt[4]=buffer[15];
        rd[0]=buffer[16];
        rd[1]=buffer[17];
        rd[2]=buffer[18];
        rd[3]=buffer[19];
        rd[4]=buffer[20];
        shmt[0]=buffer[21];
        shmt[1]=buffer[22];
        shmt[2]=buffer[23];
        shmt[3]=buffer[24];
        shmt[4]=buffer[25];
        func[0]=buffer[26];
        func[1]=buffer[27];
        func[2]=buffer[28];
        func[3]=buffer[29];
        func[4]=buffer[30];
        func[5]=buffer[31];
        unsigned char r_d = strtol(rd, NULL, 2);
        unsigned char r_s = strtol(rs, NULL, 2);
        unsigned char r_t = strtol(rt, NULL, 2);
        unsigned char shamt = strtol(shmt, NULL, 2);
        short function = strtol(func,NULL,2);
        instr.r_t.r_i.rs = r_s;
        instr.r_t.r_i.rt = r_t;
        instr.r_t.r_i.r_i.r.rd = r_d;
        instr.r_t.r_i.r_i.r.shamt = shamt;
        instr.func_code=function;
        //printf("%d %u %u %u %u %d\n",i, r_d, r_s, r_t, shamt, function);
    }
    else if(i==9||i==12||i==15||i==13||i==11||i==35||i==43||i==4||i==5){
        rs[0]=buffer[6];
        rs[1]=buffer[7];
        rs[2]=buffer[8];
        rs[3]=buffer[9];
        rs[4]=buffer[10];
        rt[0]=buffer[11];
        rt[1]=buffer[12];
        rt[2]=buffer[13];
        rt[3]=buffer[14];
        rt[4]=buffer[15];
        imm[0]=buffer[16];
        imm[1]=buffer[17];
        imm[2]=buffer[18];
        imm[3]=buffer[19];
        imm[4]=buffer[20];
        imm[5]=buffer[21];
        imm[6]=buffer[22];
        imm[7]=buffer[23];
        imm[8]=buffer[24];
        imm[9]=buffer[25];
        imm[10]=buffer[26];
        imm[11]=buffer[27];
        imm[12]=buffer[28];
        imm[13]=buffer[29];
        imm[14]=buffer[30];
        imm[15]=buffer[31];
        unsigned char r_s = strtol(rs, NULL, 2);
        unsigned char r_t = strtol(rt, NULL, 2);
        short immediate = strtol(imm, NULL, 2);
        instr.r_t.r_i.rs = r_s;
        instr.r_t.r_i.rt = r_t;
        instr.r_t.r_i.r_i.imm=immediate;
        
    }
    else if(i==2||i==3){
        add[0]='0';
        add[1]='0';
        add[2]='0';
        add[3]='0';
        add[4]=buffer[6];
        add[5]=buffer[7];
        add[6]=buffer[8];
        add[7]=buffer[9];
        add[8]=buffer[10];
        add[9]=buffer[11];
        add[10]=buffer[12];
        add[11]=buffer[13];
        add[12]=buffer[14];
        add[13]=buffer[15];
        add[14]=buffer[16];
        add[15]=buffer[17];
        add[16]=buffer[18];
        add[17]=buffer[19];
        add[18]=buffer[20];
        add[19]=buffer[21];
        add[20]=buffer[22];
        add[21]=buffer[23];
        add[22]=buffer[24];
        add[23]=buffer[25];
        add[24]=buffer[26];
        add[25]=buffer[27];
        add[26]=buffer[28];
        add[27]=buffer[29];
        add[28]=buffer[30];
        add[29]=buffer[31];
        add[30]='0';
        add[31]='0';
        uint32_t jump = strtol(add, NULL, 2);
        instr.r_t.target= jump;
        //printf("0x%08x HI\n",instr.r_t.target);
    }
    instr.value=MEM_TEXT_START+index;
    //mem_write_32(MEM_TEXT_START+index,buffer);
	/** Implement this function */
    //printf("%c%c%c%c%c%c\n",buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5]);
    return instr;
}

void parsing_data(const char *buffer, const int index)
{
	/** Implement this function */
    uint32_t data = strtol(buffer,NULL,2);
    mem_write_32(MEM_DATA_START+index,data);
    //printf("0x%08x\n",mem_read_32(MEM_DATA_START+index));
    //printf("\n");
}

void print_parse_result()
{
    int i;
    printf("Instruction Information\n");

    for(i = 0; i < text_size/4; i++)
    {
        printf("INST_INFO[%d].value : %x\n",i, INST_INFO[i].value);
        printf("INST_INFO[%d].opcode : %d\n",i, INST_INFO[i].opcode);

	    switch(INST_INFO[i].opcode)
        {
            //Type I
            case 0x9:		//(0x001001)ADDIU
            case 0xc:		//(0x001100)ANDI
            case 0xf:		//(0x001111)LUI	
            case 0xd:		//(0x001101)ORI
            case 0xb:		//(0x001011)SLTIU
            case 0x23:		//(0x100011)LW
            case 0x2b:		//(0x101011)SW
            case 0x4:		//(0x000100)BEQ
            case 0x5:		//(0x000101)BNE
                printf("INST_INFO[%d].rs : %d\n",i, INST_INFO[i].r_t.r_i.rs);
                printf("INST_INFO[%d].rt : %d\n",i, INST_INFO[i].r_t.r_i.rt);
                printf("INST_INFO[%d].imm : %d\n",i, INST_INFO[i].r_t.r_i.r_i.imm);
                break;

            //TYPE R
            case 0x0:		//(0x000000)ADDU, AND, NOR, OR, SLTU, SLL, SRL, SUBU  if JR
                printf("INST_INFO[%d].func_code : %d\n",i, INST_INFO[i].func_code);
                printf("INST_INFO[%d].rs : %d\n",i, INST_INFO[i].r_t.r_i.rs);
                printf("INST_INFO[%d].rt : %d\n",i, INST_INFO[i].r_t.r_i.rt);
                printf("INST_INFO[%d].rd : %d\n",i, INST_INFO[i].r_t.r_i.r_i.r.rd);
                printf("INST_INFO[%d].shamt : %d\n",i, INST_INFO[i].r_t.r_i.r_i.r.shamt);
                break;

            //TYPE J
            case 0x2:		//(0x000010)J
            case 0x3:		//(0x000011)JAL
                printf("INST_INFO[%d].target : %d\n",i, INST_INFO[i].r_t.target);
                break;

            default:
                printf("Not available instruction\n");
                assert(0);
        }
    }

    printf("Memory Dump - Text Segment\n");
    for(i = 0; i < text_size; i+=4)
        printf("text_seg[%d] : %x\n", i, mem_read_32(MEM_TEXT_START + i));
    for(i = 0; i < data_size; i+=4)
        printf("data_seg[%d] : %x\n", i, mem_read_32(MEM_DATA_START + i));
    printf("Current PC: %x\n", CURRENT_STATE.PC);
}
