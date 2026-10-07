
// Include Standard LIB  files
#include "project.h"
// 2009-02-13 수정
void extin_display(char no)
{   
   
   if (ExtIn[0] == ON)  printf("\nEmergenct Stop [ON] "); else printf("\nEmergenct Stop [OFF]");
   if (ExtIn[1] == ON)  printf("\nAC.E.M.P.R     [ON] "); else printf("\nAC.E.M.P.R     [OFF]");
   if (ExtIn[2] == ON)  printf("\nFUSE CUT       [ON] "); else printf("\nFUSE CUT       [OFF]");
   if (ExtIn[3] == ON)  printf("\nManual Operate [ON] "); else printf("\nManual Operate [OFF]");
   if (ExtIn[4] == ON)  printf("\nSCR_OVER_TEMP  [ON] "); else printf("\nSCR_OVER_TEMP  [OFF]");
   if (ExtIn[5] == ON)  printf("\nTR_OVER_TEMP   [ON] "); else printf("\nTR_OVER_TEMP   [OFF]");
   if (ExtIn[6] == ON)  printf("\nWATER_CUT      [ON] "); else printf("\nWATER_CUT      [OFF]");
   if (ExtIn[7] == ON)  printf("\nACB OFF        [ON] "); else printf("\nACB OFF        [OFF]");
   
    
}
void extin_display1(char no)
{   

   if (ExtIn[8] == ON)  printf("\nTR_COOLIN FAN1 [ON] "); else printf("\nTR_COOLIN FAN1 [OFF]");
   if (ExtIn[9] == ON)  printf("\nTR_COOLIN FAN2 [ON] "); else printf("\nTR_COOLIN FAN2 [OFF]");
   if (ExtIn[10] == ON)  printf("\nOIL PUMP       [ON] "); else printf("\nOIL PUMP       [OFF]");
   if (ExtIn[11] == ON)  printf("\nSCR COOLINGFAN [ON] "); else printf("\nSCR COOLINGFAN [OFF]");
   if (ExtIn[12] == ON)  printf("\nWATER TEMP     [ON] "); else printf("\nWATER TEMP     [OFF]");
   if (ExtIn[13] == ON)  printf("\nSpare          [ON] "); else printf("\nSpare          [OFF]");
   if (ExtIn[14] == ON)  printf("\nSpare          [ON] "); else printf("\nSpare          [OFF]");
   if (ExtIn[15] == ON)  printf("\nSpare          [ON] "); else printf("\nSpare          [OFF]");
}
   // 2009-02-13 수정
void menu_display_extout(char no)
{  
  if (no == 0)      printf("<<ExtOUT Test Menu>>");
  else if (no == 1)  {printf("\nREMOTE      "); if (TestOutBuf & 0x0001) printf("[ON] "); else printf("[OFF]");}
  else if (no == 2)  {printf("\nREADY       "); if (TestOutBuf & 0x0002) printf("[ON] "); else printf("[OFF]");}
  else if (no == 3)  {printf("\nRunning     "); if (TestOutBuf & 0x0004) printf("[ON] "); else printf("[OFF]");}
  else if (no == 4)  {printf("\nReverse Op  "); if (TestOutBuf & 0x0008) printf("[ON] "); else printf("[OFF]");}
  else if (no == 5)  {printf("\nAlarm Out   "); if (TestOutBuf & 0x0010) printf("[ON] "); else printf("[OFF]");}
  else if (no == 6)  {printf("\nFault Out   "); if (TestOutBuf & 0x0020) printf("[ON] "); else printf("[OFF]");}
  else if (no == 7)  {printf("\nMCCB TRIP   "); if (TestOutBuf & 0x0040) printf("[ON] "); else printf("[OFF]");}
  else if (no == 8)  {printf("\nDAC Output  "); if (TestOutBuf & 0x0080) printf("[ON] "); else printf("[OFF]");}
  else if (no == 9)  printf("\nReturn To Menu");
}

// External output update;
unsigned short TestOutBuf;
void testout_onoff(char no, char onoff)
{
  unsigned short onmask;
  no -= 1;
  onmask = 1 << no; 
  if (onoff == ON) TestOutBuf &= (onmask ^ 0xFFFF);
   else if (onoff == OFF) TestOutBuf |= onmask;
   else if (onoff == TOGLE) TestOutBuf ^= onmask;
}

