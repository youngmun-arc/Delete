/*******************************************************************************
   다중옵션 지정 방법이나 다중 파일 지정 방법을 사용할 수 있습니다.
   옵션은 어느곳이든지 위치할수 있으며 옵션과 옵션은 반드시 공백으로
   구분되야합니다.
   같은 옵션을 여러번 쓸경우 뒤의것만 유효 합니다.
   파일 또는 디렉토리 지정시 와일드카드를 쓸수 있으며, 옵션들은
   서로의 연계를 가지며, 각각의 고유기능을 발휘합니다.
   그러므로 각옵션의 기능을 확실히 알고, 알맞게 혼합해서 쓰는것이 좋습니다.
   단, /F  와 /L 옵션은 함께 쓸수없습니다.

   Usage : FD /A[:][arhs] /B /D /F[[>]filename] /L[:]filename /S /Y  files...

  /A          지정한 속성을 갖는 파일만을 작업대상으로합니다.( /A는 모든속성)
              옵션을 안쓸경우엔 디폴트로 A 속성만 적용됨.
              속성부여시 해당속성을 가지고 있는 파일에 모두 적용.

  선택속성    A  기록할 파일       R  읽기전용 파일
              H  숨김 파일         S  시스템 파일


  /B          지정한 파일정보를 출력( /B 는 모든정보출력)
              지정한 순서로 파일정보를 출력한다.
              정보출력시 80칼럼이 넘으면 디렉토리명등을 짭게 줄여서 경로를 보여줍니다.
              이때의 '.'하나는 생락된 디렉토리하나를 말합니다.

  선택정보    B  용량 출력
              A  속성 출력
              D  생성날짜 출력
              T  생성시간 출력

  /D          디렉토리 삭제 ( 디렉토리안에 포함된 모든파일까지 )
              디렉토리 삭제를 원할경우에는 /D 옵션을 써야됩니다.
              /D 옵션은 디렉토리만을 작업대상으로 합니다.

  /F          파일 찾기 또는 쟨은파일을 리스트파일로 생성
              찾은 파일을 리스트파일로 저장시는 아래와 같이 하면 됩니다.
              FD  *.OLD *.BAK [기타옵션] /F > LIST.TXT
              위의파일에 추가시는 FD /F *.$$$ >> LIST.TXT
              리스트파일을 이용해서 삭제시는 아래와 같습니다.
              FD /L:LIST.TXT [기타옵션..]

  /L          리스트로 작성된 파일 또는 디렉토리들을 삭제합니다.
              /L 옵션뒤에 리스트로 작성된 파일명을 지정하면 됩니다.
              리스트작성시 삭제할 파일명 또는 디렉토리명은 드라이브와
              전체경로를 포함한 이름이여야 하며 첫자가 공백이거나 ';'이면
              그행은 무시됩니다. 리스트파일 작성시 와일드카드를 쓸수 있습니다.
              디렉토리 삭제시엔 /D 옵션을 함께 써야함.
              /L 옵션 사용시 모든 작업이 리스트파일을 중심으로 이루어 집니다.

  /S          지정한 디렉토리 파일과 하위디렉토리까지 포함합니다.

  /Y          삭제시 사용자의 확인을 받지 않습니다.

               Delete [ A/Y/N/R/Q ] ? 질문시 뜻은 아래와 같습니다.
               A 모든 파일에 Yes적용.
               Y 현재파일만 Yes.
               N 현재파일만 No.
               R 삭제된 파일들을 리스트로 저장( DELETE.TXT )  !미정
               Q 종료.
               ENTER 은 Y를 뜻함.

 [활용] 지운파일의 리스트만들기
        FD *.BAK *.OLD /S /A /B /Y > DEL.TXT

  추가사항
  *:\*.* ==> 모든 드라이브를 나타냄. 즉 모든 드라이브의 모든파일.
*******************************************************************************/

