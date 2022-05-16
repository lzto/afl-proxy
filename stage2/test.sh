#clang test.c -c -emit-llvm -o  test.bc -O0 -g

bc=~/linux/drivers/net/ethernet/dlink/sundance.o.bc
# bc=~/linux/drivers/misc/hpilo.o.bc
# bc=~/linux/drivers/net/ethernet/realtek/r8169.o.bc
# bc=~/linux/drivers/atm/fore200e.o.bc
bc=~/linux/drivers/atm/idt77252.o.bc
# bc=~/linux/drivers/atm/eni.o.bc
# bc=~/linux/drivers/firewire/nosy.o.bc

#opt \
#  -load ./build/src/libapstage2.so \
#  -passes='apstage2' \
#  -stats \
#  -disable-output \
#  <${bc} 2>&1

opt-13 \
  -load ./build/src/libapstage2.so \
  -apstage2 \
  -enable-new-pm=0 \
  -stats \
  -disable-output \
  <${bc} 2>&1

