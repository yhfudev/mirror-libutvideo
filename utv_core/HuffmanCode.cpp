/* �����R�[�h�͂r�i�h�r ���s�R�[�h�͂b�q�k�e */
/* $Id: HuffmanCode.cpp 1145 2014-04-14 12:08:31Z umezawa $ */

#include "stdafx.h"
#include "utvideo.h"
//#include <windows.h>
//#include <algorithm>
//using namespace std;
#include "HuffmanCode.h"

struct hufftree {
	struct hufftree *left;
	struct hufftree *right;
	uint32_t count;
	uint32_t symbol;	// uint8_t �ł͂Ȃ��V���{�������邱�Ƃ�����
};

inline bool hufftree_gt(const struct hufftree *a, const struct hufftree *b)
{
	return (a->count > b->count);
}

bool generate_code_length(uint8_t *codelen, const struct hufftree *node, uint8_t curlen)
{
	if (node->left == NULL) {
		codelen[node->symbol] = curlen;
		return (curlen > 24);
	} else {
		return
			generate_code_length(codelen, node->left, curlen+1) ||
			generate_code_length(codelen, node->right, curlen+1);
	}
}

static void GenerateLengthLimitedHuffmanCodeLengthTable(uint8_t *pCodeLengthTable, int nSymbolBits)
{
	// �Ƃ肠��������œ�����B
	memset(pCodeLengthTable, nSymbolBits, 1 << nSymbolBits);
}

void GenerateHuffmanCodeLengthTable(uint8_t *pCodeLengthTable, const uint32_t *pCountTable, int nSymbolBits)
{
	struct hufftree **huffsort = (hufftree**)malloc(sizeof(hufftree *) * (size_t)(1 << nSymbolBits));
	struct hufftree *huffleaf = (hufftree*)malloc(sizeof(hufftree) * (size_t)(1 << nSymbolBits));
	struct hufftree *huffnode = (hufftree*)malloc(sizeof(hufftree) * (size_t)(1 << nSymbolBits));
	int nsym;

	nsym = 0;
	for (int i = 0; i < (1 << nSymbolBits); i++) {
		if (pCountTable[i] != 0) {
			huffleaf[nsym].left = NULL;
			huffleaf[nsym].right = NULL;
			huffleaf[nsym].count = pCountTable[i];
			huffleaf[nsym].symbol = i;
			huffsort[nsym] = &huffleaf[nsym];
			nsym++;
		}
		else
			pCodeLengthTable[i] = 255;
	}

	sort(huffsort, huffsort+nsym, hufftree_gt);
	for (int i = nsym - 2; i >= 0; i--) {
		huffnode[i].left = huffsort[i];
		huffnode[i].right = huffsort[i+1];
		huffnode[i].count = huffsort[i]->count + huffsort[i+1]->count;

		struct hufftree **insptr = upper_bound(huffsort, huffsort+i, &huffnode[i], hufftree_gt);
		struct hufftree **movptr;
		for (movptr = huffsort+i-1; movptr >= insptr; movptr--)
			*(movptr+1) = *movptr;
		*insptr = &huffnode[i];
	}

	if (generate_code_length(pCodeLengthTable, huffsort[0], 0))
		GenerateLengthLimitedHuffmanCodeLengthTable(pCodeLengthTable, nSymbolBits);

	free(huffsort);
	free(huffleaf);
	free(huffnode);
}

struct CODE_LENGTH_SORT
{
	uint32_t symbol;
	uint8_t codelen;
};

bool cls_less(const CODE_LENGTH_SORT &a, const CODE_LENGTH_SORT &b)
{
	if (a.codelen != b.codelen)
		return a.codelen < b.codelen;
	else
		return a.symbol < b.symbol;
}

