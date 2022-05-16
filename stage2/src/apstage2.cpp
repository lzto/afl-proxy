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

  if (ci->isInlineAsm()) {
    Value *cv = ci->getCalledOperand();
    auto * iasm = dyn_cast<InlineAsm>(cv->stripPointerCasts());
    if (iasm->getAsmString() == "movl $1,$0") {
      callIoReads.push_back(ci);
    } else if (iasm->getAsmString() == "movl $0,$1") {
      callIoWrite.push_back(ci);
    }
    return;
  }

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

  if (name.startswith("dma_map_single")
     || name.startswith("dma_map_page")) {
    callDmaAllocPage.push_back(ci);
  }
  if (name.startswith("dma_sync_single")) {
    // errs() << *ci << "\n";
  }
  // could write DMA address to device register ?
  if (name.startswith("iowrite32") || name.startswith("writel")) {
    callIoWrite.push_back(ci);
  }
  if (name.startswith("ioread8") || name.startswith("ioread16")
      || name.startswith("ioread32") || name.startswith("ioread64")) {
    callIoReads.push_back(ci);
  }
  // map device pio/mmio region
  if (name.startswith("pci_iomap") || name.startswith("ioremap") || name.startswith("pcim_iomap_table")) {
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

/* Need to handle Bitcasts ? */
Value * APStage2::processPCIM_IOMAP(CallInst *ci) {
  for (auto * user : ci->users()) {
    if (auto * gep = dyn_cast<GetElementPtrInst>(user)) {
      for (auto * u : gep->users()) {
        if (auto * ld = dyn_cast<LoadInst>(u)) {
          return ld;
        }
      }
    }
  }
  return nullptr;
}

static Type* getTypeByOffset(Module & m, Type * base, uint64_t offset) {
  auto & dl = m.getDataLayout();
  StructType * st = dyn_cast<StructType>(base);
  if (!st) 
    return nullptr;
  const StructLayout * struct_dl = dl.getStructLayout(st);
  if (offset >= struct_dl->getSizeInBytes()) {
    return nullptr;
  }
  int n_fields = st->getNumElements();
  int idx = -1;
  for (int i=0; i<n_fields; i++) {
    if (struct_dl->getElementOffset(i) == offset) {
      idx = i;
      break;
    }
  }
  if (idx == -1) {
    return nullptr;
  }
  return st->getElementType(idx);
}

void APStage2::findPointerStoredToType(Value *v, std::set<std::string> & srcs, 
                                        std::set<Type *> & results)
{
  for (auto *u : v->users()) {
    // got store
    if (StoreInst *st = dyn_cast<StoreInst>(u)) {
      if (st->getValueOperand() == v) {
        Value * dst = st->getPointerOperand()->stripPointerCasts();
        GetElementPtrInst * gep = dyn_cast<GetElementPtrInst>(dst);
        if (gep) {
          int offset = getGEPOffset(gep);
          errs() << "gep: " << *gep << "\n";
          if (offset == -1) {
            errs() << "Can't extract offset from GEP\n";
          } else {
            // errs() << "GEP offset = " << offset << "\n";
            Type * base = gep->getSourceElementType();
            if (!isa<StructType>(base)) {
              continue;
            }
            Type * t = getTypeByOffset(*m_, base, offset);
            if (t) {
              errs() << "Loc: ";
              gep->getDebugLoc().print(errs());
              errs() << "Type: " << *(t) << "\n";
              results.insert(t);
            } else {
              std::string type_off = getTypeOffsetStr(base, offset);
              srcs.insert(type_off);
            }
          }
        } else {
          // std::vector<Value *> lds;
          // collectLoadFromAddress(st->getPointerOperand(), lds);
          // for (auto * ld : lds) {
          //   collectStoreDstStruct(m, ld);
          // }
        }
      } 
    } else if (isa<CastInst>(u) || isa<PHINode>(u)) {
      findPointerStoredToType(u, srcs, results);
    }
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

void APStage2::findDMABufferPtrReg() {

} 

bool APStage2::findTypeOffsetAlias(Type * t, int offset, TypeOffset & result) {
  std::set<std::string>  type_offsets;
  std::set<Type *> res;

  const DataLayout & dl = m_->getDataLayout();
  if (!isa<StructType>(t)) {
    return false;
  }
  const StructLayout * struct_dl = dl.getStructLayout((StructType*)t);
  type_offsets.insert(getTypeOffsetStr(t, offset));
  findMemRegionTypeByCast(*m_, type_offsets, res);
  if (res.empty()) {
    return false;
  }
  int offset_into_alias = (int)struct_dl->getSizeInBytes() - offset;
  for (auto * aat: res) {
    const StructLayout * struct_dl = dl.getStructLayout((StructType*)aat);
    if ((uint64_t)offset_into_alias < struct_dl->getSizeInBytes()) {
      result.type = aat;
      result.offset = offset_into_alias;
      return true;
    }
  }
  return false;
}

void APStage2::findMemRegionTypeByCast(Module &module, std::set<std::string> & type_offsets, 
                                  std::set<Type *> & results) 
{
  for (Module::iterator fi = module.begin(), f_end = module.end(); fi != f_end; ++fi) {
    Function *func = dyn_cast<Function>(fi);
    if (func->isDeclaration() || func->isIntrinsic())
      continue;
    for (Function::iterator fi = func->begin(), fe = func->end(); fi != fe;
         ++fi) 
    {
      BasicBlock *bb = dyn_cast<BasicBlock>(fi);
      for (BasicBlock::iterator ii = bb->begin(), ie = bb->end(); ii != ie;
           ++ii) 
      {
        if (auto *bc = dyn_cast<CastInst>(ii)) {
          Type * dst_type= getPointerBaseType(bc->getType());
          
          if (!isa<StructType>(dst_type)) {
            continue;
          }

          if (auto *gep = dyn_cast<GetElementPtrInst>(bc->getOperand(0))) {
            Type * base_type = gep->getSourceElementType();
            if (!isa<StructType>(base_type)) {
              continue;
            }
       
            int offset = getGEPOffset(gep);
            if (offset == -1) {
              continue;
            }
            std::string tp_off =  getTypeOffsetStr(base_type, offset);
            if (type_offsets.count(tp_off)) {
              errs() << "Mem Region Type Found: " << dst_type->getStructName() << "\n";
              errs() << "bc: " << *bc << "\n";
              results.insert(dst_type);
            }
          }
        }
      }
    }
  }
}

int APStage2::getStructMemberOffset(GetElementPtrInst * gep) {
  int offset = -1;
  if (gep->hasAllConstantIndices()) {
    return getGEPOffset(gep);
  }
  Value * idx0 = gep->getOperand(1);
  // errs() << "gep " << *gep << "\n";
  if (!isa<ConstantInt>(idx0)) {
    bool rest_all_constants=true;
    for (auto i = gep->idx_begin()+1; i != gep->idx_end(); ++i) {
      if (!isa<ConstantInt>(i)) {
        rest_all_constants = false;
        break;
      }
    }
    if (rest_all_constants) {
      ConstantInt * idx0_const = ConstantInt::get(Type::getInt64Ty(gep->getContext()), 0, true);
      gep->replaceUsesOfWith(idx0, idx0_const);
      offset = getGEPOffset(gep);
      gep->replaceUsesOfWith(idx0_const, idx0);
      // delete idx0_const;
      return offset;
    }
  }
  return offset;
}


void APStage2::findDMAPageAddrOffset(Value * page_addr) {
  for (auto * user : page_addr->users()) {
    if (isa<StoreInst>(user)) {
      StoreInst * st = dyn_cast<StoreInst>(user);
      Value * ptr_op = st->getPointerOperand();
      if (GetElementPtrInst * gep = dyn_cast<GetElementPtrInst>(ptr_op)) {
        Type * struct_type = gep->getSourceElementType();
        if (secondDMABufferOffsets.find(struct_type) != secondDMABufferOffsets.end()) {
          continue;
        }
        if (dmaBufferTypes.count(struct_type)) {
          errs() << "hit\n";
          int offset = getStructMemberOffset(gep);
          if (offset != -1) {
            Type * type = getTypeByOffset(*m_, struct_type, offset);
            if (type) {
              secondDMABufferOffsets[struct_type] = offset;
              errs() << struct_type->getStructName() << ": @" << (*type) << " offset " << offset << " type: " << *(gep->getResultElementType()) <<  " secondary DMA buffer " << "\n" ; 
            }
          }
        }
      }
    } else if (isa<PHINode>(user)) {
      findDMAPageAddrOffset(user);
    } else if (isa<CastInst>(user)) {
      findDMAPageAddrOffset(user);
    }
  }
}



void APStage2::findMMIORegionPtrs() {
  std::set<Value*> mmio_bases;
  Module & module = *m_;
  for (auto * mmio_base : deviceMemAddr) {
    errs() << *mmio_base << "\n";
    for (auto * u : mmio_base->users()) {
      doFindMMIORegionPtrs(u, 0);
    }
  }
  Type * base_struct_type = nullptr;
  int base_field_offset;
  for (auto & pair : mmio_regions) {
    MMIORegionPtr mr = pair.second;
    if (mr.base_offset == 0) {
      if (base_struct_type) {
        errs() << "Already have a base\n";
        continue;
      }
      base_struct_type = mr.host_type;
      base_field_offset = mr.host_offset;
    }
  }
  
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
        if (auto * ld = dyn_cast<LoadInst>(ii)) {
          Value * src = ld->getPointerOperand();
          if (!isa<GetElementPtrInst>(src)) {
            src = src->stripPointerCasts();
          }
          if (auto * gep = dyn_cast<GetElementPtrInst>(src)) {
            Type * src_type = gep->getSourceElementType(); 
            int offset = getGEPOffset(gep);
            if (src_type == base_struct_type && 
                offset == base_field_offset)
            {
              mmio_bases.insert(ld);
            }
          }
        }
          
      }
    }
  }
  for (auto * mb : mmio_bases) {
    for (auto * u : mb->users()) {
      doFindMMIORegionPtrs(u, 0);
    }
  }
}

