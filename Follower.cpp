#include "Follower.hpp"
#include <cmath>
#include <cstring>

namespace meromorph { namespace follower {



TJBox_Float64 clamp(const TJBox_Float64 lo,const TJBox_Float64 hi,const TJBox_Float64 value) {
	return std::min(hi,std::max(lo,value));
}

const uint32 CFollower::BUFFER_SIZE = 64;

CFollower::CFollower() : left("1"), right("2") {
	props = JBox_GetMotherboardObjectRef("/custom_properties");
	gateCV=JBox_GetMotherboardObjectRef("/cv_outputs/gate");
}

void CFollower::setGate(const float32 g) {
	int32 gate = (g>0) ? 2 : (g<0) ? 0 : 1;
	set(Tags::GATE,gate);
	JBox_StoreMOMPropertyAsNumber(gateCV,kJBox_CVOutputValue,gate);
};



void CFollower::process() {
	switch(state) {
	case State::Off:
	default:
		break;
	case State::Bypassed:
		left.bypass();
		right.bypass();
		setGate(0);
		break;
	case State::On: {
		auto gateL = connL ? left(mode,rho,threshold,inputGain,outputGain) : 0;
		auto gateR = connR ? right(mode,rho,threshold,inputGain,outputGain) : 0;
		float32 gate = gateScale*(gateL+gateR);
		setGate(gate);
		break;
	}
	}
	if(stateChanged) {
		stateChanged=false;
		left.reset();
		right.reset();
	}
}

inline float32 toFloat(const TJBox_Value diff) {
	return static_cast<float32>(JBox_GetNumber(diff));
}
inline bool toBool(const TJBox_Value diff) {
	return static_cast<float32>(JBox_GetNumber(diff))>0;
}
inline int32 toInt(const TJBox_Value diff) {
	return static_cast<int32>(static_cast<float32>(JBox_GetNumber(diff)));
}



void CFollower::RenderBatch(const TJBox_PropertyDiff diffs[], TJBox_UInt32 nDiffs) {
		for(auto i=0;i<nDiffs;i++) {
			auto diff=diffs[i];
			switch(diff.fPropertyTag) {
			case kJBox_CustomPropertiesOnOffBypass: {
				auto s = static_cast<State>(toInt(diff.fCurrentValue));
				if(s!=state) stateChanged=true;
				state=s;
				trace("State is now ^0",static_cast<int32>(state));
				break; }
			case Tags::LR: {
				auto f=toFloat(diff.fCurrentValue)*0.0625f;
				rho = log2(1+f);
				trace("Rho is now ^0",rho);
				break; }
			case Tags::TH:
				threshold = log2(1+toFloat(diff.fCurrentValue));
				trace("Threshold is now ^0",threshold);
				break;
			case Tags::RECTIFICATION:
				mode = toInt(diff.fCurrentValue);
				trace("Mode is now ^0",mode);
				break;
			case Tags::INPUT_GAIN: {
				auto f=toFloat(diff.fCurrentValue);
				inputGain=exp(2.0*f-1.0);
				trace("Input is now ^0",inputGain);
				break;
			}
			case Tags::OUTPUT_GAIN: {
				auto f=toFloat(diff.fCurrentValue);
				outputGain=exp(2.0*f-1.0);
				trace("Output is now ^0",outputGain);
				break;
			}

			case kJBox_AudioInputConnected:
				connL=left.isConnected();
				connR=right.isConnected();
				gateScale = (connL && connR) ? 1.0 : 2.0;
				trace("Gate scale is ^0",gateScale);
				break;
			}
		}
	process();
}

}}


