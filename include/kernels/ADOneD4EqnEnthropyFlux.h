#pragma once

#include "ADKernel.h"
// #include "ADOneDHeatFluxBase.h"

/**
 * Compute enthropy flux term in enthropy conservation equation
 * The form of this term is as following:
 *   \frac{\partial}{\partial z}( k·A/T * \partial T/\partial z ) 
 */
class ADOneD4EqnEnthropyFlux : public ADKernel 
{
public:
  ADOneD4EqnEnthropyFlux(const InputParameters & parameters);

protected:
  virtual ADReal computeQpResidual() ;

  // Heat conductivity coefficient
  const ADMaterialProperty<ADReal> & _thermal_conductivity;
  
  // Flow channel area
  const ADVariableValue & _A;

  // Flow channel average temperature
  const ADVariableValue & _T;

  // Temperature gradient
  const ADVariableGradient & _grad_T;

  // The direction of the flow channel
  const MaterialProperty<RealVectorValue> & _dir;

public:
  static InputParameters validParams();
};