Type * findStructTypeCasted(Value * v) {
  for (auto * u : v->users()) {
    if (isa<CastInst>(u)) {
      Type * t = u->getType();
      if (t->isPointerTy())
        t = getPointerBaseType(t);
      if (isa<StructType>(t)) {
        return t;
      }
    }
  }
  return nullptr;
}

void APStage2::doFindMMIORegionPtrs(Value *mmio_ptr, int base_off) {
  if (isa<StoreInst>(mmio_ptr)) {
    // sink 
    StoreInst * store = (StoreInst*)mmio_ptr;
    Value * dst = store->getPointerOperand()->stripPointerCasts();
    if (auto * gep = dyn_cast<GetElementPtrInst>(dst)) {
      // errs() << "mmio gep\n";
      // errs() << *gep << "\n";
      Type * src_type = gep->getSourceElementType();  
      if (!isa<StructType>(src_type)) {
        src_type = findStructTypeCasted(gep->getPointerOperand());
        if (!src_type)
          return;
      }
      int offset = getGEPOffset(gep);
      if (offset != -1) {
        std::string key = getTypeOffsetStr(src_type, offset);
        if (mmio_regions.find(key) == mmio_regions.end()) {
          errs() << "MMIO Host: " << src_type->getStructName() << " offset: " << offset << " base off:" << base_off << "\n";
          mmio_regions[key] = {src_type, offset, base_off};
          Type * t = getTypeByOffset(*m_, src_type, offset);
          if (!t) {
            TypeOffset alias;
            if (findTypeOffsetAlias(src_type, offset, alias)) {
              key = getTypeOffsetStr(alias.type, alias.offset);
              mmio_regions[key] = {alias.type, alias.offset, base_off};
              errs() << "MMIO Host Alias: " << alias.type->getStructName() << " offset: " << alias.offset << " base off:" << base_off << "\n";
            }
          }
        }
      } else {
        errs() << "findMMIORegionPtrs: GEP offsets are not all constants\n"; 
      }
    }
  } else if (isa<BinaryOperator>(mmio_ptr)) {
    // pointer arithmetics
    ConstantInt * cint;
    int64_t cint_val;
    int new_base_off;
    auto * bin_op = (BinaryOperator*)mmio_ptr;
    Value * op1 = bin_op->getOperand(0);
    Value * op2 = bin_op->getOperand(1);
    if (!isa<ConstantInt>(op1) && !isa<ConstantInt>(op2)) {
      return;
    }
    cint = isa<ConstantInt>(op1) ? (ConstantInt*)op1 : (ConstantInt*)op2;
    cint_val = cint->getSExtValue();
    auto opcode = bin_op->getOpcode();
    if (opcode == Instruction::BinaryOps::Add) {
      new_base_off = base_off + cint_val;
    } else if (opcode == Instruction::BinaryOps::Sub) {
      new_base_off = base_off - cint_val;
    }
    for (auto * u : mmio_ptr->users()) {
      doFindMMIORegionPtrs(u, new_base_off);
    }
  } else if (isa<CastInst>(mmio_ptr)) {
    for (auto * u : mmio_ptr->users()) {
      doFindMMIORegionPtrs(u, base_off);
    }
  } else if (isa<GetElementPtrInst>(mmio_ptr)) {
    auto * gep = (GetElementPtrInst*)mmio_ptr;
    int off = getGEPOffset(gep);
    if (off != -1) {
      int new_base_off = base_off + off;
      for (auto * u : mmio_ptr->users()) {
        doFindMMIORegionPtrs(u, new_base_off);
      }
    } 
  }
  
}


