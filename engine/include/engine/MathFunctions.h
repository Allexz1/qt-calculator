#pragma once

#include "engine/AngleMode.h"

namespace engine::mathfn {

double squareRoot(double x);
double reciprocal(double x);
double power(double base, double exponent);
double factorial(double x);

double sinOf(double x, AngleMode mode);
double cosOf(double x, AngleMode mode);
double tanOf(double x, AngleMode mode);
double asinOf(double x, AngleMode mode);
double acosOf(double x, AngleMode mode);
double atanOf(double x, AngleMode mode);

double log10Of(double x);
double lnOf(double x);
double expOf(double x);

} // namespace engine::mathfn
