/*
 * AP Stage2
 * 2022 Tong Zhang<ztong0001@gmail.com>
 * Note: Compile kernel module with -O0 to preserve all information needed
 * by APStage2 Analysis
 */

#include "apstage2.h"

// my aux headers
#include "color.h"
#include "stopwatch.h"

using namespace llvm;

char APStage2::ID;

bool APStage2::runOnModule(Module &module) {
  // AA = &getAnalysis<AAResultsWrapperPass>().getAAResults();
  return APStage2Pass(module);
}

bool APStage2::isLLVMPrefix(Value *v) {
  Value *nv = v->stripPointerCasts();
  if (auto *cnv = dyn_cast<ConstantExpr>(nv)) {
    nv = cnv->getAsInstruction();
  }
  if (auto *gep = dyn_cast<GetElementPtrInst>(nv)) {
    nv = gep->getPointerOperand();
  }
  if (nv->getName().startswith("__llvm"))
    return true;
  if (nv->getName().startswith(".sink"))
    return true;
  // errs()<<" ===>"<<*nv<<"\n";
  return false;
}

bool APStage2::isDMAType(Value *v) {
  v = v->stripPointerCasts();
  if (auto *trunc = dyn_cast<TruncInst>(v)) {
    v = trunc->getOperand(0);
  }
  if (auto *ld = dyn_cast<LoadInst>(v)) {
    v = ld->getOperand(0)->stripPointerCasts();
  }
  errs() << "isDMAType?" << *v << " - " << *(v->getType()) << "\n";
  if (auto *gep = dyn_cast<GetElementPtrInst>(v)) {
    errs() << " sink - " << *gep << "\n";
  }
  // if (Instruction* i = dyn_cast<Instruction>(v)) {
  // }
  return true;
}

void APStage2::processStore(StoreInst *st) {
  // skip gcov
  auto *p = st->getPointerOperand();
  if (isLLVMPrefix(p))
    return;
  auto *v = st->getValueOperand();
  // store constant is not what we are interested in
  if (isa<Constant>(v))
    return;
  // MMIO use store instruction instead of out
  mmioSt.push_back(st);
  // errs() << *st<<"\n";
  // if (auto *gep = dyn_cast<GetElementPtrInst>(p)) {
  //   errs() << " source + " << *st << "\n";
  // }
}

void APStage2::processCall(CallInst *ci) {
  if (ci->isInlineAsm())
    return;
#if (LLVM_VERSION_MAJOR <= 10)
  Value *cv = ci->getCalledValue();
#else
  Value *cv = ci->getCalledOperand();
#endif
  Function *bcf = dyn_cast<Function>(cv->stripPointerCasts());
  // indirect call?
  if (!bcf)
    return;
  auto name = bcf->getName();
  if (name.startswith("__asan_") || (name.startswith("llvm."))) {
    return;
  }
  // all CallInst
  callInsts.push_back(ci);
  if (name.startswith("dma_alloc_attrs")) {
    callDmaAlloc.push_back(ci);
  }
  if (name.startswith("dma_alloc_coherent")) {
    callDmaAlloc.push_back(ci);
  }

  if (name.startswith("dma_map_single")) {
    // errs() << *ci << "\n";
  }
  if (name.startswith("dma_sync_single")) {
    // errs() << *ci << "\n";
  }
  // could write DMA address to device register ?
  if (name.startswith("iowrite32") || name.startswith("writel")) {
    callIoWrite.push_back(ci);
  }
  // map device pio/mmio region
  if (name.startswith("pci_iomap") || name.startswith("ioremap")) {
    callIoMap.push_back(ci);
  }
  // errs()<<*ci<<"\n";
}

void APStage2::collectLoadFromAddress(Value *v, std::vector<Value *> &lds) {
  for (auto *u : v->users()) {
    // got load
    if (isa<LoadInst>(u))
      lds.push_back(u);
    // cast is OK
    if (isa<CastInst>(u))
      collectLoadFromAddress(u, lds);
  }
}

void APStage2::collectStoreOfValue(Value *v, std::vector<Value *> &sts) {
  for (auto *u : v->users()) {
    // got store
    if (StoreInst *st = dyn_cast<StoreInst>(u)) {
      if (st->getValueOperand() == v) {
        sts.push_back(st);
      } else {
        // what else?
      }
    }
    // cast is OK
    if (isa<CastInst>(u))
      collectStoreOfValue(u, sts);
  }
}

