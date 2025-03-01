/* Project 16 Source Code~
 * Copyright (C) 2012-2024 sparky4 & pngwen & andrius4669 & joncampbell123 & yakui-lover
 *
 * This file is part of Project 16.
 *
 * Project 16 is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Project 16 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You screen.heightould have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>, or
 * write to the Free Software Foundation, Inc., 51 Franklin Street,
 * Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include "src/bakapi.h"

/*
 * BAKAPEE!
 */
static bakapee_t bakapee;
word key,d,xpos,ypos,xdir,ydir;
sword vgamodex_mode = 1; //	1 = 320x240 with buffer
void TL_VidInit(global_game_variables_t *gvar){}
//int ch=0x0;

void drawboxesmodex(page_t *pg)
{
	VL_ClearRegion(pg, 0, 0, pg->width, pg->height, 1);
	VL_ClearRegion(pg, 16, 16, pg->sw, pg->sh, 2);
	VL_ClearRegion(pg, 32, 32, pg->sw-32, pg->sh-32, 3);
	VL_ClearRegion(pg, 48, 48, pg->sw-64, pg->sh-64, 2);
}

void copyboxesmodex(page_t *page, boolean pn)
{
	VL_CopyPageRegion(&page[pn], &page[!pn], 0, 0, 0, 0, page[pn].width, page[pn].height);
}


