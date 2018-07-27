#include "head.h"
#include <stdio.h>
void file_list(struct music0 *music, int *m_num , int *clupsec_num, int *data_sector , int *fat_addr)
{
  int sector,i,j,k,flag_01;
  int num_dw,find_2e ,flag_done;

  int fat_num,sectpfat_num,rese_fat_num;
  UINT32 s_num ;
  UINT32 secu_sect_num ;
  char file_type[4]={'\0'};
  char f_wav[4]="WAV";
  char temp_name[64]={'\0'};


  BYTE Buffer[0x200]={0};
  sector = 0;
  num_dw = 0x0;
  *m_num = 0;
  s_num = 0;
  find_2e=0;
  flag_done = 0;
  k=0;
  j=0;
  i=0;
  flag_01=0;
printf("read 1st block\n");
 SD_read_lba(Buffer,sector,1);
 printf("begin finding wav files!\n");
 while(1)
 {
  if(Buffer[0]==0xeb && (Buffer[510]<<8 | Buffer[511])== 0x55aa )
  {
    rese_fat_num=Buffer[0x0f]<<8|Buffer[0x0e];
    *clupsec_num=Buffer[0x0d];
    fat_num=Buffer[0x10];
    sectpfat_num=Buffer[0x17]<<8 | Buffer[0x16];
    secu_sect_num=(Buffer[0x1f]<<24)|(Buffer[0x1e]<<16)|(Buffer[0x1d]<<8|Buffer[0x1c]);
    sector=fat_num*sectpfat_num+secu_sect_num+rese_fat_num;
    *data_sector=sector+32;
    *fat_addr=rese_fat_num+secu_sect_num;
    SD_read_lba(Buffer,sector,1);
    break;
  }
  else
  {
	  printf(".");
    SD_read_lba(Buffer,sector,1);
    sector++;
  }
 }

  while(Buffer[num_dw*DW]!=0x00)
  {
    while(Buffer[num_dw*DW]==0xE5 )
    {
       num_dw+=0x1;
       if (num_dw==16)
       {
         s_num++;
         SD_read_lba(Buffer,sector+s_num,1);
         num_dw=0;
       }
    }
    if(Buffer[num_dw*DW+0xb]!=0x0f && Buffer[num_dw*DW+0x6]!=0x7e)
    {
      for(i=0; i<3; i++)
        file_type[i]=(char)Buffer[num_dw*DW+8+i];
      file_type[i]='\0';
      if(strcmp(file_type,f_wav)==0)
      {
        for(i=0; i<8; i++)
          music[*m_num].m_name[i]=Buffer[num_dw*DW+i];
        for(i=8;i<32;i++)
          music[*m_num].m_name[i]=0x20;
        music[*m_num].m_name[i]='\0';
        music[*m_num].cluster =(Buffer[num_dw*DW+0x1b]<<8)|Buffer[num_dw*DW+0x1a];
        (*m_num)++;
        printf("found a wav file\n");
      }
    }
    else if(Buffer[num_dw*DW+0xb]==0x0f)
    {
      if(((Buffer[(num_dw)*DW] >>6 ==1) || j==1)&& find_2e==0)
      {
        for(i=0x1e;i>0x1a;i-=2)
        {
          if((Buffer[(num_dw)*DW+i])==0x2e)
          {
            find_2e=i;
            break;
          }
        }
        if(i==0x1a)
        {
          for(i=0x18;i>0x0d;i-=2)
          {
            if((Buffer[(num_dw)*DW+i])==0x2e)
            {
              find_2e=i;
              break;
            }
          }
        }
        if(i==0x0c)
        {
          for(i=0x09;i>0;i-=2)
          {
            if((Buffer[(num_dw)*DW+i])==0x2e)
            {
              find_2e=i;
              break;
            }
          }
        }
        j++;
      }
      if(find_2e!=0 && k==0 && flag_01==0)
      {
        if(find_2e==0x1e || find_2e==0x1c)
        {
          if(find_2e==0x1e)
           temp_name[k++]=Buffer[num_dw*DW+0x1c];
          for(j=0x18;j>0x0d;j-=2)
            temp_name[k++]=Buffer[num_dw*DW+j];
          for(j=0x09;j>0;j-=2)
            temp_name[k++]=Buffer[num_dw*DW+j];
        }
        else if(find_2e>=0x0e && find_2e <= 0x18)
        {
          for(j=find_2e-2;j>0x0d;j-=2)
            temp_name[k++]=Buffer[num_dw*DW+j];
          for(j=0x09;j>0;j-=2)
            temp_name[k++]=Buffer[num_dw*DW+j];
        }
        else if(find_2e>0x01 && find_2e <= 0x09)
        {
          for(j=find_2e-2;j>0;j-=2)
            temp_name[k++]=Buffer[num_dw*DW+j];
        }
        else if(find_2e==0x01)
            flag_01=1;
        if (num_dw+1==16)
        {
          s_num++;
          SD_read_lba(Buffer,sector+s_num,1);
          num_dw=0;
          if(Buffer[(num_dw)*DW+0xb]!=0x0f)
            flag_done =1;
          num_dw=-1;
        }
        else if(Buffer[(num_dw+1)*DW+0xb]!=0x0f)
        {
          num_dw++;
          flag_done =1;
        }
      }
      else if((flag_done!=1 && k != 0) || (flag_done!=1 && flag_01==1) )
      {
        temp_name[k++]=Buffer[num_dw*DW+0x1e];
        temp_name[k++]=Buffer[num_dw*DW+0x1c];
        for(j=0x18;j>0x0d;j-=2)
          temp_name[k++]=Buffer[num_dw*DW+j];
        for(j=0x09;j>0;j-=2)
          temp_name[k++]=Buffer[num_dw*DW+j];
        if (num_dw+1==16)
        {
          s_num++;
          SD_read_lba(Buffer,sector+s_num,1);
          num_dw=0;
          if(Buffer[(num_dw)*DW+0xb]!=0x0f)
            flag_done =1;
          else
            num_dw=-1;
        }
        else if(Buffer[(num_dw+1)*DW+0xb]!=0x0f)
        {
          num_dw++;
          flag_done =1;
        }
      }
      if(flag_done ==1)
      {

        for(i=0; i<3; i++)
          file_type[i]=(char)Buffer[(num_dw)*DW+8+i];
        if(strcmp(file_type,f_wav)==0)
        {
          for(j=k-1;j>=0;j--)
            music[*m_num].m_name[k-1-j]=temp_name[j];
          if(k<32)
            for(j=k;j<32;j++)
              music[*m_num].m_name[j]=' ';
          else if(k>32 && k<48)
            for(j=k;j<48;j++)
              music[*m_num].m_name[j]=' ';
          else if(k>48 && k<64)
            for(j=k;j<64;j++)
              music[*m_num].m_name[j]=' ';
          if(k==32 || k==64 || k==96)
            j=k;
          music[*m_num].m_name[j]='\0';
          music[*m_num].cluster =(Buffer[num_dw*DW+0x1b]<<8)|Buffer[num_dw*DW+0x1a];
          (*m_num)++;
        }
        k=0;
        find_2e=0;
        flag_done=0;
        flag_01=0;
        j=0;
      }
    }
    num_dw++;
    if (num_dw==16)
    {
      s_num++;
      SD_read_lba(Buffer,sector+s_num,1);
      num_dw=0;
    }
  }
 printf("listing done");
}
//-------------------------------------------------------------
