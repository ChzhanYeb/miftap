#pragma once

#include "ClosuresBase.h"

class PlateFlowChannel1Phase;

/**
 * Base class for 1-phase closures
 */
class PlateClosures1PhaseBase : public ClosuresBase
{
public:
  PlateClosures1PhaseBase(const InputParameters & params);

protected:
  /**
   * Adds material that computes wall friction factor from a specified function
   *
   * This function assumes that the flow channel component has the parameter
   * 'f' as a valid parameter, so this function should be guarded appropriately.
   *
   * @param[in] flow_channel   Flow channel component
   */
  void addWallFrictionFunctionMaterial(const PlateFlowChannel1Phase & flow_channel) const;

  /**
   * Adds average wall temperature material
   *
   * @param[in] flow_channel   Flow channel component
   */
  void addAverageWallTemperatureMaterial(const PlateFlowChannel1Phase & flow_channel) const;

public:
  static InputParameters validParams();
};
