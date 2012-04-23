/*  */
/* $Id: QTDecoder.h 862 2012-01-23 10:17:30Z umezawa $ */

#pragma once

#include "QTCodec.h"
#include "Codec.h"
#include "Mutex.h"

struct CQTDecoder : public CQTCodec
{
	OSType **wantedDestinationPixelTypes;
	int beginBandDone;
};
