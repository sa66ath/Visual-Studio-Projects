// Category     : VST 2.x SDK Samples
// Filename     : myplug.h
// Created by   : Shoichi Ikeda
// Description  : FX Experimental.
// Modified from : myplug.h, Steinberg VST SDK Sample
//-------------------------------------------------------------------------------------------------------

#ifndef _MYPLUGIN_H
#define _MYPLUGIN_H

#include <vector>

#include "public.sdk/source/vst2.x/audioeffectx.h"

#include "ringbuff.h"
#include "bfilter.h"
#include "bosc.h"



//-------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------
class MyPlug : public AudioEffectX
{
	void setSampleRate(float freq);
public:
	MyPlug (audioMasterCallback audioMaster);
	~MyPlug ();

	void setupParam();

	// Processing
	virtual void processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames);
//	virtual void processDoubleReplacing (double** inputs, double** outputs, VstInt32 sampleFrames);

	// Program
	virtual void setProgramName (char* name);
	virtual void getProgramName (char* name);

	// Parameters
	virtual void setParameter (VstInt32 index, float value);
	virtual float getParameter (VstInt32 index);
	virtual void getParameterLabel (VstInt32 index, char* label);
	virtual void getParameterDisplay (VstInt32 index, char* text);
	virtual void getParameterName (VstInt32 index, char* text);

	virtual bool getEffectName (char* name);
	virtual bool getVendorString (char* text);
	virtual bool getProductString (char* text);
	virtual VstInt32 getVendorVersion ();

protected:
	char programName[kVstMaxProgNameLen + 1];
};

extern AudioEffect* pEffectInstance;

//==============================================================================
//	Processing Instances
//==============================================================================
class DualOsc {
public:
	BOscSin<float>	osc0, osc1;

	void restart() {
		osc0.restart();
		osc1.restart();
	}

	void setFreq(float freq) {
		osc0.setFreq(freq);
		osc1.setFreq(freq);
		restart();
	}
	void setPhase(float phase) {	///< phase = -1.0 - 0 - +1.0
		osc0.setPhase(0.f);
		osc1.setPhase(phase);
		restart();
	}

	void process(float* out0, float* out1) {
		*out0 = osc0.process();		
		*out1 = osc1.process();		
	}
};

//------------------------------------------------------------------------------
extern DualOsc	osc;

extern BFilterIIR1D<float>	filResoHPF, filResoLPF;

#endif	/* _MYPLUGIN_H */
//-------------------------------------------------------------------------------------------------------
//	End 
//-------------------------------------------------------------------------------------------------------
