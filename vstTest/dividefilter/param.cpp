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

VSTParamHasMinMax<unsigned int>	paramBand("", "BAND", 0, kNumOfBand);



//------------------------------------------------------------------------------


ParamGain			fGain("dB", "BPF GAIN"); 
ParamQ				fQ("Q","BPF RESO", 0.5f, 50.f, &devideFilter);

//==============================================================================
//	Parameters Setup
//==============================================================================
VSTParam* params[] = {
	&paramBand,

	&paramDry,
	&paramWet,

	&fQ,
};

const VstInt32 kNumParams = sizeof(params)/sizeof(params[0]);

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void MyPlug::setupParam()
{
	paramBand.setReadout(70);

	paramDry.setVal(0.f);
	paramWet.setVal(1.f);
	
	fQ.setReadout(0.707f);
	fGain.setVal(1.0);

}
