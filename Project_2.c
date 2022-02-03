/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <stdio.h>
#include <stdlib.h>
int memory[256];
int reg[32];
int i, inst, PC, cycles_to_halt,Cycle ;
int op,rs,rt,rd,immed,funct,extend,ctrl,RegDst,ALUSrc,rd1,rd2,wn,wd,ALUOut,pc4;
int offset,btgt,Zero,PCSrc,pc_next,MemRead,memout,MemtoReg,RegWrite,MemWrite,mem;

int WB_memout,WB_ALUOut,WB_RegRd,WB_ctrl,WB_MemtoReg,WB_RegRd,WB_RegWrite;
int MEMWB_memout, MEMWB_ALUOut, MEMWB_RegRd, MEMWB_ctrl,MEM_PCSrc;
int EXMEM_btgt, EXMEM_zero, EXMEM_ALUOut, EXMEM_rd2, EXMEM_RegRd, EXMEM_ctrl;
int EX_MemtoReg,EX_RegWrite,EX_MemRead,EX_MemWrite,EX_rd1,EX_rd2,EX_PCSrc;
int IDEX_pc4, IDEX_rd1, IDEX_rd2, IDEX_extend, IDEX_rs, IDEX_rt, IDEX_rd, IDEX_ctrl,IDEX_inst;
int IFID_pc4, IFID_inst;

int MEM_memout, MEM_ALUOut, MEM_RegRd, MEM_ctrl,MEM_btgt,MEM_zero,MEM_rd2,MEM_MemWrite,MEM_RegWrite;
int MEM_MemRead,MEM_MemtoReg;
int EX_btgt, EX_zero, EX_ALUOut, EX_rd2, EX_RegRd,EX_inst;
int EXMEM_RegRd, EXMEM_ctrl,EX_offset,EX_funct,EX_RegDst;
int ID_pc4, ID_rd1, ID_rd2, ID_extend, ID_rs, ID_rt, ID_rd, ID_ctrl,ID_inst,ID_immed,ID_op;
int IF_pc4, IF_inst,IF_pc,IF_pc_next;
int EX_pc4, EX_rd1, EX_rd2, EX_extend, EX_rs, EX_rt, EX_rd, EX_ctrl,EX_shamt,EX_ALUSrc;



// Set up and initialize the register array (e.g., register[32]).
// Initialize the memory array (e.g. memory[256]) by copying the contents in the input binary file.
// Note that it is 128 data words and 128 instruction words, totaling 256 items in the memory array.
// The data segment begins at memory[0] while the text segment begins at memory[128].

void initialize() {

    
   for (int i=0; i<32; i++) // skip data section
     reg[i]=0;
    //printf("Register Initializion done \n");
  FILE *sourcefile;
  
 //printf("Memory Initializion started \n");
 // Add your File Path
  sourcefile = fopen("/Users/mehulsrivastava/Desktop/Proj/memory.out", "rb");
  for (int i=0; i<256; i++) // skip data section
        fread(&memory[i], sizeof(int), 1, sourcefile);
    //printf("Memory Initializion done \n");

// initialize all pipeline registers
    WB_memout = 0, WB_ALUOut = 0, WB_RegRd = 0, WB_ctrl = 0;

// MEMWB_memout, MEMWB_ALUOut, MEMWB_RegRd, MEMWB_ctrl
    MEMWB_memout=0;
    MEMWB_ALUOut=0;
    MEMWB_RegRd=0;
    MEMWB_ctrl=0;
    MEM_MemtoReg=0;

// EXMEM_btgt, EXMEM_zero, EXMEM_ALUOut, EXMEM_rd2, EXMEM_RegRd, EXMEM_ctrl
    EXMEM_btgt=0;
    EXMEM_zero =0, EXMEM_ALUOut=0, EXMEM_rd2=0, EXMEM_RegRd=0, EXMEM_ctrl=0,EX_inst=0;

// IDEX_pc4, IDEX_rd1, IDEX_rd2, IDEX_extend, IDEX_rs, IDEX_rt, IDEX_rd, IDEX_ctrl
    IDEX_pc4=0, IDEX_rd1=0, IDEX_rd2=0, IDEX_extend=0, IDEX_rs=0, IDEX_rt=0, IDEX_rd=0, IDEX_ctrl=0;
    ID_extend=0;

// IFID_pc4, IFID_inst
    IFID_pc4 = 0; 
    IFID_inst=0;
}



