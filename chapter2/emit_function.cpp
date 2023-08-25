#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Verifier.h"

using namespace llvm;

int main() {
    LLVMContext context;
    Module *module = new Module("test", context);
    IRBuilder<> builder(context);


    // 创建一个函数
    llvm::Function *func = llvm::Function::Create(
        llvm::FunctionType::get(builder.getInt32Ty(), false),
        llvm::Function::ExternalLinkage,
        "main",
        module
    );

    verifyFunction(*func);

    module->dump();
    return 0;

}