/* options analyst program -- main source   */
/* started 14/5/88.        A and P Tridgell */
/* First version completed and manual printed 18/7/88 */

#define MAIN


#include <conio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <graphics.h>

#include "optdef.h"
#include "options.h"



int tosavesys = FALSE;
double totalvalue,totaldelta,weightedvol;

char months[12][3]
= {"JAN","FEB","MAR","APR","MAY","JUN","JUL","AUG","SEP","OCT","NOV","DEC"};



char filename[20] = "DEMO";

int numpts  = 30;
pagetype page = PAGEUP;

int syear;
int smonth;
int sday;

long totalcalls;
long totalputs;
double weightedvolc,weightedvolp;
int recalcvolvalues = TRUE;

static char *utilchoice[8] =
{{"Utilities"},
 {"Directory"},
 {"Bonus issue"},
 {"Cash issue"},
{"Graph resolution"},
{"Print details"},
{"Delete File"},
{"Set Up"}
};

static char *printchoice[4] =
{{"Print Style Choice"},
{"Position Style"},
{"Short Data"},
{"Long Data"}
};

static char *clearchoice[4] =
{{"Clearing Utility"},
  {"Clear Row"},
  {"Clear Column"},
  {"Clear All"}
  };

static char *resolutionchoice[4] =
{{"Graph resolution"},
 {"Coarse graph"},
 {"Medium graph"},
 {"Fine graph"}
 };

static char *quitchoice[4] =
{{"Quit?"},
 {"Quit - Don't save"},
 {"Save then Quit"},
 {"Return to Program"}
 };

static char *graphchoice[7] =
{{"Graphing"},
 {"Whole Position - Price"},
 {"               - Time"},
 {"This Option    - Price"},
 {"               - Time"},
 {"Previous       - Price"},
 {"               - Time"}};


static char *screenchoice[4] =
{{"Screens"},
 {"Main Screen"},
 {"Dividends"},
 {"Inversion"}
 };



/* COLOR CGA CARD */
graphicstype ccgagraphics =
	{"CCGA",
	 {RED,YELLOW,LIGHTGREEN,LIGHTGRAY,LIGHTCYAN,LIGHTBLUE,
			  BLACK,RED,LIGHTGRAY,LIGHTGRAY,BLUE},
         {-1,0x0608},
	 {LIGHTGRAY,1,3,2,2,2,2,2},
	 {1,2,1,1,4},
	 {1,4,3}};

/* MONO CGA CARD */
graphicstype mcgagraphics =
        {"MCGA",
	{ LIGHTGRAY,LIGHTGRAY,LIGHTGRAY,LIGHTGRAY,LIGHTGRAY,
	LIGHTGRAY,BLACK,BLACK,
	LIGHTGRAY,LIGHTGRAY,BLACK},
         {-1,1750},
	 {LIGHTGRAY,1,1,1,1,1,1,1},
	 {1,2,1,3,4},
	 {1,4,3}};

/* HERCULES CARD */
graphicstype hercgraphics =
        {"HERC",
	{ LIGHTGRAY,LIGHTGRAY,LIGHTGRAY,LIGHTGRAY,LIGHTGRAY,
	LIGHTGRAY,BLACK,BLACK,
	LIGHTGRAY,LIGHTGRAY,BLACK},
         {-1,1750},
	 {0,1,1,1,1,1,1,1},
	 {1,2,1,3,4},
	 {2,4,4}};

/* COLOR EGA CARD */
graphicstype cegagraphics =
	{"CEGA",
	{RED,YELLOW,LIGHTGREEN,LIGHTGRAY,LIGHTCYAN,LIGHTBLUE,
			  BLACK,RED,LIGHTGRAY,LIGHTGRAY,BLUE},
         {2304,1750},
	 {BLACK,GREEN,YELLOW,LIGHTRED,BLUE,CYAN,LIGHTMAGENTA,LIGHTGREEN},
	 {1,2,1,1,4},
	 {2,4,4}};