// Read pipeline registers to set stage signals which comes directly from the pipeline registers

read_pipeline_registers() {

// WB stage – WB_memout, WB_ALUOut, WB_RegRd, WB_ctrl
WB_memout = MEMWB_memout, WB_ALUOut = MEMWB_ALUOut, WB_RegRd = MEMWB_RegRd, WB_ctrl = MEMWB_ctrl;

// MEM stage – MEM_btgt, MEM_zero, MEM_ALUOut, MEM_rd2, MEM_RegRd, MEM_ctrl
MEM_btgt = EXMEM_btgt, MEM_zero = EXMEM_zero, MEM_ALUOut = EXMEM_ALUOut;
MEM_rd2 = EXMEM_rd2, MEM_RegRd = EXMEM_RegRd, MEM_ctrl = EXMEM_ctrl;

// EX stage – EX_pc4, EX_rd1, EX_rd2, EX_extend, EX_rs, EX_rt, EX_rd, EX_ctrl
EX_ctrl = IDEX_ctrl;
EX_pc4 = IDEX_pc4;
EX_rd1 = IDEX_rd1; EX_rd2 = IDEX_rd2;
EX_extend = IDEX_extend;
EX_rs = IDEX_rs; EX_rt = IDEX_rt; EX_rd = IDEX_rd;
EX_inst=IDEX_inst;

// ID stage – ID_pc4, ID_inst
ID_pc4 = IFID_pc4, ID_inst = IFID_inst;

}

