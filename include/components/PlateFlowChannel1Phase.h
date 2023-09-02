#pragma once

#include "FlowChannelBase.h"

class ClosuresBase;

/**
 * A class representing a 1-phase plate flow channel
 *
 * A flow channel is defined by its position, direction, length and area.
 */
class PlateFlowChannel1Phase : public FlowChannelBase
{
public:
  PlateFlowChannel1Phase(const InputParameters & params);

  virtual void addMooseObjects() override;

  /**
   * Gets 1-phase wall heat transfer coefficient names for connected heat transfers
   */
  std::vector<MaterialPropertyName> getWallHTCNames1Phase() const { return _Hw_1phase_names; }

  virtual const THM::FlowModelID & getFlowModelID() const override { return THM::FM_SINGLE_PHASE; }

  /**
   * Gets the numerical flux user object name
   */
  const UserObjectName & getNumericalFluxUserObjectName() const { return _numerical_flux_name; }

  /**
   * Gets the slope reconstruction option used
   */
  const MooseEnum & getSlopeReconstruction() const { return _rdg_slope_reconstruction; }

protected:
  virtual void init() override;
  virtual std::shared_ptr<FlowModel> buildFlowModel() override;
  virtual void check() const override;

  /**
   * Adds a material for the hydraulic diameter
   */
  virtual void addHydraulicDiameterMaterial();

  /**
   * Populates heat connection variable names lists
   */
  virtual void getHeatTransferVariableNames() override;

  /// 1-phase wall heat transfer coefficient names for connected heat transfers
  std::vector<MaterialPropertyName> _Hw_1phase_names;

  /// Numerical flux user object name
  const UserObjectName _numerical_flux_name;

  /// Slope reconstruction type for rDG
  const MooseEnum _rdg_slope_reconstruction;

public:
  static InputParameters validParams();
};