void GenerateHuffmanEncodeTable(HUFFMAN_ENCODE_TABLE *pEncodeTable, const uint8_t *pCodeLengthTable)
{
	struct CODE_LENGTH_SORT cls[256];
	uintenc_t curcode;

	for (int i = 0; i < 256; i++)
	{
		cls[i].symbol = i;
		cls[i].codelen = pCodeLengthTable[i];
	}

	sort(cls, cls + 256, cls_less);

	if (cls[0].codelen == 0)
	{
		memset(pEncodeTable, 0, sizeof(HUFFMAN_ENCODE_TABLE));
		return;
	}

	memset(pEncodeTable, 0xff, sizeof(HUFFMAN_ENCODE_TABLE));

	curcode = 0;
	for (int i = 255; i >= 0; i--)
	{
		if (cls[i].codelen == 255)
			continue;
		pEncodeTable->dwTableMux[cls[i].symbol] = curcode | cls[i].codelen;
		curcode += UINTENC_MSB >> (cls[i].codelen - 1);
	}
}

void GenerateHuffmanEncodeTable10(HUFFMAN_ENCODE_TABLE10 *pEncodeTable, const uint8_t *pCodeLengthTable)
{
	struct CODE_LENGTH_SORT cls[1024];
	uintenc_t curcode;

	for (int i = 0; i < 1024; i++)
	{
		cls[i].symbol = i;
		cls[i].codelen = pCodeLengthTable[i];
	}

	sort(cls, cls + 1024, cls_less);

	if (cls[0].codelen == 0)
	{
		memset(pEncodeTable, 0, sizeof(HUFFMAN_ENCODE_TABLE10));
		return;
	}

	memset(pEncodeTable, 0xff, sizeof(HUFFMAN_ENCODE_TABLE10));

	curcode = 0;
	for (int i = 1023; i >= 0; i--)
	{
		if (cls[i].codelen == 255)
			continue;
		pEncodeTable->dwTableMux[cls[i].symbol] = curcode | cls[i].codelen;
		curcode += UINTENC_MSB >> (cls[i].codelen - 1);
	}
}

// IA-32 �� BSR ����
// �{���� BSR ���߂ł͓��͂� 0 �̏ꍇ�ɏo�͂��s��ɂȂ�B
inline int bsr(uint32_t x)
{
	_ASSERT(x != 0);

	for (int i = 31; i >= 0; i--)
		if (x & (1 << i))
			return i;
	return rand() % 32;
}

// LZCNT ���邢�� CLZ �ƌĂ΂�閽��
inline int lzcnt(uint32_t x)
{
	for (int i = 31; i >= 0; i--)
		if (x & (1 << i))
			return 31 - i;
	return 32;
}

