/*
 * find_cluster.c
 *
 *  Created on: 04-06-2018
 *      Author: Vuong Dinh Hung
 */


#include "head.h"


void find_cluster(int m_num,int fat1_addr,unsigned int *cluster)
{
  UINT16 f_clu;
  int fat_off,fat_num,sect_num;
  int i=1 ;
  BYTE Buffer[512];
  f_clu=cluster[0];

  if(f_clu!=0xffff && f_clu!=0)
  {
    fat_off=f_clu%0x0100;
    fat_num=f_clu/0x0100;
    sect_num=fat1_addr+fat_num;
    SD_read_lba(Buffer,sect_num,1);
  }
  else if(f_clu==0xffff)
  {
    cluster[1]=1;
    return;
  }
  else if(f_clu==0)
  {
    cluster[0]=1;
    return;
  }

  while((Buffer[fat_off*2+1]<<8|Buffer[fat_off*2])!=0xffff)
  {
    cluster[i]=Buffer[fat_off*2+1]<<8|Buffer[fat_off*2];
    fat_off=cluster[i]%0x0100;
    fat_num=cluster[i]/0x0100;
    if(fat_num!=0)
    {
      sect_num=fat1_addr+fat_num;
      SD_read_lba(Buffer,sect_num,1);
    }
    i++;
  }
  cluster[i]=1;
}