/* collect DMA inputs source */
void APStage2::collectDMAReads() {
  Module & module = *m_;
  for (Module::iterator fi = module.begin(), f_end = module.end(); fi != f_end; ++fi) {
    Function *func = dyn_cast<Function>(fi);
    if (func->isDeclaration() || func->isIntrinsic())
      continue;
    for (Function::iterator fi = func->begin(), fe = func->end(); fi != fe;
         ++fi) 
    {
      BasicBlock *bb = dyn_cast<BasicBlock>(fi);
      for (BasicBlock::iterator ii = bb->begin(), ie = bb->end(); ii != ie;
           ++ii) 
      {
        if (auto * gep = dyn_cast<GetElementPtrInst>(ii)) {
          Type * type = gep->getSourceElementType();
          if (!dmaBufferTypes.count(type)) {
            continue;
          }
          int offset = getStructMemberOffset(gep);
          if (offset == -1) {
            continue;
          }
          std::vector<Value*> loads;
          collectLoadFromAddress(gep, loads);
          for (auto * ld : loads) {
            // errs() << "DMA Read: offset = " << offset << "\n";
            // ((LoadInst*)ld)->getDebugLoc().print(errs());
            // errs() << "\n";
            dma_reads[ld] = {type, offset};
          }
        }
      }
    }
  }
}