void APStage2::collectLoadStoreStruct(Value *v, std::set<Value *> &r) {
  std::vector<Value *> users;
  // contains dma buffer address
  std::vector<Value *> lds, sts;
  collectLoadFromAddress(v, lds);
  for (auto *ld : lds)
    collectStoreOfValue(ld, sts);
  //
  for (auto *ist : sts) {
    StoreInst *st = dyn_cast<StoreInst>(ist);
    // errs()<<" -- "<<*st<<"\n";
    r.insert(st->getPointerOperand());
  }
}

bool APStage2::isLoadedFromAddress(Value *v, std::set<Value *> &addrSet) {
  v = v->stripPointerCasts();
  if (auto *trunc = dyn_cast<TruncInst>(v)) {
    v = trunc->getOperand(0);
  }
  if (auto *zext = dyn_cast<ZExtInst>(v)) {
    v = zext->getOperand(0);
  }
  if (auto *gep = dyn_cast<GetElementPtrInst>(v)) {
    v = gep->getPointerOperand();
  }
  // errs()<<__func__<<" ? "<<*v<<"\n";
  if (auto *x = dyn_cast<TruncInst>(v))
    v = x->getOperand(0);
  if (Instruction *i = dyn_cast<Instruction>(v)) {
    if (i->getOpcode() == Instruction::Add) {
      // errs()<<" Got an add "<<*i<<"\n";
      if (isLoadedFromSameStructField(i->getOperand(0), addrSet) ||
          isLoadedFromSameStructField(i->getOperand(1), addrSet))
        return true;
      return false;
    } else if (i->isShift()) {
      if (isLoadedFromSameStructField(i->getOperand(0), addrSet) ||
          isLoadedFromSameStructField(i->getOperand(1), addrSet))
        return true;
      return false;
    }
  }
  LoadInst *ld = dyn_cast<LoadInst>(v->stripPointerCasts());
  if (!ld)
    return false;
  Value *addr = ld->getPointerOperand()->stripPointerCasts();
  for (auto *a : addrSet)
    if (addr == a)
      return true;
  // errs()<<__func__<<":"<<__LINE__<<" - addr:"<<*addr<<"\n";
  if (isGEPFromSameStructField(addr, addrSet)) {
    return true;
  }
  return false;
}

bool APStage2::isGEPFromSameStructField(Value *addr,
                                        std::set<Value *> &addrSet) {
  // must be a GEP
  GetElementPtrInst *gep = dyn_cast<GetElementPtrInst>(addr);
  if (!gep)
    return false;
  for (auto *a : addrSet) {
    Value *actual = a->stripPointerCasts();
    GetElementPtrInst *actualGEP = dyn_cast<GetElementPtrInst>(actual);
    if (!actualGEP)
      continue;
    // OK now we can compare them
    if (actualGEP->getSourceElementType() != gep->getSourceElementType())
      continue;
    if (actualGEP->getNumIndices() != gep->getNumIndices())
      continue;
    std::vector<int> A, B;
    get_gep_indicies(actualGEP, A);
    get_gep_indicies(gep, B);
    if (A == B)
      return true;
  }
  return false;
}

bool APStage2::isLoadedFromSameStructField(Value *v,
                                           std::set<Value *> &addrSet) {
  if (auto *x = dyn_cast<TruncInst>(v))
    v = x->getOperand(0);
  LoadInst *ld = dyn_cast<LoadInst>(v->stripPointerCasts());
  if (!ld)
    return false;
  Value *addr = ld->getPointerOperand()->stripPointerCasts();
  if (isGEPFromSameStructField(addr, addrSet))
    return true;
  return false;
}

