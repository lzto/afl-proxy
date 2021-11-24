///
/// hw model
/// 2021 Tong Zhang<ztong0001@gmail.com>
///
#include "hw_model.h"

#include "HWModel_3c59x.h"
#include "HWModel_3w_9xxx.h"
#include "HWModel_acp3x.h"
#include "HWModel_adm8211.h"
#include "HWModel_adv_pci1760.h"
#include "HWModel_aic7xxx.h"
#include "HWModel_alcor_pci.h"
#include "HWModel_ambassador.h"
#include "HWModel_amd_xgbe.h"
#include "HWModel_amdgpu.h"
#include "HWModel_apex.h"
#include "HWModel_applicom.h"
#include "HWModel_arcmsr.h"
#include "HWModel_arkfb.h"
#include "HWModel_ast.h"
#include "HWModel_ath10k.h"
#include "HWModel_atl1.h"
#include "HWModel_atl1c.h"
#include "HWModel_atl1e.h"
#include "HWModel_atl2.h"
#include "HWModel_atlantic.h"
#include "HWModel_aty128fb.h"
#include "HWModel_atyfb.h"
#include "HWModel_avmfritz.h"
#include "HWModel_bcma.h"
#include "HWModel_bnxt.h"
#include "HWModel_bochs_drm.h"
#include "HWModel_bt878.h"
#include "HWModel_bttv.h"
#include "HWModel_budget_av.h"
#include "HWModel_c_can_pci.h"
#include "HWModel_cafe_ccic.h"
#include "HWModel_cb710.h"
#include "HWModel_cirrus.h"
#include "HWModel_cobalt.h"
#include "HWModel_com20020_pci.h"
#include "HWModel_cpcihp_zt5550.h"
#include "HWModel_crypto_safexcel.h"
#include "HWModel_cx18.h"
#include "HWModel_cxgb4vf.h"
#include "HWModel_cyber2000fb.h"
#include "HWModel_dc395x.h"
#include "HWModel_de2104x.h"
#include "HWModel_denali_pci.h"
#include "HWModel_dw_dmac_pci.h"
#include "HWModel_earth_pt1.h"
#include "HWModel_efa.h"
#include "HWModel_ems_pci.h"
#include "HWModel_emu10k1_gp.h"
#include "HWModel_eni.h"
#include "HWModel_epic100.h"
#include "HWModel_f81601.h"
#include "HWModel_farsync.h"
#include "HWModel_fealnx.h"
#include "HWModel_felix_vsc9959.h"
#include "HWModel_firestream.h"
#include "HWModel_fm801_gp.h"
#include "HWModel_fore_200e.h"
#include "HWModel_geode.h"
#include "HWModel_gve.h"
#include "HWModel_gxt4500.h"
#include "HWModel_habanalabs.h"
#include "HWModel_he.h"
#include "HWModel_hfi1.h"
#include "HWModel_hns3_enet.h"
#include "HWModel_horizon.h"
#include "HWModel_hpilo.h"
#include "HWModel_hpsa.h"
#include "HWModel_hypervfb.h"
#include "HWModel_i710fb.h"
#include "HWModel_i810.h"
#include "HWModel_i82092aa.h"
#include "HWModel_i915.h"
#include "HWModel_ich9_ehci.h"
#include "HWModel_idt77252.h"
#include "HWModel_intel_rng.h"
#include "HWModel_intel_th_pci.h"
#include "HWModel_intelfb.h"
#include "HWModel_ionic.h"
#include "HWModel_iphase.h"
#include "HWModel_ipmi_si.h"
#include "HWModel_isci.h"
#include "HWModel_isp1760.h"
#include "HWModel_isst_if_pci.h"
#include "HWModel_ivtv.h"
#include "HWModel_iwlwifi.h"
#include "HWModel_k10temp.h"
#include "HWModel_kpc2000.h"
#include "HWModel_kvaser.h"
#include "HWModel_kyro.h"
#include "HWModel_lanai.h"
#include "HWModel_lmc.h"
#include "HWModel_lpfc.h"
#include "HWModel_mISDNinfineon.h"
#include "HWModel_macb_pci.h"
#include "HWModel_mantis.h"
#include "HWModel_matroxfb.h"
#include "HWModel_mb862xxfb.h"
#include "HWModel_megaraid_sas.h"
#include "HWModel_mei_me.h"
#include "HWModel_mgag200.h"
#include "HWModel_mlx5.h"
#include "HWModel_mlxsw_spectrum.h"
#include "HWModel_mt7615e.h"
#include "HWModel_mt76x0.h"
#include "HWModel_mt76x0e.h"
#include "HWModel_myrb.h"
#include "HWModel_ne2k.h"
#include "HWModel_net2272.h"
#include "HWModel_netjet.h"
#include "HWModel_netup_unidvb.h"
#include "HWModel_nicstar.h"
#include "HWModel_nouveau.h"
#include "HWModel_nozomi.h"
#include "HWModel_ntb_hw_amd.h"
#include "HWModel_nvidiafb.h"
#include "HWModel_nvme.h"
#include "HWModel_pata_pdc202xx_old.h"
#include "HWModel_pc300too.h"
#include "HWModel_pci200syn.h"
#include "HWModel_pcnet32.h"
#include "HWModel_pcwd_pci.h"
#include "HWModel_peak_pci.h"
#include "HWModel_plx_pci.h"
#include "HWModel_pm2fb.h"
#include "HWModel_qla2xxx.h"
#include "HWModel_qtnfmac_pcie.h"
#include "HWModel_qxl.h"
#include "HWModel_r8169.h"
#include "HWModel_radeon_fb.h"
#include "HWModel_radio_maxiradio.h"
#include "HWModel_rivafb.h"
#include "HWModel_rp2.h"
#include "HWModel_rtsx_pci.h"
#include "HWModel_rtw88_8723de.h"
#include "HWModel_rtw88_8822be.h"
#include "HWModel_rtw88_8822ce.h"
#include "HWModel_s2io.h"
#include "HWModel_s3fb.h"
#include "HWModel_saa7134.h"
#include "HWModel_sbni.h"
#include "HWModel_sfc.h"
#include "HWModel_sis190.h"
#include "HWModel_sis5595.h"
#include "HWModel_sisfb.h"
#include "HWModel_skfp.h"
#include "HWModel_skge.h"
#include "HWModel_sky2.h"
#include "HWModel_sm712fb.h"
#include "HWModel_sm750fb.h"
#include "HWModel_snd_ad1889.h"
#include "HWModel_snd_atiixp.h"
#include "HWModel_snd_bt87x.h"
#include "HWModel_snd_fm801.h"
#include "HWModel_snd_hda_intel.h"
#include "HWModel_snd_intel8x0.h"
#include "HWModel_snd_intel8x0m.h"
#include "HWModel_snd_maestro3.h"
#include "HWModel_snd_mia.h"
#include "HWModel_snd_rme32.h"
#include "HWModel_snd_rme96.h"
#include "HWModel_snd_trident.h"
#include "HWModel_snd_vx222.h"
#include "HWModel_solo6x10.h"
#include "HWModel_sstfb.h"
#include "HWModel_sundance.h"
#include "HWModel_test.h"
#include "HWModel_tg3.h"
#include "HWModel_tifm_7xx1.h"
#include "HWModel_tlan.h"
#include "HWModel_tsi721.h"
#include "HWModel_ufshcd_pci.h"
#include "HWModel_via_rhine.h"
#include "HWModel_via_velocity.h"
#include "HWModel_via_wdt.h"
#include "HWModel_viafb.h"
#include "HWModel_vme_ca91cx42.h"
#include "HWModel_vme_tsi148.h"
#include "HWModel_vmw_pvrdma.h"
#include "HWModel_vmwgfx.h"
#include "HWModel_vmxnet3.h"
#include "HWModel_vt8623fb.h"
#include "HWModel_vxge.h"
#include "HWModel_w6692.h"
#include "HWModel_wanxl.h"
#include "HWModel_zatm.h"

