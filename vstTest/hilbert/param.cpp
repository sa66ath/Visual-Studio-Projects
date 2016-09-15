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
VSTParamHasMinMax<unsigned int>	paramBypass("", "BYPASS", 0, 1);

ParamOscFreq	paramOscFreq("Hz", "OSC Freq", 0.1f, 10.f, &osc);
ParamOscPhase	paramOscPhase("deg.", "OSC Phase", -0.5f, +0.5f, &osc);

VSTParam	paramDry("", "DRY");
VSTParam	paramWet("", "WET");

VSTParamHasMinMax<float>	paramReso("", "RESO", 0.f, 0.9f);

ParamResoFilter	paramResoHPF("Hz", "RESO HPF", 0.1f, 500.f, &filResoHPF);
ParamResoFilter	paramResoLPF("Hz", "RESO LPF", 250.0f, 18000.f, &filResoLPF);
		

//==============================================================================
//	Parameters Setup
//==============================================================================
VSTParam* params[] = {
	&paramBypass,

	&paramOscFreq,
	&paramOscPhase,

	&paramReso,

	&paramResoHPF,
	&paramResoLPF,
	
	&paramDry,
	&paramWet,
};


const VstInt32 kNumParams = sizeof(params)/sizeof(params[0]);

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void MyPlug::setupParam()
{
	paramBypass.setReadout(0);
	
	paramOscFreq.setReadout(8.0);
	paramOscPhase.setReadout(0.5);

	paramReso.setVal(0.75);

	paramResoHPF.setReadout(200.0);
	paramResoLPF.setReadout(8000.0);
	
	paramDry.setVal(1.0);
	paramWet.setVal(1.0);
}
