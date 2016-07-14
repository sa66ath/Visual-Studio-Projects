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


#include "param.h"
#include "bosc.h"

#include "myplug.h"


//-------------------------------------------------------------------------------------------------------
const char kVendorString[] =	"ikeda@roland.co.jp BOSS Corporation";
const char kProductString[] =	"Ikeda's Divide Filter";
const char kEffectName[] =	"Divide Filter";

#define	UNIQ_ID	'TEST'


//==============================================================================
//	Processing Instances
//==============================================================================
DevideFilter	devideFilter;

DevideFilter::DevideFilter() 
{
	setupProcessing();
}

void DevideFilter::setupProcessing()
{
	for(size_t i = 0; i < sizeof(filter0)/sizeof(filter0[0]); i++) {
		filter0[i].setPassType(BFilter::kPassTypeBPF);
		filter0[i].setFreq(kStartOfBand * pow(2.f,(float)i/12.f ));
		filter0[i].setQ(fQ.readout());
	}
	
	for(size_t i = 0; i < sizeof(filter1)/sizeof(filter1[0]); i++) {
		filter1[i].setFreq(kStartOfBand * pow(2.f,(float)i/12.f ));
		filter1[i].setQ(fQ.readout());
		filter1[i].setIngredient(1.0, 0.5, 0.0);
		filter1[i].setPassType(BFilter::kPassTypeNone);
	}
}

void DevideFilter::setSampleFreq(float freq)
{
	for(size_t i = 0; i < sizeof(filter0)/sizeof(filter0[0]); i++) {
		filter0[i].setSampleFreq(freq);
	}
	for(size_t i = 0; i < sizeof(filter0)/sizeof(filter0[0]); i++) {
		filter1[i].setSampleFreq(freq);
	}
}

float DevideFilter::process0(float in)
{

	float acc = 0.f;
	
	for(size_t band = 0; band < sizeof(filter0)/sizeof(filter0[0]); band++) {
		if(paramBand.readout() >= band) {
			acc += filter0[band].process(in);
		}
	}

	return acc;
}


float DevideFilter::process1(float in)
{

	float acc = 0.0;
	float bandBuff[kNumOfBand];

	float	tmp = in;
	for(size_t band = 0; band < paramBand.readout(); band++) {
		bandBuff[band] = filter1[band].process(tmp);
		acc += bandBuff[band];
		tmp -= bandBuff[band];
	}
	acc += tmp;

		
	return acc;
}



//------------------------------------------------------------------------------

//==============================================================================
//	AudioEffect
//==============================================================================
void MyPlug::setSampleRate(float freq)
{
	devideFilter.setSampleFreq(freq);
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

	float outL = paramDry.getVal()*in + paramWet.getVal()*devideFilter.process0(in);
	float outR = paramDry.getVal()*in + paramWet.getVal()*devideFilter.process1(in);

	// L channel
	//
	(*out1++) = outL;
	
	// R channel
	//
	(*out2++) = outR;


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