char ExtOutTestMode;
#define EXTOUT_MAIN       10
#define EXTOUT_TEST_EXE   20
/********************************/
/*     system_test_function     */
/********************************/
void extout_test_function(void)
{ 
  short lp;
  
  switch (sExecStep)
 {   
  case 0:
    display_mode(0); 
    debug_monit(MONOUT);
    CursorUse = 1;
    MenuStart = 0;
    MenuEnd = 7;
    MenuSize = 10;
    TestOutBuf = ExtOutBuf;
    //testout_onoff(EXTOUT_BUZZER, TOGLE);
    ExtOutTestMode = 1;
    sExecStep = EXTOUT_MAIN;
    return;
    
  case EXTOUT_MAIN:
    screen_clear();
    LineBlink = 1;
    DelayStep = 0;
    for (lp = MenuStart; lp <= MenuEnd; lp++) menu_display_extout(lp);
    cursor_move_home();
    sExecStep++;
    return;
  
  case EXTOUT_MAIN+1:
    if (ROLL_UP) popup_menu_update(EXTOUT_TEST,UP); 
    else if (ROLL_DN) popup_menu_update(EXTOUT_TEST,DOWN); 
    else if (ENTER_KEY) sExecStep = EXTOUT_TEST_EXE; 
    return;

  case EXTOUT_MAIN+2:
    sExecStep--;
    return;
    
  case EXTOUT_TEST_EXE:
    DelayStep = 0;
    MenuNo = MenuStart + find_cursor_vpos();
    if ((MenuNo == 0)|(MenuNo == 9)) 
    {
      ExtOutTestMode = 0;
      execmode_change(SYSTEM_TEST);
    }
    else 
    {
      testout_onoff(MenuNo, TOGLE);
      cursor_move_up();
      menu_display_extout(MenuNo);
      sExecStep = EXTOUT_MAIN+1; 
    }
    return;

  default:
    sExecStep = EXTOUT_MAIN;
    return;
  }
}

//
// 2008/12/28 CSLab_SAM7_lin.c 에서 copy
//
//
// Ext Output update
//
unsigned short ExtOutBuf;
void ext_out_update(void)
{
  int out;
  
  pio_is_out( PIOB, EXT_DATA ) ;
  
  if (ExtOutTestMode == 1) out = TestOutBuf & 0x00FF;
    else out = ExtOutBuf & 0x00FF;

  pio_write( PIOB, out << 8 );
  delay_us(1);
  pio_set( PIOB, EXT_E1);
  delay_us(1);
  pio_clear( PIOB, EXT_E1);
}
//
// ExtOutBuf bit ON/OFF
//
void extout_onoff(char no, char onoff)
{
  unsigned short mask;
  
  mask = 1 << no;  
  if (onoff == ON) ExtOutBuf |= mask;
  else if (onoff == OFF) ExtOutBuf &= (mask ^ 0xFFFF);
  else if (onoff == TOGLE) ExtOutBuf ^= mask;
}

//
// 2008/12/28 Ext_in에 대해 Noise Cancel 특성 부여.
//
//
// External Input Read & ExiIn[[] update;
//
unsigned short usExtInBuf;
char ExtIn[16];
char ExtInCount[16];
#define EXTIN_DELAY   30    // 60ms delay

// Increment  ExtInCount[]
void inc_extin(char no)
{
  char byte;
  byte = ExtInCount[no];
  if ((byte & 0x3F) < EXTIN_DELAY) byte++;
  if ((byte & 0x3F) >= EXTIN_DELAY) 
    if ((byte & 0x80) == 0) byte |= 0x80; 
  ExtInCount[no] = byte;                            
}

// Deccrement  ExtInCount[]
void dec_extin(char no) 
{
  char byte;
  byte = ExtInCount[no];
  if ((byte & 0x3F) > 0) byte--;
  if ((byte & 0x3F) == 0) byte = 0;
  ExtInCount[no] = byte;
}
//
// EXT input scanning
//
void ext_in_scan(unsigned short data)
{
  char lp;
  unsigned short mask;
  mask = 0x0001;
  for (lp = 0; lp < 8; lp++)
  {
    if (data & mask) dec_extin(lp); else inc_extin(lp);
    mask <<= 1;
  } 
  for (lp = 0; lp < 8; lp++)
    if (ExtInCount[lp] & 0x80) ExtIn[lp] = ON; else ExtIn[lp] = OFF; 
}

void ext_in_scan2(unsigned short data)
{
  char lp;
  unsigned short mask;
  mask = 0x0001;
  for (lp = 8; lp < 16; lp++)
  {
    if (data & mask) dec_extin(lp); else inc_extin(lp);
    mask <<= 1;
  } 
  for (lp = 8; lp < 16; lp++)
    if (ExtInCount[lp] & 0x80) ExtIn[lp] = ON; else ExtIn[lp] = OFF; 
}
//
// External Input Read & ExiIn[[] update;
//
void ext_in_update(unsigned short data)
{
  char lp;
  unsigned short mask;
  mask = 0x0001;
  for (lp = 0; lp < 8; lp++)
  {
    if (data & mask) ExtIn[lp] = OFF; else ExtIn[lp] = ON;
    mask <<= 1;
  }    
}

void ext_in_read(void)
{
  pio_is_in( PIOB, EXT_DATA ) ;

  pio_clear( PIOB, EXT_E2);
  delay_us(2);
  usExtInBuf = (pio_read(PIOB) & EXT_DATA) >> 8;
  pio_set( PIOB, EXT_E2);
  
  //ext_in_update( usExtInBuf );
  // 2008-12-27
  ext_in_scan( usExtInBuf );
  // 추가
  pio_is_in( PIOB, EXT_DATA ) ;

  pio_clear( PIOA, EXT_E3);
  delay_us(2);
  usExtInBuf = (pio_read(PIOB) & EXT_DATA) >> 8;
  pio_set( PIOA, EXT_E3);
  
  //ext_in_update( usExtInBuf );
  // 2008-12-27
  ext_in_scan2( usExtInBuf ); 
  
}
