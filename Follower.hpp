#pragma once

#include "base.hpp"
#include <vector>
#include <cmath>
#include "Rectifier.hpp"

namespace meromorph { namespace follower {

enum Tags : uint32 {
		LR=1,
		TH=2,
		RECTIFICATION=3,
		MODE=4,
		GATE=5,
		INPUT_GAIN=6,
		OUTPUT_GAIN=7
	};


class CFollower {


private:



	TJBox_ObjectRef props;
	TJBox_ObjectRef gateCV;

	float32 threshold = 0;
		float32 rho = 0;
		int32 mode = 0;
		State state = State::On;
		bool stateChanged = false;
	bool connL = false;
	bool connR = false;
	float32 gateScale=1.0;
	Rectifier left, right;
	float32 inputGain =1.0;
	float32 outputGain = 1.0;

	template<typename T>
		void set(const Tags tag,const T value) {
			JBox_StoreMOMPropertyByTag(props,static_cast<uint32>(tag),JBox_MakeNumber(static_cast<float32>(value)));
		}

	void reset();
	void setGate(const float32 gate);
	void process();

public:
	static const uint32 BUFFER_SIZE;
	explicit CFollower();
//	~CFollower();


    void RenderBatch(const TJBox_PropertyDiff iPropertyDiffs[], TJBox_UInt32 iDiffCount);

};
}}
