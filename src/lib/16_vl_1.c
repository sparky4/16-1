/* Project 16 Source Code~
 * Copyright (C) 2012-2017 sparky4 & pngwen & andrius4669 & joncampbell123 & yakui-lover
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

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include "src/lib/16_vl.h"

/*
=============================================================================

						PALETTE OPS

		To avoid snow, do a WaitVBL BEFORE calling these

=============================================================================
*/


/*
=================
=
= VL_FillPalette
=
=================
*/

void VL_FillPalette (int red, int green, int blue)
{
	int	i;

	outportb (PAL_WRITE_REG,0);
	for (i=0;i<256;i++)
	{
		outportb (PAL_DATA_REG,red);
		outportb (PAL_DATA_REG,green);
		outportb (PAL_DATA_REG,blue);
	}
}

//===========================================================================

/*
=================
=
= VL_SetColor
=
=================
*/

void VL_SetColor	(int color, int red, int green, int blue)
{
	outportb (PAL_WRITE_REG,color);
	outportb (PAL_DATA_REG,red);
	outportb (PAL_DATA_REG,green);
	outportb (PAL_DATA_REG,blue);
}

//===========================================================================

/*
=================
=
= VL_GetColor
=
=================
*/

void VL_GetColor	(int color, int *red, int *green, int *blue)
{
	outportb (PAL_READ_REG,color);
	*red = inportb (PAL_DATA_REG);
	*green = inportb (PAL_DATA_REG);
	*blue = inportb (PAL_DATA_REG);
}

//===========================================================================

/*
=================
=
= VL_SetPalette
=
= If fast palette setting has been tested for, it is used
= (some cards don't like outsb palette setting)
=
=================
*/

void VL_SetPalette (byte far *palette, video_t *v)
{
//	int	i;
	boolean fastpalette;
	fastpalette=v->fastpalette;

//	outportb (PAL_WRITE_REG,0);
//	for (i=0;i<768;i++)
//		outportb(PAL_DATA_REG,*palette++);

	__asm {
		mov	dx,PAL_WRITE_REG
		mov	al,0
		out	dx,al
		mov	dx,PAL_DATA_REG
		lds	si,[palette]

		test	[ss:fastpalette],1
		jz	slowset
//
// set palette fast for cards that can take it
//
		//mov	cx,768
		//rep outsb
		//jmp	done

//
// set palette slowly for some video cards
//
#ifdef __BORLANDC__
	}
#endif
slowset:
#ifdef __BORLANDC__
	__asm {
#endif
		mov	cx,256
#ifdef __BORLANDC__
	}
#endif
setloop:
#ifdef __BORLANDC__
	__asm {
#endif
		lodsb
		out	dx,al
		lodsb
		out	dx,al
		lodsb
		out	dx,al
		loop	setloop
#ifdef __BORLANDC__
	}
#endif
done:
#ifdef __BORLANDC__
	__asm {
#endif
		mov	ax,ss
			mov	ds,ax
	}
	v->fastpalette=fastpalette;
}


//===========================================================================

/*
=================
=
= VL_GetPalette
=
= This does not use the port string instructions,
= due to some incompatabilities
=
=================
*/

void VL_GetPalette (byte far *palette)
{
	int	i;

	outportb (PAL_READ_REG,0);
	for (i=0;i<768;i++)
		*palette++ = inportb(PAL_DATA_REG);
}


//===========================================================================

/*
=================
=
= VL_FadeOut
=
= Fades the current palette to the given color in the given number of steps
=
=================
*/

void VL_FadeOut (int start, int end, int red, int green, int blue, int steps, video_t *v)
{
	int		i,j,orig,delta;
	byte	far *origptr, far *newptr;

	VL_WaitVBL(1);
	VL_GetPalette (&v->palette1[0][0]);
	_fmemcpy (v->palette2,v->palette1,PALSIZE);

//
// fade through intermediate frames
//
	for (i=0;i<steps;i++)
	{
		origptr = &v->palette1[start][0];
		newptr = &v->palette2[start][0];
		for (j=start;j<=end;j++)
		{
			orig = *origptr++;
			delta = red-orig;
			*newptr++ = orig + delta * i / steps;
			orig = *origptr++;
			delta = green-orig;
			*newptr++ = orig + delta * i / steps;
			orig = *origptr++;
			delta = blue-orig;
			*newptr++ = orig + delta * i / steps;
		}

		VL_WaitVBL(1);
		VL_SetPalette (&v->palette2[0][0], v);
	}

//
// final color
//
	VL_FillPalette (red,green,blue);

	v->screenfaded = true;
}


/*
=================
=
= VL_FadeIn
=
=================
*/

void VL_FadeIn (int start, int end, byte far *palette, int steps, video_t *v)
{
	int		i,j,delta;

	VL_WaitVBL(1);
	VL_GetPalette (&v->palette1[0][0]);
	_fmemcpy (&v->palette2[0][0],&v->palette1[0][0],sizeof(v->palette1));

	start *= 3;
	end = end*3+2;

//
// fade through intermediate frames
//
	for (i=0;i<steps;i++)
	{
		for (j=start;j<=end;j++)
		{
			delta = palette[j]-v->palette1[0][j];
			v->palette2[0][j] = v->palette1[0][j] + delta * i / steps;
		}

		VL_WaitVBL(1);
		VL_SetPalette (&v->palette2[0][0], v);
	}

//
// final color
//
	VL_SetPalette (palette, v);
	v->screenfaded = false;
}



/*
=================
=
= VL_TestPaletteSet
=
= Sets the palette with outsb, then reads it in and compares
= If it compares ok, fastpalette is set to true.
=
=================
*/

