/* �����R�[�h�͂r�i�h�r ���s�R�[�h�͂b�q�k�e */
/* $Id: Predict.h 999 2013-04-28 19:35:47Z umezawa $ */

#pragma once

#include "TunedFunc.h"

// intra-frame prediction
void PredictWrongMedianAndCount(uint8_t *pDst, const uint8_t *pSrcBegin, const uint8_t *pSrcEnd, size_t dwStride, uint32_t *pCountTable);
#define PredictLeftAndCount tfn.pfnPredictLeftAndCount_align1
//void RestoreWrongMedian(uint8_t *pDst, const uint8_t *pSrcBegin, const uint8_t *pSrcEnd, size_t dwStride);
#define RestoreWrongMedian tfn.pfnRestoreWrongMedian_align1

void cpp_PredictWrongMedianAndCount(uint8_t *pDst, const uint8_t *pSrcBegin, const uint8_t *pSrcEnd, size_t dwStride, uint32_t *pCountTable);
void cpp_PredictLeftAndCount(uint8_t *pDst, const uint8_t *pSrcBegin, const uint8_t *pSrcEnd, uint32_t *pCountTable);
void cpp_RestoreWrongMedian(uint8_t *pDst, const uint8_t *pSrcBegin, const uint8_t *pSrcEnd, size_t dwStride);

#if defined(__i386__) || defined(__x86_64__)
extern "C" void sse2_PredictWrongMedianAndCount_align16(uint8_t *pDst, const uint8_t *pSrcBegin, const uint8_t *pSrcEnd, size_t dwStride, uint32_t *pCountTable);
extern "C" void sse2_PredictWrongMedianAndCount_align1(uint8_t *pDst, const uint8_t *pSrcBegin, const uint8_t *pSrcEnd, size_t dwStride, uint32_t *pCountTable);
extern "C" void sse2_PredictLeftAndCount_align1(uint8_t *pDst, const uint8_t *pSrcBegin, const uint8_t *pSrcEnd, uint32_t *pCountTable);
extern "C" void i686_RestoreWrongMedian_align1(uint8_t *pDst, const uint8_t *pSrcBegin, const uint8_t *pSrcEnd, size_t dwStride);
extern "C" void sse1mmx_RestoreWrongMedian_align1(uint8_t *pDst, const uint8_t *pSrcBegin, const uint8_t *pSrcEnd, size_t dwStride);
#endif