/* MONO EGA CARD */
graphicstype megagraphics =
     {"MEGA",
     { LIGHTGRAY,LIGHTGRAY,LIGHTGRAY,LIGHTGRAY,LIGHTGRAY,
	LIGHTGRAY,BLACK,BLACK,
	LIGHTGRAY,LIGHTGRAY,BLACK},
         {2304,1750},
	 {BLACK,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE},
	 {1,2,1,3,4},
	 {2,4,4}};



statustype status
=	{
        "OPTIONS ANALYST V1.0",
        "EFAM RESOURCES"};

systype sys
=       {
        "OPTIONS SYSTEM V1.0",
        "01JAN80",
	 1,
	 "SAMPLE.SAV",
	 {8,MONTH},
	 SCREEN1,
	 2,
	 HELDS,
       };


divtype divmatrix[27];

char codename[65] =  "AAAAA";
char startname[65] =  "\x20\x8\DEMONSTRATION VERSION";


/* FUNCTIONS */


void demo()
{
static char *demotext =
"   This is a demonstration version of the Options Analyst.\n"
"In this version you may not save or recall positions to\n"
"disk or alter the stockprice. In the full version, of \n"
"course, there is no such restriction.\n"
"\n"
"   Please make un-altered copies of this program and\n"
"distribute them. This version, however, may be used for \n"
"NON-COMMERCIAL purposes only and remains the property \n"
"of EFAM RESOURCES\n"
"\n"
"   TO BUY YOUR COPY CONTACT : IS2 Pty. Ltd.\n"
"PO. BOX  H37 Australia Square Sydney, N.S.W. 2000.\n"
"Telephone (02) 27 4686  Fax: 27 5898\n"
"                                           PRESS ANY KEY";

showhelp("DEMONSTRATION VERSION",demotext);
}

void demo1()
{
static char *demo1text =
"   This Function allows tables of values to printed. An   \n"
"initial stockprice is entered, and a stockprice increment.\n"
"These tables show the value of the options for either 5 or\n"
"10 shareprices. This function is useful if you are going\n"
"somewhere where you do not have access to a computer.\n"
"\n"
"   Please make un-altered copies of this program and\n"
"distribute them. This version, however, may be used for \n"
"NON-COMMERCIAL purposes only and remains the property \n"
"of EFAM RESOURCES\n"
"   TO BUY YOUR COPY CONTACT : IS2 Pty. Ltd.\n"
"PO. BOX  H37 Australia Square Sydney, N.S.W. 2000.\n"
"Telephone (02) 27 4686  Fax: 27 5898\n"
"                                           PRESS ANY KEY";

showhelp("DEMONSTRATION VERSION",demo1text);
}




void clearrow()
{
celltype 	cell;
	textbackground(sys.graphics.colors.databack);
	status.data[sys.cell.row-8].month = -1;
	status.data[sys.cell.row-8].strike= 0;
	status.data[sys.cell.row-8].heldc= 0;
	status.data[sys.cell.row-8].heldp = 0;
	status.data[sys.cell.row-8].valuec=0;
	status.data[sys.cell.row-8].valuep=0;
        status.data[sys.cell.row-8].volc= 0;
	status.data[sys.cell.row-8].volp = 0;
	status.data[sys.cell.row-8].marketc=0;
	status.data[sys.cell.row-8].marketp=0;
	cell.row= sys.cell.row;
	for (cell.col = MONTH ; cell.col <= HELDP ;cell.col++)
	blankcell(cell);
	for (cell.col = VOLC ; cell.col <= MARKETP ;cell.col++)
	blankcell(cell);
	 }



