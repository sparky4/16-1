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
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>, or
 * write to the Free Software Foundation, Inc., 51 Franklin Street,
 * Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */
/*
 * 16 tail library
 * ment for lower level stuff
 */

#include "src/lib/16_tail.h"
//#include "src/lib/16text.h"

//===========================================================================

/*
==================
=
= DebugMemory
=
==================
*/

void DebugMemory_(boolean q)
{
	/*VW_FixRefreshBuffer ();
	US_CenterWindow (16,7);

	US_CPrint ("Memory Usage");
	US_CPrint ("------------");
	US_Print ("Total	 :");
	US_PrintUnsigned (mminfo.mainmem/1024);
	US_Print ("k\nFree	  :");
	US_PrintUnsigned (MM_UnusedMemory()/1024);
	US_Print ("k\nWith purge:");
	US_PrintUnsigned (MM_TotalFree()/1024);
	US_Print ("k\n");
	VW_UpdateScreen();*/
	if(q){
	printf("========================================\n");
	printf("		DebugMemory_\n");
	printf("========================================\n");}
	if(q) { printf("Memory Usage\n");
	printf("------------\n"); }else printf("	%c%c", 0xD3, 0xC4);
	printf("Total:	"); if(q) printf("	"); printf("%uk", mminfo.mainmem/1024);
	if(q) printf("\n"); else printf("	");
	printf("Free:	"); if(q) printf("	"); printf("%uk", MM_UnusedMemory()/1024);
	if(q) printf("\n"); else printf("	");
	printf("With purge:"); if(q) printf("	"); printf("%uk\n", MM_TotalFree()/1024);
	if(q) printf("------------\n");
#ifdef __WATCOMC__
	//IN_Ack ();
#endif
//	if(q) MM_ShowMemory ();
}

/*
==========================
=
= ClearMemory
=
==========================
*/

void ClearMemory (void)
{
#ifdef __16_PM__
	PM_UnlockMainMem();
#endif
	//sd
	MM_SortMem ();
}

/*
==========================
=
= Quit
=
==========================
*/

void Quit (char *error)
{
	//unsigned		finscreen;
	memptr	screen=0;

	//ClearMemory ();
	ClearMemory();
	if (!*error)
	{
// #ifndef JAPAN
// 		CA_CacheGrChunk (ORDERSCREEN);
// 		screen = grsegs[ORDERSCREEN];
// #endif
// 		WriteConfig ();
	}
	else
	{
// 		CA_CacheGrChunk (ERRORSCREEN);
// 		screen = grsegs[ERRORSCREEN];
	}
//	Shutdown16();
//shut down included managers

/*
	US_Shutdown ();
	SD_Shutdown ();
	PM_Shutdown ();
	IN_Shutdown ();
	VW_Shutdown ();
	CA_Shutdown ();
	MM_Shutdown ()
*/
#ifdef __16_US__
	US_Shutdown ();
#endif
#ifdef __16_SD__
	SD_Shutdown ();
#endif
#ifdef __16_PM__
	PM_Shutdown();
#endif
#ifdef __16_IN__
	IN_Shutdown();
#endif
#ifdef __16_VW__
	VW_Shutdown ();
#endif
#ifdef __16_CA__
	CA_Shutdown();
#endif
#ifdef __16_MM__
	MM_Shutdown();
#endif

	if (error && *error)
	{
		//movedata((unsigned)screen,7,0xb800,0,7*160);
		gotoxy (10,4);
		fprintf(stderr, "%s\n", error);
		gotoxy (1,8);
		exit(1);
	}
	else
	if (!error || !(*error))
	{
		clrscr();
#ifndef JAPAN
		movedata ((unsigned)screen,7,0xb800,0,4000);
		gotoxy(1,24);
#endif
//asm	mov	bh,0
//asm	mov	dh,23	// row
//asm	mov	dl,0	// collumn
//asm	mov ah,2
//asm	int	0x10
	}

	exit(0);
}

//===========================================================================

//
// for mary4 (XT)
// this is from my XT's BIOS
// http://www.phatcode.net/downloads.php?id=101
//
void turboXT(byte bakapee)
{
	__asm {
		push	ax
		push	bx
		push	cx
		in	al, 61h 			//; Read equipment flags
		xor	al, bakapee			//;   toggle speed
		out	61h, al 			//; Write new flags back

		mov	bx, 0F89h			//; low pitch blip
		and	al, 4				//; Is turbo mode set?
		jz	do_beep
		mov	bx, 52Eh			//; high pitch blip

#ifdef __BORLANDC__
	}
#endif
	do_beep:
#ifdef __BORLANDC__
	__asm {
#endif
		mov	al, 10110110b		//; Timer IC 8253 square waves
		out	43h, al 			//;   channel 2, speaker
		mov	ax, bx
		out	42h, al 			//;   send low order
		mov	al, ah				//;   load high order
		out	42h, al 			//;   send high order
		in	al, 61h 			//; Read IC 8255 machine status
		push	ax
		or	al, 00000011b
		out	61h, al 			//; Turn speaker on
		mov	cx, 2000h
#ifdef __BORLANDC__
	}
#endif
	do_delay:
#ifdef __BORLANDC__
	__asm {
#endif
		loop	do_delay
		pop	ax
		out	61h, al 			//; Turn speaker off
		pop	cx
		pop	bx
		pop	ax
	}
}
