///
/// 2021 Tong Zhang<ztong0001@gmail.com>
/// hardware model generated from s2e output
/// specialized for USB devices
///
#ifndef _HW_MODEL_USB_
#define _HW_MODEL_USB_

#define HW_MODEL_USB_DESC hw_model_usb_gen_desc_dln2();

static void *hw_model_usb_gen_desc_dln2() {
  static void *desc;
  if (!desc)
    desc = malloc(4096);
  return desc;
}
#endif //_HW_MODEL_USB_