static std::unordered_map<Instruction::BinaryOps, Expression::ExpType> op_map {
  {Instruction::BinaryOps::Add, Expression::ExpType::ADD},
  {Instruction::BinaryOps::Sub, Expression::ExpType::SUB},
  {Instruction::BinaryOps::Mul, Expression::ExpType::MUL},
  {Instruction::BinaryOps::SDiv, Expression::ExpType::DIV},
  {Instruction::BinaryOps::UDiv, Expression::ExpType::DIV},
  {Instruction::BinaryOps::And, Expression::ExpType::BITAND},
  {Instruction::BinaryOps::Or, Expression::ExpType::BITOR},
  {Instruction::BinaryOps::Xor, Expression::ExpType::BITXOR},
  {Instruction::BinaryOps::Shl, Expression::ExpType::LSHIFT},
  {Instruction::BinaryOps::LShr, Expression::ExpType::RSHIFT},
};

static std::unordered_map<CmpInst::Predicate, Expression::ExpType> pred_map {
  {CmpInst::Predicate::ICMP_EQ, Expression::ExpType::EQ},
  {CmpInst::Predicate::ICMP_NE, Expression::ExpType::NE},
  {CmpInst::Predicate::ICMP_SLT, Expression::ExpType::LT},
  {CmpInst::Predicate::ICMP_ULT, Expression::ExpType::LT},
  {CmpInst::Predicate::ICMP_SLE, Expression::ExpType::LE},
  {CmpInst::Predicate::ICMP_ULE, Expression::ExpType::LE},
  {CmpInst::Predicate::ICMP_SGT, Expression::ExpType::GT},
  {CmpInst::Predicate::ICMP_UGT, Expression::ExpType::GT},
  {CmpInst::Predicate::ICMP_SGE, Expression::ExpType::GE},
  {CmpInst::Predicate::ICMP_UGE, Expression::ExpType::GE},
};