void
main(int argc, char *argvar[])
{
//	static global_game_variables_t gvar;
//	struct glob_game_vars	*ggvv;
	char *a;
	int i,c;
	word panq=1, pand=0,showding=0;
	boolean panswitch=0,bptest=0,runding=1;

//	ggvv=&gvar;

	// allow changing default mode from command line
	for (i=1;i < argc;) {
		a = argvar[i++];

		if (*a == '-') {
			do { a++; } while (*a == '-');

			if (!strcmp(a,"mx")) {
				// (based on src/lib/modex16.c)
				// 1 = 320x240
				// 2 = 160x120
				// 3 = 320x200
				// 4 = 192x144
				// 5 = 256x192
				vgamodex_mode = (sword)strtoul(argvar[i++],NULL,0);
			}
			else {
				fprintf(stderr,"Unknown switch %s\n",a);
				return;
			}
		}
		else {
			fprintf(stderr,"Unknown command arg %s\n",a);
			return;
		}
	}

	// initiate doslib //
//	TL_DosLibStartup(&gvar);

	// main variables values
	d=4; // switch variable
	key=2; // default screensaver number
	xpos=TILEWHD; ypos=TILEWHD; xdir=1; ydir=1;

	//VGAmodeX(vgamodex_mode, 0, &gvar); // TODO: Suggestion: Instead of magic numbers for the first param, might I suggest defining an enum or some #define constants that are easier to remember? --J.C.
	VL_Startup ();
	VL_SetVGAPlaneMode ();
	page[0] = VL_InitPage();
	page[1] = VL_NextPage(&page[0]);
	page[2] = VL_NextPageFlexibleSize(&page[1], page[1].sw, 88);
	page[3] = VL_NextPageFlexibleSize(&page[2], page[2].sw, 88);
	displayofs = PAGE1START;//+ylookup[16];
	bufferofs = displayofs;//PAGE2START;
//draw vidtest junk on screen for testin purposes
/*
	drawboxesmodex(&page[0]);
	copyboxesmodex(&page, 1);
	VL_ClearRegion(&page[2], 0, 0, page[2].sw, page[2].sh, 4);
	VL_ClearRegion(&page[3], 0, 0, page[3].sw, page[3].sh, 6);
*/
		// this code is written around modex16 which so far is a better fit than using DOSLIB vga directly, so leave MXLIB code in.
		// we'll integrate DOSLIB vga into that part of the code instead for less disruption. -- J.C.
	bakapee.xx = rand()&0%page[0].width; bakapee.yy = rand()&0%page[0].height;
	bakapee.gq = 0; bakapee.sx=bakapee.sy=0; bakapee.bakax=bakapee.bakay=0; bakapee.color=0;
	//once where #defines
	bakapee.tile=0; bakapee.keepcurrentcolor=400; bakapee.lgq=32; bakapee.hgq=55;

	switch(WCPU_detectcpu())
	{
		case 0:
			bakapee.tile=1;
		break;
		default:
			bakapee.tile=0;
		break;
	}

	// setup camera and screen~ //
	//SETUPPAGEBAKAPI

	VL_ShowPage(&page[0], 1, 0);
	//BAKAPIINITFIZZTEST
	VL_ClearVideo(0);

	while (bptest)
	{
/*		if (key > 0)
		{*/
		while (!kbhit() && runding)
			{
				//{ word w; for(w=0;w<(page[0].width*page[0].height);w++) {}}
				ding(&page[showding], &bakapee, 4);
			}
		if (kbhit())
//			{
			getch(); // eat keyboard input
//				break;
//			}
//		}*/

		{

			c = getch();
			switch (c)
			{
				case 27: // Escape key //
				case '0':
				default:
					bptest = false;
				break;
				case 'b': // test tile change //
					switch (bakapee.tile)
					{
						case 0:
							bakapee.tile=1;
						break;
						case 1:
							bakapee.tile=0;
						break;
					}
				break;
				case 'r':
					runding = false;
//					BAKAPIINITFIZZTEST
				break;
				case 'e':
					runding = 1;
				break;
				case 'z':
					runding = false;
//					FIZZFADEFUN
//					runding = true;
				break;
				case '3':
				case '4':
					runding = 1;
					showding = c - '0' - 3;
				break;
				case '1':
				case '2':
	//			case '5':
	//			case '6':
	//			case '9':
					key = c - '0' - 1;
					VL_ShowPage(&page[key], 1, 0);
				break;
			}
		}
	}


//while(!kbhit()){}

// screen savers
//#ifdef BOINK
	while(d>0)	// on!
	{
		/* run screensaver routine until keyboard input */
		while (key > 0) {
			if (kbhit()) {
				if(!panswitch)
				{
					getch(); // eat keyboard input
					break;
				}else c=getch();
			}

			if(!panswitch){
				if(key==9)
				{
					ding(&page[1], &bakapee, 4);
					ding(&page[0], &bakapee, 4);
//					FIZZFADEFUN
				}else ding(&page[0], &bakapee, key); }
			else			ding(&page[0], &bakapee, 2);
			if(panswitch!=0)
			{
				//right movement
				if((c==0x4d && pand == 0) || pand == 2)
				{
					if(pand == 0){ pand = 2; }
					if(panq<=(TILEWH/(4)))
					{
						page[0].dx++;
						VL_ShowPage(&page[0], 0, 0);
						panq++;
					} else { panq = 1; pand = 0; }
				}
				//left movement
				if((c==0x4b && pand == 0) || pand == 4)
				{
					if(pand == 0){ pand = 4; }
					if(panq<=(TILEWH/(4)))
					{
						page[0].dx--;
						VL_ShowPage(&page[0], 0, 0);
						panq++;
					} else { panq = 1; pand = 0; }
				}
				//down movement
				if((c==0x50 && pand == 0) || pand == 3)
				{
					if(pand == 0){ pand = 3; }
					if(panq<=(TILEWH/(4)))
					{
						page[0].dy++;
						VL_ShowPage(&page[0], 0, 0);
						panq++;
					} else { panq = 1; pand = 0; }
				}
				//up movement
				if((c==0x48 && pand == 0) || pand == 1)
				{
					if(pand == 0){ pand = 1; }
					if(panq<=(TILEWH/(4)))
					{
						page[0].dy--;
						VL_ShowPage(&page[0], 0, 0);
						panq++;
					} else { panq = 1; pand = 0; }
				}
				if((c==0x4d && pand == 0) || pand == 2)
				{
					if(pand == 0){ pand = 2; }
					if(panq<=(TILEWH/(4)))
					{
						page[0].dx++;
						VL_ShowPage(&page[0], 0, 0);
						panq++;
					} else { panq = 1; pand = 0; }
				}
				if(c==0x01+1)
				{
					VL_ShowPage(&page[0], 0, 0);
				}
				if(c==0x02+1)
				{
					VL_ShowPage(&page[1], 0, 0);
				}
				if(c==27 || c==0x71 || c==0xb1)
				{
					//getch(); // eat keyboard input
					panswitch=0;
					break; // 'q' or 'ESC' or 'p'
				}
			}
		}

		{

		// this code is written around modex16 which so far is a better fit than using DOSLIB vga directly, so leave MXLIB code in.
		// we'll integrate DOSLIB vga into that part of the code instead for less disruption. -- J.C.
			//VGAmodeX(0, 0, &gvar);
			VL_SetTextMode ();
			clrscr();	//added to clear screen wwww
			// user imput switch
			//fprintf(stderr, "xx=%d	yy=%d	tile=%d\n", bakapee.xx, bakapee.yy, bakapee.tile);
			//fprintf(stderr, "dx=%d	dy=%d	", page[0].dx, page[0].dy);
			printf("Tiled mode is ");
			switch (bakapee.tile)
			{
				case 0:
					printf("off.	");
				break;
				case 1:
					printf("on.	");
				break;
			}
			printf("Pan mode is ");
			switch (panswitch)
			{
				case 0:
					printf("off.");
				break;
				case 1:
					printf("on.");
				break;
			}
			printf("\n");
			printf("Incrementation of color happens at every %uth plot.\n", bakapee.keepcurrentcolor);
			printf("Enter 1, 2, 3, 4, 5, 6, 8, or 9 to run a screensaver, or enter 0 to quit.\n");
pee:
			c = getch();
			switch (c) {
				case 27: /* Escape key */
				case '0':
					d=0;
					break;
				case 'p': // test pan
					switch (panswitch)
					{
						case 0:
							panswitch=1;
						break;
						case 1:
							panswitch=0;
						break;
					}
					key=0;
					goto pee;
				break;
				case 'b': // test tile change
					switch (bakapee.tile)
					{
						case 0:
							bakapee.tile=1;
						break;
						case 1:
							bakapee.tile=0;
						break;
					}
					key=0;
				break;
				case '8':
					c+=8;
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '9':
					key = c - '0';
//					VL_Shutdown ();
//					VGAmodeX(vgamodex_mode, 0, &gvar);
//					SETUPPAGEBAKAPI
					VL_Startup ();
					VL_SetVGAPlaneMode ();
					VL_ClearVideo(0);
		// this code is written around modex16 which so far is a better fit than using DOSLIB vga directly, so leave MXLIB code in.
		// we'll integrate DOSLIB vga into that part of the code instead for less disruption. -- J.C.
					VL_ShowPage(&page[0], 0, 0);
				break;
				case '-':
					if(bakapee.keepcurrentcolor>0)
						bakapee.keepcurrentcolor-=100;
				break;
				case '=':
				case '+':
					if(bakapee.keepcurrentcolor<1000)
						bakapee.keepcurrentcolor+=100;
				break;
				default:
					key=0;
				break;
			}
		}
	}
	clrscr();	//added to clear screen wwww
#if 0
//#else // !defined(BOINK)
// FIXME: Does not compile. Do you want to remove this?
// INFO: This is a testing section for textrendering and panning for project 16 --sparky4
	while(1)
	{ // conditions of screen saver
// 		while(!kbhit())
// 		{
// 			ding(&page[0], &bakapee, key);
// 		}
		//end of screen savers
		//pdump(&page[0]);

// 		mxOutText(xpos+1, ypos+page[0].height-48, "========================================");
// 		mxOutText(xpos+1, ypos+page[0].height-40, "|    |Chikyuu:$line1");
// 		mxOutText(xpos+1, ypos+page[0].height-32, "|    |$line2");
// 		mxOutText(xpos+1, ypos+page[0].height-24, "|    |$line3");
// 		mxOutText(xpos+1, ypos+page[0].height-16, "|    |$line4");
// 		mxOutText(xpos+1, ypos+page[0].height-8,  "========================================");

	ding(&page[0], &bakapee, key);
	modexPanPage(&page[0], xpos, ypos);
	c = getch();

// 	xpos+=xdir;
// 	ypos+=ydir;
// 	if( (xpos>(page[0].sw-page[0].width-1))  || (xpos<1)){xdir=-xdir;}
// 	if( (ypos>(page[0].sh-page[0].height-1)) || (ypos<1)){ydir=-ydir;}
//	ch=getch();
	if(ch==0x71)break; // 'q'
	if(ch==0x1b)break; // 'ESC'
	}
	VGAmodeX(0, 1, &gvar);
#endif // defined(BOINK)
//	printf("page.width=%u	", page[0].width); printf("page.height=%u\n", page[0].height);
//	for(i=0;i<8;i++)	{		printf("[%d]	", ylookup[i]);	}	//test code
#ifdef __BORLANDC__
	printf("bcbakapi ");
#endif
#ifdef __WATCOMC__
	printf("bakapi ");
#endif
	printf("ver. 1.04.16.04\nis made by sparky4�i���ց��j feel free to use it ^^\nLicence: GPL v3\n");
//	printf("compiled on 2016/04/04\n");	// old --sparky4
	printf("compiled on %s\n", VERSION);
//	printf("[%u]%dx%d	[%dx%d]	%u %u %u\n[%u	%u	%u]", key, bakapee.bakax, bakapee.bakay, bakapee.xx, bakapee.yy,
//bakapee.coor, bakapee.tile, bakapee.gq, bakapee.keepcurrentcolor, bakapee.lgq, bakapee.hgq);
}
//pee!