void GenerateHuffmanDecodeTable(HUFFMAN_DECODE_TABLE *pDecodeTable, const uint8_t *pCodeLengthTable)
{
	struct CODE_LENGTH_SORT cls[256];
	int nLastIndex;

	for (int i = 0; i < 256; i++)
	{
		cls[i].symbol = i;
		cls[i].codelen = pCodeLengthTable[i];
	}

	sort(cls, cls + 256, cls_less);

	// �o������V���{�����P��ނ����Ȃ��ꍇ�̏���
	if (cls[0].codelen == 0)
	{
		memset(pDecodeTable, 0, sizeof(HUFFMAN_DECODE_TABLE));
		for (int i = 0; i < _countof(pDecodeTable->nCodeShift); i++)
			pDecodeTable->nCodeShift[i] = 31;
		for (int i = 0; i < _countof(pDecodeTable->SymbolAndCodeLength); i++)
		{
			pDecodeTable->SymbolAndCodeLength[i].nCodeLength = 0;
			pDecodeTable->SymbolAndCodeLength[i].bySymbol    = cls[0].symbol;
		}
		for (int i = 0; i < _countof(pDecodeTable->LookupSymbolAndCodeLength); i++)
		{
			pDecodeTable->LookupSymbolAndCodeLength[i].nCodeLength = 0;
			pDecodeTable->LookupSymbolAndCodeLength[i].bySymbol    = cls[0].symbol;
		}
		return;
	}

	// �ł����������������V���{���� cls ��ł̈ʒu�����߂�
	for (nLastIndex = 255; nLastIndex >= 0; nLastIndex--)
	{
		if (cls[nLastIndex].codelen != 255)
			break;
	}

	// �ᑬ�e�[�u���̐���
	{
		uint32_t curcode = 1; // bsr �΍�� 0 �ł͂Ȃ� 1�B�����꒷�� 24 �ȉ��Ȃ̂� 1 �ɂ��Ă����Ă����Ȃ��B
		int j = 0;
		int base = 0;
		int nextfillidx = 0;
		int prevbsrval = 0;

		for (int i = nLastIndex; i >= 0; i--)
		{
			// �Z��������̏ꍇ�͍����e�[�u���Ńf�R�[�h�����̂ŁA�ᑬ�e�[�u���̐����͕s�v�ł���B
			if (cls[i].codelen <= HUFFMAN_DECODE_TABLELOOKUP_BITS)
				break;

			int bsrval = bsr(curcode);
			if (bsrval != prevbsrval)
			{
				base = nextfillidx - (curcode >> (32 - cls[i].codelen));
			}
			for (; j <= bsrval; j++)
			{
				pDecodeTable->nCodeShift[j] = 32 - cls[i].codelen;
				pDecodeTable->dwSymbolBase[j] = base;
			}
			int lastfillidx = nextfillidx + (1 << (32 - pDecodeTable->nCodeShift[bsrval] - cls[i].codelen));
			for (; nextfillidx < lastfillidx; nextfillidx++)
			{
				pDecodeTable->SymbolAndCodeLength[nextfillidx].bySymbol    = cls[i].symbol;
				pDecodeTable->SymbolAndCodeLength[nextfillidx].nCodeLength = cls[i].codelen;
			}
			curcode += 0x80000000 >> (cls[i].codelen - 1);
			prevbsrval = bsrval;
		}
	}

	// �����e�[�u���̐����i�e�[�u���ꔭ�Q�Ɨp�j
	{
		uint32_t curcode = 0;

		for (int i = 0; i < _countof(pDecodeTable->LookupSymbolAndCodeLength); i++)
			pDecodeTable->LookupSymbolAndCodeLength[i].nCodeLength = 255;

		for (int i = nLastIndex; i >= 0; i--)
		{
			if (cls[i].codelen <= HUFFMAN_DECODE_TABLELOOKUP_BITS)
			{
				int idx = curcode >> (32 - HUFFMAN_DECODE_TABLELOOKUP_BITS);
				for (int j = 0; j < (1 << (HUFFMAN_DECODE_TABLELOOKUP_BITS - cls[i].codelen)); j++)
				{
					pDecodeTable->LookupSymbolAndCodeLength[idx+j].nCodeLength = cls[i].codelen;
					pDecodeTable->LookupSymbolAndCodeLength[idx+j].bySymbol = cls[i].symbol;
				}
			}
			curcode += 0x80000000 >> (cls[i].codelen - 1);
		}
	}
}