char editcell(celltype cell)
{
int x,y;
long z;
char ch;
char sharestring[12];
int  pageint;
if (page == PAGEUP || sys.screen == SCREEN2 || cell.row < 8) pageint = 0;
else pageint = 15;
x = endcell(cell.col)-1;
y = cell.row - pageint ;

   switch (cell.col)
       {case MONTH :  entercell(sys.cell);
		     if (status.data[cell.row-8].month == -1 &&
			 cell.row != 8 )
			 status.data[cell.row-8].month =
			inputmonth(x-2,y,(status.data[cell.row-9].month)-1);
			else
			 status.data[cell.row-8].month =
			inputmonth(x-2,y,status.data[cell.row-8].month);
		     wait();
		     if (sys.screen == SCREEN3) calcvolrow(cell.row);
                     calcrow(cell.row);
		     showrow(cell.row);
		     if ( cell.row != 22 && cell.row != 37)
		     {
		     cell.row += 1;
		     showcell(cell);
		     cell.row = cell.row -1;
		     }
		      /* show date of line below if it
			    had been hidden by showall suppression */
		     break;
	case STRIKE : entercell(sys.cell);
		      if (status.stockprice != 4.0) exit(0);
		      status.data[cell.row-8].strike =
		      inputreal(x,y,status.data[cell.row-8].strike,8,sys.decimal);
		      wait();
		     if (sys.screen == SCREEN3) calcvolrow(cell.row);
		      calcrow(cell.row);
                      showrow(cell.row);
		      break;
	case VALUEC : getch();break;
	case VALUEP : getch();break;
	case HELDC : entercell(sys.cell);
		     if (status.stockprice != 4.0) exit(0);
		     status.data[cell.row-8].heldc =
		     inputinteger(x,y,status.data[cell.row-8].heldc,4);
		     wait();
		     showcell(sys.cell);
		     break;
	case HELDP : entercell(sys.cell);
		     if (status.stockprice != 4.0) exit(0);
		     status.data[cell.row-8].heldp =
		     inputinteger(x,y,status.data[cell.row-8].heldp,4);
		     wait();
		     showcell(sys.cell);
		     break;
	case STOCKHELD :
			entercell(sys.cell);
			z = status.stockheld;
		     z =
		     inputlint(x,y,&z,8);
		     wait();
		     break;
	case SHAREPRICE : entercell(sys.cell);
			status.stockprice = 4.0;
			getch();
			demo();
			getch();
			break;
        case VOLATILITY : entercell(sys.cell);
			status.volatility =
			inputreal(x,y,status.volatility,5,2);
			wait();
			calcall();
			calcalloverval();
			showall();
			break;
        case INTEREST : entercell(sys.cell);
			status.interest=
			inputreal(x,y,status.interest,5,2);
			wait();
			initdivmtx(0);
			calcall();
			if (sys.screen == SCREEN3) calcallvol();
			else recalcvolvalues = TRUE;
			showall();
			break;
        case DATE    : 	entercell(sys.cell);
			inputdate(x,y,sys.date);
			wait();
			initdates();
			initsizepay();
			calcdays();
			initdivmtx(0);
			calcall();
			if (sys.screen == SCREEN3) calcallvol();
			else recalcvolvalues = TRUE;
			showall();
			break;
	case SHARESPER : entercell(sys.cell);
			status.sizepay[y].sharesper=
			inputinteger(x,y,status.sizepay[y].sharesper,4);
                        if (status.sizepay[y].sharesper < 1)
			status.sizepay[y].sharesper = 1;
			wait();
                        break;
	case EXPIRYDAY : entercell(sys.cell);
			sys.expiry[y].eday =inputinteger(x,y,sys.expiry[y].eday,2);
			if (sys.expiry[y].eday < 1) sys.expiry[y].eday = 1;
			else if (sys.expiry[y].eday > 31) sys.expiry[y].eday = 28;
			wait();
			initdates();
			calcdays();
			initdivmtx(0);
			calcall();
			tosavesys = TRUE;
		      	showall();
			break;
	case DIVIDENDDAY : entercell(sys.cell);
			status.sizepay[y].dday=inputinteger(x,y,status.sizepay[y].dday,2);
			if (status.sizepay[y].dday < 0)
			status.sizepay[y].dday = 0;
			else if (status.sizepay[y].dday > 31)
			status.sizepay[y].dday = 28;
			wait();
			calcdays();
			initdivmtx(0);
                        calcall();
			showrow(cell.row);
                        break;
	case DIVIDENDCENTS : entercell(sys.cell);
			status.sizepay[y].payout
			= inputinteger(x,y,status.sizepay[y].payout,3);
			if (status.sizepay[y].payout < 0)
			status.sizepay[y].payout = 0;
			wait();
			initdivmtx(0);
                        calcall();
			showrow(cell.row);
			break;
	case VOLC :     getch();
			break;
	case VOLP :     getch();
			break;
	case MARKETC : entercell(sys.cell);
			if (status.stockprice != 4.0) exit(0);
		      status.data[cell.row-8].marketc =
		      inputreal(x,y,status.data[cell.row-8].marketc,8,sys.decimal);
		      if (status.data[cell.row-8].marketc < 0)
		      status.data[cell.row-8].marketc = 0;
			wait();
			status.data[cell.row-8].volc = invertvolc(cell.row);
			calcoverval(cell.row);
			showrow(cell.row);
			break;
	case MARKETP : entercell(sys.cell);
		      if (status.stockprice != 4.0) exit(0);
		      status.data[cell.row-8].marketp =
		      inputreal(x,y,status.data[cell.row-8].marketp,8,sys.decimal);
		      if (status.data[cell.row-8].marketp < 0)
		      status.data[cell.row-8].marketp = 0;
			wait();
			status.data[cell.row-8].volp = invertvolp(cell.row);
			calcoverval(cell.row);
			showrow(cell.row);
			break;
       }
       entercell(cell);
       totals();
       showtotals();
       if (status.stockprice != 4.0) exit(0);
       ready();
       ch = getch();
       return(ch);
}




