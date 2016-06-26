/*
*********************************************************************************************************
*                                       7 Greedy Snake
*********************************************************************************************************
*/

#include "includes.h"
/*********************************************************************************************************
*                                               CONSTANTS
*********************************************************************************************************
*/

#define  TASK_STK_SIZE                 512       /* Size of each task's stacks (# of WORDs)            */
#define  N_TASKS                        4       /* Number of identical tasks                          */

/*
*********************************************************************************************************
*                                               VARIABLES
*********************************************************************************************************
*/

#define WIDTH 43
#define HEIGHT 17

OS_STK        TaskStk[N_TASKS][TASK_STK_SIZE];        /* Tasks stacks                                  */
OS_STK        TaskStartStk[TASK_STK_SIZE];
INT8U         TaskData[N_TASKS];                      /* Parameters to pass to each task               */
INT8U	      Map[HEIGHT][WIDTH];//map
INT16S        score;
/*
*********************************************************************************************************
*                                           FUNCTION PROTOTYPES
*********************************************************************************************************
*/
		
        void  Shooter(void *data);/*射击者*/
        void  UpdateGui(void *data);/*更新显示*/
        void  TaskStart(void *data);/* Function prototypes of Startup task           */
static  void  TaskStartCreateTasks(void);
static  void  TaskStartDispInit(void);
static  void  TaskStartDisp(void);
		void  Init(void);//初始化
		void  shoot(INT8U* x);//射击
		void  bullet(void *data);//子弹

/*
*********************************************************************************************************
*                                                MAIN
*********************************************************************************************************
*/

void  main (void)
{
    PC_DispClrScr(DISP_FGND_WHITE + DISP_BGND_BLACK);      /* Clear the screen                         */

    OSInit();                                              /* Initialize uC/OS-II                      */
	
    PC_DOSSaveReturn();                                    /* Save environment to return to DOS        */
    PC_VectSet(uCOS, OSCtxSw);                             /* Install uC/OS-II's context switch vector */
	
    OSTaskCreate(TaskStart, (void *)0, &TaskStartStk[TASK_STK_SIZE - 1], 3);
    OSStart();                                             /* Start multitasking                       */
}


/*
*********************************************************************************************************
*                                              STARTUP TASK
*********************************************************************************************************
*/
void  TaskStart (void *pdata)
{
#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr;
#endif
    char       s[100];

    pdata = pdata;                                         /* Prevent compiler warning                 */
    score=0;
    TaskStartDispInit();                                   /* Initialize the display                   */

    OS_ENTER_CRITICAL();
    PC_VectSet(0x08, OSTickISR);                           /* Install uC/OS-II's clock tick ISR        */
    PC_SetTickRate(OS_TICKS_PER_SEC);                      /* Reprogram tick rate                      */
    OS_EXIT_CRITICAL();


    TaskStartCreateTasks();                                /* Create all the application tasks         */

    for (;;) {
		TaskStartDisp();
        OSCtxSwCtr = 0;                                    /* Clear context switch counter             */
        OSTimeDlyHMSM(0, 0, 1, 0);                         /* Wait one second                          */
    }
}


/*
*********************************************************************************************************
*                                        INITIALIZE THE DISPLAY
*********************************************************************************************************
*/

static  void  TaskStartDispInit (void)
{
/*                                1111111111222222222233333333334444444444555555555566666666667777777777 */
/*                      01234567890123456789012345678901234567890123456789012345678901234567890123456789 */
    PC_DispStr( 0,  0, "                         uC/OS-II, The Real-Time Kernel                         ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0,  1, "                                   Bee Bee                                      ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0,  2, "                               201308010118   dengxin                           ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0,  3, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0,  4, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0,  5, "        -------------------------------------------                             ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0,  6, "       |                                           | Welcome to play this game  ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0,  7, "       |                                           |  <-   :left                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0,  8, "       |                                           |   S   :start               ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0,  9, "       |                                           |   ->  :rigth               ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0, 10, "       |                                           |   A   :attack              ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0, 11, "       |                                           |   T   :stop                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0, 12, "       |                                           |                            ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0, 13, "       |                                           |                            ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0, 14, "       |                                           |   Scores:                  ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0, 15, "       |                                           |                            ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0, 16, "       |                                           |                            ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0, 17, "       |                                           |                            ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0, 18, "       |                                           |                            ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0, 19, "       |                                           |                            ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0, 20, "       |                                           |                            ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0, 21, "       |                                           |                            ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0, 22, "       |                                           |                            ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0, 23, "        -------------------------------------------                             ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
    PC_DispStr( 0, 24, "                            <-PRESS 'ESC' TO QUIT->                             ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY + DISP_BLINK);
/*                                1111111111222222222233333333334444444444555555555566666666667777777777 */
/*                      01234567890123456789012345678901234567890123456789012345678901234567890123456789 */

}


/*
*********************************************************************************************************
*                                           UPDATE THE DISPLAY
*********************************************************************************************************
*/

static  void  TaskStartDisp (void)
{
    char   s[80];


    sprintf(s, "Number of tasks:   %5d", OSTaskCtr);                                  /* Display #tasks running               */
    PC_DispStr(53, 21, s, DISP_FGND_YELLOW + DISP_BGND_BLUE);


    sprintf(s, "Number of ctxsw/s: %5d", OSCtxSwCtr);                                 /* Display #context switches per second */
    PC_DispStr(53, 22, s, DISP_FGND_YELLOW + DISP_BGND_BLUE);

    sprintf(s, "Number of ticks:   %d", OSTimeGet());                       /* Display time tick number after start    */
    PC_DispStr(53, 23, s, DISP_FGND_YELLOW + DISP_BGND_BLUE);
				

 }

/*
*********************************************************************************************************
*                                             CREATE TASKS
*********************************************************************************************************
*/

static  void  TaskStartCreateTasks (void)
{
	Init();
	OSTaskCreate(Shooter, (void *)0, &TaskStk[1][TASK_STK_SIZE - 1], 1);
	OSTaskCreate(UpdateGui, (void *)0, &TaskStk[2][TASK_STK_SIZE - 1], 2);
}

/*Init Function*/
void Init(){
	
INT8U i,j;
INT8U map[HEIGHT][WIDTH]={
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},//1
	{0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0},//5
	{0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0},//6
	{0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},//10
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}//17
};
/*init*/
for(i=0;i<HEIGHT;i++){
	for(j=0;j<WIDTH;j++){
		Map[i][j]=map[i][j];
	}
}
for(i=0;i<HEIGHT;i++){
	for(j=0;j<WIDTH;j++){
		PC_DispStr(j+8,i+6," ",DISP_FGND_YELLOW + DISP_BGND_BLUE);
	}
}

}