#include <sys\stat.h>
#include "del.c"
#define ALL_ATTRIB (FA_ARCH|FA_RDONLY|FA_HIDDEN|FA_SYSTEM)
#define ALL_DIR (FA_ARCH|FA_RDONLY|FA_HIDDEN|FA_SYSTEM|FA_DIREC)

unsigned long delete_bytes=0;    /*  지운 파일의 용량 저장                 */
unsigned long find_bytes=0;      /*  찾은 파일의 용량 저장                 */
unsigned delete_files=0;         /* 지운 파일의 갯수를 저장                */
unsigned find_files=0;           /* 찾은 파일의 갯수                       */
int ALL_FILE=FA_ARCH;            /* 파일찾기시 기본 속성만 찾는다          */
int position[20][2];             /* 옵션이 아닌 인자의 위치를 저장         */
int positioning=0;               /* 옵션이 아닌 인자의 갯수를 저장         */
int Aop_attr[4];    /* 파일속성을 검사하기위해서 속성값을 저장(사용자지정) */
int Aop=0;          /* /A 옵션의 초기화(아카이브속성)                      */
int Sop=0;          /* /S 옵션의 초기화(서브디렉토리 검색안함)             */
int Dop=0;          /* /D 옵션의 초기화 ( 기능상실 )                       */
int Yop=1;          /* /Y 옵션의 초기화 ( 물어봄 )                         */
int Fop=0;          /* /F 옵션의 초기화 ( 파일찾기 안함)                   */
int Lop=0;          /* /L 옵션의 초기화 ( 리스트파일로 삭제안함 )          */
char Bop[4];
char mask[81];      /* 찾을파일의 마스크(와일드카드 포함)저장             */
char attrib[5];     /* 파일 속성을 문자열로 저장해서 표시하는 문자열      */
char date[10+1];    /* 파일 생성 날짜를 문자열로 저장해서 표시하는 문자열 */
char time[5+1];     /* 파일 생성 시간을 문자열로 저장해서 표시하는 문자열 */
char byte[25];      /* 파일 용량을 문자열로 저장해서 표시하는 문자열      */
FILE *fp;


void del_dir()      /* 디렉토리를 지운다. 디렉토리에 포하된 모든 서브디렉토리와 파일까지 */
{
    struct ffblk info;
    struct ffblk ffblk;

    if (findfirst("*.*", &ffblk,ALL_ATTRIB) == 0)  {
        do {
             _chmod(ffblk.ff_name,1,FA_ARCH);     /* 속성제거   */
             unlink(ffblk.ff_name);               /*  파일 지움 */
             ++delete_files;
             delete_bytes+=ffblk.ff_fsize;
          } while ( findnext(&ffblk) == 0 );
     }

    if (findfirst("*.*", &info, ALL_DIR) == 0) {
        do {
            if ( info.ff_name[0] == '.' ) continue;
            if ( info.ff_attrib & FA_DIREC ) {
                 chdir(info.ff_name);
                 del_dir();
                 chdir("..");
                _chmod(info.ff_name,1,FA_ARCH);
                rmdir(info.ff_name);           /* 디렉토리 지움 */
            }
        } while (findnext(&info) == 0);
   }
}

void dir_size()
{                           /* 디렉토리의 용량을 알아낸다. */
    struct ffblk info;
    struct ffblk ffblk;

    if (findfirst("*.*", &ffblk,ALL_ATTRIB) == 0)  {
        do {
            find_bytes+=ffblk.ff_fsize;
            find_files+=1;
          } while ( findnext(&ffblk) == 0 );
     }

    if (findfirst("*.*", &info, ALL_DIR) == 0) {
        do {
            if (info.ff_name[0] == '.' ) continue;
            if (info.ff_attrib & FA_DIREC) {
                chdir(info.ff_name);
                dir_size();
                chdir("..");
            }
        } while (findnext(&info) == 0);
   }
}

