/*==============================================================================
	param.h: VST Plug-in User parameter 
					Shoichi Ikeda
==============================================================================*/
#ifndef _PARAM_H
#define _PARAM_H

#include "public.sdk/source/vst2.x/audioeffectx.h"
#include "myplug.h"
#include "bosc.h"

#include "vst_param.h"


//==============================================================================
//	User Defined classes
//==============================================================================
//------------------------------------------------------------------------------
//	OSC Frequency
//------------------------------------------------------------------------------
class ParamOscFreq : public VSTParamHasMinMax<float>
{
	DualOsc* osc;	
public:
	ParamOscFreq(const char* label, const char* name, float min, float max, DualOsc* osc) : 
			VSTParamHasMinMax<float>(label, name, min, max), osc(osc) {}
	void setVal(float val) {
		VSTParamHasMinMax<float>::setVal(val);
		osc->setFreq(readout());
	}	
};


//------------------------------------------------------------------------------
//	OSC Phase
//------------------------------------------------------------------------------
class ParamOscPhase : public VSTParamHasMinMax<float>
{
	DualOsc* osc;	
public:
	ParamOscPhase(const char* label, const char* name, float min, float max, DualOsc* osc) : 
			VSTParamHasMinMax<float>(label, name, min, max), osc(osc) {}
	void setVal(float val) {
		VSTParamHasMinMax<float>::setVal(val);
		osc->setPhase(readout());
	}	
};

//------------------------------------------------------------------------------
//	RESO HPF/LPF
//------------------------------------------------------------------------------
class ParamResoFilter : public VSTParamHasMinMax<float>
{
	BFilterIIR1D<float>*	fp;
public:
	ParamResoFilter(const char* label, const char* name, float min, float max, BFilterIIR1D<float>* filter) : 
			VSTParamHasMinMax<float>(label, name, min, max), fp(filter) {}
	void setVal(float val) {
		VSTParamHasMinMax<float>::setVal(val);
		if(fp) {
			fp->setFreq(readout());
		}
	}
};

//==============================================================================
//	Parameters
//==============================================================================
extern VSTParamHasMinMax<unsigned int>	paramBypass;

extern VSTParam	paramDry;
extern VSTParam	paramWet;

extern VSTParamHasMinMax<float> paramReso;

extern VSTParam* params[];

extern const VstInt32 kNumParams;

#endif /* _PARAM_H */
/*==============================================================================
	End
==============================================================================*/