void GenerateHuffmanDecodeTable10(HUFFMAN_DECODE_TABLE10 *pDecodeTable, const uint8_t *pCodeLengthTable)
{
	struct CODE_LENGTH_SORT cls[1024];
	int nLastIndex;

	for (int i = 0; i < 1024; i++)
	{
		cls[i].symbol = i;
		cls[i].codelen = pCodeLengthTable[i];
	}

	sort(cls, cls+1024, cls_less);

	// �o������V���{�����P��ނ����Ȃ��ꍇ�̏���
	if (cls[0].codelen == 0)
	{
		memset(pDecodeTable, 0, sizeof(HUFFMAN_DECODE_TABLE10));
		for (int i = 0; i < _countof(pDecodeTable->nCodeShift); i++)
			pDecodeTable->nCodeShift[i] = 31;
		for (int i = 0; i < _countof(pDecodeTable->SymbolAndCodeLength); i++)
		{
			pDecodeTable->SymbolAndCodeLength[i].nCodeLength = 0;
			pDecodeTable->SymbolAndCodeLength[i].bySymbol    = cls[0].symbol;
		}
		for (int i = 0; i < _countof(pDecodeTable->LookupSymbolAndCodeLength); i++)
		{
			pDecodeTable->LookupSymbolAndCodeLength[i].nCodeLength = 0;
			pDecodeTable->LookupSymbolAndCodeLength[i].bySymbol    = cls[0].symbol;
		}
		return;
	}

	// �ł����������������V���{���� cls ��ł̈ʒu�����߂�
	for (nLastIndex = 1023; nLastIndex >= 0; nLastIndex--)
	{
		if (cls[nLastIndex].codelen != 255)
			break;
	}

	// �ᑬ�e�[�u���̐���
	{
		uint32_t curcode = 1; // bsr �΍�� 0 �ł͂Ȃ� 1�B�����꒷�� 24 �ȉ��Ȃ̂� 1 �ɂ��Ă����Ă����Ȃ��B
		int j = 0;
		int base = 0;
		int nextfillidx = 0;
		int prevbsrval = 0;

		for (int i = nLastIndex; i >= 0; i--)
		{
			// �Z��������̏ꍇ�͍����e�[�u���Ńf�R�[�h�����̂ŁA�ᑬ�e�[�u���̐����͕s�v�ł���B
			if (cls[i].codelen <= HUFFMAN_DECODE_TABLELOOKUP_BITS10)
				break;

			int bsrval = bsr(curcode);
			if (bsrval != prevbsrval)
			{
				base = nextfillidx - (curcode >> (32 - cls[i].codelen));
			}
			for (; j <= bsrval; j++)
			{
				pDecodeTable->nCodeShift[j] = 32 - cls[i].codelen;
				pDecodeTable->dwSymbolBase[j] = base;
			}
			int lastfillidx = nextfillidx + (1 << (32 - pDecodeTable->nCodeShift[bsrval] - cls[i].codelen));
			for (; nextfillidx < lastfillidx; nextfillidx++)
			{
				pDecodeTable->SymbolAndCodeLength[nextfillidx].bySymbol    = cls[i].symbol;
				pDecodeTable->SymbolAndCodeLength[nextfillidx].nCodeLength = cls[i].codelen;
			}
			curcode += 0x80000000 >> (cls[i].codelen - 1);
			prevbsrval = bsrval;
		}
	}

	// �����e�[�u���̐����i�e�[�u���ꔭ�Q�Ɨp�j
	{
		uint32_t curcode = 0;

		for (int i = 0; i < _countof(pDecodeTable->LookupSymbolAndCodeLength); i++)
			pDecodeTable->LookupSymbolAndCodeLength[i].nCodeLength = 255;

		for (int i = nLastIndex; i >= 0; i--)
		{
			if (cls[i].codelen <= HUFFMAN_DECODE_TABLELOOKUP_BITS10)
			{
				int idx = curcode >> (32 - HUFFMAN_DECODE_TABLELOOKUP_BITS10);
				for (int j = 0; j < (1 << (HUFFMAN_DECODE_TABLELOOKUP_BITS10 - cls[i].codelen)); j++)
				{
					pDecodeTable->LookupSymbolAndCodeLength[idx+j].nCodeLength = cls[i].codelen;
					pDecodeTable->LookupSymbolAndCodeLength[idx+j].bySymbol = cls[i].symbol;
				}
			}
			curcode += 0x80000000 >> (cls[i].codelen - 1);
		}
	}
}

inline void FlushEncoded(uint32_t *&pDst, uintenc_t &dwTmpEncoded, int &nBits)
{
#if defined(__x86_64__)
	if (nBits > 0)
		*pDst++ = (uint32_t)(dwTmpEncoded >> 32);
	if (nBits > 32)
		*pDst++ = (uint32_t)(dwTmpEncoded & 0xffffffff);
#else
	if (nBits > 0)
		*pDst++ = dwTmpEncoded;
#endif
}

