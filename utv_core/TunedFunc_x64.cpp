/* �����R�[�h�͂r�i�h�r ���s�R�[�h�͂b�q�k�e */
/* $Id: TunedFunc_x64.cpp 990 2013-04-21 12:03:02Z umezawa $ */

#include "stdafx.h"
#include "utvideo.h"
#include "TunedFunc.h"
#include "Predict.h"
#include "HuffmanCode.h"
#include "Convert.h"

const TUNEDFUNC tfnSSE2 = {
	x64_sse2_PredictWrongMedianAndCount_align16,
	x64_sse2_PredictWrongMedianAndCount_align1,
	x64_sse2_PredictLeftAndCount_align1,
	x64_sse1mmx_RestoreWrongMedian_align1,
	x64_i686_HuffmanEncode,
	x64_i686_HuffmanDecode,
	x64_i686_HuffmanDecodeAndAccum,
	x64_i686_HuffmanDecodeAndAccumStep2,
	x64_i686_HuffmanDecodeAndAccumStep4,
	x64_i686_HuffmanDecodeAndAccumStep4ForBottomupBGRXGreen,
	x64_i686_HuffmanDecodeAndAccumStep4ForBottomupBGRXBlue,
	x64_i686_HuffmanDecodeAndAccumStep4ForBottomupBGRXRed,
	x64_i686_HuffmanDecodeAndAccumStep4ForBottomupBGRXRedAndDummyAlpha,
	x64_i686_HuffmanDecodeAndAccumStep3ForBottomupBGRGreen,
	x64_i686_HuffmanDecodeAndAccumStep3ForBottomupBGRBlue,
	x64_i686_HuffmanDecodeAndAccumStep3ForBottomupBGRRed,
	x64_i686_HuffmanDecodeAndAccumStep4ForTopdownXRGBGreen,
	x64_i686_HuffmanDecodeAndAccumStep4ForTopdownXRGBBlue,
	x64_i686_HuffmanDecodeAndAccumStep4ForTopdownXRGBRed,
	x64_i686_HuffmanDecodeAndAccumStep4ForTopdownXRGBRedAndDummyAlpha,
	x64_i686_HuffmanDecodeAndAccumStep3ForTopdownRGBGreen,
	x64_i686_HuffmanDecodeAndAccumStep3ForTopdownRGBBlue,
	x64_i686_HuffmanDecodeAndAccumStep3ForTopdownRGBRed,
	x64_i686_HuffmanDecodeAndAccumStep4ForTopdownBGRXRedAndDummyAlpha,
	x64_sse2_ConvertULY2ToBottomupBGR,
	x64_sse2_ConvertULY2ToBottomupBGRX,
	x64_sse2_ConvertULY2ToTopdownBGR,
	x64_sse2_ConvertULY2ToTopdownBGRX,
	x64_sse2_ConvertULY2ToTopdownRGB,
	x64_sse2_ConvertULY2ToTopdownXRGB,
	x64_sse2_ConvertBottomupBGRToULY2,
	x64_sse2_ConvertBottomupBGRXToULY2,
	x64_sse2_ConvertTopdownBGRToULY2,
	x64_sse2_ConvertTopdownBGRXToULY2,
	x64_sse2_ConvertTopdownRGBToULY2,
	x64_sse2_ConvertTopdownXRGBToULY2,
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

const TUNEDFUNC &tfnSSE3 = tfnSSE2;

const TUNEDFUNC &tfnSSSE3 = tfnSSE2;

const TUNEDFUNC &tfnSSE41 = tfnSSE2;

const TUNEDFUNC &tfnSSE42 = tfnSSE2;

const TUNEDFUNC tfnAVX1 = {
	x64_sse2_PredictWrongMedianAndCount_align16,
	x64_sse2_PredictWrongMedianAndCount_align1,
	x64_sse2_PredictLeftAndCount_align1,
	x64_sse1mmx_RestoreWrongMedian_align1,
	x64_i686_HuffmanEncode,
	x64_i686_HuffmanDecode,
	x64_i686_HuffmanDecodeAndAccum,
	x64_i686_HuffmanDecodeAndAccumStep2,
	x64_i686_HuffmanDecodeAndAccumStep4,
	x64_i686_HuffmanDecodeAndAccumStep4ForBottomupBGRXGreen,
	x64_i686_HuffmanDecodeAndAccumStep4ForBottomupBGRXBlue,
	x64_i686_HuffmanDecodeAndAccumStep4ForBottomupBGRXRed,
	x64_i686_HuffmanDecodeAndAccumStep4ForBottomupBGRXRedAndDummyAlpha,
	x64_i686_HuffmanDecodeAndAccumStep3ForBottomupBGRGreen,
	x64_i686_HuffmanDecodeAndAccumStep3ForBottomupBGRBlue,
	x64_i686_HuffmanDecodeAndAccumStep3ForBottomupBGRRed,
	x64_i686_HuffmanDecodeAndAccumStep4ForTopdownXRGBGreen,
	x64_i686_HuffmanDecodeAndAccumStep4ForTopdownXRGBBlue,
	x64_i686_HuffmanDecodeAndAccumStep4ForTopdownXRGBRed,
	x64_i686_HuffmanDecodeAndAccumStep4ForTopdownXRGBRedAndDummyAlpha,
	x64_i686_HuffmanDecodeAndAccumStep3ForTopdownRGBGreen,
	x64_i686_HuffmanDecodeAndAccumStep3ForTopdownRGBBlue,
	x64_i686_HuffmanDecodeAndAccumStep3ForTopdownRGBRed,
	x64_i686_HuffmanDecodeAndAccumStep4ForTopdownBGRXRedAndDummyAlpha,
	x64_sse2_ConvertULY2ToBottomupBGR,
	x64_sse2_ConvertULY2ToBottomupBGRX,
	x64_sse2_ConvertULY2ToTopdownBGR,
	x64_sse2_ConvertULY2ToTopdownBGRX,
	x64_sse2_ConvertULY2ToTopdownRGB,
	x64_sse2_ConvertULY2ToTopdownXRGB,
	x64_sse2_ConvertBottomupBGRToULY2,
	x64_sse2_ConvertBottomupBGRXToULY2,
	x64_sse2_ConvertTopdownBGRToULY2,
	x64_sse2_ConvertTopdownBGRXToULY2,
	x64_sse2_ConvertTopdownRGBToULY2,
	x64_sse2_ConvertTopdownXRGBToULY2,
	x64_avx1_ConvertBGRToULRG,
	x64_avx1_ConvertBGRXToULRG,
	x64_avx1_ConvertXRGBToULRG,
	x64_avx1_ConvertBGRAToULRA,
	x64_avx1_ConvertARGBToULRA,
	x64_avx1_ConvertYUYVToULY2,
	x64_avx1_ConvertUYVYToULY2,
	x64_avx1_ConvertULRGToBGR,
	x64_avx1_ConvertULRGToBGRX,
	x64_avx1_ConvertULRGToXRGB,
	x64_avx1_ConvertULRAToBGRA,
	x64_avx1_ConvertULRAToARGB,
	x64_avx1_ConvertULY2ToYUYV,
	x64_avx1_ConvertULY2ToUYVY,
	DummyTunedFunc
};

const TUNEDFUNC &tfnAVX2 = tfnAVX1;

#include "TunedFunc_x86x64.cpp"