// Carry out the operations and produce signals accordingly
carryout_operations() {
// WB stage – update WB_wd register[WB_RegRd]
// update the destination “register” if (WB_RegWrite==1)
//wn=rd ,wd=wb_regrd
//// if (MEM_MemRead) MEM_memout = memory[MEM_ALUOut/4];

if (WB_MemtoReg==1) ////LW //Addi // R type 

    WB_RegRd = MEMWB_ALUOut; //wn=rt  wd= [aluout/4]

if (WB_RegWrite==1) 
//reg[WB_RegRd] = wd; //wn=rt  wd= [aluout/4]
reg[MEMWB_RegRd]=WB_RegRd;


// MEM stage – update MEM_PCSrc and MEM_memout
// when accessing memory, be sure to divide the memory address by four


if (MEM_PCSrc==0) 
    pc_next=PC+4;
else 
    pc_next=EX_btgt;

 if (MEM_MemRead) 
{
 MEM_memout = memory[EX_ALUOut/4];
    if (MEM_RegWrite== 1)
    {
   
reg[EXMEM_RegRd]=MEM_memout;
   
    }

}

// EX stage – update EX_offset, EX_btgt, EX_ALUOut, EX_Zero, EX_funct, and EX_RegRd
EX_offset = EX_extend << 2;
EX_offset = (EX_inst>>6) &0x1F; 
EX_zero = 0;

EX_funct=(EX_inst>>0) &0x3F;
EX_shamt = (EX_inst>>6) &0x1F; 
EX_ALUSrc = (EX_ctrl >> 9) & 1; //ALUSrc = (EX_ctrl >> 9) & 1;
EX_btgt=pc4+ (immed*4);
//get all control signals
EX_RegDst   = (EX_ctrl >>10) & 1;//RegDst = (ctrl >> 10) & 1;
EX_MemtoReg = (EX_ctrl >> 8) & 1;
EX_RegWrite = (EX_ctrl >> 7) & 1;
EX_MemRead  = (EX_ctrl >> 6) & 1;
EX_MemWrite = (EX_ctrl >> 5) & 1;
EX_PCSrc=(EX_ctrl >> 4) & 1; //branch
EX_rd1 = ID_rd1; EX_rd2 = ID_rd2;

if (EX_RegDst==0) EX_RegRd=0;
else EX_RegRd=EX_ALUOut; // R Type


if (EX_ctrl==1416) { // R-type
if (EX_funct==32) EX_ALUOut =reg[ID_rs] +reg[ID_rt] ; // add // add //reg[rd] = rd1+rd2;
else if (EX_funct==34) EX_ALUOut = reg[ID_rs]-reg[ID_rt]; // sub //reg[rd] = rd1 - rd2;
else if (EX_funct==0) EX_ALUOut = reg[ID_rt]<< EX_shamt; // sll sll R R[rd] = R[rt] << shamt
else if (EX_funct==2) EX_ALUOut = reg[ID_rt]>> EX_shamt; // srl // srl  R[rd] = R[rt] >> shamt
else if (EX_funct==42) EX_ALUOut = (reg[ID_rs]<reg[ID_rt] )? 1 : 0;  // slt R[rd] = (R[rs] < R[rt]) ? 1 : 0
else if (EX_funct==12) EX_ALUOut = 0; // halt
}

else if ((EX_ctrl==704) || (EX_ctrl==544) || (EX_ctrl==896)) 
{
EX_ALUOut = reg[ID_rs] + ID_immed;//ALUOUT = R[rs]+SignExtImm

//lw or sw or addi
EX_RegRd=IDEX_rt;
}

pc4 = PC;
EX_zero = 0;
 if ((EX_ctrl==20) || (EX_ctrl==6)) 
{


 //if immed is positive prepone it with 0 
    if ( ID_immed > 0 )
    {
   ID_immed=00000000+ID_immed;
    }
    //else 1 
    else
    {
   ID_immed=11111111+ID_immed;
   }

EX_btgt=IDEX_pc4 + (ID_immed*4);


  
    if (EX_ctrl==20) 
        {
           if(ID_rd1 == ID_rd2) //beq
           EX_zero = 1;// this will be set oly when for beq rs - rt==0 , they will be same
        }
    if  (EX_ctrl==6)//bne
        {
           if(ID_rd1 != ID_rd2) //bnq
           EX_zero = 1;// this will be set oly when for beq rs - rt !=0 , 
        }
}

if ( EX_zero==0) 
    pc_next=PC+4;
else 
    pc_next=EX_btgt;

// ID stage – update ID_op, ID_rs, ID_rt, ID_rd, ID_immed, ID_extend, ID_rd1, ID_rd2, and ID_ctrl

ID_op = (ID_inst>>26) & 0x3F; // as we need 6 bits so & 0x3F
ID_rs = (ID_inst>>21) &0x1F;  // as we need 5 bits so & 0x1F
ID_rt =  (ID_inst>>16)  &0x1F;  //as we need 5 bits so & 0x1F
ID_rd = (ID_inst>>11)  &0x1F;  //as we need 5 bits so & 0x1F
ID_immed = (ID_inst>>0) &0xFFFF;  // as we need 16 bits0xFFFF
ID_rd1 = reg[ID_rs]; 
ID_rd2 = reg[ID_rt]; //Register Data
ID_extend =ID_immed; 

// R-type
if(ID_op== 0x0)
{
ID_ctrl=1416; // R-type
}
// Beq-type
if(ID_op== 0x4)
{
ID_ctrl=20; // Beq
}
//Addi
else if(ID_op== 0x8)
{
//addi 
ID_ctrl=896;
}
else if(ID_op== 0x23)
{
//LW 
ID_ctrl=704;
}
else if(ID_op== 0x2b)
{
//SW 
ID_ctrl=544;
}


// IF stage – update IF_inst, IF_pc, IF_pc4, and IF_pc_next
IF_inst = memory[PC/4];
IF_pc4=pc_next;
 if (MEM_PCSrc==0) 
 IF_pc_next=pc_next;
 else 
{
  IF_pc_next=EX_btgt;
}
}


update_mem() {
// update memory if appropriate ( Only for SW)
//for Sw
if (MEM_MemWrite==1) 
    {   
    memory[MEM_RegRd]=MEM_ALUOut;
    }
// Update memory based on signals as a result of carryout_operations()
// WB stage – nothing to update
// MEM stage – update memory if appropriate
// if (MEM_MemWrite==1) memory[MEM_ALUOut]=________;
// EX stage – nothing to update
// ID stage – nothing to update
// IF stage – nothing to update
}

