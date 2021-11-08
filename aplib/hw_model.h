///
/// hardware model
///
#ifndef _HW_MODEL_
#define _HW_MODEL_

#include "HWModel.h"
#include "HWModel_3c59x.h"
#include "HWModel_acp3x.h"
#include "HWModel_adv_pci1760.h"
#include "HWModel_aic7xxx.h"
#include "HWModel_wanxl.h"
#include "HWModel_zatm.h"

HWModel *init_hw_instance(const char *name);
HWModel *init_hw_instance(const std::string &);
HWModel *get_hw_instance();

#endif //_HW_MODEL_
