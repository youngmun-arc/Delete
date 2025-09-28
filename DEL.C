#include <dir.h>
#include <dos.h>
#include <io.h>
#include <conio.h>
#include <stdio.h>
#include <string.h>
int DIR_UP(const char *path,char *UP_DIR,char *NAME,int paste);
void file_display(char *info,char *fileread);
void file_time(unsigned time,char *time_string);
void file_data(unsigned date,char *date_string);
void file_attrib(char att,char *attrib_string);
int set_disk_change(char *path,int disk_change);
int find_attrib(char att, int set, int *att_mask);
void comma_byte(unsigned long number,char *string);


void comma_byte(unsigned long number,char *string)
{
/*
  정수를 문자열로 바꿔서 string에 저장한다.
  1000 Byte마다 ','를 찍어준다.
*/
char tmp[25];
int i=0,j=0;

        ultoa(number, string, 10);

        strrev(string);

        while ( string[i] != '\0') {
                if (i == 3 || i == 6 || i == 9 ) {
                    tmp[j++] = ',';
                    tmp[j++] = string[i++];
                  }

                else tmp[j++] = string[i++];
            }
        tmp[j] = '\0';
        strrev(tmp);
        strcpy(string,tmp);
}


int find_attrib(char att, int set, int *att_mask)   /*  set 이 0이면 기본,1이면 지정속성) */
{
/*
   지정한 속성으로 파일의 속성을 검사해서 결과를 리턴.
   지정한 속성이 맞을경우엔 1,그렇지 않을경우엔 0을 리턴.
   인수 att는 검사할 파일의 속성값,
        set는 0일때는 기본적으로 FA_ARCH 속성만을 가졌는지 검사하며.
              1일때는 지정한 속성을 가졌는지 검사한다.
        att_mask 는 원하는 속성값을 넣으며.속성값은 0x01,0x02,0x04,0x20만 유효.
*/

 int a=0;
 if ( set == 7 ) return 1;               /* 모든 속성    */
 if ( set != 0 ) {                       /* 지정한 속성이 포함된 파일만 */
     if ( att & att_mask[a] ) ++a;
     if ( att & att_mask[a] ) ++a;
     if ( att & att_mask[a] ) ++a;
     if ( att & att_mask[a] ) ++a;
     if ( set == a ) return 1;
     else return 0;
  }
 else {                               /*  기본속성만, 아카이브속성만  */
     if ( att & 0x04 ) return 0;      /* 시스템 */
     if ( att & 0x02 ) return 0;      /* 히든 */
     if ( att & 0x01 ) return 0;      /* 읽기 */
     if ( att & 0x20 ) return 1;      /* FA_ARCH 속성 */
    }
}


void file_display(char *info,char *fileread)
{
int a=0,b=0,c=0;
int set=0;
char name[13];
char temp[20];
char from[80];
char path[80];
char filename[13];

     while ( info[a] != '\0') { if ( info[a] == '\\' ) set = a; ++a; }
     a=0;
     while ( info[++set] != '\0' ) filename[a++] = info[set];
     filename[a] = '\0';

     a=strlen(info);                   /*  경로명을 포함한 파일명의 길이           */
     b=strlen(fileread);               /*  화면에 출력될 파일의 정보 길이          */
     a+=b; c=1;
     if ( a > 79 ) {                   /*  더한값이 화면의 80칼럼이 넘으면         */
         strcpy(temp,"");              /*  경로를 80칼럼에 맞게 줄여서 보여준다.   */
         strcpy(from,info);            /*  '.' 한개는 생략된 디렉토리 한개이다.    */
         while ( c ) {
          /*      DIR_UP(from,path);       상위디렉토리를 구한다. 즉 한구간 삭제   */
		DIR_UP(from,path,name,1);
                a=strlen(path);        /*  한구간이 삭제된 경로의 길이를 구한다.   */
                a=(a+b+13);++c;        /*  13은 파일명의길이,b는 파일정보길이      */
                if ( a > 79 ) { strcat(temp,"."); strcpy(from,path); }
                else { strcat(path,temp); strcat(path,"\\");strcat(path,filename);c=0; }
         }
        printf("%-0s%-s\n",fileread,path);
      }
     else printf("%-0s%-s\n",fileread,info);
}


void file_data(unsigned date,char *date_string)
{
/*
 file_date()-파일 생성 날짜 정보를 문자열 형태로 변경하는 함수
 인수   date       -파일 생성 날짜 정보를 전달
        date_string-문자열 형태의 파일 생성 날짜 정보가 저장됨
 리턴값-없음
*/

 sprintf(date_string,"%4d-%02d-%02d",(date>>9)+1980,     /* 년 */
				     (date>>5) & 0x000f, /* 월 */
				     date & 0x001f);     /* 일 */
}


