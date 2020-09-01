
#include <stdio.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#define Concurrency 25
//#define writetime 500
//#define readtime 400
#define Blocksize 256
//#define filesize (10 * 1024 * 1024)
#define maxline (2 * 1024 * 1024)
#define maxread (60 * 1024 * 1024)

char examtext[maxline] = "abcdefghijklmnopabcdefghijklmnopabcdefghijklmnopabcdefghijklmnopabcdefghijklmnopabcdefghijklmnopabcdefghijklmnopabcdefghijklmnopabcdefghijklmnopabcdefghijklmnopabcdefghijklmnopabcdefghijklmnopabcdefghijklmnopabcdefghijklmnopabcdefghijklmnopabcdefghijklmnop";
struct timespec starttime, endtime, spendtimeSpec;
/*
struct timespec {
	time_t	tv_sec;		 seconds
	long	tv_nsec;	 and nanoseconds
};

*/
char x[10];
char *filepath;
int fp;
char readbuffer[maxread];

/*写文件:打开文件，判断返回值，如果正常打开文件就判断是否随机写，进行写操作*/
void write_file(int blocksize, bool isrand, char *filepath,int filesize)
{
  //to do....
  fp = open(filepath, O_SYNC | O_CREAT | O_RDWR);
  if (fp < 0) {
    printf("open %s error!\n", filepath);
    return;
  }
  int writetime = 60000;
  while(writetime > 0){
    if(isrand == true){
      srand(time(NULL));
      //随机写操作
      off_t offset = rand()%filesize;
      lseek(fp, offset, SEEK_SET);

      if(write(fp, examtext, blocksize) < 0){
        printf(" isrand write error!\n");
        return;
      }
    }else{
      //顺序写
      if(write(fp, examtext, blocksize) < 0){
        printf("write error!\n");
        return;
      }
    }
    writetime--;
  }
}


/*读文件:打开文件，判断返回值，如果正常打开就判断是否随机读，进行读操作*/
void read_file(int blocksize, bool isrand, char *filepath,int filesize)
{
 //to do....
 fp = open(filepath, O_SYNC | O_CREAT | O_RDWR);
 if (fp < 0) {
   printf("open %s error!\n", filepath);
   return;;
 }
 struct stat buf;
 fstat(fp, &buf);
 filesize = buf.st_size;
 int readtime = 60000;
 while(readtime > 0){
   if(isrand == true){
     srand(time(NULL));
     //随机读操作
     off_t offset = rand()%filesize;
     lseek(fp, offset, SEEK_SET);

     if(read(fp, readbuffer, blocksize) < 0){
       printf("isrand read error!\n");
       return;
     }
   }
   else{
     //顺序读
     if(read(fp, readbuffer, blocksize) < 0){
       lseek(fp, 0, SEEK_SET);
       read(fp, readbuffer, blocksize);
       //printf("read error!\n");
       //return;
     }
   }
   readtime--;
 }
}

//计算时间差，在读或写操作前后分别取系统时间，然后计算差值即为时间差。
long get_time_left(long starttime, long endtime)
{
  long spendtime = endtime - starttime;

  return spendtime;
}

/*主函数：首先创建和命名文件，通过循环执行read_file和write_file函数测试读写差异。
测试blocksize和concurrency对测试读写速度的影响，最后输出结果。*/
int main()
{
  int status,i;
  int filesize = 20971520;
  clock_gettime(CLOCK_REALTIME,&starttime);
  for (i = 0; i < Concurrency; i++)
  {
    status = fork();
    if (status == 0 || status == -1) break;
  }
  if (status == -1)
  {
    printf("fork error!\n");
    return 0;
  }
  else if (status == 0) //每个子进程都会执行的代码
  {
    filepath = "/root/disk/";
    x[0]=(char)(i+97);
    x[1]='\0';
    strcat(x,".txt");
    strcat(filepath, x);
    //printf("%s",filepath);
    //write_file(Blocksize, true, filepath,filesize);
    //write_file(Blocksize, false, filepath,filesize);
    //read_file(Blocksize, true, filepath,filesize);
    read_file(Blocksize, false, filepath,filesize);

  }

   //随机写
 //write_file(blocksize, true, &filepath, filesize/concurrency);
   //顺序写
   //write_file(blocksize, false, &filepath, filesize/concurrency);
   //随机读
 //read_file(blocksize, true, &filepath, filesize / concurrency);
   //顺序读
   //read_file(blocksize, false, &filepath, filesize / concurrency);

/*等待子进程完成后，获取计算时间，计算读写操作所花时间，延时，吞吐量等*/
    //to do....
    else{
      while( -1 !=  wait(NULL) ){
        ;
      }
      clock_gettime(CLOCK_REALTIME,&endtime);
      printf("%lld %ld\n",starttime.tv_sec,starttime.tv_nsec);
      printf("%lld %ld\n",endtime.tv_sec,endtime.tv_nsec);
      spendtimeSpec.tv_nsec = get_time_left(starttime.tv_nsec, endtime.tv_nsec);
      printf("%ld\n",spendtimeSpec.tv_nsec);
    }
return 0;
}
