///
/// hardware model
///
#ifndef _HW_MODEL_
#define _HW_MODEL_

#include "HWModel.h"
#include "HWModel_3c59x.h"

HWModel *init_hw_instance(const char *name);
HWModel *init_hw_instance(const std::string &);
HWModel *get_hw_instance();

#endif //_HW_MODEL_
