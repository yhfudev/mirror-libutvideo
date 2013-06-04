/* �����R�[�h�͂r�i�h�r ���s�R�[�h�͂b�q�k�e */
/* $Id: TunedFunc.cpp 1001 2013-04-29 14:48:23Z umezawa $ */

#include "stdafx.h"
#include "utvideo.h"
#include "TunedFunc.h"
#include "Predict.h"
#include "HuffmanCode.h"
#include "Convert.h"

const TUNEDFUNC tfnCPP = {
	cpp_PredictWrongMedianAndCount,
	cpp_PredictWrongMedianAndCount,
	cpp_PredictLeftAndCount,
	cpp_RestoreWrongMedian,
	cpp_HuffmanEncode,
	cpp_HuffmanDecode,
	cpp_HuffmanDecodeAndAccum,
	cpp_HuffmanDecodeAndAccumStep2,
	cpp_HuffmanDecodeAndAccumStep3,
	cpp_HuffmanDecodeAndAccumStep4,
	cpp_HuffmanDecodeAndAccumStep3ForBGRBlue,
	cpp_HuffmanDecodeAndAccumStep3ForBGRRed,
	cpp_HuffmanDecodeAndAccumStep4ForBGRXBlue,
	cpp_HuffmanDecodeAndAccumStep4ForBGRXRed,
	cpp_HuffmanDecodeAndAccumStep4ForBGRXRedAndDummyAlpha,
	cpp_HuffmanDecodeAndAccumStep4ForXRGBRedAndDummyAlpha,
	cpp_ConvertULY2ToBGR,
	cpp_ConvertULY2ToBGRX,
	cpp_ConvertULY2ToRGB,
	cpp_ConvertULY2ToXRGB,
	cpp_ConvertBGRToULY2,
	cpp_ConvertBGRXToULY2,
	cpp_ConvertRGBToULY2,
	cpp_ConvertXRGBToULY2,
	cpp_ConvertBGRToULRG,
	cpp_ConvertBGRXToULRG,
	cpp_ConvertXRGBToULRG,
	cpp_ConvertBGRAToULRA,
	cpp_ConvertARGBToULRA,
	cpp_ConvertYUYVToULY2,
	cpp_ConvertUYVYToULY2,
	cpp_ConvertULRGToBGR,
	cpp_ConvertULRGToBGRX,
	cpp_ConvertULRGToXRGB,
	cpp_ConvertULRAToBGRA,
	cpp_ConvertULRAToARGB,
	cpp_ConvertULY2ToYUYV,
	cpp_ConvertULY2ToUYVY,
	DummyTunedFunc
};

TUNEDFUNC tfn = tfnCPP;

#ifdef STATIC_LIB_WITH_ASM
# ifdef __x86_64__
#  include "TunedFunc_x64.cpp"
# elif __i386__
#  include "TunedFunc_x86.cpp"
# endif
#endif
