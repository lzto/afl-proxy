#!/bin/bash

echo "./run-sfp.sh SHMID console"
echo "./run-sfp.sh SHMID -- for non console"

SHMID=${1}
CONSOLE=${2}
export SFP_SHMID=${SHMID}
# SFP -- symbolic - fuzz phase
export AP_DISABLED=1
#0-disable, 1-r, 2-w, 3-rw
export AP_DUMP_RW=0
export USE_DMA=0
export USE_IRQ=0
export EXPORT_DEVMEM=1

#echo press ctrl-a x to quit
DIR=/home/tong/build-qemu-exp/x86_64-softmmu/
#DIR=/home/tong/build-qemu/x86_64-softmmu/
KERNEL="-kernel ./bzImage"
INITRD=""
#KERNEL="-kernel /boot/vmlinuz-5.11.0-34-generic"
#INITRD="-initrd /boot/initrd.img-5.11.0-34-generic"

ROFS="-snapshot"

ROOTFS="rootfs.ext4,format=raw ${ROFS}"
ROOTFS="stretch.img,format=raw ${ROFS}"

#dd if=/dev/urandom of=./sfp-rom.rom bs=1M count=1

KVM=-no-kvm
KVM=-enable-kvm

#echo ${KVM}

SFP=""
SFP="-device sfp,bus=pcie.1 "
#SFP="-usb -device usb-sfp"

#NIC="-net none"
#NIC="-net nic,model=ne2k_pci"
#NIC="-net user,host=10.0.2.10,hostfwd=tcp:127.0.0.1:10061-:22 -net nic,model=e1000"
#NIC="-netdev user,id=net0 -device e1000,netdev=net0,bus=pcie.1"
#NIC="-netdev user,id=net0 -device virtio-net-pci,netdev=net0,disable-legacy=on,disable-modern=off,iommu_platform=on,ats=on"

APPEND="nokaslr console=ttyS0 root=/dev/sda earlyprintk=serial biosdevname=0 net.ifnames=0 loglevel=8 security=none ro rootfstype=ext4 mitigations=off cryptomgr.notests clocksource=tsc audit=0 parport=0 kmemleak=on nosmp intel_iommu=on iommu=pt"

GDB=""
#GDB="-s"
GDBSTOP=""
#GDBSTOP="-S"

unset PCI_CLASS
unset SFP_USB_VID
unset SFP_USB_PID