int del_file(unsigned long size,char *filename,int prompt,char *string)
{ /* 에러시 0리턴,정상삭제시 1리턴,강제 종료시 문자열상수 Q의 아스키코드 리턴 */
int it=1;
char done;

  if ( prompt ) {
         cprintf("%s",string);
      do {
          done=toupper(getch());                             /* 키입력을 대문자로 바꿈 */
          if ( done == 27 ) done = 'Q';                      /* ESC키를 Q로 지정       */
          if ( done == 13 ) done = 'Y';                      /* ENTER 은 Y 로 간주     */
          if ( done == 'Q' ) { cprintf("Quit\r\n"); return 'Q'; }
          if ( done == 'A' ) { Yop=0; prompt=0; it=0; }      /* 전역변수인 Yop의 값을 바꾼다 */
          if ( done == 'Y' ) { prompt = 0; it=0; }
          if ( done == 'N' ) it = 0;
        } while ( it );
        cprintf("%c\r\n\n",done);       /* 입력받은 문자 출력 */
        if ( done == 'N' ) return 0;
    }

  if ( !prompt ) {                                          /* 실질적인 파일삭제         */
       if ( Dop != 0 ) {                                    /* 디렉토리이면 디렉토리삭제 */
           chdir(filename);
           del_dir();                                       /* 디렉토리삭제함수          */
           chdir("..");
           rmdir(filename);
           return 1;
        }
       else {
              _chmod(filename,1,FA_ARCH);
              if ( unlink(filename) == 0 ) {                /* 파일삭제  */
                   ++delete_files;
                   delete_bytes+=size;
                   return 1;
              }
             else { cprintf("Access denied\r\n"); return 0;}
       }
  }
}


int findfile(char *string)     /* 파일에대한 전반적인 기능을 수행한다. */
{
    int a,b,ch,d,done;
    char path[80];
    char tmp[15];
    char file_print[255];
    struct ffblk info;
    file_print[0] = '\0';
    if (findfirst(string, &info, ALL_FILE) == 0) {
       do {
           if ( Dop == 1 ) {                             /* /D 옵션이면 ,즉 디렉토리만 골라낸다. */
              if ( info.ff_name[0] == '.' && info.ff_attrib & FA_DIREC ) continue;
                if ( !(info.ff_attrib & FA_DIREC) ) continue;          /* 디렉토리가 아니면 다시 */
                else {
                     chdir(info.ff_name);
                     dir_size();            /* 디렉토리의 크기를 출력하기위해서 크기를 알아낸다. */
                     chdir("..");
                }
           }



           a=find_attrib(info.ff_attrib, Aop, Aop_attr);  /* 파일속성을 검사한다     */
           if ( a == 0 ) continue;                        /* 찾는 속성이 아니면 다시 */
           if ( Dop == 0 ) {
                ++find_files;
                find_bytes+=info.ff_fsize;
            }
           getcwd(path, 80);  /* 현재의 작업 디렉토리를 구한다. 즉 파일이 있는 경로가 된다. */

           if ( Bop[0] != '\0' ) {
             a=0;
	     while ( a < 256) file_print[a++] = '\0'; a=0;
             while ( a < 4 ) {    /* /B 에 따라 주어진 파일정보를 알아낸다 */

                   if ( Dop == 1 && Bop[a] == 'B' ) {
                        comma_byte(find_bytes,byte);    /* 디렉토리용량을 문자열로 변환 ','붙여서 */
                        sprintf(tmp,"%10s  ",byte);
                        strcat(file_print,tmp);
                    }
                   if ( Dop == 0 && Bop[a] == 'B' ) {
                        comma_byte(info.ff_fsize,byte); /* 파일용량을 문자열로 변환 ','붙여서 */
                        sprintf(tmp,"%10s  ",byte);
                        strcat(file_print,tmp);
                    }

                   if ( Bop[a] == 'A' ) {
                       file_attrib(info.ff_attrib,attrib); /* 파일속성을 문자열로 변환 */
                       sprintf(tmp,"%s  ",attrib);
                       strcat(file_print,tmp);
                    }
                   if ( Bop[a] == 'D' ) {
                       file_data(info.ff_fdate,date);  /* 파일생성날짜를 문자열로 변환 */
                       sprintf(tmp,"%s  ",date);
                       strcat(file_print,tmp);
                    }
                   if ( Bop[a] == 'T' ) {
                       file_time(info.ff_ftime,time);  /* 파일생성시간을 문자열로 변환 */
                       sprintf(tmp,"%s  ",time);
                       strcat(file_print,tmp);
                    }
                    ++a;
              }
          }

          strcat(path,strlen(path) == 3 ? "" : "\\");  /* 파일의 경로명을 구해서 파일명과 합친다. */
          strcat(path,info.ff_name);

          if ( Bop[0] != '\0' ) file_display(path,file_print);  /* 80칼럼에 맞게 출력 */
          if ( Bop[0] == '\0' ) printf("%-0s\n",path);

            /*    확인하며 삭제 또는 그냥 삭제 */
          if ( !Fop ) done=del_file(info.ff_fsize,info.ff_name,Yop,"Delete (/A/Y/N/Q) ? ");
          if ( done == 'Q') return 'Q';
          file_print[0] = '\0';
          tmp[0] = '\0';
       } while (findnext(&info) == 0);
    }
}

