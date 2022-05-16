/*
 * 2022 Tong Zhang<ztong0001@gmail.com>
 */
#ifndef _AP_STAGE2_ANALYZER_H_
#define _AP_STAGE2_ANALYZER_H_

#include <fstream>
#include <unordered_map>
#include <sstream>
#include <stack>
#include "hw_input_model.h"
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
#include "llvm/IR/Instruction.h"
#include "llvm/IR/InstrTypes.h"
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
#include "llvm/IR/InlineAsm.h"

#define DEBUG_TYPE "apstage2"

using namespace llvm;

/* helper function */
std::string getTypeOffsetStr(Type * tp, int offset) {
  return tp->getStructName().str() + "##" + std::to_string(offset);
}

Type * getPointerBaseType(Type * t) {
  while (t->isPointerTy()) {
    t = t->getPointerElementType();
  }
  return t;
}

struct TypeOffset {
  Type * type;
  int offset;
};

struct MMIORegionPtr {
  Type * host_type;
  int  host_offset;
  int base_offset;
};

struct FieldConstraint {
  Type * type;
  int offset;
  int size;
  Expression * expr;
};



class APStage2 : public ModulePass {
public:
  
private:
  virtual bool runOnModule(Module &) override;
  void processCall(CallInst *);
  void processStore(StoreInst *);
  Value *processPCIM_IOMAP(CallInst *ci);
  bool isLLVMPrefix(Value *);
  AliasAnalysis *AA;
  std::vector<CallInst *> callInsts;
  std::vector<CallInst *> callDmaAlloc;
  std::vector<CallInst *> callDmaAllocPage;
  std::vector<CallInst *> callIoWrite;
  std::vector<CallInst *> callIoReads;
  // map device PIO/MMIO region
  std::vector<CallInst *> callIoMap;
  // for storeInst(ioaddr, PointerOperand), we collect PointerOperand
  std::set<Value *> deviceMemAddr;
  // for storeInst(dmaBuffer, PointerOperand), we collect PointerOperand
  std::set<Value *> dmaBufferAddr;
  std::set<Type *> dmaBufferTypes;
  std::set<std::string> dmaAddrStoreDst;
  std::unordered_map<Type*, int> secondDMABufferOffsets;
  // store instructions that store something to device
  std::vector<StoreInst *> mmioSt;
  std::unordered_map<std::string, struct MMIORegionPtr> mmio_regions;
  std::unordered_map<Value*, int> mmio_reads;
  std::unordered_map<Value*, TypeOffset> dma_reads;  /* Value : Type::Offset */
  std::vector<FieldConstraint> field_constraints;
  bool findTypeOffsetAlias(Type * t, int offset, TypeOffset & result);
  void findMemRegionTypeByCast(Module &module, std::set<std::string> & srcs, 
                                  std::set<Type *> & results); 
  void findPointerStoredToType(Value *v, std::set<std::string> & srcs, 
                                        std::set<Type *> & results);
  int findMMIO_Offset(CallInst * mmio);
  void doFindMMIORegionPtrs(Value * mmio_ptr, int base_off);
  void findMMIORegionPtrs();
  void findDMABufferPtrReg();
  bool buildExpr(Value * inst, Expression * prev_expr, Expression * prev_bb_expr, 
                        std::vector<Expression*> & res, std::unordered_map<Value*, Expression*> & val2expr);
  void extractFieldConstraint(Value * v, TypeOffset type_offset);
  
  void collectDMAReads();
  void extractMMIOFieldConstraint();
  void collectLoadStoreStruct(Value *, std::set<Value *> &);
  void collectLoadFromAddress(Value *, std::vector<Value *> &);
  void collectStoreOfValue(Value *, std::vector<Value *> &);
  bool isGEPFromSameStructField(Value *, std::set<Value *> &);
  bool isLoadedFromAddress(Value *, std::set<Value *> &);
  bool isLoadedFromSameStructField(Value *, std::set<Value *> &);
  void findDMAPageAddrOffset(Value * page_addr);
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
  int getGEPOffset(GetElementPtrInst * gep) {
    APInt apint(64, 0);
    bool is_constant;
    is_constant = gep->accumulateConstantOffset(m_->getDataLayout(), apint);
    return is_constant ? (int)(apint.getZExtValue()) : -1;
  }

  int getStructMemberOffset(GetElementPtrInst * gep);

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
  Module * m_;
};

class FieldConstraintBuilder {
public:
  FieldConstraintBuilder(const std::string & name, int off, Value * hw_input) {
    offset = off;
    struct_name = name;
    hw_input_src = hw_input;
    Type * field_type = hw_input->getType();
    size_in_bytes = field_type->getScalarSizeInBits() / 8;
    hw_value_expr = new ValueExpr(name, off, size_in_bytes);
  }

  Expression * buildPredExprTopDown(int level);
  void buildPreds(Value * hw_input_op);
  void buildConstraints();
  std::vector<Expression*> & getResults() { return results; }
 
private:
  std::string struct_name;
  int offset;
  int size_in_bytes;
  Value * hw_input_src;
  ValueExpr * hw_value_expr;
  std::vector<Value*>  path;
  std::unordered_map<Instruction*, Expression*> preds;
  std::vector<Expression*> results;
  std::unordered_map<BasicBlock*, BranchInst*> bb2branch;
};


#endif //_AP_STAGE2_ANALYZER_H_
