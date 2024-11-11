#include "function.h"

size_t Function::FunctionBase::getParameterCount() const {
    return _parameters.size();
}

const std::vector<TypeInfo>& Function::FunctionBase::getParameters() const {
    return _parameters;
}

TypeInfo Function::FunctionBase::getReturn() const {
    return _return;
}

Function::Function() : _function(nullptr) {}

Function::Function(const Function& function) {
    _function = function._function->clone();
}

Function::Function(Function::FunctionBase *function) {
    _function = function;
}

Function::~Function() {
    delete _function;
}

Function& Function::operator=(FunctionBase* function) {
    delete _function;
    _function = function;
    return *this;
}

Function& Function::operator=(const Function& function){
    if(this == &function)
        return *this;
    delete _function;
    _function = function._function->clone();
    return *this;
}

size_t Function::getParameterCount() const {
    return _function->getParameterCount();
}
const std::vector<TypeInfo>& Function::getParameters() const {
    auto& parameters = _function->getParameters();
    return parameters;
}

TypeInfo Function::getReturn() const {
    return _function->getReturn();
}