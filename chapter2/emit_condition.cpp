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

llvm::Value *createArith(llvm::IRBuilder<> &builder, llvm::Value *L, llvm::Value *R){
    return builder.CreateMul(L, R, "multmp");
}

llvm::Value* createIfElse(llvm::LLVMContext& context, llvm::IRBuilder<> &builder, BBList list, ValList VL){
    llvm::Value* condition = VL[0];
    llvm::Value* arg1 =VL[1];
    llvm::BasicBlock* thenBB = list[0];
    llvm::BasicBlock* elseBB = list[1];
    llvm::BasicBlock* mergeBB = list[2];
    builder.CreateCondBr(condition, thenBB, elseBB);
    builder.SetInsertPoint(thenBB);
    llvm::Value* thenVal = builder.CreateAdd(arg1, builder.getInt32(1), "thenaddtmp");
    builder.CreateBr(mergeBB);
    builder.SetInsertPoint(elseBB);
    llvm::Value* elseVal = builder.CreateAdd(arg1, builder.getInt32(2), "elseaddtmp");
    builder.CreateBr(mergeBB);

    unsigned PhiBBSize = list.size() - 1;
    llvm::PHINode* phi = builder.CreatePHI(llvm::Type::getInt32Ty(context), PhiBBSize, "iftmp");
    phi->addIncoming(thenVal, thenBB);
    phi->addIncoming(elseVal, elseBB);
    return phi;
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
    llvm::BasicBlock* entry = create_BB(context, func, "entry");
    builder.SetInsertPoint(entry);
    llvm::Value *arg1 = func->arg_begin();
    llvm::Value *arg2 = builder.getInt32(16);
    llvm::Value *val = createArith(builder, arg1, arg2);

    llvm::Value* val2 = builder.getInt32(100);
    llvm::Value* compare = builder.CreateICmpULT(val, val2, "cmptmp");// Integer Compare Unsigned Less Than
    // 报错提示CreateICmpNE中两个错作数不是同一个类型，所以需要先将unsigned int 转换成int
    llvm::Value* int_compare = builder.CreateIntCast(compare, builder.getInt32Ty(), false, "inttmp");
    llvm::Value* condition = builder.CreateICmpNE(int_compare, builder.getInt32(0), "ifcond"); // Integer Compare Not Equal
    ValList VL;
    VL.push_back(condition);
    VL.push_back(arg1);

    llvm::BasicBlock* thenBB = create_BB(context, func, "then");
    llvm::BasicBlock* elseBB = create_BB(context, func, "else");
    llvm::BasicBlock* mergeBB = create_BB(context, func, "merge");
    BBList list;
    list.push_back(thenBB);
    list.push_back(elseBB);
    list.push_back(mergeBB);
    
    llvm::Value* v = createIfElse(context, builder, list, VL);

    builder.CreateRet(builder.getInt32(0));
    llvm::verifyFunction(*func);
    module->dump();
    return 0;
}