#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Verifier.h"
#include <iostream>

using namespace llvm;



llvm::Function *create_function(Module* module, IRBuilder<> &Builder, std::string Name){
    FunctionType* funcType = llvm::FunctionType::get(Builder.getInt32Ty(), false);
    Function* func = llvm::Function::Create(
        funcType,
        llvm::Function::ExternalLinkage,
        Name,
        module
    );
    return func;
}



int main() {
    LLVMContext context;

    Module *module = new Module("test", context);
    IRBuilder<> builder(context);

    llvm::Function *func = create_function(module, builder, "main");
    verifyFunction(*func);

    module->dump();
    return 0;

}