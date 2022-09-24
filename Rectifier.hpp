/*
 * Rectifier.hpp
 *
 *  Created on: 23 Sep 2022
 *      Author: julianporter
 */

#ifndef RECTIFIERF_HPP_
#define RECTIFIERF_HPP_

#include "base.hpp"
#include <vector>

namespace meromorph { namespace follower {

class Rectifier {
private:
	static const uint32 BUFFER_SIZE;

	TJBox_ObjectRef signal;
	TJBox_ObjectRef output;
	float32 last = 0;
	std::vector<float32> audio;

	uint32 read();
	void write();

public:
	Rectifier(const char *code);

	float32 operator()(const int32 mode,const float32 rho,const float32 threshold,const float32 inG,const float32 outG);
	void bypass();
	void reset();

	bool isConnected() const;


};

}}



#endif /* RECTIFIER_HPP_ */
