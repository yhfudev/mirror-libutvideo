/* �����R�[�h�͂r�i�h�r ���s�R�[�h�͂b�q�k�e */
/* $Id: QuickTimeFormat.h 1152 2014-04-15 07:37:21Z umezawa $ */

#pragma once

DLLEXPORT int UtVideoFormatToQuickTimeFormat(OSType *pixelFormat, utvf_t utvf);
DLLEXPORT int QuickTimeFormatToUtVideoFormat(utvf_t *utvf, OSType pixelFormat);
