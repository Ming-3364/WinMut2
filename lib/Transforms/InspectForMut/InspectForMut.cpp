#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace{
    struct InspectForMut : public ModulePass{
        static char ID;
        InspectForMut() : ModulePass(ID) {}

        bool runOnModule(Module &M) override {
            printf("inspect for mut");
            errs() << "InsepctForMut: ";
            errs().write_escaped(M.getName()) << '\n';
            return false;
        }
    }; // end of struct InspectForMut
}

char InspectForMut::ID = 0;
static RegisterPass<InspectForMut> X("inspectForMut", "Inspect For Mut Pass",
                                     false /* Only looks at CFG */,
                                     false /* Analysis Pass */);