/* helper function, find the first user of the HW Input */
Value * getFirstHWInputInBB(BasicBlock * bb, 
                           std::unordered_map<Value*, Expression*> & val2expr) 
{
  for (BasicBlock::iterator ii = bb->begin(), ie = bb->end(); ii != ie;
      ++ii)
  {
    Value * v = &*ii;
    if (val2expr.find(v) != val2expr.end()) {
      return v;
    }
  } 
  return nullptr;                                              
}

static void collectValueAlias(Value * v, std::set<Value*> & alias, std::set<Value*> visited) {
  if (visited.count(v)) {
    return;
  }
  visited.insert(v);
  for (auto * u : v->users()) {
    if (isa<CastInst>(u) || isa<PHINode>(u)) {
      alias.insert(u);
      collectValueAlias(u, alias, visited);
    }
  }
}

Expression * FieldConstraintBuilder::buildPredExprTopDown(int level) {
  if (level == 0) {
    return hw_value_expr;
  }
  int idx = level -1;
  Instruction * cur_op = static_cast<Instruction*>(path[idx]);

  if (isa<SwitchInst>(cur_op)) {
    SwitchInst * sw = (SwitchInst*)cur_op;
    Expression * lhs = buildPredExprTopDown(level-1);
    if (!lhs) {
      errs() << "can't build expr\n";
      return nullptr;
    }
    for (auto case_it = sw->case_begin(); case_it != sw->case_end(); ++case_it) {
      ConstantInt * cst = case_it->getCaseValue();
      ValueExpr * cst_expr = new ValueExpr(cst->getZExtValue());
      ArithExpr * eq_expr = new ArithExpr(Expression::ExpType::EQ, lhs, cst_expr);
      results.push_back(eq_expr);
    }
    return nullptr;
  }


  Value * const_v = isa<ConstantInt>(cur_op->getOperand(0)) ? cur_op->getOperand(0) : cur_op->getOperand(1);
  ConstantInt * const_int = static_cast<ConstantInt*>(const_v);
  Expression::ExpType exp_type;

  if (isa<BinaryOperator>(cur_op)) {
    auto * bin_op = (BinaryOperator*)cur_op;
    auto opcode = bin_op->getOpcode();
    if (op_map.find(opcode) == op_map.end()) {
      errs() << "Unhandled Operation type!!\n";
      return nullptr;
    }
    exp_type = op_map[opcode];
  } else if (isa<CmpInst>(cur_op)) {
    auto * cmp = (CmpInst*)cur_op;
    auto pred = cmp->getPredicate();
    if (pred_map.find(pred) == pred_map.end()) {
      errs() << "Pred type Undefined!\n";
      return nullptr;
    }
    exp_type = pred_map[pred];
  }  
  Expression * left_expr = buildPredExprTopDown(level-1);
  if (!left_expr) {
    return nullptr;
  }
  ValueExpr * const_expr = new ValueExpr(const_int->getZExtValue());
  ArithExpr * expr = new ArithExpr(exp_type, left_expr, const_expr);
  return expr;
}

void FieldConstraintBuilder::buildPreds(Value * hw_input_op) 
{
  if (isa<BinaryOperator>(hw_input_op) || isa<CmpInst>(hw_input_op)) {
    Instruction * inst = (Instruction*)hw_input_op;
    Value * op1 = inst->getOperand(0);
    Value * op2 = inst->getOperand(1);
    if (!isa<ConstantInt>(op1) && !isa<ConstantInt>(op2)) {
      return;
    }
    path.push_back(hw_input_op);
    for (auto * u : hw_input_op->users()) {
      buildPreds(u);
    }
    path.pop_back();
  } else if (isa<BranchInst>(hw_input_op)) {
    /* sink */
    BranchInst * branch = (BranchInst*)hw_input_op;
    Expression * expr = buildPredExprTopDown(path.size());
    if (!expr) {
      errs() << "Fatal Error: can't build expression\n";
    } else {
      preds[branch] = expr;
      bb2branch[branch->getParent()] = branch;
    }
  } else if (isa<SwitchInst>(hw_input_op)) {
    /* sink */
    path.push_back(hw_input_op);
    buildPredExprTopDown(path.size());
    path.pop_back();
  }
  return;
}