void file_time(unsigned time,char *time_string)
{
/*
 file_time()-파일 생성 시간 정보를 문자열 형태로 변경하는 함수
 인수   time       -파일 생성 시간 정보를 전달
        time_string-문자열 형태의 파일 생성 시간 정보가 저장됨
 리턴값-없음
*/
 sprintf(time_string,"%02d:%02d",time>>11,            /* 시 */
				 (time>>5) & 0x003f); /* 분 */
}

void file_attrib(char att,char *attrib_string)
{
 attrib_string[0]='_';
 attrib_string[1]='_';
 attrib_string[2]='_';
 attrib_string[3]='_';
 attrib_string[4]='_';
 attrib_string[5]='\0';

 if(att & FA_ARCH) attrib_string[0]='A';
 if(att & FA_RDONLY) attrib_string[1]='R';
 if(att & FA_HIDDEN) attrib_string[2]='H';
 if(att & FA_SYSTEM) attrib_string[3]='S';
 if(att & FA_DIREC) attrib_string[4]='D';

}

int set_disk_change(char *path,int disk_change)
{
 /*
     *path는 바꿀 디렉토리명이며, disk_change는 드라이브까지 바꿀것인지를
     정수로 판단한다. 1은 바꾸기 이외의 수는 NO.
     디렉토리가 존재하지않으면 리턴값으로 -1을 리턴하고
     그외에는 드라이브번호를 리턴한다.
     디폴트는 현재 드라이브 번호.
     리턴값은 setdisk() 와 같음 ....
 */

int ch,drive=-1;
int org_disk=getdisk();
strupr(path);

   if ( disk_change == 1) {
      if (path[1] == ':') {            /* 드라이브명을 포함하고 있으면      */
          ch=(int)path[0];             /* 첫번쩨문자를 정수형을로 저장한다. */
          drive=ch-'A';                /* 드라이브명을 저장한다.            */
          setdisk(drive);              /* 드라이브를 바꾼다.                */
       }
      if (chdir(path) == -1 ) {              /* 디렉토리를 바꾼다 */
         cprintf("Invalid path or file name => ");
         cprintf("%s\r\n",path);
         return -1;
         }
    }

    else {
         if (path[1] == ':') {         /* 드라이브명을 포함하고 있으면      */
             ch=(int)path[0];          /* 첫번쩨문자를 정수형을로 저장한다. */
             drive=ch-'A';             /* 드라이브명을 저장한다.            */
             if (org_disk != drive) chdir(path);              /* 해당 드라이브의 작업 디렉토리를 바꿔준다 */
          }
       }
    if (drive != -1) return (drive);
    if (drive == -1) return (org_disk);
}


int DIR_UP(const char *path,char *UP_DIR,char *NAME,int paste)
{
 /*
   상위디렉토리명을 얻어낸다.
   상위디렉토리이름엔 드라이브명까지 포함됨.
   예를 들면 a21a\util 일경우 상위디렉토리는 a21a가되며 a21a가 c:\test
   디렉토리안에 있다면  c:\test\a21a가됨.
   UP_DIR은 상위디렉토리가 저장될 영역
   NAME은 상위디렉토리뒤의 이름이 저장
   paste는 분리한 상위디렉토리명끝에 '\'를 붙일지의 여부를 결정 0은 NO,1은 YES
   리턴값은 의미 없음.
 */
int z,j=0,k=0,count=0,x=0,type=0;
char orgpath[80];
char testpath[80];
char temp[80];
UP_DIR[0] = '\0';
NAME[0] = '\0';
temp[0] = '\0';
count=strlen(path);
strcpy(temp,path);
getcwd(orgpath,80);

if ( path[count-1] == '\\' && count != 1 ) --count;   /* 끝에 '\'가 붙었으면 문자열의 총길이에서 1을 뺀다. */

while ( x < count) {
       if ( path[x] == '\\') type+=1;   /* '\'이면 1씩증가 시킨다. 즉,'\'의 갯수를 구함 */
       ++x;
}

if ( type == 0 ) {
    strcpy(UP_DIR,orgpath);
    if ( paste == 1 && UP_DIR[3] != '\0' ) strcat(UP_DIR,"\\");
    strcpy(NAME,path);
    return 0;
}

while ( k < type ) {          /* 드라이브명이 포함되지 않을경우를 대비해서 */
       temp[j]=path[j];
       temp[j+1]='\0';
       if (path[j] == '\\') ++k;
       ++j;
     }
k=0;
while ( j < count ) {         /* 상위디렉토리 뒤의 이름을 따로 분리  */
       NAME[k]=path[j++];
       NAME[++k]='\0';
}

set_disk_change(temp,1);
getcwd(testpath,80);          /* 드라이브명까지 포함한 경로를 구함 */
strcpy(UP_DIR,testpath);
set_disk_change(orgpath,1);
if ( paste == 1 && UP_DIR[3] != '\0' ) strcat(UP_DIR,"\\");
return 1;

}



