/*
 * Encoder.h
 *
 *  Created on: 6. 5. 2017
 *      Author: Michal
 */

#ifndef ENCODER_H_
#define ENCODER_H_
#include "stm32f30x.h"
#include <math.h>

class Encoder {
public:
	Encoder(double _razvor, double _polomer_kolesa);
	void Timer(void);
	void GetSpeedAndPosition(double *_speedL, double *_speedR, double *_posX, double *_posY);
private:
	void InitTimer(void);
	void InitGPIO(void);

	double speedL;
	double speedR;

	double X;
	double Y;
	double rotation;

	double radius;
	double axle_track;
};

#endif /* ENCODER_H_ */