inline void EncodeSymbol(uint8_t bySymbol, const HUFFMAN_ENCODE_TABLE *pEncodeTable, uint32_t *&pDst, uintenc_t &dwTmpEncoded, int &nBits)
{
	int nCurBits = (int)(pEncodeTable->dwTableMux[bySymbol] & 0xff);
	uintenc_t dwCurEncoded = pEncodeTable->dwTableMux[bySymbol] & UINTENC_MASK;

	dwTmpEncoded |= dwCurEncoded >> nBits;
	nBits += nCurBits;
	if (nBits >= UINTENC_BITS)
	{
		FlushEncoded(pDst, dwTmpEncoded, nBits);
		nBits -= UINTENC_BITS;
		dwTmpEncoded = dwCurEncoded << (nCurBits - nBits);
	}
}

size_t cpp_HuffmanEncode(uint8_t *pDstBegin, const uint8_t *pSrcBegin, const uint8_t *pSrcEnd, const HUFFMAN_ENCODE_TABLE *pEncodeTable)
{
	int nBits;
	uintenc_t dwTmpEncoded;
	uint32_t *pDst;
	const uint8_t *p;

	if (pEncodeTable->dwTableMux[0] == 0)
		return 0;

	nBits = 0;
	dwTmpEncoded = 0;
	pDst = (uint32_t *)pDstBegin;

	for (p = pSrcBegin; p < pSrcEnd; p++)
		EncodeSymbol(*p, pEncodeTable, pDst, dwTmpEncoded, nBits);

	FlushEncoded(pDst, dwTmpEncoded, nBits);

	return ((uint8_t *)pDst) - pDstBegin;
}

inline void EncodeSymbol10(uint16_t wSymbol, const HUFFMAN_ENCODE_TABLE10 *pEncodeTable, uint32_t *&pDst, uintenc_t &dwTmpEncoded, int &nBits)
{
	_ASSERT(wSymbol < 0x400);
	int nCurBits = (int)(pEncodeTable->dwTableMux[wSymbol] & 0xff);
	_ASSERT(nCurBits > 0 && nCurBits != 0xff);
	uintenc_t dwCurEncoded = pEncodeTable->dwTableMux[wSymbol] & UINTENC_MASK;

	dwTmpEncoded |= dwCurEncoded >> nBits;
	nBits += nCurBits;
	if (nBits >= UINTENC_BITS)
	{
		FlushEncoded(pDst, dwTmpEncoded, nBits);
		nBits -= UINTENC_BITS;
		dwTmpEncoded = dwCurEncoded << (nCurBits - nBits);
	}
}

size_t cpp_HuffmanEncode10(uint8_t *pDstBegin, const uint16_t *pSrcBegin, const uint16_t *pSrcEnd, const HUFFMAN_ENCODE_TABLE10 *pEncodeTable)
{
	int nBits;
	uintenc_t dwTmpEncoded;
	uint32_t *pDst;
	const uint16_t *p;

	if (pEncodeTable->dwTableMux[0] == 0)
		return 0;

	nBits = 0;
	dwTmpEncoded = 0;
	pDst = (uint32_t *)pDstBegin;

	for (p = pSrcBegin; p < pSrcEnd; p++)
		EncodeSymbol10(*p, pEncodeTable, pDst, dwTmpEncoded, nBits);

	FlushEncoded(pDst, dwTmpEncoded, nBits);

	return ((uint8_t *)pDst) - pDstBegin;
}

