///
/// hardware model
///
#ifndef _HW_MODEL_
#define _HW_MODEL_

#include "HWModel.h"
void hw_model_internal_init();
HWModel *init_hw_instance(const char *name);
HWModel *init_hw_instance(const std::string &);
HWModel *get_hw_instance();
void hw_instance_export_devmem(const std::string &prefix);

#endif //_HW_MODEL_
