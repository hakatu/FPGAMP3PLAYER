#ifndef _HEAD_H_
#define _HEAD_H_

struct music0
{
  char m_name[50];
  unsigned int cluster;
};


#include <string.h>
#include <io.h>
#define DW 0X20
#define BYTE    unsigned char
#define UINT16  unsigned int
#define UINT32  unsigned long


void Ncr(void);
void Ncc(void);

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------


BYTE SD_read_block(UINT32 block_number, BYTE *);
BYTE SD_write_block(UINT32 block_number, BYTE *);
void show_name(char *name);
void Ncr(void);
void Ncc(void);
BYTE response_R(BYTE);
BYTE send_cmd(BYTE *);
BYTE SD_read_lba(BYTE *,UINT32,UINT32);
BYTE SD_card_init(void);
void find_cluster(int m_num,int fat1_addr,unsigned int *cluster);
void time(int j,int n,int num);
void file_list(struct music0 *music, int *m_num , int *clupsec_num, int *data_sect,int *fat_addr);
#endif //_HEAD_H_