inline void DecodeSymbol(uint32_t *&pSrc, int &nBits, const HUFFMAN_DECODE_TABLE *pDecodeTable, bool bAccum, uint8_t &byPrevSymbol, uint8_t *pDst, int nCorrPos, int nDummyAlphaPos)
{
	uint32_t code;
	uint8_t symbol;
	int codelen;

	if (nBits == 0)
		code = (*pSrc) | 0x00000001;
	else
		code = ((*pSrc) << nBits) | ((*(pSrc+1)) >> (32 - nBits)) | 0x00000001;

	int tableidx = code >> (32 - HUFFMAN_DECODE_TABLELOOKUP_BITS);
	if (pDecodeTable->LookupSymbolAndCodeLength[tableidx].nCodeLength != 255)
	{
		symbol = pDecodeTable->LookupSymbolAndCodeLength[tableidx].bySymbol;
		codelen = pDecodeTable->LookupSymbolAndCodeLength[tableidx].nCodeLength;
	}
	else
	{
		int bsrval = bsr(code);
		int codeshift = pDecodeTable->nCodeShift[bsrval];
		code >>= codeshift;
		tableidx = pDecodeTable->dwSymbolBase[bsrval] + code;
		symbol = pDecodeTable->SymbolAndCodeLength[tableidx].bySymbol;
		codelen = pDecodeTable->SymbolAndCodeLength[tableidx].nCodeLength;
	}

	if (bAccum)
	{
		symbol += byPrevSymbol;
		byPrevSymbol = symbol;
	}

	if (nCorrPos != 0)
		symbol += *(pDst + nCorrPos) + 0x80;

	*pDst = symbol;
	if (nDummyAlphaPos != 0)
		*(pDst + nDummyAlphaPos) = 0xff;
	nBits += codelen;

	if (nBits >= 32)
	{
		nBits -= 32;
		pSrc++;
	}
}

static void cpp_HuffmanDecode_common(uint8_t *pDstBegin, uint8_t *pDstEnd, const uint8_t *pSrcBegin, const HUFFMAN_DECODE_TABLE *pDecodeTable, bool bAccum, int nStep, size_t cbWidth, ssize_t scbStride, int nCorrPos, int nDummyAlphaPos)
{
	int nBits;
	uint32_t *pSrc;
	uint8_t *p;
	uint8_t prevsym;
	uint8_t *pStripeBegin;

	nBits = 0;
	pSrc = (uint32_t *)pSrcBegin;
	prevsym = 0x80;

	for (pStripeBegin = pDstBegin; pStripeBegin != pDstEnd; pStripeBegin += scbStride)
	{
		uint8_t *pStripeEnd = pStripeBegin + cbWidth;
		for (p = pStripeBegin; p < pStripeEnd; p += nStep)
			DecodeSymbol(pSrc, nBits, pDecodeTable, bAccum, prevsym, p, nCorrPos, nDummyAlphaPos);
	}
}

inline void DecodeSymbol10(uint32_t *&pSrc, int &nBits, const HUFFMAN_DECODE_TABLE10 *pDecodeTable, bool bAccum, uint16_t &byPrevSymbol, uint16_t *pDst, int nCorrPos, int nDummyAlphaPos)
{
	uint32_t code;
	uint16_t symbol;
	int codelen;

	if (nBits == 0)
		code = (*pSrc) | 0x00000001;
	else
		code = ((*pSrc) << nBits) | ((*(pSrc+1)) >> (32 - nBits)) | 0x00000001;

	int tableidx = code >> (32 - HUFFMAN_DECODE_TABLELOOKUP_BITS10);
	if (pDecodeTable->LookupSymbolAndCodeLength[tableidx].nCodeLength != 255)
	{
		symbol = pDecodeTable->LookupSymbolAndCodeLength[tableidx].bySymbol;
		codelen = pDecodeTable->LookupSymbolAndCodeLength[tableidx].nCodeLength;
	}
	else
	{
		int bsrval = bsr(code);
		int codeshift = pDecodeTable->nCodeShift[bsrval];
		code >>= codeshift;
		tableidx = pDecodeTable->dwSymbolBase[bsrval] + code;
		symbol = pDecodeTable->SymbolAndCodeLength[tableidx].bySymbol;
		codelen = pDecodeTable->SymbolAndCodeLength[tableidx].nCodeLength;
	}

	_ASSERT(symbol < 0x400);

	if (bAccum)
	{
		symbol = (symbol + byPrevSymbol) & 0x3ff;
		byPrevSymbol = symbol;
	}

	if (nCorrPos != 0)
		symbol += (*(pDst + nCorrPos) + 0x200) & 0x3ff;

	*pDst = symbol;
	if (nDummyAlphaPos != 0)
		*(pDst + nDummyAlphaPos) = 0x3ff;
	nBits += codelen;

	if (nBits >= 32)
	{
		nBits -= 32;
		pSrc++;
	}
}

