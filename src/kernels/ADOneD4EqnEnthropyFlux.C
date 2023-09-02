#include "ADOneD4EqnEnthropyFlux.h"

registerMooseObject("MIFTAPApp", ADOneD4EqnEnthropyFlux);

InputParameters
ADOneD4EqnEnthropyFlux::validParams()
{
  InputParameters params = ADKernel::validParams();
  params.addParam<MaterialPropertyName>("thermal_conductivity",
                                        "thermal_conductivity",
                                        "the name of the thermal conductivity material property");
  params.addRequiredCoupledVar("A", "Cross-sectional area");
  params.addRequiredCoupledVar("T", "Temperature");
  params.addRequiredParam<MaterialPropertyName>(
      "direction", "The direction of the flow channel material property");
  params.addRequiredCoupledVar("grad_T", "Gradient of temperature");  
  params.addClassDescription("Computes the gravity term for the energy equation in 1-phase flow");
  return params;
}

ADOneD4EqnEnthropyFlux::ADOneD4EqnEnthropyFlux(const InputParameters & parameters)
  : ADKernel(parameters),
    _thermal_conductivity(getADMaterialProperty<ADReal>("thermal_conductivity")),
    _A(adCoupledValue("A")),
    _T(adCoupledValue("T")),
    _dir(getMaterialProperty<RealVectorValue>("direction")),
    _grad_T(adCoupledGradient("grad_T"))
{
}

ADReal
ADOneD4EqnEnthropyFlux::computeQpResidual()
{
  return  _thermal_conductivity[_qp] * _A[_qp] / _T[_qp] * _grad_T[_qp] *  _dir[_qp] * _test[_i][_qp];
}
