/*
 * 2022 Tong Zhang<ztong0001@gmail.com>
 */
#ifndef _AP_STAGE2_ANALYZER_H_
#define _AP_STAGE2_ANALYZER_H_

#include <fstream>

#include "llvm-c/Core.h"
#include "llvm/ADT/SetVector.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/Analysis/AliasSetTracker.h"
#include "llvm/Analysis/GlobalsModRef.h"
#include "llvm/Analysis/MemoryBuiltins.h"
#include "llvm/Analysis/ScalarEvolution.h"
#include "llvm/Analysis/ScalarEvolutionAliasAnalysis.h"
#include "llvm/Analysis/TargetLibraryInfo.h"
#include "llvm/Analysis/ValueTracking.h"
#include "llvm/IR/CFG.h"
#if (LLVM_VERSION_MAJOR <= 10)
#include "llvm/IR/CallSite.h"
#endif
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Dominators.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/GetElementPtrTypeIterator.h"
#include "llvm/IR/GlobalValue.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Operator.h"
#include "llvm/IR/Use.h"
#include "llvm/IR/ValueMap.h"
#include "llvm/Pass.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Instrumentation.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/Transforms/Utils/ModuleUtils.h"

#define DEBUG_TYPE "apstage2"

using namespace llvm;

class APStage2 : public ModulePass {
private:
  virtual bool runOnModule(Module &) override;
  void processCall(CallInst *);
  void processStore(StoreInst *);
  bool isLLVMPrefix(Value *);
  bool isDMAType(Value *);
  AliasAnalysis *AA;
  std::vector<CallInst *> callInsts;
  std::vector<CallInst *> callDmaAlloc;
  std::vector<CallInst *> callIoWrite;
  // map device PIO/MMIO region
  std::vector<CallInst *> callIoMap;
  // for storeInst(ioaddr, PointerOperand), we collect PointerOperand
  std::set<Value *> deviceMemAddr;
  // for storeInst(dmaBuffer, PointerOperand), we collect PointerOperand
  std::set<Value *> dmaBufferAddr;

  // store instructions that store something to device
  std::vector<StoreInst *> mmioSt;

  void collectLoadStoreStruct(Value *, std::set<Value *> &);
  void collectLoadFromAddress(Value *, std::vector<Value *> &);
  void collectStoreOfValue(Value *, std::vector<Value *> &);
  bool isGEPFromSameStructField(Value *, std::set<Value *> &);
  bool isLoadedFromAddress(Value *, std::set<Value *> &);
  bool isLoadedFromSameStructField(Value *, std::set<Value *> &);

  void get_gep_indicies(GetElementPtrInst *gep, std::vector<int> &indices) {
    if (!gep)
      return;
    // replace all non-constant with zero
    // because they are literally an array...
    // and we are only interested in the type info
    for (auto i = gep->idx_begin(); i != gep->idx_end(); ++i) {
      ConstantInt *idc = dyn_cast<ConstantInt>(i);
      if (idc)
        indices.push_back(idc->getSExtValue());
      else
        indices.push_back(0);
    }
  }
  void dumpDeviceOffset(Value *v) {
    GetElementPtrInst *gep = dyn_cast<GetElementPtrInst>(v);
    if (!gep) {
      errs() << " offset: 0x0\n";
      return;
    }
    std::vector<int> indices;
    get_gep_indicies(gep, indices);
    errs() << " offset: " << format_hex(indices.back(), 8) << "\n";
  }

public:
  static char ID;
  APStage2() : ModulePass(ID){};

  virtual StringRef getPassName() const override { return "apstage2"; }
  bool APStage2Pass(Module &module);

  virtual void print(raw_ostream &OS, const Module *M) const override {
    OS << "Analysis Result\n";
  }

  void getAnalysisUsage(AnalysisUsage &au) const override {
    ModulePass::getAnalysisUsage(au);
    // au.addRequired<AAResultsWrapperPass>();
    // au.addRequired<TargetLibraryInfoWrapperPass>();
    // au.addRequired<ScalarEvolutionWrapperPass>();
    au.setPreservesAll();
  }

private:
};

#endif //_AP_STAGE2_ANALYZER_H_
