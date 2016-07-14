/*==============================================================================
	bfade.h:	Fade In/Out
				ikeda@roland.co.jp
==============================================================================*/
#ifndef	_BFADE_H
#define	_BFADE_H

#include <cmath>
#if !defined(M_PI)
#define M_PI       3.14159265358979323846
#endif  /* M_PI */

#include <algorithm>

// #include "fs.h"

//==============================================================================
//	
//==============================================================================
template <class TDATA>
class BFade {
	bool in;
	TDATA time;

	TDATA level;
	TDATA delta;

	virtual void calcDelta() { delta = Fs.getRate()/time; } 
public:
	BFade(bool in = false, TDATA time = 0.005) : in(in), time(time) { calcDelta(); }

	virtual void setFade(bool in) { this->in = in; }
	virtual void setFade(bool in, TDATA time) { this->in = in; this->time = time; calcDelta(); }

	virtual void setTime(TDATA t) { time = t; } 
	virtual void setSampleFreq(TDATA freq) { calcDelta(); } 

	virtual TDATA process() { level += (in)? delta:-delta; return level = std::min(std::max(0.f, level), 1.f); }
};


#endif	/* _BFADE_H */
/*==============================================================================
	End
==============================================================================*/
