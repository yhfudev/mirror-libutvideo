/* �����R�[�h�͂r�i�h�r ���s�R�[�h�͂b�q�k�e */
/* $Id: ULY2Codec.cpp 988 2013-04-20 09:28:02Z umezawa $ */

#include "stdafx.h"
#include "utvideo.h"
#include "ULY2Codec.h"
#include "Predict.h"
#include "Convert.h"

const utvf_t CULY2Codec::m_utvfEncoderInput[] = {
#ifndef __APPLE__
	UTVF_YUY2, UTVF_YUYV, UTVF_YUNV,
	UTVF_UYVY, UTVF_UYNV,
#endif
	UTVF_NFCC_BGR_BU,
	UTVF_NFCC_BGRX_BU,
	UTVF_NFCC_BGR_TD,
	UTVF_NFCC_BGRX_TD,
	UTVF_NFCC_RGB_TD,
	UTVF_NFCC_ARGB_TD,
	UTVF_INVALID,
};

const utvf_t CULY2Codec::m_utvfDecoderOutput[] = {
#ifndef __APPLE__
	UTVF_YUY2, UTVF_YUYV, UTVF_YUNV,
	UTVF_UYVY, UTVF_UYNV,
#endif
	UTVF_NFCC_BGR_BU,
	UTVF_NFCC_BGRX_BU,
	UTVF_NFCC_BGR_TD,
	UTVF_NFCC_BGRX_TD,
	UTVF_NFCC_RGB_TD,
	UTVF_NFCC_ARGB_TD,
	UTVF_INVALID,
};

const utvf_t CULY2Codec::m_utvfCompressed[] = {
	UTVF_ULY2,
	UTVF_INVALID,
};

CULY2Codec::CULY2Codec(const char *pszInterfaceName) : CUL00Codec("ULY2", pszInterfaceName)
{
}

CULY2Codec::~CULY2Codec(void)
{
}

CCodec *CULY2Codec::CreateInstance(const char *pszInterfaceName)
{
	return new CULY2Codec(pszInterfaceName);
}

void CULY2Codec::CalcPlaneSizes(unsigned int width, unsigned int height)
{
	m_dwPlaneSize[0]          = width * height;
	m_dwPlaneSize[1]          = width * height / 2;
	m_dwPlaneSize[2]          = width * height / 2;

	m_dwPlaneWidth[0]         = width;
	m_dwPlaneWidth[1]         = width / 2;
	m_dwPlaneWidth[2]         = width / 2;

	m_dwPlaneStripeSize[0]    = width;
	m_dwPlaneStripeSize[1]    = width / 2;
	m_dwPlaneStripeSize[2]    = width / 2;

	m_dwPlanePredictStride[0] = width;
	m_dwPlanePredictStride[1] = width / 2;
	m_dwPlanePredictStride[2] = width / 2;
}

void CULY2Codec::ConvertToPlanar(uint32_t nBandIndex)
{
	uint8_t *y, *u, *v;
	const uint8_t *pSrcBegin, *pSrcEnd;

	pSrcBegin = ((uint8_t *)m_pInput) + m_dwRawStripeBegin[nBandIndex] * m_dwRawStripeSize;
	pSrcEnd   = ((uint8_t *)m_pInput) + m_dwRawStripeEnd[nBandIndex]   * m_dwRawStripeSize;
	y = m_pCurFrame->GetPlane(0) + m_dwPlaneStripeBegin[nBandIndex] * m_dwPlaneStripeSize[0];
	u = m_pCurFrame->GetPlane(1) + m_dwPlaneStripeBegin[nBandIndex] * m_dwPlaneStripeSize[1];
	v = m_pCurFrame->GetPlane(2) + m_dwPlaneStripeBegin[nBandIndex] * m_dwPlaneStripeSize[2];

	switch (m_utvfRaw)
	{
	case UTVF_YUY2:
	case UTVF_YUYV:
	case UTVF_YUNV:
		ConvertYUYVToULY2(y, u, v, pSrcBegin, pSrcEnd);
		break;
	case UTVF_UYVY:
	case UTVF_UYNV:
		ConvertUYVYToULY2(y, u, v, pSrcBegin, pSrcEnd);
		break;
	case UTVF_NFCC_BGR_BU:
		ConvertBottomupBGRToULY2(y, u, v, pSrcBegin, pSrcEnd, m_dwRawGrossWidth, m_dwRawNetWidth);
		break;
	case UTVF_NFCC_BGRX_BU:
		ConvertBottomupBGRXToULY2(y, u, v, pSrcBegin, pSrcEnd, m_dwRawGrossWidth, m_dwRawNetWidth);
		break;
	case UTVF_NFCC_BGR_TD:
		ConvertTopdownBGRToULY2(y, u, v, pSrcBegin, pSrcEnd, m_dwRawGrossWidth, m_dwRawNetWidth);
		break;
	case UTVF_NFCC_BGRX_TD:
		ConvertTopdownBGRXToULY2(y, u, v, pSrcBegin, pSrcEnd, m_dwRawGrossWidth, m_dwRawNetWidth);
		break;
	case UTVF_NFCC_RGB_TD:
		ConvertTopdownRGBToULY2(y, u, v, pSrcBegin, pSrcEnd, m_dwRawGrossWidth, m_dwRawNetWidth);
		break;
	case UTVF_NFCC_ARGB_TD:
		ConvertTopdownXRGBToULY2(y, u, v, pSrcBegin, pSrcEnd, m_dwRawGrossWidth, m_dwRawNetWidth);
		break;
	}
}

