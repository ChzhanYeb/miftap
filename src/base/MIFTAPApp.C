#include "MIFTAPApp.h"
#include "THMSyntax.h"
#include "HeatConductionApp.h"
#include "FluidPropertiesApp.h"
#include "NavierStokesApp.h"
#include "RayTracingApp.h"
#include "RdgApp.h"
#include "SolidPropertiesApp.h"
#include "MiscApp.h"

#include "AppFactory.h"

#include "SinglePhaseFluidProperties.h"
#include "TwoPhaseFluidProperties.h"
#include "TwoPhaseNCGFluidProperties.h"

std::map<THM::FlowModelID, std::string> MIFTAPApp::_flow_model_map;

std::map<THM::FlowModelID, std::map<std::string, std::string>>
    MIFTAPApp::_closures_class_names_map;

namespace THM
{

FlowModelID
registerFlowModelID()
{
  static FlowModelID flow_model_id = 0;
  flow_model_id++;
  return flow_model_id;
}

FlowModelID FM_INVALID = registerFlowModelID();
FlowModelID FM_SINGLE_PHASE = registerFlowModelID();
FlowModelID FM_TWO_PHASE = registerFlowModelID();
FlowModelID FM_TWO_PHASE_NCG = registerFlowModelID();

} // namespace THM

InputParameters
MIFTAPApp::validParams()
{
  InputParameters params = MooseApp::validParams();
  params.set<bool>("use_legacy_output_syntax") = false;
  params.set<bool>("use_legacy_material_output") = false;
  return params;
}

registerKnownLabel("MIFTAPApp");

MIFTAPApp::MIFTAPApp(InputParameters parameters) : MooseApp(parameters)
{
  MIFTAPApp::registerAll(_factory, _action_factory, _syntax);
}

MIFTAPApp::~MIFTAPApp() {}

void
MIFTAPApp::registerObjects(Factory & /*factory*/)
{
  mooseError("registerObjects() is deprecated and not supported in THM");
}

void
MIFTAPApp::associateSyntax(Syntax & /*syntax*/, ActionFactory & /*action_factory*/)
{
  mooseError("associateSyntax() is deprecated and not supported in THM");
}

void
MIFTAPApp::registerExecFlags(Factory & /*factory*/)
{
  mooseError("registerExecFlags() is deprecated and not supported in THM");
}

void
MIFTAPApp::registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  Registry::registerObjectsTo(f, {"MIFTAPApp"});
  Registry::registerActionsTo(af, {"MIFTAPApp"});

  HeatConductionApp::registerAll(f, af, s);
  FluidPropertiesApp::registerAll(f, af, s);
  NavierStokesApp::registerAll(f, af, s);
  RayTracingApp::registerAll(f, af, s);
  RdgApp::registerAll(f, af, s);
  SolidPropertiesApp::registerAll(f, af, s);
  MiscApp::registerAll(f, af, s);

  THM::associateSyntax(s);
  THM::registerActions(s);

  registerClosuresOption("simple", "Closures1PhaseSimple", THM::FM_SINGLE_PHASE);
  registerClosuresOption("none", "Closures1PhaseNone", THM::FM_SINGLE_PHASE);

  // flow models
  registerFlowModel(THM::FM_SINGLE_PHASE, FlowModelSinglePhase);
}

const std::string &
MIFTAPApp::getFlowModelClassName(const THM::FlowModelID & flow_model_id)
{
  const auto it = _flow_model_map.find(flow_model_id);
  if (it == _flow_model_map.end())
    mooseError("Flow model with ID '" + Moose::stringify(flow_model_id) +
               "' is not associated with any flow models class. Register your flow model class to "
               "a flow model ID by calling registerFlowModel().");
  return it->second;
}

void
MIFTAPApp::registerApps()
{
  registerApp(MIFTAPApp);
}

const std::string &
MIFTAPApp::getClosuresClassName(const std::string & closures_option,
                                           const THM::FlowModelID & flow_model_id)
{
  const std::string closures_option_lc = MooseUtils::toLower(closures_option);

  if (_closures_class_names_map.find(flow_model_id) != _closures_class_names_map.end())
  {
    const auto & map_for_flow_model = _closures_class_names_map.at(flow_model_id);
    if (map_for_flow_model.find(closures_option_lc) != map_for_flow_model.end())
    {
      const std::string & closures_class = map_for_flow_model.at(closures_option_lc);
      mooseDeprecated("The closures system now uses objects created in the input file instead of "
                      "enumerated options.\n",
                      "To remove this warning, add the following block to your input file "
                      "(replacing 'my_closures' as you choose):\n",
                      "  [Closures]\n    [my_closures]\n      type = ",
                      closures_class,
                      "\n    []\n  []\n",
                      "Then, set the 'closures' parameter in your flow channel to this name:\n",
                      "  closures = my_closures");
      return closures_class;
    }
    else
      mooseError("The closures option '" + closures_option_lc + "' is not registered.");
  }
  else
    mooseError("The closures option '" + closures_option_lc + "' is not registered.");
}

void
MIFTAPApp::registerClosuresOption(const std::string & closures_option,
                                             const std::string & class_name,
                                             const THM::FlowModelID & flow_model_id)
{
  const std::string closures_option_lc = MooseUtils::toLower(closures_option);
  _closures_class_names_map[flow_model_id][closures_option_lc] = class_name;
}

//
// Dynamic Library Entry Points - DO NOT MODIFY
//
extern "C" void
MIFTAPApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  MIFTAPApp::registerAll(f, af, s);
}

extern "C" void
MIFTAPApp__registerApps()
{
  MIFTAPApp::registerApps();
}
