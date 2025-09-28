#include <dos.h>
#include <stdlib.h>
#include <time.h>

void Delay(clock_t t) {
    clock_t end;

    end = t + clock();
    while( end >= clock() ) ;
}

void  SetSpeakerOn() {
    int val;
 
    val = inportb(0x61) ;
    val |= 03 ;
    outportb(0x61, val) ;
}

void SetSpeakerOff() {
    int val;
 
    val = inportb(0x61) ;
    val &=~ 03 ;
    outportb(0x61, val) ;
}        

void SetFrequency(int hertz) {
    unsigned divisor = 1193180L/hertz ;
    
    outportb(0x43, 0xB6) ;
    outportb(0x42, divisor & 0xFF) ;
    outportb(0x42, divisor >> 8) ;
}

void Sound(int hertz, int delay) {
    SetFrequency(hertz);
    SetSpeakerOn();
    Delay(delay);
    SetSpeakerOff();
}

/***********************************************************************
 * void SirenSound(int no)   
 * - ¬a·¡E ­¡Ÿ¡Ÿi “e Ðq®
 * ·³b : no - ¤e¥¢ ÒU®
 * Â‰b : ´ô·q
 * Ÿ¡Èåˆt : ´ô·q
 ***********************************************************************/
void SirenSound(int no) {
    int i, j;
  
    for(j=no; j>0; j--) {
        for(i=500; i<1000; i+=20) 
            Sound(i, 1);
        for(i=1000; i>500; i-=20)
            Sound(i, 1);
    }
}

/***********************************************************************
 * void LaserSound(int no)
 * - A·¡¸á ­¡Ÿ¡Ÿi “e Ðq®
 * ·³b : no - ¤e¥¢ ÒU®
 * Â‰b : ´ô·q
 * Ÿ¡Èåˆt : ´ô·q
 ***********************************************************************/
void LaserSound(int no) {
    int i,j;

    for(i=no; i>0; i--) 
        for(j=1000; j>500; j-=20)
            Sound(j, 1);
}

/***********************************************************************
 * void NoiseSound(int no)
 * - ¸s·q ­¡Ÿ¡Ÿi “e Ðq®
 * ·³b : no - ¤e¥¢ ÒU®
 * Â‰b : ´ô·q
 * Ÿ¡Èåˆt : ´ô·q
 ***********************************************************************/
void NoiseSound(int no) {
    int i, freq;

    for(i=no; i>0; i--) {
        do {
            freq = rand();
        } while( freq > 800 );
        Sound(freq, 1);
    }
}

/***********************************************************************
 * void ClickSound()
 * - ¸s·q ­¡Ÿ¡Ÿi “e Ðq®
 * ·³b : ´ô·q
 * Â‰b : ´ô·q
 * Ÿ¡Èåˆt : ´ô·q
 ***********************************************************************/
void ClickSound() {
        Sound(500, 1);
}

void main() {
    printf("·qÐ· Òa‰ÁŸi ÉA¯aËa Ða“e Ïa¡‹aœ‘\n");
    printf("¬a·¡E ­¡Ÿ¡ - ´a¢Ç¡a ’Ÿa¯¡¶a\n");
    getch();
    SirenSound(1);
    printf("A·¡¸á ­¡Ÿ¡ - ´a¢ Ç¡a ’Ÿa¯¡¶a\n");
    getch();
    LaserSound(1);
    printf("¸s·q ­¡Ÿ¡ - ´a¢ Ç¡a ’Ÿa¯¡¶a\n");
    getch();
    NoiseSound(50);
    printf(" a¶¯a ÇiŸ¢ ­¡Ÿ¡ - ´a¢ Ç¡a ’Ÿa¯¡¶a\n");
    getch();
    ClickSound();
    printf("´a¢ Ç¡a ’Ÿa¡e Ïa¡‹aœ‘·¡ {s“¡”a\n");
    getch();
}
