#include "MIFTAPApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"
#include "MooseSyntax.h"

InputParameters
MIFTAPApp::validParams()
{
  InputParameters params = MooseApp::validParams();
  params.set<bool>("use_legacy_material_output") = false;
  return params;
}

MIFTAPApp::MIFTAPApp(InputParameters parameters) : MooseApp(parameters)
{
  MIFTAPApp::registerAll(_factory, _action_factory, _syntax);
}

MIFTAPApp::~MIFTAPApp() {}

void 
MIFTAPApp::registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  ModulesApp::registerAllObjects<MIFTAPApp>(f, af, s);
  Registry::registerObjectsTo(f, {"MIFTAPApp"});
  Registry::registerActionsTo(af, {"MIFTAPApp"});

  /* register custom execute flags, action syntax, etc. here */
}

void
MIFTAPApp::registerApps()
{
  registerApp(MIFTAPApp);
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
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
