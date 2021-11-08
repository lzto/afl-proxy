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
  else if (name == "vme_ca91cx42")
    instance = new HWModel_vme_ca91cx42();
  else if (name == "via_velocity")
    instance = new HWModel_via_velocity();
  else if (name == "via_rhine")
    instance = new HWModel_via_rhine();
  else if (name == "ufshcd_pci")
    instance = new HWModel_ufshcd_pci();
  else if (name == "tsi721")
    instance = new HWModel_tsi721();
  else if (name == "tlan")
    instance = new HWModel_tlan();
  else if (name == "sundance")
    instance = new HWModel_sundance();
  else if (name == "solo6x10")
    instance = new HWModel_solo6x10();
  else if (name == "snd_trident")
    instance = new HWModel_snd_trident();
  else if (name == "snd_intel8x0m")
    instance = new HWModel_snd_intel8x0m();
  else if (name == "snd_intel8x0")
    instance = new HWModel_snd_intel8x0();
  else if (name == "snd_hda_intel")
    instance = new HWModel_snd_hda_intel();
  else if (name == "snd_ad1889")
    instance = new HWModel_snd_ad1889();
  else if (name == "sky2")
    instance = new HWModel_sky2();
  else if (name == "skge")
    instance = new HWModel_skge();
  else if (name == "sis190")
    instance = new HWModel_sis190();
  else if (name == "sfc")
    instance = new HWModel_sfc();
  else if (name == "s2io")
    instance = new HWModel_s2io();
  else if (name == "rtw88_8822ce")
    instance = new HWModel_rtw88_8822ce();
  else if (name == "rtsx_pci")
    instance = new HWModel_rtsx_pci();
  else if (name == "r8169")
    instance = new HWModel_r8169();
  else if (name == "pm2fb")
    instance = new HWModel_pm2fb();
  else if (name == "plx_pci")
    instance = new HWModel_plx_pci();
  else if (name == "pcwd_pci")
    instance = new HWModel_pcwd_pci();
  else if (name == "pcnet32")
    instance = new HWModel_pcnet32();
  else if (name == "pci200syn")
    instance = new HWModel_pci200syn();
  else if (name == "pc300too")
    instance = new HWModel_pc300too();
  else if (name == "pata_pdc202xx_old")
    instance = new HWModel_pata_pdc202xx_old();
  else if (name == "nicstar")
    instance = new HWModel_nicstar();
  else if (name == "netjet")
    instance = new HWModel_netjet();
  else if (name == "net2272")
    instance = new HWModel_net2272();
  else if (name == "myrb")
    instance = new HWModel_myrb();
  else if (name == "mt76x0e")
    instance = new HWModel_mt76x0e();
  else if (name == "amdgpu")
    instance = new HWModel_amdgpu();
  else if (name == "amd_xgbe")
    instance = new HWModel_amd_xgbe();
  else if (name == "arcmsr")
    instance = new HWModel_arcmsr();
  else if (name == "mlxsw_spectrum")
    instance = new HWModel_mlxsw_spectrum();
  else if (name == "mlx5")
    instance = new HWModel_mlx5();
  else if (name == "mISDNinfineon")
    instance = new HWModel_mISDNinfineon();
  else if (name == "mgag200")
    instance = new HWModel_mgag200();

  else
    assert(0 && "unknown hw");
  return instance;
}

HWModel *init_hw_instance(const char *name) {
  return init_hw_instance(std::string(name));
}

HWModel *get_hw_instance() { return instance; }
