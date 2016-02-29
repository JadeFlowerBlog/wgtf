//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
//  cubic_bezier_interpolator.hpp
//
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  Copyright (c) Wargaming.net. All rights reserved.
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#ifndef CUBIC_BEZIER_INTERPOLATOR_H_
#define CUBIC_BEZIER_INTERPOLATOR_H_

#pragma once

#include "interfaces/i_curve_interpolator.hpp"

class CubicBezierInterpolator : public ICurveInterpolator
{
public:
	virtual BezierPointData interpolate(float time, const BezierPoint& p1, const BezierPoint& p2) override;

	virtual float timeAtX(float x, const BezierPoint& p1, const BezierPoint& p2) override;

	virtual void updateControlPoints(BezierPoint& point, BezierPoint* prevPoint, BezierPoint* nextPoint) override;

};

#endif // CUBIC_BEZIER_INTERPOLATOR_H_