#export SFP_DEV_MODEL="test"
#export SFP_DEV_MODEL="3c59x"
#export SFP_DEV_MODEL="3w_9xxx"
#export SFP_DEV_MODEL="acp3x"
#export SFP_DEV_MODEL="adm8211"
#export SFP_DEV_MODEL="adv_pci1760"
#export SFP_DEV_MODEL="aic7xxx"
#export SFP_DEV_MODEL="alcor_pci"
#export SFP_DEV_MODEL="ambassador"
#export SFP_DEV_MODEL="amdgpu"
#export SFP_DEV_MODEL="amd_xgbe"
#export SFP_DEV_MODEL="apex"
#export SFP_DEV_MODEL="applicom"
#export SFP_DEV_MODEL="arcmsr"
#export SFP_DEV_MODEL="arkfb"
#export SFP_DEV_MODEL="ast"
#export SFP_DEV_MODEL="ath10k"
#export SFP_DEV_MODEL="atl1"
#export SFP_DEV_MODEL="atl1c"
#export SFP_DEV_MODEL="atl1e"
#export SFP_DEV_MODEL="atl2"
#export SFP_DEV_MODEL="atlantic"
#export SFP_DEV_MODEL="aty128fb"
#export SFP_DEV_MODEL="atyfb"
#export SFP_DEV_MODEL="avmfritz"
#export SFP_DEV_MODEL="bcma"
#export SFP_DEV_MODEL="bnxt"
#export SFP_DEV_MODEL="bochs_drm"
#export SFP_DEV_MODEL="bt878"
#export SFP_DEV_MODEL="bttv"
#export SFP_DEV_MODEL="budget_av"
#export SFP_DEV_MODEL="cafe_ccic"
#export SFP_DEV_MODEL="cb710"
#export SFP_DEV_MODEL="c_can_pci"
#export SFP_DEV_MODEL="cirrus"
#export SFP_DEV_MODEL="cobalt"
#export SFP_DEV_MODEL="com20020_pci"
#export SFP_DEV_MODEL="cpcihp_zt5550"
#export SFP_DEV_MODEL="crypto_safexcel"
#export SFP_DEV_MODEL="cx18"
#export SFP_DEV_MODEL="cxgb4vf"
#export SFP_DEV_MODEL="cyber2000fb"
#export SFP_DEV_MODEL="dc395x"
#export SFP_DEV_MODEL="de2104x"
#export SFP_DEV_MODEL="denali_pci"
#export SFP_DEV_MODEL="dw_dmac_pci"
#export SFP_DEV_MODEL="earth_pt1"
#export SFP_DEV_MODEL="efa"
#export SFP_DEV_MODEL="ems_pci"
#export SFP_DEV_MODEL="emu10k1_gp"
#export SFP_DEV_MODEL="eni"
#export SFP_DEV_MODEL="epic100"
#export SFP_DEV_MODEL="f81601"
#export SFP_DEV_MODEL="farsync"
#export SFP_DEV_MODEL="fealnx"
#export SFP_DEV_MODEL="felix_vsc9959"
#export SFP_DEV_MODEL="firestream"
#export SFP_DEV_MODEL="fm801_gp"
#export SFP_DEV_MODEL="fore_200e"
#export SFP_DEV_MODEL="geode"
#export SFP_DEV_MODEL="gve"
#export SFP_DEV_MODEL="gxt4500"
#export SFP_DEV_MODEL="habanalabs"
#export SFP_DEV_MODEL="he"
#export SFP_DEV_MODEL="hfi1"
#export SFP_DEV_MODEL="hns3_enet"
#export SFP_DEV_MODEL="horizon"
#export SFP_DEV_MODEL="hpilo"
#export SFP_DEV_MODEL="hpsa"
#export SFP_DEV_MODEL="hypervfb"
#export SFP_DEV_MODEL="i710fb"
#export SFP_DEV_MODEL="i810"
#export SFP_DEV_MODEL="i82092aa"
#export SFP_DEV_MODEL="i915"
#export SFP_DEV_MODEL="ich9_ehci"
#export SFP_DEV_MODEL="idt77252"
#export SFP_DEV_MODEL="intelfb"
#export SFP_DEV_MODEL="intel_rng"
#export SFP_DEV_MODEL="intel_th_pci"
#export SFP_DEV_MODEL="ionic"
#export SFP_DEV_MODEL="iphase"
#export SFP_DEV_MODEL="ipmi_si"
#export SFP_DEV_MODEL="isci"
#export SFP_DEV_MODEL="isp1760"
#export SFP_DEV_MODEL="isst_if_pci"
#export SFP_DEV_MODEL="ivtv"
#export SFP_DEV_MODEL="iwlwifi"
#export SFP_DEV_MODEL="k10temp"
#export SFP_DEV_MODEL="kpc2000"
#export SFP_DEV_MODEL="kvaser"
#export SFP_DEV_MODEL="kyro"
#export SFP_DEV_MODEL="lanai"
#export SFP_DEV_MODEL="lmc"
#export SFP_DEV_MODEL="lpfc"
#export SFP_DEV_MODEL="macb_pci"
#export SFP_DEV_MODEL="mantis"
#export SFP_DEV_MODEL="matroxfb"
#export SFP_DEV_MODEL="mb862xxfb"
#export SFP_DEV_MODEL="megaraid_sas"
#export SFP_DEV_MODEL="mei_me"
#export SFP_DEV_MODEL="mgag200"
#export SFP_DEV_MODEL="mISDNinfineon"
#export SFP_DEV_MODEL="mlx5"
#export SFP_DEV_MODEL="mlxsw_spectrum"
#export SFP_DEV_MODEL="mt7615e"
#export SFP_DEV_MODEL="mt76x0"
#export SFP_DEV_MODEL="mt76x0e"
#export SFP_DEV_MODEL="myrb"
#export SFP_DEV_MODEL="ne2k"
#export SFP_DEV_MODEL="net2272"
#export SFP_DEV_MODEL="netjet"
#export SFP_DEV_MODEL="netup_unidvb"
#export SFP_DEV_MODEL="nicstar"
#export SFP_DEV_MODEL="nouveau"
export SFP_DEV_MODEL="nozomi"
#export SFP_DEV_MODEL="ntb_hw_amd"
#export SFP_DEV_MODEL="nvidiafb"
#export SFP_DEV_MODEL="nvme"
#export SFP_DEV_MODEL="pata_pdc202xx_old"
#export SFP_DEV_MODEL="pc300too"
#export SFP_DEV_MODEL="pci200syn"
#export SFP_DEV_MODEL="pcnet32"
#export SFP_DEV_MODEL="pcwd_pci"
#export SFP_DEV_MODEL="peak_pci"
#export SFP_DEV_MODEL="plx_pci"
#export SFP_DEV_MODEL="pm2fb"
#export SFP_DEV_MODEL="qla2xxx"
#export SFP_DEV_MODEL="qtnfmac_pcie"
#export SFP_DEV_MODEL="qxl"
#export SFP_DEV_MODEL="r8169"
#export SFP_DEV_MODEL="radeon_fb"
#export SFP_DEV_MODEL="radio_maxiradio"
#export SFP_DEV_MODEL="rivafb"
#export SFP_DEV_MODEL="rp2"
#export SFP_DEV_MODEL="rtsx_pci"
#export SFP_DEV_MODEL="rtw88_8723de"
#export SFP_DEV_MODEL="rtw88_8822be"
#export SFP_DEV_MODEL="rtw88_8822ce"
#export SFP_DEV_MODEL="s2io"
#export SFP_DEV_MODEL="s3fb"
#export SFP_DEV_MODEL="saa7134"
#export SFP_DEV_MODEL="sbni"
#export SFP_DEV_MODEL="sfc"
#export SFP_DEV_MODEL="sis190"
#export SFP_DEV_MODEL="sis5595"
#export SFP_DEV_MODEL="sisfb"
#export SFP_DEV_MODEL="skfp"
#export SFP_DEV_MODEL="skge"
#export SFP_DEV_MODEL="sky2"
#export SFP_DEV_MODEL="sm712fb"
#export SFP_DEV_MODEL="sm750fb"
#export SFP_DEV_MODEL="snd_ad1889"
#export SFP_DEV_MODEL="snd_atiixp"
#export SFP_DEV_MODEL="snd_bt87x"
#export SFP_DEV_MODEL="snd_fm801"
#export SFP_DEV_MODEL="snd_hda_intel"
#export SFP_DEV_MODEL="snd_intel8x0"
#export SFP_DEV_MODEL="snd_intel8x0m"
#export SFP_DEV_MODEL="snd_maestro3"
#export SFP_DEV_MODEL="snd_mia"
#export SFP_DEV_MODEL="snd_rme32"
#export SFP_DEV_MODEL="snd_rme96"
#export SFP_DEV_MODEL="snd_trident"
#export SFP_DEV_MODEL="snd_vx222"
#export SFP_DEV_MODEL="solo6x10"
#export SFP_DEV_MODEL="sstfb"
#export SFP_DEV_MODEL="sundance"
#export SFP_DEV_MODEL="tg3"
#export SFP_DEV_MODEL="tifm_7xx1"
#export SFP_DEV_MODEL="tlan"
#export SFP_DEV_MODEL="tsi721"
#export SFP_DEV_MODEL="ufshcd_pci"
#export SFP_DEV_MODEL="viafb"
#export SFP_DEV_MODEL="via_rhine"
#export SFP_DEV_MODEL="via_velocity"
#export SFP_DEV_MODEL="via_wdt"
#export SFP_DEV_MODEL="vme_ca91cx42"
#export SFP_DEV_MODEL="vme_tsi148"
#export SFP_DEV_MODEL="vmwgfx"
#export SFP_DEV_MODEL="vmw_pvrdma"
#export SFP_DEV_MODEL="vmxnet3"
#export SFP_DEV_MODEL="vt8623fb"
#export SFP_DEV_MODEL="vxge"
#export SFP_DEV_MODEL="w6692"
#export SFP_DEV_MODEL="wanxl"
#export SFP_DEV_MODEL="zatm"