void FieldConstraintBuilder::buildConstraints() {
  std::set<Value*> alias;
  std::set<Value*> visited;
  assert(path.empty());

  collectValueAlias(hw_input_src, alias, visited);
  alias.insert(hw_input_src);
  for (auto * v : alias) {
    for (auto * u : v->users()) {
      buildPreds(u);
    }
  }
  
  // for (auto & pair : preds) {
  //   BranchInst * b = dyn_cast<BranchInst>(pair.first);
  //   if (!b) {
  //     continue;
  //   }
  //   auto * true_bb = b->getSuccessor(0);
  //   Expression * expr = preds[pair.first];
  //   if (bb2branch.find(true_bb) != bb2branch.end()) {
  //     // AND
  //     BranchInst * btb = bb2branch[true_bb];
  //     Expression * expr_tbt = preds[btb];
  //     if (!expr) {
  //       errs() << "Unhandled condition\n";
  //       continue;
  //     }
  //     errs() << "And Expression\n";
  //     ArithExpr * and_expr = new ArithExpr(Expression::ExpType::AND, expr, expr_tbt);
  //     preds[btb] = nullptr;
  //     preds[b] = and_expr;
  //   } 
  // }

  for (auto & pair : preds) {
    if (pair.second) {
      results.push_back(pair.second);
    }
  }
  
}


int APStage2::findMMIO_Offset(CallInst * mmio) {
  Value * src = mmio->getNumArgOperands() == 1 ? 
                mmio->getArgOperand(0) : mmio->getArgOperand(1);
  src = src->stripPointerCasts();
  int offset;
  int res = -1;
  if (auto * gep = dyn_cast<GetElementPtrInst>(src)) {
    // errs() << "gep: " << *gep << "\n";
    offset = getGEPOffset(gep);
    if (offset == -1) {
      errs() << "MMIO Offset is not constant\n";
      return -1;
    }
    /* find load instruction */
    Value * ptr = gep->getPointerOperand()->stripPointerCasts();
    if (auto *load = dyn_cast<LoadInst>(ptr)) {
      //  errs() << "load: " << *load << "\n";
      Value * load_src = load->getPointerOperand();
      if (!isa<GetElementPtrInst>(load_src)) {
        load_src = load_src->stripPointerCasts();
        // all-zero GEP?
        // if (!isa<Instruction>(load_src) && isa<StructType>(load_src->getType())) {
        //   auto key = getTypeOffsetStr(load_src->getType(), 0);
        //   if (mmio_regions.find(key) != mmio_regions.end()) {
        //     MMIORegionPtr & mrp = mmio_regions[key];
        //     res = mrp.base_offset + offset;
        //     return res;
        //   }
        // }
      }

      if (auto * gep_load = dyn_cast<GetElementPtrInst>(load_src)) {
        int field_offset = getGEPOffset(gep_load);
        if (field_offset != -1) {
          Type * gep_src_type = gep_load->getSourceElementType();
          if (!isa<StructType>(gep_src_type)) {
            errs() << "conservatively set base_off to be 0\n";
            return offset;
          }
          std::string key = getTypeOffsetStr(gep_src_type, field_offset);
          if (mmio_regions.find(key) != mmio_regions.end()) {
            MMIORegionPtr & mrp = mmio_regions[key];
            res = mrp.base_offset + offset;
            return res;
          } else {
            errs() << "MMIORegion Type can't be found\n";
          }
        } else {
          errs() << "MMIO Ptr GEP is not constant\n";
        }
      } else {
        errs() << "Load Src Not GEP: " << *((Instruction*)load_src) << "\n";
      }
    } else {
      errs() << "gep pointer is not loaded\n";
    }
   
    errs() << "conservatively set base_off to be 0\n";
    return offset;
   
  } else {
    errs() << "Not GEP\n";
  }
  return res;
}