void CULY2Codec::ConvertFromPlanar(uint32_t nBandIndex)
{
	const uint8_t *y, *u, *v;
	uint8_t *pDstBegin, *pDstEnd;

	pDstBegin = ((uint8_t *)m_pOutput) + m_dwRawStripeBegin[nBandIndex] * m_dwRawStripeSize;
	pDstEnd   = ((uint8_t *)m_pOutput) + m_dwRawStripeEnd[nBandIndex]   * m_dwRawStripeSize;
	y = m_pCurFrame->GetPlane(0) + m_dwPlaneStripeBegin[nBandIndex] * m_dwPlaneStripeSize[0];
	u = m_pCurFrame->GetPlane(1) + m_dwPlaneStripeBegin[nBandIndex] * m_dwPlaneStripeSize[1];
	v = m_pCurFrame->GetPlane(2) + m_dwPlaneStripeBegin[nBandIndex] * m_dwPlaneStripeSize[2];

	switch (m_utvfRaw)
	{
	case UTVF_YUY2:
	case UTVF_YUYV:
	case UTVF_YUNV:
		ConvertULY2ToYUYV(pDstBegin, pDstEnd, y, u, v);
		break;
	case UTVF_UYVY:
	case UTVF_UYNV:
		ConvertULY2ToUYVY(pDstBegin, pDstEnd, y, u, v);
		break;
	case UTVF_NFCC_BGR_BU:
		ConvertULY2ToBottomupBGR(pDstBegin, pDstEnd, y, u, v, m_dwRawGrossWidth, m_dwRawNetWidth);
		break;
	case UTVF_NFCC_BGRX_BU:
		ConvertULY2ToBottomupBGRX(pDstBegin, pDstEnd, y, u, v, m_dwRawGrossWidth, m_dwRawNetWidth);
		break;
	case UTVF_NFCC_BGR_TD:
		ConvertULY2ToTopdownBGR(pDstBegin, pDstEnd, y, u, v, m_dwRawGrossWidth, m_dwRawNetWidth);
		break;
	case UTVF_NFCC_BGRX_TD:
		ConvertULY2ToTopdownBGRX(pDstBegin, pDstEnd, y, u, v, m_dwRawGrossWidth, m_dwRawNetWidth);
		break;
	case UTVF_NFCC_RGB_TD:
		ConvertULY2ToTopdownRGB(pDstBegin, pDstEnd, y, u, v, m_dwRawGrossWidth, m_dwRawNetWidth);
		break;
	case UTVF_NFCC_ARGB_TD:
		ConvertULY2ToTopdownXRGB(pDstBegin, pDstEnd, y, u, v, m_dwRawGrossWidth, m_dwRawNetWidth);
		break;
	}
}

bool CULY2Codec::DecodeDirect(uint32_t nBandIndex)
{
	if ((m_fi.dwFlags0 & FI_FLAGS0_INTRAFRAME_PREDICT_MASK) != FI_FLAGS0_INTRAFRAME_PREDICT_LEFT)
		return false;

	uint8_t *pDstBegin = ((uint8_t *)m_pOutput) + m_dwRawStripeBegin[nBandIndex] * m_dwRawStripeSize;
	uint8_t *pDstEnd   = ((uint8_t *)m_pOutput) + m_dwRawStripeEnd[nBandIndex]   * m_dwRawStripeSize;

	switch (m_utvfRaw)
	{
	case UTVF_YUY2:
	case UTVF_YUYV:
	case UTVF_YUNV:
		HuffmanDecodeAndAccumStep2(pDstBegin+0, pDstEnd+0, m_pDecodeCode[0][nBandIndex], &m_hdt[0]);
		HuffmanDecodeAndAccumStep4(pDstBegin+1, pDstEnd+1, m_pDecodeCode[1][nBandIndex], &m_hdt[1]);
		HuffmanDecodeAndAccumStep4(pDstBegin+3, pDstEnd+3, m_pDecodeCode[2][nBandIndex], &m_hdt[2]);
		return true;
	case UTVF_UYVY:
	case UTVF_UYNV:
		HuffmanDecodeAndAccumStep2(pDstBegin+1, pDstEnd+1, m_pDecodeCode[0][nBandIndex], &m_hdt[0]);
		HuffmanDecodeAndAccumStep4(pDstBegin+0, pDstEnd+0, m_pDecodeCode[1][nBandIndex], &m_hdt[1]);
		HuffmanDecodeAndAccumStep4(pDstBegin+2, pDstEnd+2, m_pDecodeCode[2][nBandIndex], &m_hdt[2]);
		return true;
	}

	return false;
}
