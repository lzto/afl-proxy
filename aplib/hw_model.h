///
/// hardware model
///
#ifndef _HW_MODEL_
#define _HW_MODEL_

//#include "hw_model_r8169.h"
//#include "hw_model_bnxt.h"
//#include "hw_model_nvme.h"
//#include "hw_model_mlxsw_spectrum.h"
//#include "hw_model_lpfc.h"
//#include "hw_model_mt76x0e.h"
//#include "hw_model_ath10kl.h"
//#include "hw_model_net2272.h"
//#include "hw_model_kpc2000.h"
//#include "hw_model_arcmsr.h"
//#include "hw_model_qxl.h"
//#include "hw_model_iwlwifi.h"

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

#endif //_HW_MODEL_
