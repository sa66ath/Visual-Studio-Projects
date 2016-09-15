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
const char kVendorString[] = "ikeda@roland.co.jp BOSS Corporation";
const char kProductString[] = "Ikeda's MultiBand";
const char kEffectName[] = "Octave";

#define	UNIQ_ID	'TEST'


//==============================================================================
//	Processing Instances
//==============================================================================

MultiBand	multiBand;

MultiBand::MultiBand() 
		: 	preDelay(0.008f)
{
	setupProcessing();
}

void MultiBand::setupProcessing()
{
	for(size_t i = 0; i < sizeof(filter)/sizeof(filter[0]); i++) {
		filter[i].setPassType(BFilter::kPassTypeBPF);
		filter[i].setFreq(55.f * pow(2.f,(float)i/12.f ));
		filter[i].setQ(fQ.readout());
		
		filter2[i].setPassType(BFilter::kPassTypeBPF);
		filter2[i].setFreq(55.f * pow(2.f,(float)i/12.f ));
		filter2[i].setQ(fQ.readout());
	}
}

void MultiBand::setSampleFreq(float freq)
{
	for(size_t i = 0; i < sizeof(filter)/sizeof(filter[0]); i++) {
		filter[i].setSampleFreq(freq);
		filter2[i].setSampleFreq(freq);
	}
}

RingBuff<float, (size_t)(0.1*44100.0)>	dly;	
float buf[kNumOfBand];
float div2[kNumOfBand];
float div3[kNumOfBand];
int n[kNumOfBand];

float	upOct[kNumOfBand];
float	down5th[kNumOfBand];
float	downOct[kNumOfBand];
float	downOct5th[kNumOfBand];

float MultiBand::process(float in)
{
	float acc = 0.0;
	for(size_t band = 0; band < sizeof(filter)/sizeof(filter[0]); band++) {
		acc = filter[band].process(in);
		if((buf[band] < 0.f) && (0.f <= acc)) {
			div2[band] = (0 == n[band]%2)? -1.f:1.f;
			div3[band] = (0 == n[band]%3)? -1.f:1.f;
			n[band]++;
			if(6 == n[band])	n[band] = 0;
		}
		upOct[band] = 2.f * std::abs(acc);
		downOct[band] = div2[band] * acc;
		downOct5th[band] = 1.5f * div3[band] * acc;
		down5th[band] = 2.f * std::abs(downOct5th[band]);

		buf[band] = acc;
	}

	acc = 0.f;

	for(size_t band = 0; band < kNumOfBand; band++) {
		float tmp = 0.f;
		if(12 <= band) {
			tmp += paramUpOct.getVal() * upOct[band - 12];	
		}
		if((band + 12) < kNumOfBand) {
			tmp += paramDownOct.getVal() * downOct[band + 12];
		}
		if((band + (7+12)) < kNumOfBand) {
			tmp += paramDownOct5th.getVal() * downOct5th[band + (7+12)];
		}
		if((band + 7) < kNumOfBand) {
			tmp += paramDown5th.getVal() * downOct5th[band + 7];
		}
		acc += filter2[band].process(tmp);
	}


	// LFO
	float lfo = (oscChorus.process() + 1.f)/2.f; 
	lfo *= chorusDepthSample;	// Depth
	lfo += chorusPreDelaySample;	// Pre Delay

	dly.decBP();
	dly[0] = acc;
	acc = dly.lerpRead(lfo);

	return acc;
}



//==============================================================================
//	AudioEffect
//==============================================================================
void MyPlug::setSampleRate(float freq)
{
	multiBand.setSampleFreq(freq);
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

	float out = paramDry.getVal()*in + paramWet.getVal()*multiBand.process(in);

	// L channel
	// (*out1++) = in;
	//
	(*out1++) = out;
	
	// R channel
	//	(*out2++) = paramDry.getVal()*in + paramWet.getVal()*multiBand.process(in);
	//
	(*out2++) = out;


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

