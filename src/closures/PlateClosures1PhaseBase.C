#include "PlateClosures1PhaseBase.h"
#include "FlowModelSinglePhase.h"
#include "PlateFlowChannel1Phase.h"

InputParameters
PlateClosures1PhaseBase::validParams()
{
  InputParameters params = ClosuresBase::validParams();
  return params;
}

PlateClosures1PhaseBase::PlateClosures1PhaseBase(const InputParameters & params) : ClosuresBase(params) {}

void
PlateClosures1PhaseBase::addWallFrictionFunctionMaterial(const PlateFlowChannel1Phase & flow_channel) const
{
  const FunctionName & f_D_fn_name = flow_channel.getParam<FunctionName>("f");
  flow_channel.makeFunctionControllableIfConstant(f_D_fn_name, "f");

  const std::string class_name = "ADWallFrictionFunctionMaterial";
  InputParameters params = _factory.getValidParams(class_name);
  params.set<std::vector<SubdomainName>>("block") = flow_channel.getSubdomainNames();
  params.set<MaterialPropertyName>("f_D") = FlowModelSinglePhase::FRICTION_FACTOR_DARCY;
  params.set<FunctionName>("function") = f_D_fn_name;
  _sim.addMaterial(class_name, genName(flow_channel.name(), "f_wall_fn_mat"), params);
}

void
PlateClosures1PhaseBase::addAverageWallTemperatureMaterial(const PlateFlowChannel1Phase & flow_channel) const
{
  const std::string class_name = "ADAverageWallTemperature3EqnMaterial";
  InputParameters params = _factory.getValidParams(class_name);
  params.set<std::vector<SubdomainName>>("block") = flow_channel.getSubdomainNames();
  params.set<std::vector<VariableName>>("T_wall_sources") = flow_channel.getWallTemperatureNames();
  params.set<std::vector<MaterialPropertyName>>("Hw_sources") =
      flow_channel.getWallHTCNames1Phase();
  params.set<std::vector<VariableName>>("P_hf_sources") = flow_channel.getHeatedPerimeterNames();
  params.set<std::vector<VariableName>>("P_hf_total") = {FlowModel::HEAT_FLUX_PERIMETER};
  params.set<MaterialPropertyName>("Hw_average") =
      FlowModelSinglePhase::HEAT_TRANSFER_COEFFICIENT_WALL;
  params.set<std::vector<VariableName>>("T_fluid") = {FlowModelSinglePhase::TEMPERATURE};
  _sim.addMaterial(class_name, genName(flow_channel.name(), "avg_T_wall_3eqn_mat"), params);
}
