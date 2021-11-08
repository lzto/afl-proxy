///
/// hw model
/// 2021 Tong Zhang<ztong0001@gmail.com>
///
#include "hw_model.h"

static HWModel *instance;

///
/// create new hardware instance
///
HWModel *init_hw_instance(const std::string &name) {
  if (instance)
    return instance;
  if (name == "3c59x")
    instance = new HWModel_3c59x();
  else if (name == "acp3x")
    instance = new HWModel_acp3x();
  else if (name == "adv_pci1760")
    instance = new HWModel_adv_pci1760();
  else if (name == "aic7xxx")
    instance = new HWModel_aic7xxx();
  else if (name == "zatm")
    instance = new HWModel_zatm();
  else if (name == "wanxl")
    instance = new HWModel_wanxl();
  else if (name == "w6692")
    instance = new HWModel_w6692();
  else if (name == "applicom")
    instance = new HWModel_applicom();
  else if (name == "vxge")
    instance = new HWModel_vxge();
  else if (name == "vmxnet3")
    instance = new HWModel_vmxnet3();
  else if (name == "vmw_pvrdma")
    instance = new HWModel_vmw_pvrdma();
  else if (name == "vme_tsi148")
    instance = new HWModel_vme_tsi148();
  else
    assert(0 && "unknown hw");
  return instance;
}

HWModel *init_hw_instance(const char *name) {
  return init_hw_instance(std::string(name));
}

HWModel *get_hw_instance() { return instance; }
