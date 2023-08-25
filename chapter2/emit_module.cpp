#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

using namespace llvm;

int main(){
    llvm::LLVMContext context;
    llvm::Module *module = new llvm::Module("new module", context);
    module->dump();
}