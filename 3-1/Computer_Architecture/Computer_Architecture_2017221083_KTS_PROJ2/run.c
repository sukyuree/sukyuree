/***************************************************************/
/*                                                             */
/*   MIPS-32 Instruction Level Simulator                       */
/*                                                             */
/*   SCE212 Ajou University                                    */
/*   run.c                                                     */
/*   Adapted from CS311@KAIST                                  */
/*                                                             */
/***************************************************************/

#include <stdio.h>

#include "util.h"
#include "run.h"

/***************************************************************/
/*                                                             */
/* Procedure: get_inst_info                                    */
/*                                                             */
/* Purpose: Read insturction information                       */
/*                                                             */
/***************************************************************/
instruction* get_inst_info(uint32_t pc)
{
    return &INST_INFO[(pc - MEM_TEXT_START) >> 2];
}

/***************************************************************/
/*                                                             */
/* Procedure: process_instruction                              */
/*                                                             */
/* Purpose: Process one instrction                             */
/*                                                             */
/***************************************************************/
void process_instruction()
{
    instruction *inst = get_inst_info(CURRENT_STATE.PC);
    //printf("%d hi\n",inst->opcode);
    //int i =INSTRUCTION_COUNT;
    //printf("%d",inst->opcode);
    if(inst->opcode==0)/*(0x000000)ADDU, AND, NOR, OR, SLTU, SLL, SRL, SUBU  if JR*/{
        /*addu*/
        if(inst->func_code==33)
            CURRENT_STATE.REGS[inst->r_t.r_i.r_i.r.rd]=CURRENT_STATE.REGS[inst->r_t.r_i.rs]+CURRENT_STATE.REGS[inst->r_t.r_i.rt];
        /*and*/
        else if(inst->func_code==36)
            CURRENT_STATE.REGS[inst->r_t.r_i.r_i.r.rd]=CURRENT_STATE.REGS[inst->r_t.r_i.rs]&CURRENT_STATE.REGS[inst->r_t.r_i.rt];
        /*nor*/
        else if(inst->func_code==39)
            CURRENT_STATE.REGS[inst->r_t.r_i.r_i.r.rd]=~(CURRENT_STATE.REGS[inst->r_t.r_i.rs]|CURRENT_STATE.REGS[inst->r_t.r_i.rt]);
        /*or*/
        else if(inst->func_code==37)
            CURRENT_STATE.REGS[inst->r_t.r_i.r_i.r.rd]=(CURRENT_STATE.REGS[inst->r_t.r_i.rs]|CURRENT_STATE.REGS[inst->r_t.r_i.rt]);
        /*sltu*/
        else if(inst->func_code==43)
            CURRENT_STATE.REGS[inst->r_t.r_i.r_i.r.rd]=(CURRENT_STATE.REGS[inst->r_t.r_i.rs]<CURRENT_STATE.REGS[inst->r_t.r_i.rt])?1:0;
        /*sll*/
        else if(inst->func_code==0)
            CURRENT_STATE.REGS[inst->r_t.r_i.r_i.r.rd]=CURRENT_STATE.REGS[inst->r_t.r_i.rt]<<inst->r_t.r_i.r_i.r.shamt;
        /*srl*/
        else if(inst->func_code==2)
            CURRENT_STATE.REGS[inst->r_t.r_i.r_i.r.rd]=CURRENT_STATE.REGS[inst->r_t.r_i.rt]>>inst->r_t.r_i.r_i.r.shamt;
        /*subu*/
        else if(inst->func_code==35)
            CURRENT_STATE.REGS[inst->r_t.r_i.r_i.r.rd]=CURRENT_STATE.REGS[inst->r_t.r_i.rs]-CURRENT_STATE.REGS[inst->r_t.r_i.rt];
        /*jr*/
        else if(inst->func_code==8){
            CURRENT_STATE.PC = CURRENT_STATE.REGS[inst->r_t.r_i.rs]-4;
        }
    }
    else if(inst->opcode==9)/*(0x001001)ADDIU*/{CURRENT_STATE.REGS[inst->r_t.r_i.rt]=CURRENT_STATE.REGS[inst->r_t.r_i.rs]+inst->r_t.r_i.r_i.imm;}
    else if(inst->opcode==12)/*(0x001100)ANDI*/{CURRENT_STATE.REGS[inst->r_t.r_i.rt]=CURRENT_STATE.REGS[inst->r_t.r_i.rs]&inst->r_t.r_i.r_i.imm;}
    else if(inst->opcode==15)/*(0x001111)LUI*/{
        CURRENT_STATE.REGS[inst->r_t.r_i.rt]=inst->r_t.r_i.r_i.imm<<16;
        //printf("%d HI",CURRENT_STATE.REGS[inst->r_t.r_i.rt]);
        }
    else if(inst->opcode==13)/*0x001101)ORI*/{
        CURRENT_STATE.REGS[inst->r_t.r_i.rt]=CURRENT_STATE.REGS[inst->r_t.r_i.rs]|inst->r_t.r_i.r_i.imm;
    //    CURRENT_STATE.REGS[inst->r_t.r_i.rt]=((CURRENT_STATE.REGS[inst->r_t.r_i.rs]+(inst->r_t.r_i.r_i.imm)<<16));
        //printf("%d BYE\n",CURRENT_STATE.REGS[inst->r_t.r_i.rt]);
    }
    else if(inst->opcode==4)/*(0x000100)BEQ*/{
        if(CURRENT_STATE.REGS[inst->r_t.r_i.rs]==CURRENT_STATE.REGS[inst->r_t.r_i.rt]){
            CURRENT_STATE.PC=CURRENT_STATE.PC+(4*inst->r_t.r_i.r_i.imm);
        }
    }
    else if(inst->opcode==5)/*(0x000101)BNE*/{
        if(CURRENT_STATE.REGS[inst->r_t.r_i.rs]!=CURRENT_STATE.REGS[inst->r_t.r_i.rt]){
            CURRENT_STATE.PC=CURRENT_STATE.PC+(4*inst->r_t.r_i.r_i.imm);
        }
    }
    else if(inst->opcode==2)/*(0x000010)J*/{
        CURRENT_STATE.PC=inst->r_t.target-4;
    }
    else if(inst->opcode==3)/*(0x000011)JAL*/{
        CURRENT_STATE.REGS[31]=CURRENT_STATE.PC+8;
        CURRENT_STATE.PC=inst->r_t.target-4;
    }
    else if(inst->opcode==11)/*(0x001011)SLTIU*/{
        CURRENT_STATE.REGS[inst->r_t.r_i.rt]=(CURRENT_STATE.REGS[inst->r_t.r_i.rs]<inst->r_t.r_i.r_i.imm)?1:0;
    }
    else if(inst->opcode==35)/*(0x100011)LW*/{
        CURRENT_STATE.REGS[inst->r_t.r_i.rt]=mem_read_32(CURRENT_STATE.REGS[inst->r_t.r_i.rs]+inst->r_t.r_i.r_i.imm);
    }
    else if(inst->opcode==43)/*(0x101011)SW*/{
        mem_write_32(CURRENT_STATE.REGS[inst->r_t.r_i.rs]+inst->r_t.r_i.r_i.imm,CURRENT_STATE.REGS[inst->r_t.r_i.rt]);
        //CURRENT_STATE.REGS[inst->r_t.r_i.rt]=mem_read_32(CURRENT_STATE.REGS[CURRENT_STATE.REGS[inst->r_t.r_i.rs]]+inst->r_t.r_i.r_i.imm);
    }
    if(MEM_TEXT_START+NUM_INST*4==CURRENT_STATE.PC){
        RUN_BIT=FALSE;
        CURRENT_STATE.PC-=4;
    }
    CURRENT_STATE.PC+=4;
    //printf("0x%08x\n",CURRENT_STATE.PC);
}
