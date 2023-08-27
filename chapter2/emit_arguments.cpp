#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include <vector>

using namespace llvm;

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
    builder.CreateRet(builder.getInt32(0));
    llvm::verifyFunction(*func);
    module->dump();
    return 0;
}