bool APStage2::APStage2Pass(Module &module) {
  errs() << ANSI_COLOR_CYAN << "--- PROCESS FUNCTIONS ---" << ANSI_COLOR_RESET
         << "\n";
  // Step 1, process CallInst
  errs() << ANSI_COLOR_GREEN << "Collect CallInst" << ANSI_COLOR_RESET << "\n";
  for (Module::iterator fi = module.begin(), f_end = module.end(); fi != f_end;
       ++fi) {
    Function *func = dyn_cast<Function>(fi);
    if (func->isDeclaration() || func->isIntrinsic())
      continue;
    for (Function::iterator fi = func->begin(), fe = func->end(); fi != fe;
         ++fi) {
      BasicBlock *bb = dyn_cast<BasicBlock>(fi);
      for (BasicBlock::iterator ii = bb->begin(), ie = bb->end(); ii != ie;
           ++ii) {
        if (auto *ci = dyn_cast<CallInst>(ii))
          processCall(ci);
        if (auto *st = dyn_cast<StoreInst>(ii))
          processStore(st);
      }
    }
  }
  // Step 2, DMA buffer value either store to global value or to some struct
  // field
  errs() << "==================\n";
  for (auto *ci : callDmaAlloc) {
    // DMA address is stored in a buffer pointed to by ptr
    Value *ptr = ci->getOperand(2);
    dmaBufferAddr.insert(ptr);
    errs() << " dma buffer:" << *ptr << "\n";
    // TODO: collect load and store to global value
    // collect load and store to some struct field
    collectLoadStoreStruct(ptr, dmaBufferAddr);
  }
  errs() << "DMA Buffer Addr are stored to:\n";
  for (auto *v : dmaBufferAddr) {
    errs() << " - " << *v << "\n";
  }
  errs() << "==================\n";
  // Step 3, MMIO PIO Address either store to global value or to some struct
  // field
  for (auto *ci : callIoMap) {
    errs() << " device memory:" << *ci << "\n";
    std::vector<Value *> sts;
    collectStoreOfValue(ci, sts);
    for (auto *ist : sts) {
      StoreInst *st = dyn_cast<StoreInst>(ist);
      // errs()<<" -- "<<*st<<"\n";
      deviceMemAddr.insert(st->getPointerOperand());
    }
    while (1) {
      auto old = deviceMemAddr.size();
      for (auto *p : deviceMemAddr) {
        collectLoadStoreStruct(p, deviceMemAddr);
        if (old != deviceMemAddr.size())
          break;
      }
      if (old == deviceMemAddr.size())
        break;
    }
  }
  errs() << "Device Addr are stored to:\n";
  for (auto *v : deviceMemAddr) {
    errs() << " - " << *v << "\n";
  }
  errs() << "===Check iowrite===========\n";
  // Step 4, check iowrite's value are loaded from dmaBufferAddr
  for (auto *ci : callIoWrite) {
    // errs()<<" Checking iowrite:"<<*ci<<"\n";
    // this is the value we write
    Value *wv = ci->getOperand(0)->stripPointerCasts();
    Value *wp = ci->getOperand(1)->stripPointerCasts();
    // errs()<<"Testing ; "<<*wv<<"\n";
    bool detected = isLoadedFromAddress(wv, dmaBufferAddr) ||
                    isLoadedFromSameStructField(wv, dmaBufferAddr);
    // if (detected && isLoadedFromAddress(wp, deviceMemAddr)) {
    if (detected) {
      // Good! we detected writing dma buffer address to device
      errs() << " detected : " << ANSI_COLOR_MAGENTA;
      // errs()<< *ci << "\n";
      //  now, dump the offset
      dumpDeviceOffset(wp);
      if (auto &di = ci->getDebugLoc())
        di.print(errs());
      errs() << ANSI_COLOR_RESET << "\n";
    }
  }
  errs() << "===Check store============\n";
  // Step 5, check store through MMIO to device
  if (1)
    for (auto *st : mmioSt) {
      // errs()<<" Checking MMIO Store"<<*st<<"\n";
      Value *wv = st->getValueOperand()->stripPointerCasts();
      Value *wp = st->getPointerOperand()->stripPointerCasts();
      bool detected = isLoadedFromAddress(wv, dmaBufferAddr) ||
                      isLoadedFromSameStructField(wv, dmaBufferAddr);
      if (detected) {
        // if (detected && isLoadedFromAddress(wp, deviceMemAddr)) {
        errs() << " detected : " << ANSI_COLOR_MAGENTA;
        dumpDeviceOffset(st->getPointerOperand()->stripPointerCasts());
        if (auto &di = st->getDebugLoc())
          di.print(errs());
        errs() << ANSI_COLOR_RESET << "\n";
      }
    }
  errs() << ANSI_COLOR_CYAN << "--- DONE! ---" << ANSI_COLOR_RESET << "\n";

  // just quit
  exit(0);
  // never reach here
  return false;
}

static RegisterPass<APStage2>
    XXX("apstage2", "APStage2 Pass (with getAnalysisUsage implemented)", false,
        false);