void dirfile(void)           /* 서브디렉토리까지 검색하는 함수 */
{
    struct ffblk info;
    if (findfirst("*.*", &info, ALL_DIR) == 0) {
       do {
            if ( info.ff_name[0] == '.' ) continue;
            if ( info.ff_attrib & FA_DIREC ) {
                chdir(info.ff_name);
                findfile(mask);          /* 이곳에서 파일찾는 루틴을 호출한다. */
                dirfile();               /* 재귀호출 */
                chdir("..");
             }
       } while (findnext(&info) == 0);
    }
}

int main(int argc, char *argv[])
{
    struct ffblk fn;
    int a=1,b,c,d;
    char LIST_FILE[80];   /* 사용자가 입력한 리스트파일이름이 저장됨 */
    char LIST_NAME[80];
    char curpath[80];     /* 현재의 작업디렉토리를 저장              */
    char buff[256];       /* 파일에서 한줄을 읽어서 저장하는 영역    */
    char path[80];
    path[0] = '\0';       /* 문자열 초기화                           */
    Bop[0] = '\0';

    if (argc < 2)
        {
         textcolor(14);
         cprintf(" File erase FD v1.0 Copyright (C) 1996.12  Wild horse Software.\r\n\n");
         textcolor(7);
         printf(" Usage : FD /A[:][arhs] /B /D /F[[>]filename] /L[:]filename /S /Y  files...\n\n");
         printf("            /A:(ttribute select)         /B:(file info display)\n");
         printf("            /D:(irectory delete)         /F:(ile Find)\n");
         printf("            /L:(delete in list file)     /S(ubdirectories)\n");
         printf("            /Y(es to all prompts)\n");
         return 0;
    }

   getcwd(curpath, 80);

   for ( a=1 ; a < argc ;) {    /* 모든 옵션의 셋팅 */
        strupr(argv[a]);
        if ( argv[a][0] == '/' ) {                 /* 옵션이면 */
             if ( argv[a][1] == 'B') {             /* /B???? 옵션에대한 셋팅 */
                  if ( argv[a][2] != '\0' ) {
                       b=2,c=0;
                       while ( argv[a][b] != '\0' ) {
                               if ( argv[a][b] == 'B' ) Bop[c++] = 'B';
                               if ( argv[a][b] == 'A' ) Bop[c++] = 'A';
                               if ( argv[a][b] == 'D' ) Bop[c++] = 'D';
                               if ( argv[a][b] == 'T' ) Bop[c++] = 'T';
                               ++b;
                       }
                       Bop[c] = '\0';    /* 문자열로 만든다 */
                  }
                  else {
                         Bop[0] = 'D';                /* /B 옵션에대한 설정 */
                         Bop[1] = 'T';                /* 모든 정보 출력임   */
                         Bop[2] = 'A';
                         Bop[3] = 'B';
                         Bop[4] = '\0';
                  }
             }

            if ( argv[a][1] == 'D' ) {     /* /D 옵션에대한 셋팅   */
                 Dop=1;                    /* 1은 디렉토리 삭제    */
                 ALL_FILE=( ALL_FILE+FA_DIREC );  /* 찾을파일에 디렉토리를 추가 */
            }

            if ( argv[a][1] == 'A' ) {                                        /* /A 옵션에대한 셋팅      */
                 ALL_FILE=(ALL_FILE+FA_ARCH+FA_RDONLY+FA_HIDDEN+FA_SYSTEM);   /* 모든 속성의 파일을 검색 */
                 if ( argv[a][2] != '\0' ) {                                  /* /A 옵션에대한 셋팅      */
                      if ( argv[a][2] == ':' ) b=3;    /* 구분자로 ':'를 사용했을경우 다음문자부터 */
                      else b=2;
                      c=0;
                      while ( argv[a][b] != '\0' ) {                          /* 지정한 속성이 있는지 알아내기위해서 */
                              if ( argv[a][b] == 'A' ) Aop_attr[c]=FA_ARCH;   /* 지정속성일 경우 셋팅 */
                              if ( argv[a][b] == 'R' ) Aop_attr[c]=FA_RDONLY;
                              if ( argv[a][b] == 'H' ) Aop_attr[c]=FA_HIDDEN;
                              if ( argv[a][b] == 'S' ) Aop_attr[c]=FA_SYSTEM;
                              ++c;               /* 사용자가 지정한 속성의 갯수 */
                              ++b;
                      }
                      Aop=c;                     /* 지정한 속성의 갯수 */
                 }
                 else Aop=7;                     /* 모든 속성을 지정했을시 즉, /A 옵션일때 */
            }

            if ( argv[a][1] == 'F' ) Fop = 1;                        /* /F 옵션에대한 셋팅 */
            if ( argv[a][1] == 'S' ) Sop = 1;                        /* /S 옵션에대한 셋팅 */
            if ( argv[a][1] == 'Y' && argv[a][2] == '\0' ) Yop = 0;  /* /Y 옵션에대한 셋팅 */
            if ( argv[a][1] == 'L' ) {                               /* /L 옵션에대한 셋팅 */
                 b=2,c=0,Lop=1;
                 if ( argv[a][2] == ':' ) b = 3;
                 while ( argv[a][b] != '\0' ) {                      /* /L 뒤의 파일명 얻기 */
                         LIST_FILE[c] = argv[a][b++];
                         LIST_FILE[++c] = '\0';
                 }

                DIR_UP(LIST_FILE,path,mask,1);               /* 디렉토리와 파일명을 각각 분리한다. */
                strcat(path,mask);                           /* 리스트파일이 있는 전체경로를 구하기위해 */
                strcpy(LIST_FILE,path);                      /* 리스트파일의 전체경로와 파일명 합침 */
                path[0] = '\0';
                mask[0] = '\0';
                if ( access(LIST_FILE,4) == 0 ) {                   /* 파일의 존재여부(읽기가능) 타진 */
                     fp=fopen(LIST_FILE,"rt");                      /* 파일을 읽기속성의 텍스트모드로 오픈 */
                     fseek(fp,0L,SEEK_SET);                         /* 파일 포인터를 처음으로 */
                }
                else {
                       cprintf("file not fount \"%s\"\n\r",LIST_FILE);
                       exit(0);
                }
            }
        }   /* IF END */

	if ( argv[a][0] != '/' ) position[positioning++][0] = a;  /* 옵션이 아닌 인자의 위치와 갯수를 저장 */
   ++a;
   }    /* FOR END */

  if ( positioning == 0 && Lop == 0 ) {                    /* 필요한 요소가 없을경우 종료 */
       printf(" Required parameter missing\n");            /* 파일명이나 /L 옵션둘다 없으면 종료 */
       exit(1);
  }

  if ( Lop == 1 && Fop == 1 ) {                            /* /L 과 /F 를 함께 사용했으면 종료 */
       printf(" Choice  \"/L\" or \"/F\"  Option\n");
       exit(0);
  }

  if ( Lop == 1 ) {                                        /* /L 옵션 수행루틴 */
       while ( !feof(fp) ) {
               a=0;b=0;c=0,d=0;
               path[0] = '\0';
               LIST_NAME[0] = '\0';                      /* 문자열의 끝 또는 초기화 */
               if ( fgets(buff,255,fp) == NULL ) continue;
               if ( buff[0] == 0x20 ) continue;          /* 첫문자가 공백문자이면 다음줄 읽음            */
               if ( buff[0] == 0x3b ) continue;          /* 첫문자가 ';'문자이면 다음줄 읽음             */
               if (( b=strlen(buff)) <= 3 ) continue;    /* 문자열이 3자보다 적으면 다음줄 읽음(빈줄포함) */
                while ( a < b ) {                  /* 드라이브명과 전체경로를 포함한 파일명 추출 */
                      if ( buff[a] == 0x0a ) buff[a] = '\0';
                      if ( buff[a+1] == ':' && buff[a+2] == '\\' ) c=1;
                      if ( c ) LIST_NAME[d++] = buff[a];
                      ++a;
                }
                if ( d == 0 ) continue;                             /* 없으면 다움줄 읽기 */
                if ( strcmp(LIST_FILE,LIST_NAME) == 0 ) continue;   /* 리스트파일은 지우지 않는다 */

                DIR_UP(LIST_NAME,path,mask,0);          /* 디렉토리와 파일명을 각각 분리한다. */
                b=0;
                b=set_disk_change(path,1);              /* 드라이브와 디렉토리를 바꾼다. */
                if ( b == -1 ) {                        /* 에러시 원래작업디렉토리로 복귀후 종료 */
                     set_disk_change(curpath,1);
                     exit(0);
                }
                a=findfile(mask);                        /* 파일찾기 */
                if ( a == 'Q' ) { set_disk_change(curpath,1); exit(0); }
                if ( Sop == 1 ) dirfile();             /* 서브디렉토리까지 */
       }
       comma_byte(delete_bytes,byte);   /* 삭제된 파일의 용량을 ','를 붙여서 문자열로 바꾼다. */
       printf("\n %5d files Deleted     %10s Byte freed\n",delete_files,byte);
       fclose(fp);
       exit(0);
  }


 for (a=1; a < argc; ) {
     strupr(argv[a]);                              /* 문자열을 대문자로 바꾼다. */
     if ( argv[a][0] != '/') {                     /* 옵션이 아니면 */
          DIR_UP(argv[a],path,mask,0);
          b=0;
          b=set_disk_change(path,1);
          if ( b == -1 ) {
               set_disk_change(curpath,1);
               exit(0);
          }

          b=findfile(mask);
          if ( b == 'Q' ) { set_disk_change(curpath,1); exit(0); }
          if ( Sop == 1 ) dirfile();                /* 서브 디렉토리까지 */
          path[0] = '\0';
     }
 ++a;
 }

   if ( find_files == 0 ) {
        cprintf("File not found\r\n");
        set_disk_change(curpath,1);
        return 0;
    }

   if ( Fop == 1 ) {
         comma_byte(find_bytes,byte);
         printf("\n %5d files    %10s Bytes\n",find_files,byte);
        }

   if ( Fop == 0 ) {
         comma_byte(delete_bytes,byte);
         printf("\n %5d files Deleted     %10s Byte freed\n",delete_files,byte);
        }

    set_disk_change(curpath,1);
    return 0;
}
