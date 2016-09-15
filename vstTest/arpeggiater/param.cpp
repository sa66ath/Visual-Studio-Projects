/*------------------------------------------------------------------------------
	parameter.cpp: VST Plug-in User parameter 
						Shoichi Ikeda
------------------------------------------------------------------------------*/
#include "public.sdk/source/vst2.x/audioeffectx.h"

#include "bfilter.h"
#include "myplug.h"

#include "param.h"


//==============================================================================
//	Parameter Definitions
//==============================================================================

VSTParam			paramDry("x", "DRY");
VSTParam			paramWet("x", "WET");

ParamRate	paramRate("Hz", "RATE", 0.01f, 5.f);

VSTParamHasMinMax<float>			paramAttack("sec", "ATTACK", 0.001f, 0.050f);

//------------------------------------------------------------------------------
ParamGain			fGain("dB", "BPF GAIN"); 
ParamQ				fQ("Q","BPF RESO", 0.5f, 50.f, &multiBand);


//==============================================================================
//	Parameters Setup
//==============================================================================
VSTParam* params[] = {
	&paramDry,
	&paramWet,

	&paramRate,

	&paramAttack,

	&fQ,
	&fGain,
};

const VstInt32 kNumParams = sizeof(params)/sizeof(params[0]);

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void MyPlug::setupParam()
{
	paramDry.setVal(0.f);
	paramWet.setVal(1.f);
	
	// paramRate.setReadout(0.8f);
	paramRate.setReadout(1.2f);
	
	paramAttack.setReadout(0.003f);

	fQ.setReadout(20.0);
	fGain.setVal(1.0);
}
