#include "stdafx.h"
#include "Recorder.h"


SplitMemnto::SplitMemnto(const SplitType & splitInfo)
{
	_Data = splitInfo;
}

SplitMemnto::~SplitMemnto()
{
}

void SplitMemnto::Restore(SplitType * ref)
{
	*ref = _Data;
}

HandCardsMemnto::HandCardsMemnto(const HandCards & ref)
{
	memcpy(&_Data[0][0],&ref.Flags[0][0], CARD_VALUE_LEN * 4 * sizeof(uint8_t));
}

HandCardsMemnto::~HandCardsMemnto()
{

}

void HandCardsMemnto::Restore(HandCards * ref)
{
	memcpy(&( ref->Flags[0][0]),&_Data[0][0], CARD_VALUE_LEN * 4 * sizeof(uint8_t));
	ref->UpdateByFlag();
}