static void cpp_HuffmanDecode_common10(uint16_t *pDstBegin, uint16_t *pDstEnd, const uint8_t *pSrcBegin, const HUFFMAN_DECODE_TABLE10 *pDecodeTable, bool bAccum, int nStep, size_t cbWidth, ssize_t scbStride, int nCorrPos, int nDummyAlphaPos, uint16_t initial)
{
	int nBits;
	uint32_t *pSrc;
	uint16_t *p;
	uint16_t prevsym;
	uint16_t *pStripeBegin;

	nBits = 0;
	pSrc = (uint32_t *)pSrcBegin;
	prevsym = initial;

	for (pStripeBegin = pDstBegin; pStripeBegin != pDstEnd; *(uint8_t **)&pStripeBegin += scbStride)
	{
		uint16_t *pStripeEnd = (uint16_t*)((uint8_t *)pStripeBegin + cbWidth);
		for (p = pStripeBegin; p < pStripeEnd; p += nStep)
			DecodeSymbol10(pSrc, nBits, pDecodeTable, bAccum, prevsym, p, nCorrPos, nDummyAlphaPos);
	}
}

void cpp_HuffmanDecode(uint8_t *pDstBegin, uint8_t *pDstEnd, const uint8_t *pSrcBegin, const HUFFMAN_DECODE_TABLE *pDecodeTable, size_t cbWidth, ssize_t scbStride)
{
	cpp_HuffmanDecode_common(pDstBegin, pDstEnd, pSrcBegin, pDecodeTable, false, 1, cbWidth, scbStride, 0, 0);
}

void cpp_HuffmanDecodeStep4(uint8_t *pDstBegin, uint8_t *pDstEnd, const uint8_t *pSrcBegin, const HUFFMAN_DECODE_TABLE *pDecodeTable, size_t cbWidth, ssize_t scbStride)
{
	cpp_HuffmanDecode_common(pDstBegin, pDstEnd, pSrcBegin, pDecodeTable, false, 4, cbWidth, scbStride, 0, 0);
}

void cpp_HuffmanDecodeAndAccum(uint8_t *pDstBegin, uint8_t *pDstEnd, const uint8_t *pSrcBegin, const HUFFMAN_DECODE_TABLE *pDecodeTable, size_t cbWidth, ssize_t scbStride)
{
	cpp_HuffmanDecode_common(pDstBegin, pDstEnd, pSrcBegin, pDecodeTable, true, 1, cbWidth, scbStride, 0, 0);
}

void cpp_HuffmanDecodeAndAccumStep2(uint8_t *pDstBegin, uint8_t *pDstEnd, const uint8_t *pSrcBegin, const HUFFMAN_DECODE_TABLE *pDecodeTable, size_t cbWidth, ssize_t scbStride)
{
	cpp_HuffmanDecode_common(pDstBegin, pDstEnd, pSrcBegin, pDecodeTable, true, 2, cbWidth, scbStride, 0, 0);
}

void cpp_HuffmanDecodeAndAccumStep3(uint8_t *pDstBegin, uint8_t *pDstEnd, const uint8_t *pSrcBegin, const HUFFMAN_DECODE_TABLE *pDecodeTable, size_t cbWidth, ssize_t scbStride)
{
	cpp_HuffmanDecode_common(pDstBegin, pDstEnd, pSrcBegin, pDecodeTable, true, 3, cbWidth, scbStride, 0, 0);
}

