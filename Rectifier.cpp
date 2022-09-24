/*
 * Rectifier.cpp
 *
 *  Created on: 23 Sep 2022
 *      Author: julianporter
 */

#include "Rectifier.hpp"
#include <cstring>

namespace meromorph { namespace follower {

const uint32 Rectifier::BUFFER_SIZE = 64;

void append(char *str,const char *root,const char *ext) {
	strcpy(str,root);
	strcat(str,ext);
}

Rectifier::Rectifier(const char *code) : audio(Rectifier::BUFFER_SIZE) {
	char inP[80];
	append(inP,"/audio_inputs/signal",code);
	trace(inP);
	signal = JBox_GetMotherboardObjectRef(inP);
	char outP[80];
	append(outP,"/audio_outputs/envelope",code);
	trace(outP);
	output = JBox_GetMotherboardObjectRef(outP);
}

void Rectifier::reset() { last = 0; }

uint32 Rectifier::read() {
	auto ref = JBox_LoadMOMPropertyByTag(signal, kJBox_AudioInputBuffer);
	uint32 size = std::min((uint32)JBox_GetDSPBufferInfo(ref).fSampleCount,Rectifier::BUFFER_SIZE);
	if(size>0) {
		JBox_GetDSPBufferData(ref, 0, size, audio.data());
	}
	return size;
}
void Rectifier::write() {
	auto ref = JBox_LoadMOMPropertyByTag(output, kJBox_AudioOutputBuffer);
	if(audio.size()>0) { JBox_SetDSPBufferData(ref, 0, audio.size(), audio.data()); }
}

bool Rectifier::isConnected() const {
	auto sock = JBox_LoadMOMPropertyByTag(signal,kJBox_AudioInputConnected);
	if(JBox_GetType(sock)==kJBox_Boolean) return JBox_GetBoolean(sock);
	else return false;
}


float32 Rectifier::operator()(const int32 mode,const float32 rho,const float32 threshold,const float32 inG,const float32 outG) {
	if(read()>0) {
		auto start=audio.begin();
		auto end=audio.end();
		switch(mode) {
		case 0:
		default:
			std::transform(start,end,start,[inG](float32 x) { return std::max(x*inG,0.f); });
			break;
		case 1:
			std::transform(start,end,start,[inG](float32 x) { return fabs(x*inG); });
			break;
		case 2:
			std::transform(start,end,start,[inG](float32 x) { return x*x*inG*inG; });
			break;
		}
		auto ll=last;
		std::transform(start,end,start,[rho,&ll](float32 x) {
			ll=rho*x + (1.f-rho)*ll;
			return ll;
		});
		last=ll;

		if(mode==2) {
			std::transform(start,end,start, [outG](float32 x) { return sqrt(x)*outG; });
		}
		else {
			std::transform(start,end,start, [outG](float32 x) { return x*outG; });
		}

		unsigned aboveThreshold=0;
		for(auto it=audio.begin();it!=audio.end();it++) {
			if (*it>threshold) aboveThreshold+=2;
		}
		write();
		return (aboveThreshold>audio.size()) ? 1.0 : -1.0;
	}
	else { return 0; }
}

void Rectifier::bypass() {
	if(read()) write();
}





}}