/*shoot function*/
void shoot(INT8U* x){
	
	OSTaskCreate(bullet, (void *)x, &TaskStk[3][TASK_STK_SIZE - 1], 5);
}

/*
*********************************************************************************************************
*                                                  TASKS
*********************************************************************************************************
*/
void bullet(void *pdata){
	char       s[100];
    INT8U  err;
	INT8U p=*((INT8U*)pdata);
	INT8U dx=p;
	INT8U dy=HEIGHT-1;
	pdata=pdata;
	for(;;){
		sprintf(s, "dx:%5d  dy:%5d",dx,dy );
		PC_DispStr(0, 0, s, DISP_FGND_YELLOW + DISP_BGND_BLUE);
		if(dy==0) {OSTaskDel(5);break;}
		if(Map[dy-1][dx]==0 && dy-1>=0) {
		if(dy!=HEIGHT-1)
		Map[dy][dx]=0;
		dy=dy-1;
		Map[dy][dx]=3;
		if(dy==0){Map[dy][dx]=0;
		}
		}
		if(Map[dy-1][dx]==2 && dy-1>=0) {
		Map[dy][dx]=0;
		dy=dy-1;
		Map[dy][dx]=0;
		score+=50;
		OSTaskDel(5);
		break;
		}
		if(Map[dy-1][dx]==4 && dy-1>=0) {
		Map[dy][dx]=0;
		dy=dy-1;
		Map[dy][dx]=0;
		score+=250;
		OSTaskDel(5);
		break;
		}
		OSTimeDlyHMSM(0, 0, 0, 50); //control buttet speed
	}
	
}

void  Shooter (void *pdata)
{
	INT16S     key;

    char       s[100];
    INT8U  err;
	INT8U x,y;
    pdata = pdata;
	x=WIDTH/2;
	y=HEIGHT-1;
	Map[y][x]=1;
    for (;;) {
		sprintf(s, "%d",score);
        PC_DispStr(64, 14, s, DISP_FGND_YELLOW + DISP_BGND_BLUE);
	if (PC_GetKey(&key) == TRUE) {                     /* See if key has been pressed              */
			sprintf(s, "key: %5d", key);
			PC_DispStr(0, 24, s, DISP_FGND_YELLOW + DISP_BGND_BLUE);
            switch(key){
				case 75://Left			
				/*left move*/
				Map[y][x]=0;
				if(x>0) x=x-1;
				Map[y][x]=1;
				break;
				//case 0x26://Up
				//case 0x57://W
				//break;
				case 77://Right
				/*right move*/
				Map[y][x]=0;
				if(x<WIDTH-1) x=x+1;
				Map[y][x]=1;
				break;
				//case 0x28://Down
				//case 0x53://S
				//break;
				case 116://T key for Pause
				break;
				case 115://S key for Continue
				break;
				case 97://shoot
				shoot(&x);
				break;
				case 27:
				PC_DOSReturn();
				break;
				default:break;
			}
        }
    OSTimeDlyHMSM(0, 0, 0, 80);          
    }

}

void  UpdateGui (void *pdata)
{

	char s[20];
    INT8U  err;
	INT8U x,y;
    pdata = pdata;
	
    for (;;) {
		for(x=0;x<HEIGHT;x++){
			for(y=0;y<WIDTH;y++){
				switch(Map[x][y]){
					case 0:
					PC_DispStr(y+8,x+6," ",DISP_FGND_YELLOW + DISP_BGND_BLUE);break;
					case 1:
					PC_DispStr(y+8,x+6,"H",DISP_FGND_YELLOW + DISP_BGND_BLUE);break;
					case 2:
					PC_DispStr(y+8,x+6,"W",DISP_FGND_YELLOW + DISP_BGND_BLUE);break;
					case 3://bullet
					PC_DispStr(y+8,x+6,"*",DISP_FGND_RED + DISP_BGND_BLUE);break;
					case 4://bullet
					PC_DispStr(y+8,x+6,"W",DISP_FGND_RED+ DISP_BGND_BLUE);break;
					default:
					break;
				}
			}
		}
        OSTimeDlyHMSM(0, 0, 0, 10);                        
    }

}