void APStage2::extractMMIOFieldConstraint() {
  for (auto * ci: callIoReads) {
   
    int offset = findMMIO_Offset(ci);
   
    if (offset == -1) {
      continue;
    }
    auto * builder = new FieldConstraintBuilder("mmio", offset, ci);
    builder->buildConstraints();
    auto & cstrs = builder->getResults();
    if (!cstrs.size()) {
      delete builder;
      continue;
    }
    errs() << "MMIO Read: ";
    ci->getDebugLoc().print(errs());
    errs() << *ci << "\n";
    errs() << "\nOFFSET: " << offset << "\n";
    errs() << "Results:\n";
    for (auto * expr : cstrs) {
      errs() << expr->str() << "\n";
    }
    errs() << "\n";
    delete builder;
  }
}

void APStage2::extractFieldConstraint(Value * v, TypeOffset type_offset) {
  std::vector<Expression*> res;
  std::unordered_map<Value*, Expression*> val2expr;
  auto sname = (type_offset.type)->getStructName().str();
  int offset = type_offset.offset;
  /* print some debug info */
  Instruction * inst = (Instruction*)v;
  inst->getDebugLoc().print(errs());
  errs() << "\n IR: " << *inst << "\n";

  // get first user that is cast , binary_op , cmp, branch
  auto * builder = new FieldConstraintBuilder(sname, offset, v);
  builder->buildConstraints();
  auto & cstrs = builder->getResults();
  errs() << "Results:\n";
  for (auto * expr : cstrs) {
    errs() << expr->str() << "\n";
  }
  errs() << "\n";
}

bool APStage2::APStage2Pass(Module &module) {
  m_ = &module;
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
    /* 
     * TODO(Yilun)
     * Deal with store -> load -> store pattern ?
     */
    findPointerStoredToType(ci, dmaAddrStoreDst, dmaBufferTypes);

  }
  if (!dmaBufferTypes.size()) {
    errs() << "======= Type Discovery Phase 2 ===========\n";
    for (auto & s : dmaAddrStoreDst) {
      errs() << s << "\n";
    }
    findMemRegionTypeByCast(module, dmaAddrStoreDst, dmaBufferTypes);
  }
  errs() << "Total number of DMA Buffer Types: " << dmaBufferTypes.size() << "\n";


  errs() << "==================\n";
  // Step 3, MMIO PIO Address either store to global value or to some struct
  // field
  for (auto *ci : callIoMap) {
#if (LLVM_VERSION_MAJOR <= 10)
    Value *cv = ci->getCalledValue();
#else
    Value *cv = ci->getCalledOperand();
#endif
    Function *bcf = dyn_cast<Function>(cv->stripPointerCasts());
    auto name = bcf->getName();
    if (name.startswith("pcim_iomap_table")) { 
      Value * v = processPCIM_IOMAP(ci);
      if (v) {
        deviceMemAddr.insert(v);
      }
    } else {
      deviceMemAddr.insert(ci);
    }
  }

  errs() << "=========Secondary DMA Search============\n";
  for (auto * ci : callDmaAllocPage) {
    findDMAPageAddrOffset(ci);
  }

  collectDMAReads();

  errs() << "========Finding MMIO Ptrs=============\n";
  findMMIORegionPtrs();

  errs() << "=========Construct Field Constraint============\n";
  extractMMIOFieldConstraint();
  // for (auto & pair : dma_reads) {
  //   Value * dma_val = pair.first;
  //   TypeOffset & type_offset = pair.second;
  //   extractFieldConstraint(dma_val, type_offset);
  // }
  errs() << ANSI_COLOR_CYAN << "--- DONE! ---" << ANSI_COLOR_RESET << "\n";

  // just quit
  exit(0);
  // never reach here
  return false;
}

static RegisterPass<APStage2>
    XXX("apstage2", "APStage2 Pass (with getAnalysisUsage implemented)", false,
        false);
