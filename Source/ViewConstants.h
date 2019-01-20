/*
  ==============================================================================

    ViewConstants.h
    Created: 19 Jun 2014 7:23:25pm
    Author:  hsstraub

  ==============================================================================
*/

#ifndef VIEWCONSTANTS_H_INCLUDED
#define VIEWCONSTANTS_H_INCLUDED

// General constants
#define MAINWINDOWBGCOLOUR 0xffbad0de
#define MAINWINDOWSELECTEDCOLOUR 0x66ff5e00
#define STANDARDLABELHEIGTH 15

// Main window
#define DEFAULTMAINWINDOWWIDTH 928
#define DEFAULTMAINWINDOWHEIGHT 768

#define MAINWINDOWFIRSTCOLPOS 20

// Macro button fields
#define FIRSTMACROBUTTOONCOLPOS 70
#define MACROBUTTONYPOS 1

// Macro buttons
#define DEFAULTMACROBUTTONWIDTH 100
#define DEFAULTMACROBUTTONHEIGHT 46

// Key set fields
#define TERPSTRAKEYSETFLDFIRSTYPOS 32
#define DEFAULTTERPSTRAKEYSETWIDTH 148

#define MINIMALTERPSTRAKEYSETHEIGHT 10

// Single key fields
#define TERPSTRASINGLEKEYFIELDRIMABOVE 50
#define TERPSTRASINGLEKEYFLDSIZE 56
#define TERPSTRASINGLEKEYFLDLINEWIDTH 2.0f
#define TERPSTRASELECTEDKEYFLDLINEWIDTH 3.0f
#define TERPSTRASINGLEKEYROTATIONANGLE -0.20f
#define TERPSTRASELECTEDFLDLINECOLOUR 0xfff7990d

// Edit area
#define EDITAREAFIRSTCOLPOS 480
#define EDITAREAWIDTH 428
#define MIDIEDITAREAFIRSTROWPOS 250
#define MIDIEDITAREAHEIGHT 130
#define EDITFUNCTIONAREAHEIGHT 360

#define EDITSUBWINWIDTH 428
#define NOTEASSIGNSUBWINTOP 44
#define NOTEASSIGNSUBWINHEIGHT 320

#define MAPPINGSUBWINTOP 56
#define MAPPINGSUBWINHEIGHT 128

// Minimal windows widths and heights (for resizing functionality)
// Single keys and edit area need to be whole on screen, the octave board selectors are resized at demand
#define MINIMALMAINWINDOWWIDTH EDITAREAFIRSTCOLPOS + EDITAREAWIDTH
#define MINIMALMAINWINDOWHEIGHT MINIMALTERPSTRAKEYSETHEIGHT + MIDIEDITAREAHEIGHT + EDITFUNCTIONAREAHEIGHT

#endif  // VIEWCONSTANTS_H_INCLUDED
