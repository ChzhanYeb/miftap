//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html
#include "MIFTAPTestApp.h"
#include "MIFTAPApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "MooseSyntax.h"

InputParameters
MIFTAPTestApp::validParams()
{
  InputParameters params = MIFTAPApp::validParams();
  params.set<bool>("use_legacy_material_output") = false;
  return params;
}

MIFTAPTestApp::MIFTAPTestApp(InputParameters parameters) : MooseApp(parameters)
{
  MIFTAPTestApp::registerAll(
      _factory, _action_factory, _syntax, getParam<bool>("allow_test_objects"));
}

MIFTAPTestApp::~MIFTAPTestApp() {}

void
MIFTAPTestApp::registerAll(Factory & f, ActionFactory & af, Syntax & s, bool use_test_objs)
{
  MIFTAPApp::registerAll(f, af, s);
  if (use_test_objs)
  {
    Registry::registerObjectsTo(f, {"MIFTAPTestApp"});
    Registry::registerActionsTo(af, {"MIFTAPTestApp"});
  }
}

void
MIFTAPTestApp::registerApps()
{
  registerApp(MIFTAPApp);
  registerApp(MIFTAPTestApp);
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
// External entry point for dynamic application loading
extern "C" void
MIFTAPTestApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  MIFTAPTestApp::registerAll(f, af, s);
}
extern "C" void
MIFTAPTestApp__registerApps()
{
  MIFTAPTestApp::registerApps();
}