void VL_TestPaletteSet (video_t *v)
{
	int	i;

	for (i=0;i<768;i++)
		v->palette1[0][i] = i;

	v->fastpalette = true;
	VL_SetPalette (&v->palette1[0][0], v);
	VL_GetPalette (&v->palette2[0][0]);
	if (_fmemcmp (&v->palette1[0][0],&v->palette2[0][0],768))
		v->fastpalette = false;
}


/*
=============================================================================

							PIXEL OPS

=============================================================================
*/

//byte	rightmasks[4] = {1,3,7,15};

/*
=================
=
= VL_Plot
=
=================
*/

void VL_Plot (int x, int y, int color, ofs_t *ofs)
{
	byte mask;
	VCLIPDEF

	mask = pclip[x&3];
	VGAMAPMASK(mask);
	*(byte far *)MK_FP(SCREENSEG,ofs->bufferofs+(ofs->ylookup[y]+(x>>2))) = color;
	VGAMAPMASK(15);
}


/*
=================
=
= VL_Hlin
=
=================
*/

void VL_Hlin (unsigned x, unsigned y, unsigned width, unsigned color, ofs_t *ofs)
{
	unsigned		xbyte;
	byte			far *dest;
	byte			leftmask,rightmask;
	int				midbytes;

	LRCLIPDEF

	xbyte = x>>2;
	leftmask = lclip[x&3];
	rightmask = rclip[(x+width-1)&3];
	midbytes = ((x+width+3)>>2) - xbyte - 2;

	dest = MK_FP(SCREENSEG,ofs->bufferofs+ofs->ylookup[y]+xbyte);

	if (midbytes<0)
	{
	// all in one byte
		VGAMAPMASK(leftmask&rightmask);
		*dest = color;
		VGAMAPMASK(15);
		return;
	}

	VGAMAPMASK(leftmask);
	*dest++ = color;

	VGAMAPMASK(15);
	_fmemset (dest,color,midbytes);
	dest+=midbytes;

	VGAMAPMASK(rightmask);
	*dest = color;

	VGAMAPMASK(15);
}


/*
=================
=
= VL_Vlin
=
=================
*/

void VL_Vlin (int x, int y, int height, int color, ofs_t *ofs)
{
	byte	far *dest,mask;
	VCLIPDEF

	mask = pclip[x&3];
	VGAMAPMASK(mask);

	dest = MK_FP(SCREENSEG,ofs->bufferofs+ofs->ylookup[y]+(x>>2));

	while (height--)
	{
		*dest = color;
		dest += ofs->linewidth;
	}

	VGAMAPMASK(15);
}


/*
=================
=
= VL_Bar
=
=================
*/

void VL_Bar (int x, int y, int width, int height, int color, ofs_t *ofs)
{
	byte	far *dest;
	byte	leftmask,rightmask;
	int		midbytes,linedelta;

	LRCLIPDEF

	leftmask = lclip[x&3];
	rightmask = rclip[(x+width-1)&3];
	midbytes = ((x+width+3)>>2) - (x>>2) - 2;
	linedelta = ofs->linewidth-(midbytes+1);

	dest = MK_FP(SCREENSEG,ofs->bufferofs+ofs->ylookup[y]+(x>>2));

	if (midbytes<0)
	{
	// all in one byte
		VGAMAPMASK(leftmask&rightmask);
		while (height--)
		{
			*dest = color;
			dest += ofs->linewidth;
		}
		VGAMAPMASK(15);
		return;
	}

	while (height--)
	{
		VGAMAPMASK(leftmask);
		*dest++ = color;

		VGAMAPMASK(15);
		_fmemset (dest,color,midbytes);
		dest+=midbytes;

		VGAMAPMASK(rightmask);
		*dest = color;

		dest+=linedelta;
	}

	VGAMAPMASK(15);
}


/*
==============

 VL_WaitVBL			******** NEW *********

 Wait for the vertical retrace (returns before the actual vertical sync)

==============
*/

void VL_WaitVBL(word num)
{
//PROC	VL_WaitVBL  num:WORD
//PUBLIC	VL_WaitVBL
#ifdef __WATCOMC__
	__asm {
#endif
	wait:
#ifdef __BORLANDC__
	__asm {
#endif

		mov	dx,STATUS_REGISTER_1

		mov	cx,[num]
	//
	// wait for a display signal to make sure the raster isn't in the middle
	// of a sync
	//
#ifdef __BORLANDC__
	}
#endif
	waitnosync:
#ifdef __BORLANDC__
	__asm {
#endif
		in	al,dx
		test	al,8
		jnz	waitnosync


#ifdef __BORLANDC__
	}
#endif
	waitsync:
#ifdef __BORLANDC__
	__asm {
#endif
		in	al,dx
		test	al,8
		jz	waitsync

		loop	waitnosync

		ret
	}
}

//===========================================================================

void VGAMAPMASK(byte x)
{
	__asm {
//		cli
		mov	dx,SC_INDEX
		mov	al,SC_MAPMASK
		mov	ah,x
		out	dx,ax
//		sti
	}
}

void VGAWRITEMODE(byte x)
{
	__asm {
//		cli
		mov	dx,GC_INDEX
		mov	al,GC_MODE
		out	dx,al
		inc	dx
		in	al,dx
		and	al,252
		or	al,x
		out	dx,al
//		sti
	}
}

void VGAREADMAP(byte x)
{
	__asm {
//		cli
		mov	dx,GC_INDEX
		mov	al,GC_READMAP
		mov	ah,x
		out	dx,ax
//		sti
	}
}

//===========================================================================