void docommand(char ch)
{
int count;
char dateb[7];
switch (ch)
       	{
	 case ARROWR : if (status.stockprice != 3 + 1) exit(0);goright(); break;
         case ARROWL : if (status.stockprice != 5 - 1) exit(0);goleft(); break;
	 case ARROWU : if (status.stockprice != 2 + 2) exit(0);goup(); break;
	 case ARROWD : if (status.stockprice != 3 + 1) exit(0);godown(); break;
	 case HOME :  leavecell(sys.cell);
		     if ((sys.screen == SCREEN1 || sys.screen == SCREEN3)
			  && page == PAGEDOWN)
		     {
		     sys.cell.row = 23;
		     sys.cell.col = MONTH;
		     }
		     else
                     if (sys.screen == SCREEN2)
		     {
		     sys.cell.row = 8;
		     sys.cell.col = SHARESPER;
		     }
		     else
		     {
		     sys.cell.row = 8;
		     sys.cell.col = MONTH;
		     }
		     entercell(sys.cell);
		     break;
	 case END :  leavecell(sys.cell);
		     if ((sys.screen == SCREEN1 || sys.screen == SCREEN3)
			  && page == PAGEDOWN)
		     {
		     sys.cell.row = 37;
		     sys.cell.col = MONTH;
		     }
		     else
                     if (sys.screen == SCREEN2)
		     {
		     sys.cell.row = 22;
		     sys.cell.col = SHARESPER;
		     }
		     else
		     {
		     sys.cell.row = 22;
		     sys.cell.col = MONTH;
		     }
		     entercell(sys.cell);
		     break;

	 case PGUP	: if ((sys.screen == SCREEN1 || sys.screen == SCREEN3)
			      && page == PAGEDOWN)
			{
			page = PAGEUP;
			if (sys.cell.row >= 8)
			sys.cell.row -= 15;
			disppage();
			}
			showall();
			entercell(sys.cell);
			break;
	 case PGDN	: if ((sys.screen == SCREEN1 || sys.screen == SCREEN3)
				&& page == PAGEUP)
			{
			page = PAGEDOWN;
			if (sys.cell.row >= 8)
			sys.cell.row += 15;
                        disppage();
			}
			showall();
			entercell(sys.cell);
			break;
         case F01 :	help();break;
         case F02 :     demo();
			break;
	 case F03 :     demo();
			break;
         case F04 : sortstatus(); genscreen(); entercell(sys.cell); break;
         case F05 :
		{
		    if (drawgraph(choice(6,graphchoice)) < 0)
		    errormessage("Graphics initialisation error");
		    genscreen();
		    showtotals();
		    entercell(sys.cell);
		 }
		    break;
         case F06 :
         	    leavecell(sys.cell);
		 switch (choice(3,screenchoice))
		 {
 		 case 1 :
		      if (tosavesys) savesys();
		      sys.screen = SCREEN1;
		      page = PAGEUP;
		      sys.cell.col = MONTH;
      		      sys.cell.row = 8;
		      sys.display = HELDS;
		      genscreen();
		      break;
		 case 2 :
		      sys.screen = SCREEN2;
		      sys.display = HELDS;
		      page = PAGEUP;
      		      sys.cell.row = 8;
		      sys.cell.col = EXPIRYDAY;
		      genscreen();
		      break;
		 case 3 :
		      if (tosavesys) savesys();
		      sys.cell.col = MONTH;
		      page = PAGEUP;
		      sys.screen = SCREEN3;
		      sys.display = INVVOL;
      		      sys.cell.row = 8;
		      wait();
		      if (recalcvolvalues)
		      calcallvol();
		      ready();
		      genscreen();
		      break;
		 }
		 showtotals();
		 entercell(sys.cell);
		 break;
            case F07 :
		   switch(choice(3,clearchoice))
		   {
		  case 1 :  if (sys.cell.row < 8 || sys.screen == SCREEN2) break;
			    wait();
			    clearrow();
				 calcall();
				 showall();
				 totals();
                              entercell(sys.cell);
			      break;
		   case 2 : wait();
			    switch( sys.cell.col)
			    {
                            case VOLC :
                            	case VALUEC :
				case VOLP :
				case DELTAC :
				case VALUEP :
                            	case DELTAP:
                            	case SHAREPRICE :break;
				case STOCKHELD : status.stockheld = 0; break;
				case VOLATILITY : status.volatility = 0.0;break;
				case INTEREST   : status.interest = 0.0; break;
				case  DATE      :
				case YEARMONTH :
				case SHARESPER : for (count =0;count <24; count++)
						   status.sizepay[count].sharesper = 1000;
						   break;               
				case EXPIRYDAY :
				case DAYSLEFT :
                            case MONTH : break;
                            case STRIKE : for (count = 0;count < 30; count++)
                                          {
					  status.data[count].strike = 0.0;
					  status.data[count].volc = 0.0;
					  status.data[count].volp = 0.0;
					  }
					  break;
				case MARKETC : for (count = 0;count < 30; count++)
                                        {
					status.data[count].marketc = 0.0;
					status.data[count].volc = 0.0;
					}
					  break;
				case HELDC :  for (count = 0;count < 30; count++)
					  status.data[count].heldc = 0;
					  break;
				case MARKETP : for (count = 0;count < 30; count++)
                                          {
					  status.data[count].marketp = 0.0;
					  status.data[count].volp = 0.0;
					  }
					  break;
				case HELDP :  for (count = 0;count < 30; count++)
					  status.data[count].heldp = 0;
					  break;
				case DIVIDENDDAY : for (count = 0;count < 24; count++)
					  status.sizepay[count].dday = 0;
					  break;
			        case DIVIDENDCENTS :  for (count = 0;count < 24; count++)
					  status.sizepay[count].payout = 0;
					  break;

			    }  wait();
				 calcall();
				 showall();
				 totals();  break;
			  case 3:   clearall();wait();   wait();
				 calcall();
				 showall();
				 totals();break;
			  /* clear all set shares per con */


		    }           showtotals();
				 ready();
				 entercell(sys.cell);
				 /* inverse vol now invalid */
		    break;


         case F08 :
			leavecell(sys.cell);
		   switch( choice(7, utilchoice))
		      {
			case 1 : getdir(); break;
			case 2 : bonusissue();
				 wait();
				 calcall();
				 showall();
				 totals();
				 showtotals();
				 ready();
				 break;
			case 3 : cashissue();
				 wait();
				 calcall();
				 showall();
				 totals();
				 showtotals();
				 ready();
				 break;
			 case 4: switch( choice(3,resolutionchoice))
				 {
				   case 1: numpts = 10; break;
				   case 2: numpts = 15; break;
				   case 3: numpts = 30; break;
				 }
                                    break;
			 case 5:switch (choice(3,printchoice))
				  {
				     case 1: printdetails(); break;
				     case 2: demo1();break;
				     case 3: demo1(); break;
					   } break;
			 case 6: filedelete(); break;
			 case 7 : reinstall();
		         }
			 entercell(sys.cell);
			 break;

         case F09 : switch (sys.screen)
			{
			case SCREEN1 :
			    leavecell(sys.cell);
			    if (sys.display == HELDS) sys.display = DELTAS;
			    else sys.display = HELDS;
		    	if (sys.cell.row >= 8)
		   	 sys.cell.col = MONTH;
		    	textcolor(sys.graphics.colors.heading);
		    	textbackground(sys.graphics.colors.databack);
		    	drawheadings();
		    	showall();
			showtotals();
		    	entercell(sys.cell);break;
			case SCREEN2 : break;
			case SCREEN3 :
			    leavecell(sys.cell);
			    if (sys.display == INVVOL) sys.display = OVERVALUED;
			    else sys.display = INVVOL;
 		    	    textcolor(sys.graphics.colors.heading);
		    	    textbackground(sys.graphics.colors.databack);
		    	    drawheadings();
		    	    showall();
			    showtotals();
		    	    entercell(sys.cell);break;
		       }
		    break;
         case F10 : switch (choice(3,quitchoice))
		    {
		    case 0 : break;
		    case 1 : wait();
	   		    textcolor(LIGHTGRAY);
			    textbackground(BLACK);
			    savesys();
			    clrscr();
			    gotoxy(2,2);
			    cprintf(" The Option Analyst is a product of");
			    gotoxy(2,3);
			    cprintf(" EFAM RESOURCES Pty. Ltd.\n");
			    retcursor();
			    exit(0);
			    break;
                    case 2 : demo();
			    break;
		    case 3 : break;
		    }
		    break;
	}
}