#usb rtsx-usb
#export SFP_USB_VID=0xbda
#export SFP_USB_PID=0x129
#usb8xxx
#export SFP_USB_VID=0x5a3
#export SFP_USB_PID=0x8388

function launch_console() {
NIC="-net user,host=10.0.2.10,hostfwd=tcp:127.0.0.1:10061-:22 -net nic,model=e1000"
sudo -E taskset -c 0 ${DIR}qemu-system-x86_64 \
  -machine q35,accel=kvm,kernel-irqchip=split \
  -cpu host \
  -device intel-iommu,intremap=on,caching-mode=on,device-iotlb=on \
  -m 2G  -smp 2 \
  ${KVM} ${GDB} ${GDBSTOP} \
  -device ioh3420,id=pcie.1,chassis=1 \
  ${KERNEL} ${INITRD} -append "${APPEND}"\
  -drive file=${ROOTFS} \
  $NIC $SFP \
  -nographic \
  -pidfile vm-testing-${SHMID}.pid \
  2>&1 | tee -a vm-testing-${SHMID}.log
}

function no_console() {
sudo -E taskset -c 0 ${DIR}qemu-system-x86_64 \
  -machine q35,accel=kvm,kernel-irqchip=split \
  -device intel-iommu,intremap=on,caching-mode=on,device-iotlb=on \
  -m 2G  -smp 2 \
  ${KVM} ${GDB} ${GDBSTOP} \
  -device ioh3420,id=pcie.1,chassis=1 \
  ${KERNEL} ${INITRD} -append "${APPEND}"\
  -drive file=${ROOTFS} \
  $NIC $SFP \
  -nographic \
  -pidfile vm-testing-${SHMID}.pid \
  2>&1 > vm-testing-${SHMID}.log
#  2>&1 | tee -a vm-testing-${SHMID}.log
}

if [ "${CONSOLE}" == "" ]; then
  no_console
else
  launch_console
fi


#  -monitor telnet:127.0.0.1:1234,server,nowait\
#  -vga qxl \
#append "nosmp"

#  -append "nokaslr console=ttyS0 root=/dev/sda earlyprintk=serial biosdevname=0 net.ifnames=0 loglevel=8 security=none ro acpi=off rootfstype=ext4 mitigations=off cryptomgr.notests clocksource=tsc audit=0 parport=0 usbcore.nousb kmemleak=on nosmp"\



