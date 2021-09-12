///
/// hardware model
///
#ifndef _HW_MODEL_
#define _HW_MODEL_

///
/// PCI Devices which can be used in mmio or pio mode
///

//#include "hw_model_r8169.h"
//#include "hw_model_bnxt.h"
//#include "hw_model_nvme.h"
//#include "hw_model_mlxsw_spectrum.h"
//#include "hw_model_mlx5.h"
//#include "hw_model_lpfc.h"
//#include "hw_model_mt76x0e.h"
//#include "hw_model_ath10kl.h"
//#include "hw_model_net2272.h"
//#include "hw_model_kpc2000.h"
//#include "hw_model_arcmsr.h"
//#include "hw_model_qxl.h"
//#include "hw_model_iwlwifi.h"
//#include "hw_model_crypto-safexcel.h"
//#include "hw_model_matrox.h"
//#include "hw_model_aty128fb.h"
//#include "hw_model_hfi1.h"
//#include "hw_model_rtw88_8822ce.h"
//#include "hw_model_megaraid_sas.h"
//#include "hw_model_hpsa.h"
//#include "hw_model_mei_me.h"
//#include "hw_model_mb862xxfb.h"
//#include "hw_model_ipmi_si.h"
//#include "hw_model_acp3x.h"
//#include "hw_model_snd_intel8x0.h"
//#include "hw_model_earth_pt1.h"
//#include "hw_model_cx18.h"
//#include "hw_model_firestream.h"
//#include "hw_model_nicstar.h"
//#include "hw_model_eni.h"
//#include "hw_model_fore_200e.h"
//#include "hw_model_wanxl.h"
//#include "hw_model_kvaser.h"
//#include "hw_model_plx_pci.h"
//#include "hw_model_ems_pci.h"
//#include "hw_model_lmc.h"
//#include "hw_model_mgag200.h"
//#include "hw_model_pm2fb.h"
//#include "hw_model_lanai.h"
//#include "hw_model_zatm.h"
//#include "hw_model_amd_xgbe.h"
//#include "hw_model_myrb.h"
//#include "hw_model_tsi721.h"
//#include "hw_model_ich9_ehci.h"
//#include "hw_model_3c59x.h"
//#include "hw_model_vxge.h"
//#include "hw_model_tlan.h"
//#include "hw_model_via-rhine.h"
//#include "hw_model_via-velocity.h"
//#include "hw_model_s2io.h"
//#include "hw_model_sky2.h"
//#include "hw_model_sfc.h"
//#include "hw_model_skge.h"
//#include "hw_model_amdgpu.h"

/// handle read with random value using the following model
#include "hw_model_catch_all.h"

#ifndef hw_model_r
#define hw_model_r
#endif

#ifndef hw_model_w
#define hw_model_w
#endif

#ifndef hw_model_dev_ram
#define hw_model_dev_ram
#endif

///
/// USB devices
///

//#include "hw_model_usb_mwifiex_usb.h"
//#include "hw_model_usb_dln2.h"
//#include "hw_model_usb_hso.h"
//#include "hw_model_usb_pl2303.h"
//#include "hw_model_usb_ds2490.h"
//#include "hw_model_usb_usb8xxx.h"
//#include "hw_model_usb_libertas_tf_usb.h"
//#include "hw_model_usb_zd1201.h"
//#include "hw_model_usb_zd1211rw.h"
//#include "hw_model_usb_carl9170.h"
//#include "hw_model_usb_p54usb.h"
//#include "hw_model_usb_asix.h"
//#include "hw_model_usb_dm9601.h"
//#include "hw_model_usb_atusb.h"
//#include "hw_model_usb_radio-keene.h"
//#include "hw_model_usb_radio-si470x-usb.h"
//#include "hw_model_usb_shark2.h"
//#include "hw_model_usb_radio-shark.h"
//#include "hw_model_usb_xbox_remote.h"
//#include "hw_model_usb_imon.h"
//#include "hw_model_usb_ati_remote.h"
//#include "hw_model_usb_mceusb.h"
//#include "hw_model_usb_visor.h"
//#include "hw_model_usb_whiteheat.h"
//#include "hw_model_usb_usblp.h"
//#include "hw_model_usb_usblcd.h"
//#include "hw_model_usb_chaoskey.h"
//#include "hw_model_usb_emi26.h"
//#include "hw_model_usb_hfcsusb.h"
#include "hw_model_usb_xpad.h"

#endif //_HW_MODEL_
