#include "Tuple.h"
#include "Expr.h"
#include "Func.h"
#include "Var.h"

namespace FImage {

    Tuple::operator Expr() const {
        std::vector<Expr> callArgs;
        std::vector<Expr> definitionArgs;
        
        // Note on reduction vars captured: 
        // In the call to the anonymous function they're reduction
        // vars passed as arguments
        // In the definition of the anonymous function on the LHS and
        // RHS they're just regular vars with the same name

        Var tupleIndex;
        Expr body;
        int idx = 0;
                
        // Grab the vars and reduction vars in the tuple args as arguments to the anonymous function
        for (const Expr &expr : contents) {
            Expr e = expr;
            std::vector<std::string> done;
            for (size_t i = 0; i < e.vars().size(); i++) {
                bool already_exists = false;
                for (size_t j = 0; j < done.size(); j++) {
                    if (e.vars()[i].name() == done[j]) already_exists = true;
                }
                if (!already_exists) {
                    callArgs.push_back(e.vars()[i]);
                    done.push_back(e.vars()[i].name());
                }
            }
            for (size_t i = 0; i < e.rvars().size(); i++) {
                bool already_exists = false;
                for (size_t j = 0; j < done.size(); j++) {
                    if (e.rvars()[i].name() == done[j]) already_exists = true;
                }
                if (!already_exists) {
                    callArgs.push_back(e.rvars()[i]);
                    done.push_back(e.rvars()[i].name());
                }
            }
            
            e.convertRVarsToVars();
            done.clear();
            
            for (size_t i = 0; i < e.vars().size(); i++) {
                bool already_exists = false;
                for (size_t j = 0; j < done.size(); j++) {
                    if (e.vars()[i].name() == done[j]) already_exists = true;
                }
                if (!already_exists) {
                    definitionArgs.push_back(e.vars()[i]);
                    done.push_back(e.vars()[i].name());
                }
            }

            if (idx == 0) body = e;
            else body = Select(tupleIndex == idx, e, body);
            idx++;
        }
        
        definitionArgs.push_back(tupleIndex);
        Func anon;
        anon(definitionArgs) = body;
        return anon(callArgs);
    }
    
    Tuple operator,(const Expr &a, const Expr &b) {
        return Tuple(a, b);
    }
}
