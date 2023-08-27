#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include <vector>
#include <iostream>

using namespace llvm;

typedef llvm::SmallVector<llvm::BasicBlock*, 16> BBList;
typedef llvm::SmallVector<llvm::Value*, 16> ValList;

llvm::Function* create_function(llvm::LLVMContext& context, llvm::Module* module, llvm::IRBuilder<> &builder, 
                                    std::string name, std::vector<std::string> fun_args){
    std::vector<Type *> integers(fun_args.size(), Type::getInt32Ty(context));
    llvm::FunctionType* func_type = llvm::FunctionType::get(builder.getInt32Ty(), integers, false);
    llvm::Function* func = llvm::Function::Create(
        func_type,
        llvm::Function::ExternalLinkage,
        name,
        module
    );
    
    return func;
}

llvm::BasicBlock* create_BB(llvm::LLVMContext &context, llvm::Function* func, std::string name){
    return llvm::BasicBlock::Create(context, name, func);
}

llvm::GlobalVariable* create_globalvar(llvm::Module* module, llvm::IRBuilder<> &builder, std::string name){
    module->getOrInsertGlobal(name, builder.getInt32Ty());
    llvm::GlobalVariable* gvar = module->getNamedGlobal(name);
    gvar->setLinkage(GlobalValue::CommonLinkage);
    gvar->setAlignment(Align(4));
    return gvar;
}

void setFunArgs(llvm::Function* func, std::vector<std::string> fun_args){
    unsigned Idx = 0;
    llvm::Function::arg_iterator AI, AE;
    for (AI = func->arg_begin(), AE = func->arg_end(); AI != AE; ++AI, ++Idx){
        AI->setName(fun_args[Idx]);
    }
}

llvm::Value* createArith(llvm::IRBuilder<> &builder, llvm::Value *L, llvm::Value *R){
    return builder.CreateMul(L, R, "multmp");
}

llvm::Value* create_loop(llvm::LLVMContext& context, llvm::IRBuilder<> &builder, BBList list, ValList VL, llvm::Value* startval, llvm::Value* endval){
    llvm::BasicBlock* preheaderBB = builder.GetInsertBlock();
    llvm::Value* val = VL[0];
    llvm::BasicBlock* loopBB = list[0];
    builder.CreateBr(loopBB);
    builder.SetInsertPoint(loopBB);
    llvm::PHINode* indvar = builder.CreatePHI(Type::getInt32Ty(context), 2, "i");
    indvar->addIncoming(startval, preheaderBB);
    llvm::Value* add = builder.CreateAdd(val, builder.getInt32(5), "addtmp");
    llvm::Value* stepval = builder.getInt32(1);
    llvm::Value* nextval = builder.CreateAdd(indvar, stepval, "nextval");
    llvm::Value* endcond = builder.CreateICmpULT(indvar, endval, "endcond");
    llvm::Value* int_endcond = builder.CreateIntCast(endcond, builder.getInt32Ty(), false, "inttmp");
    endcond = builder.CreateICmpNE(int_endcond, builder.getInt32(0), "loopcond");
    llvm::BasicBlock* loopendBB = builder.GetInsertBlock();
    llvm::BasicBlock* afterBB = list[1];
    builder.CreateCondBr(endcond, loopBB, afterBB);
    builder.SetInsertPoint(afterBB);
    indvar->addIncoming(nextval, loopendBB);
    return add;
}


int main(){
    std::vector<std::string> fun_args;
    fun_args.push_back("a");
    fun_args.push_back("b");
    llvm::LLVMContext context;
    llvm::Module *module = new llvm::Module("main", context);
    llvm::IRBuilder<> builder(context);
    llvm::GlobalVariable* gvar = create_globalvar(module, builder, "x");
    llvm::Function* func = create_function(context, module, builder, "func", fun_args);
    setFunArgs(func, fun_args);
    llvm::BasicBlock* entry = create_BB(context, func, "entry");
    builder.SetInsertPoint(entry);
    llvm::Function::arg_iterator AI = func->arg_begin();
    llvm::Value* arg1 = AI++;
    llvm::Value* arg2 = AI;
    llvm::Value* constant = builder.getInt32(16);
    llvm::Value* val = createArith(builder, arg1, constant);
    ValList VL;
    VL.push_back(arg1);

    BBList list;
    llvm::BasicBlock* loopBB = create_BB(context, func, "loop");
    llvm::BasicBlock* afterBB = create_BB(context, func, "afterloop");
    list.push_back(loopBB);
    list.push_back(afterBB);
    llvm::Value* startval = builder.getInt32(1);
    llvm::Value* res = create_loop(context, builder, list, VL, startval, arg2);

    builder.CreateRet(builder.getInt32(0));
    llvm::verifyFunction(*func);
    module->dump();
    return 0;
}