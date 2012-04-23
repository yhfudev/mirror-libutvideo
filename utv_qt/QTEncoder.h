/*  */
/* $Id: QTEncoder.h 866 2012-04-18 13:26:00Z umezawa $ */

#pragma once

#include "QTCodec.h"
#include "Codec.h"
#include "Mutex.h"

struct CQTEncoder : public CQTCodec
{
	ICMCompressorSessionRef session;
	ICMCompressionSessionOptionsRef sessionOptions;
};
