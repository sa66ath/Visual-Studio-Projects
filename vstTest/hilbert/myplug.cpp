//-------------------------------------------------------------------------------------------------------
// Category     : VST 2.x SDK Samples
// Filename     : myplug.cpp
// Created by   : Shoichi Ikeda
// Description  : FX Experimental.
// Modified from : myplug.cpp, Steinberg VST SDK Sample
//-------------------------------------------------------------------------------------------------------
#include <cmath>
#include <algorithm>

#include "public.sdk/source/vst2.x/audioeffectx.h"


#include "bosc.h"
#include "param.h"

#include "myplug.h"


//-------------------------------------------------------------------------------------------------------
const char kVendorString[] = "ikeda@***.co.jp *** Corporation";
const char kProductString[] = "Ikeda's Hilbert (like phaser?)";
const char kEffectName[] = "MultiBand";

#define	UNIQ_ID	'TEST'


//==============================================================================
//	Processing Instances
//==============================================================================
BFilterIIR1D<float>	ps00(BFilter::kPassTypeAPF,    48.9f),
			ps01(BFilter::kPassTypeAPF,   905.6f),
			ps02(BFilter::kPassTypeAPF,  3847.0f),
			ps03(BFilter::kPassTypeAPF, 16071.0f),
			
			ps20(BFilter::kPassTypeAPF,    48.9f),
			ps21(BFilter::kPassTypeAPF,   905.6f),
			ps22(BFilter::kPassTypeAPF,  3847.0f),
			ps23(BFilter::kPassTypeAPF, 16071.0f),
			

			ps10(BFilter::kPassTypeAPF,    48.9f),
			ps11(BFilter::kPassTypeAPF,   421.2f),
			ps12(BFilter::kPassTypeAPF,  1881.3f), 
			ps13(BFilter::kPassTypeAPF,  7866.3f),

			ps30(BFilter::kPassTypeAPF,    48.9f),
			ps31(BFilter::kPassTypeAPF,   421.2f),
			ps32(BFilter::kPassTypeAPF,  1881.3f), 
			ps33(BFilter::kPassTypeAPF,  7866.3f);


DualOsc	osc;


BFilterIIR1D<float>	filResoHPF(BFilter::kPassTypeHPF,    0.1f),
			filResoLPF(BFilter::kPassTypeLPF,    20000.f);

//==============================================================================
//	AudioEffect
//==============================================================================
void MyPlug::setSampleRate(float freq)
{
}


//-----------------------------------------------------------------------------------------
void MyPlug::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

    while (--sampleFrames >= 0)
    {
	// Input
	float in = ((*in1++) + (*in2++))/2.f;

	float oscL, oscR;
	osc.process(&oscL, &oscR);

	// L channel
	// R channel
	//
	static float fb = 0.;

	fb = filResoLPF.process(filResoHPF.process(fb));
	fb = paramReso.getVal()*fb;

	float outI = ps03.process(ps02.process(ps01.process(ps00.process(in + fb))));

	float outQ = ps13.process(ps12.process(ps11.process(ps10.process(in + fb))));

	if(paramBypass.readout()) {
		(*out1++) = in;
		(*out2++) = in;
	} else {
		float out = oscL*outI + oscR*outQ;
//		out = paramDry.getVal()*in + paramWet.getVal()*out;
//		fb = out;
//		(*out1++) = out;
//		(*out2++) = out;
//
		(*out1++) = paramDry.getVal()*in;
		(*out2++) = paramWet.getVal()*out;
	}
    }
}

//-----------------------------------------------------------------------------------------
AudioEffect* pEffectInstance;

AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
	return	pEffectInstance = new MyPlug (audioMaster);
}

//-------------------------------------------------------------------------------------------------------
MyPlug::MyPlug (audioMasterCallback audioMaster)
: AudioEffectX (audioMaster, 1, kNumParams)
{
	setNumInputs (2);		// stereo in
	setNumOutputs (2);		// stereo out
	setUniqueID (UNIQ_ID);	// identify
	canProcessReplacing ();	// supports replacing output
//	canDoubleReplacing ();	// supports double precision processing

	vst_strncpy (programName, "Default", kVstMaxProgNameLen);	// default program name

	setupParam();
}

//-------------------------------------------------------------------------------------------------------
MyPlug::~MyPlug ()
{
	// nothing to do here
}


//-------------------------------------------------------------------------------------------------------
void MyPlug::setProgramName (char* name)
{
	vst_strncpy (programName, name, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
void MyPlug::getProgramName (char* name)
{
	vst_strncpy (name, programName, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
void MyPlug::setParameter (VstInt32 index, float value)
{
	params[index]->setVal(value);
}

//-----------------------------------------------------------------------------------------
float MyPlug::getParameter (VstInt32 index)
{
	return params[index]->getVal();
}

//-----------------------------------------------------------------------------------------
void MyPlug::getParameterName (VstInt32 index, char* label)
{
	params[index]->getName(label);
}

//-----------------------------------------------------------------------------------------
void MyPlug::getParameterDisplay (VstInt32 index, char* text)
{
	params[index]->getDisplay (text);
}

//-----------------------------------------------------------------------------------------
void MyPlug::getParameterLabel (VstInt32 index, char* label)
{
	params[index]->getLabel(label);
}

//------------------------------------------------------------------------
bool MyPlug::getEffectName (char* name)
{
	vst_strncpy (name, kEffectName, kVstMaxEffectNameLen);
	return true;
}

//------------------------------------------------------------------------
bool MyPlug::getProductString (char* text)
{
	vst_strncpy (text, kProductString, kVstMaxProductStrLen);
	return true;
}

//------------------------------------------------------------------------
bool MyPlug::getVendorString (char* text)
{
	vst_strncpy (text, kVendorString, kVstMaxVendorStrLen);
	return true;
}

//-----------------------------------------------------------------------------------------
VstInt32 MyPlug::getVendorVersion ()
{ 
	return 1000; 
}

