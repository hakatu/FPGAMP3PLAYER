#include "head.h"
#include <system.h>
#include "chu_avalon_audio.h"
#include <stdio.h>
#include <unistd.h>
int main(void)
{
  UINT16 i=0,song_num=0,song_position=0,clupsec_num,data_sector;

  UINT16 num_music,j_base,base,fat_addr,cluster_num;

  UINT32 j,temporary1;
  unsigned int cl[3000];
  BYTE Buffer[512]={0};
printf("please insert SD Card");
  while(SD_card_init())  // wait for the card to be inserted
    usleep(500000);
 printf("Card read!\n");
  audio_init(CHU_AVALON_AUDIO_BASE);
  struct music0 music[30];
  printf("begin file list!");
    file_list(music,&num_music,&clupsec_num, &data_sector,&fat_addr); // search and update the database from the SD card
    printf("list done!\n");
  while(1)
  {
     usleep(1000000);
      song_num+=0;
    cl[0]=music[song_num].cluster;
    find_cluster(song_num,fat_addr,cl);
    printf("playing\n");
    for(i=0;cl[i]>1;i++)
    cluster_num=i;
    for(song_position=0;song_position<cluster_num;song_position++)
    {
      base=(cl[song_position]-2)*clupsec_num+data_sector;
      j_base=song_position*clupsec_num-base;
      for(j=base;j<base+clupsec_num;j++) //j is the section in the cluster
      {
        SD_read_lba(Buffer,j,1);    // read function from SD card controller.c
        i=0;
        while(i<512)
        {
          if(!audio_dac_fifo_full(CHU_AVALON_AUDIO_BASE))
          {
            temporary1=((Buffer[i+3]<<24)|(Buffer[i+2]<<16)|(Buffer[i+1]<<8)|Buffer[i]);   // read from SD card
	audio_dac_wr_fifo(CHU_AVALON_AUDIO_BASE, temporary1);
            i+=2;
            printf("Temp: %d\n", temporary1);
          }
        }
    }
   }
  }
  return 0;
}