void cpp_HuffmanDecodeAndAccumStep4(uint8_t *pDstBegin, uint8_t *pDstEnd, const uint8_t *pSrcBegin, const HUFFMAN_DECODE_TABLE *pDecodeTable, size_t cbWidth, ssize_t scbStride)
{
	cpp_HuffmanDecode_common(pDstBegin, pDstEnd, pSrcBegin, pDecodeTable, true, 4, cbWidth, scbStride, 0, 0);
}

void cpp_HuffmanDecodeAndAccumStep3ForBGRBlue(uint8_t *pDstBegin, uint8_t *pDstEnd, const uint8_t *pSrcBegin, const HUFFMAN_DECODE_TABLE *pDecodeTable, size_t cbWidth, ssize_t scbStride)
{
	cpp_HuffmanDecode_common(pDstBegin, pDstEnd, pSrcBegin, pDecodeTable, true, 3, cbWidth, scbStride, +1, 0);
}

void cpp_HuffmanDecodeAndAccumStep3ForBGRRed(uint8_t *pDstBegin, uint8_t *pDstEnd, const uint8_t *pSrcBegin, const HUFFMAN_DECODE_TABLE *pDecodeTable, size_t cbWidth, ssize_t scbStride)
{
	cpp_HuffmanDecode_common(pDstBegin, pDstEnd, pSrcBegin, pDecodeTable, true, 3, cbWidth, scbStride, -1, 0);
}

void cpp_HuffmanDecodeAndAccumStep4ForBGRXBlue(uint8_t *pDstBegin, uint8_t *pDstEnd, const uint8_t *pSrcBegin, const HUFFMAN_DECODE_TABLE *pDecodeTable, size_t cbWidth, ssize_t scbStride)
{
	cpp_HuffmanDecode_common(pDstBegin, pDstEnd, pSrcBegin, pDecodeTable, true, 4, cbWidth, scbStride, +1, 0);
}

void cpp_HuffmanDecodeAndAccumStep4ForBGRXRed(uint8_t *pDstBegin, uint8_t *pDstEnd, const uint8_t *pSrcBegin, const HUFFMAN_DECODE_TABLE *pDecodeTable, size_t cbWidth, ssize_t scbStride)
{
	cpp_HuffmanDecode_common(pDstBegin, pDstEnd, pSrcBegin, pDecodeTable, true, 4, cbWidth, scbStride, -1, 0);
}

void cpp_HuffmanDecodeAndAccumStep4ForBGRXRedAndDummyAlpha(uint8_t *pDstBegin, uint8_t *pDstEnd, const uint8_t *pSrcBegin, const HUFFMAN_DECODE_TABLE *pDecodeTable, size_t cbWidth, ssize_t scbStride)
{
	cpp_HuffmanDecode_common(pDstBegin, pDstEnd, pSrcBegin, pDecodeTable, true, 4, cbWidth, scbStride, -1, 1);
}

void cpp_HuffmanDecodeAndAccumStep4ForXRGBRedAndDummyAlpha(uint8_t *pDstBegin, uint8_t *pDstEnd, const uint8_t *pSrcBegin, const HUFFMAN_DECODE_TABLE *pDecodeTable, size_t cbWidth, ssize_t scbStride)
{
	cpp_HuffmanDecode_common(pDstBegin, pDstEnd, pSrcBegin, pDecodeTable, true, 4, cbWidth, scbStride, +1, -1);
}


void cpp_HuffmanDecodeAndAccum10(uint16_t *pDstBegin, uint16_t *pDstEnd, const uint8_t *pSrcBegin, const HUFFMAN_DECODE_TABLE10 *pDecodeTable, size_t cbWidth, ssize_t scbStride, uint16_t initial)
{
	cpp_HuffmanDecode_common10(pDstBegin, pDstEnd, pSrcBegin, pDecodeTable, true, 1, cbWidth, scbStride, 0, 0, initial);
}
