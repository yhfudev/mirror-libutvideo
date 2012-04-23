/*  */
/* $Id: QTCodec.h 862 2012-01-23 10:17:30Z umezawa $ */

#pragma once

#include "Codec.h"
#include "Mutex.h"

struct CQTCodec
{
	ComponentInstance self;
	ComponentInstance delegateComponent;
	ComponentInstance target;
	OSType componentSubType;
	CCodec *codec;
	CMutex *mutex;
};

pascal ComponentResult QTCodecOpen(CQTCodec *glob, ComponentInstance self);
pascal ComponentResult QTCodecClose(CQTCodec *glob, ComponentInstance self);
pascal ComponentResult QTCodecVersion(CQTCodec *glob);
pascal ComponentResult QTCodecTarget(CQTCodec *glob, ComponentInstance target);
pascal ComponentResult QTCodecGetCodecInfo(CQTCodec *glob, CodecInfo *info);