void movement()
{
char ch1,ch2;
entercell(sys.cell);

do {
	ch1 = getch();
	if (ch1 == 0)
	{
       	ch2 = getch();
        docommand(ch2);
	}
	else
	if (isalnum(ch1) || ch1 == 27 || ch1 == 8 || ch1 == '+' || ch1 == '-' || ch1 == '.')
	{
         ungetch(ch1);
	if ((sys.screen== SCREEN1) && ( status.data[sys.cell.row-8].month == -1) &&
	(sys.cell.col != MONTH) && (sys.cell.col <= HELDP))
	{
	leavecell(sys.cell);
	 clearrow();
	sys.cell.col = MONTH;
	entercell(sys.cell);
	}
	 ch2 = editcell(sys.cell);
	 docommand(ch2);
	}
   } while (1);
}



void newfile()
{
sys.cell.row = 8;
sys.cell.col = MONTH;
sys.decimal = 2;
sys.screen = SCREEN1;
sys.display = HELDS;
clearall();
}




/* MAIN */

main()
{
strcpy(sys.usernames, startname);
savecursor();
clrscr();
registerfarbgidriver(CGA_driver_far);
registerfarbgidriver(EGAVGA_driver_far);
registerfarbgidriver(Herc_driver_far);
registerfarbgifont(triplex_font_far);
registerfarbgifont(small_font_far);
newfile();
{
initialise();
if (sys.dateget == TRUE)
systemdate();
window(1,1,80,25);
clrscr();
hidecursor();
initdates();
calcdays();
initsizepay();
initdivmtx(0);
calcall();
sys.cell.row = 8;
sys.cell.col = MONTH;
sys.screen = SCREEN1;
page =  PAGEUP;
status.stockprice = 4.0;
totals();
if (status.stockprice != 3 + 1) exit(0);
sortstatus();
genscreen();
copyright();
demo();
loadfile("DEMO");
showall();
totals();
showtotals();
movement();}
retcursor();
}