// Update the pipeline registers
update_pipeline_registers() {

WB_MemtoReg=MEM_MemtoReg;
WB_RegWrite=MEM_RegWrite;

// MEM stage – MEMWB_memout, MEMWB_ALUOut, MEMWB_RegRd, MEMWB_ctrl
MEMWB_memout= MEM_memout, MEMWB_ALUOut = MEM_ALUOut, MEMWB_RegRd = MEM_RegRd;
MEMWB_ctrl = MEM_ctrl;
MEM_PCSrc = EX_PCSrc; 
MEM_MemRead =EX_MemRead;
MEM_RegWrite=EX_RegWrite;
MEM_RegRd=EX_RegRd;
MEM_MemWrite=EX_MemWrite;
MEM_MemtoReg=EX_MemtoReg;

// EX stage – EXMEM_btgt, EXMEM_zero, EXMEM_ALUOut, EXMEM_rd2, EXMEM_RegRd,
EXMEM_ctrl = EX_ctrl;
EXMEM_btgt = EX_btgt;
EXMEM_ALUOut = EX_ALUOut;
EXMEM_zero = EX_zero;
EXMEM_rd2 = EX_rd2;
EXMEM_RegRd = EX_RegRd;

// ID stage – IDEX_pc4, IDEX_rd1, IDEX_rd2, IDEX_extend, IDEX_rs, IDEX_rt, IDEX_rd, IDEX_ctrl
IDEX_inst= ID_inst;
IDEX_pc4 = ID_pc4; 
IDEX_rd1 = ID_rd1;
IDEX_rd2 = ID_rd2;
IDEX_extend = ID_extend;
IDEX_rs =  ID_rs;
IDEX_rt = ID_rt;
IDEX_rd = ID_rd;
IDEX_ctrl = ID_ctrl;

// IF stage – IFID_pc4, IFID_inst
IFID_pc4 = IF_pc4, IFID_inst = IF_inst;
}

// print the clock count, the content of PC, data memory, and register file as follows
print_results() 
{
    printf("Cycle=%d,PC=%x\n",Cycle,PC);
    printf("DM:           ");
    for (int i=0; i<16; i++) // memory section
    printf(" %d  ",memory[i]);
    printf("\nRegFile:      ");
    for (int i=0; i<16; i++) // register section
    printf(" %d  ",reg[i]);
    printf("\n\nIF/ID (pc4, inst)                                  %x  %x \n",IF_pc4,IF_inst);//pc_next
    printf("ID/EX (pc4, rd1, rd2, extend, rs, rt, rd, ctrl)    %X, %d, %d, %X, %d, %d, %d, %X\n",IDEX_pc4, IDEX_rd1, IDEX_rd2, IDEX_extend, IDEX_rs, IDEX_rt, IDEX_rd, IDEX_ctrl);
    printf("EX/MEM (btgt, zero, ALUOut, rd2, RegRd, ctrl)      %X, %X, %d, %d, %d, %X \n",EXMEM_btgt, EXMEM_zero, EXMEM_ALUOut, EXMEM_rd2, EXMEM_RegRd, EXMEM_ctrl);
    printf("MEM/WB (memout, ALUOut, RegRd, ctrl)               %d, %d, %d, %X\n",MEMWB_memout, MEMWB_ALUOut, MEMWB_RegRd, MEMWB_ctrl);
    printf("\n---------------------------------------------------------------\n");
    
}

update_pc() {
// Update pc based on signals as a result of carryout_operations()
   PC=pc_next;
}


int main() {
  // Printing Student name and Student ID 
 printf("Student Name : Anubhuti Dayal \n"); 
 printf("Student ID  : 2824826 \n");   
 printf("this is working fine for all operations\n\n\n\n\n\n"); 

 initialize();
  PC = 512; // equivalent to 0x200
  cycles_to_halt = 1000; //maximum number of cycles to execute
  Cycle=0;
  
while(1)

   {
   //inst=memory[PC/4]; 
   IF_inst = memory[PC/4];
    Cycle=Cycle+1;
    if (cycles_to_halt == 0) break;
    if (IF_inst == 12) cycles_to_halt = 4;
    if (cycles_to_halt > 0) cycles_to_halt --;

        read_pipeline_registers(); // beginning of a cycle
        carryout_operations(); // during the cycle
        // update the destination register early here (the first half of the cycle)
        update_mem(); // end of the cycle (rising edge of the clock) // make sure memory are updated in this function only
        update_pipeline_registers(); // end of the cycle (rising edge of the clock)
        print_results();
        update_pc(); // end of the cycle (rising edge of the clock)
     }

  return 0;

}