#include <functional>
#include <map>
using namespace std;

static map<string, function<HWModel *()>> modelMap;

static HWModel *instance;

#define INSERT_MODEL(NAME)                                                     \
  modelMap[#NAME] = []() { return new HWModel_##NAME(); };

void hw_model_internal_init() {
  INSERT_MODEL(qla2xxx);
  INSERT_MODEL(mt76x0);
  INSERT_MODEL(mt7615e);
  INSERT_MODEL(habanalabs);
  INSERT_MODEL(k10temp);
  INSERT_MODEL(rtw88_8723de);
  INSERT_MODEL(rtw88_8822be);
  INSERT_MODEL(ntb_hw_amd);
  INSERT_MODEL(qtnfmac_pcie);
  INSERT_MODEL(efa);
  INSERT_MODEL(sis5595);
  INSERT_MODEL(snd_rme96);
  INSERT_MODEL(snd_rme32);
  INSERT_MODEL(snd_bt87x);
  INSERT_MODEL(snd_maestro3);
  INSERT_MODEL(intel_th_pci);
  INSERT_MODEL(intel_rng);
  INSERT_MODEL(snd_vx222);
  INSERT_MODEL(snd_mia);
  INSERT_MODEL(snd_atiixp)
  INSERT_MODEL(hns3_enet);
  INSERT_MODEL(apex);
  INSERT_MODEL(radio_maxiradio);
  INSERT_MODEL(via_wdt);
  INSERT_MODEL(felix_vsc9959);
  INSERT_MODEL(geode);
  INSERT_MODEL(snd_fm801);
  INSERT_MODEL(tifm_7xx1);
  INSERT_MODEL(cb710);
  INSERT_MODEL(ne2k);
  INSERT_MODEL(kyro);
  INSERT_MODEL(gxt4500);
  INSERT_MODEL(he);
  INSERT_MODEL(horizon);
  INSERT_MODEL(intelfb);
  INSERT_MODEL(ambassador);
  INSERT_MODEL(idt77252);
  INSERT_MODEL(saa7134);
  INSERT_MODEL(cirrus);
  INSERT_MODEL(peak_pci);
  INSERT_MODEL(rp2);
  INSERT_MODEL(denali_pci);
  INSERT_MODEL(dc395x);
  INSERT_MODEL(netup_unidvb);
  INSERT_MODEL(cxgb4vf);
  INSERT_MODEL(cpcihp_zt5550);
  INSERT_MODEL(adm8211);
  INSERT_MODEL(3c59x);
  INSERT_MODEL(3w_9xxx);
  INSERT_MODEL(acp3x);
  INSERT_MODEL(adv_pci1760);
  INSERT_MODEL(aic7xxx);
  INSERT_MODEL(alcor_pci);
  INSERT_MODEL(amdgpu);
  INSERT_MODEL(amd_xgbe);
  INSERT_MODEL(applicom);
  INSERT_MODEL(arcmsr);
  INSERT_MODEL(arkfb);
  INSERT_MODEL(ast);
  INSERT_MODEL(ath10k);
  INSERT_MODEL(atl1);
  INSERT_MODEL(atl1c);
  INSERT_MODEL(atl1e);
  INSERT_MODEL(atl2);
  INSERT_MODEL(atlantic);
  INSERT_MODEL(aty128fb);
  INSERT_MODEL(atyfb);
  INSERT_MODEL(avmfritz);
  INSERT_MODEL(bcma);
  INSERT_MODEL(bnxt);
  INSERT_MODEL(bochs_drm);
  INSERT_MODEL(bt878);
  INSERT_MODEL(bttv);
  INSERT_MODEL(budget_av);
  INSERT_MODEL(cafe_ccic);
  INSERT_MODEL(c_can_pci);
  INSERT_MODEL(cobalt);
  INSERT_MODEL(com20020_pci);
  INSERT_MODEL(crypto_safexcel);
  INSERT_MODEL(cx18);
  INSERT_MODEL(cyber2000fb);
  INSERT_MODEL(de2104x);
  INSERT_MODEL(dw_dmac_pci);
  INSERT_MODEL(earth_pt1);
  INSERT_MODEL(ems_pci);
  INSERT_MODEL(emu10k1_gp);
  INSERT_MODEL(eni);
  INSERT_MODEL(epic100);
  INSERT_MODEL(f81601);
  INSERT_MODEL(farsync);
  INSERT_MODEL(fealnx);
  INSERT_MODEL(firestream);
  INSERT_MODEL(fm801_gp);
  INSERT_MODEL(fore_200e);
  INSERT_MODEL(gve);
  INSERT_MODEL(hfi1);
  INSERT_MODEL(hpilo);
  INSERT_MODEL(hpsa);
  INSERT_MODEL(hypervfb);
  INSERT_MODEL(i710fb);
  INSERT_MODEL(i810);
  INSERT_MODEL(i82092aa);
  INSERT_MODEL(i915);
  INSERT_MODEL(ich9_ehci);
  INSERT_MODEL(ionic);
  INSERT_MODEL(iphase);
  INSERT_MODEL(ipmi_si);
  INSERT_MODEL(isci);
  INSERT_MODEL(isp1760);
  INSERT_MODEL(isst_if_pci);
  INSERT_MODEL(ivtv);
  INSERT_MODEL(iwlwifi);
  INSERT_MODEL(kpc2000);
  INSERT_MODEL(kvaser);
  INSERT_MODEL(lanai);
  INSERT_MODEL(lmc);
  INSERT_MODEL(lpfc);
  INSERT_MODEL(macb_pci);
  INSERT_MODEL(mantis);
  INSERT_MODEL(matroxfb);
  INSERT_MODEL(mb862xxfb);
  INSERT_MODEL(megaraid_sas);
  INSERT_MODEL(mei_me);
  INSERT_MODEL(mgag200);
  INSERT_MODEL(mISDNinfineon);
  INSERT_MODEL(mlx5);
  INSERT_MODEL(mlxsw_spectrum);
  INSERT_MODEL(mt76x0e);
  INSERT_MODEL(myrb);
  INSERT_MODEL(net2272);
  INSERT_MODEL(netjet);
  INSERT_MODEL(nicstar);
  INSERT_MODEL(nouveau);
  INSERT_MODEL(nozomi);
  INSERT_MODEL(nvidiafb);
  INSERT_MODEL(nvme);
  INSERT_MODEL(pata_pdc202xx_old);
  INSERT_MODEL(pc300too);
  INSERT_MODEL(pci200syn);
  INSERT_MODEL(pcnet32);
  INSERT_MODEL(pcwd_pci);
  INSERT_MODEL(plx_pci);
  INSERT_MODEL(pm2fb);
  INSERT_MODEL(qxl);
  INSERT_MODEL(r8169);
  INSERT_MODEL(radeon_fb);
  INSERT_MODEL(rivafb);
  INSERT_MODEL(rtsx_pci);
  INSERT_MODEL(rtw88_8822ce);
  INSERT_MODEL(s2io);
  INSERT_MODEL(s3fb);
  INSERT_MODEL(sbni);
  INSERT_MODEL(sfc);
  INSERT_MODEL(sis190);
  INSERT_MODEL(sisfb);
  INSERT_MODEL(skfp);
  INSERT_MODEL(skge);
  INSERT_MODEL(sky2);
  INSERT_MODEL(sm712fb);
  INSERT_MODEL(sm750fb);
  INSERT_MODEL(snd_ad1889);
  INSERT_MODEL(snd_hda_intel);
  INSERT_MODEL(snd_intel8x0);
  INSERT_MODEL(snd_intel8x0m);
  INSERT_MODEL(snd_trident);
  INSERT_MODEL(solo6x10);
  INSERT_MODEL(sstfb);
  INSERT_MODEL(sundance);
  INSERT_MODEL(tg3);
  INSERT_MODEL(tlan);
  INSERT_MODEL(tsi721);
  INSERT_MODEL(ufshcd_pci);
  INSERT_MODEL(viafb);
  INSERT_MODEL(via_rhine);
  INSERT_MODEL(via_velocity);
  INSERT_MODEL(vme_ca91cx42);
  INSERT_MODEL(vme_tsi148);
  INSERT_MODEL(vmwgfx);
  INSERT_MODEL(vmw_pvrdma);
  INSERT_MODEL(vmxnet3);
  INSERT_MODEL(vt8623fb);
  INSERT_MODEL(vxge);
  INSERT_MODEL(w6692);
  INSERT_MODEL(wanxl);
  INSERT_MODEL(zatm);
  INSERT_MODEL(test);
}

///
/// create new hardware instance
///
HWModel *init_hw_instance(const std::string &name) {
  if (instance)
    return instance;
  auto it = modelMap.find(name);
  assert((it != modelMap.end()) && "unknown hardware");
  instance = it->second();
  return instance;
}

HWModel *init_hw_instance(const char *name) {
  return init_hw_instance(std::string(name));
}

HWModel *get_hw_instance() { return instance; }
