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


VSTParam			paramUpOct("x", "UP OCT");
VSTParam			paramDown5th("x", "DOWN 5th");
VSTParam			paramDownOct("x", "DOWN OCT");
VSTParam			paramDownOct5th("x", "DN Oct 5th");

//------------------------------------------------------------------------------
class ParamChorusRate : public VSTParamHasMinMax<float> {
public:
	ParamChorusRate(const char* label, const char* name, float min, float max) : VSTParamHasMinMax<float>(label, name, min, max) {}
	void setVal(float value)
	{ 
		v = value;
		multiBand.oscChorus.setFreq(readout());
	}
} paramChorusRate("Hz", "RATE", 0.1f, 10.f);


class ParamChorusDepth : public VSTParamHasMinMax<float> {
public:
	ParamChorusDepth(const char* label, const char* name, float min, float max) : VSTParamHasMinMax<float>(label, name, min, max) {}
	void setVal(float value)
	{ 
		v = value;
		multiBand.chorusDepthSample = Fs.getFreq()*readout();
	}
} paramChorusDepth("ms", "DEPTH", 0.f, 0.008f);

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class ParamPreDelay : public VSTParamHasMinMax<float>
{
public:
	ParamPreDelay(const char* label, const char* name, float min, float max) : VSTParamHasMinMax<float>(label, name, min, max) {}

	void setVal (float value)
	{
		VSTParamHasMinMax<float>::setVal(value);
		multiBand.preDelay = readout();
		multiBand.chorusPreDelaySample = Fs.getFreq()*readout();
		multiBand.setupProcessing();
	}
} paramPreDelay("sec", "PRE DLY", 0.0, 0.05f);

//------------------------------------------------------------------------------
ParamGain			fGain("dB", "BPF GAIN"); 
ParamQ				fQ("Q","BPF RESO", 0.5f, 50.f, &multiBand);


//==============================================================================
//	Parameters Setup
//==============================================================================
VSTParam* params[] = {
	&paramDry,
	&paramWet,

	&paramUpOct,
	&paramDown5th,
	&paramDownOct,
	&paramDownOct5th,

	&paramChorusRate,
	&paramChorusDepth,
	&paramPreDelay,

	&fQ,
	&fGain,
};

const VstInt32 kNumParams = sizeof(params)/sizeof(params[0]);

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void MyPlug::setupParam()
{
	/*+++++++++++++
	paramDry.setVal(1.f);
	paramWet.setVal(1.f);

	paramUpOct.setVal(1.f);
	paramDown5th.setVal(1.f);
	paramDownOct.setVal(1.f);
	paramDownOct5th.setVal(1.f);
	+++++++++++++*/

	// Gokuaku with MT-2
	paramDry.setVal(1.f);
	paramWet.setVal(1.f);

	paramUpOct.setVal(0.25f);
	paramDown5th.setVal(0.f);
	paramDownOct.setVal(0.9f);
	paramDownOct5th.setVal(0.25f);

	paramChorusRate.setReadout(0.25f);
	paramChorusDepth.setReadout(0.f);
	paramPreDelay.setReadout(0.f);

	fQ.setReadout(20.0);
	fGain.setVal(1.0);
}
