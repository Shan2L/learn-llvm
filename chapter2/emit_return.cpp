#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
// #include "llvm/IR/GlobalVariable.h"

using namespace llvm;

llvm::Function* create_function(llvm::Module* module, llvm::IRBuilder<> &builder, std::string name){
    llvm::FunctionType* func_type = llvm::FunctionType::get(builder.getInt32Ty(), false);
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

int main(){
    llvm::LLVMContext context;
    llvm::Module *module = new llvm::Module("main", context);
    llvm::IRBuilder<> builder(context);
    llvm::GlobalVariable* gvar = create_globalvar(module, builder, "x");
    llvm::Function* func = create_function(module, builder, "func");
    llvm::BasicBlock* entry = create_BB(context, func, "entry");
    builder.SetInsertPoint(entry);
    builder.CreateRet(builder.getInt32(0));
    llvm::verifyFunction(*func);
    module->dump();
    return 0;
}