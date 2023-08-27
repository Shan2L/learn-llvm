#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Verifier.h"

using namespace llvm;


llvm::Function* create_function(llvm::Module* module, IRBuilder<> & Builder, std::string Name){
    llvm::FunctionType *func_type = llvm::FunctionType::get(Builder.getInt32Ty(), false);
    llvm::Function *func = llvm::Function::Create(
        func_type,
        llvm::Function::ExternalLinkage,
        Name,
        module
    );
    return func;
}
// 引用传递: 函数声明时参数用&context,调用时直接传入context
// BasicBlock* createBB(LLVMContext &context, Function *func, std::string Name){
//     return llvm::BasicBlock::Create(context, Name, func);  
// }

// 地址传递： 函数声明时参数用*context,调用时传入&context, 函数内部调用时需要解地址，用*context
BasicBlock* createBB(LLVMContext *context, Function *func, std::string Name){
    return llvm::BasicBlock::Create(*context, Name, func);  
}

int main(){
    LLVMContext context;
    llvm::Module *module = new llvm::Module("main", context);
    IRBuilder<> builder(context);
    llvm::Function *func = create_function(module, builder, "main");
    // 这里将context作为参数传递给函数时不能使用值传递，因为context删除了拷贝构造函数
    // 可以使用引用传递或者地址传递
    // 引用传递: 函数声明时参数用&context,调用时直接传入context
    // BasicBlock *entry = create_BB(context, func, "entry");
    // 地址传递： 函数声明时参数用*context,调用时传入&context, 函数内部调用时需要解地址，用*context
    BasicBlock *entry = createBB(&context, func, "entry");

    builder.SetInsertPoint(entry);
    verifyFunction(*func);

    module->dump();
    return 0;
    

}