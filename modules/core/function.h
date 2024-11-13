#ifndef ANYFUNCTIONS_FUNCTION_H
#define ANYFUNCTIONS_FUNCTION_H
#include <vector>
#include "type.h"
#include "core/debug.h"

class Function {
public:
    class FunctionBase {
    protected:
        std::vector<TypeInfo> _parameters;
        TypeInfo _return;
    public:
        virtual ~FunctionBase() = default;
        virtual FunctionBase* clone() = 0;
        [[nodiscard]] size_t getParameterCount() const;
        [[nodiscard]] const std::vector<TypeInfo>& getParameters() const;
        [[nodiscard]] TypeInfo getReturn() const;
    };

    template<typename Return, typename...Args>
    class FunctionImpl : public FunctionBase {
        using FunctionType = Return(*)(Args...);
    private:
        FunctionType _function;

    public:
        explicit FunctionImpl(FunctionType function) : _function(function) {
            _parameters = std::vector<TypeInfo>();
            (_parameters.push_back(TypeInfo::get<Args>()), ...);

            _return = TypeInfo::get<Return>();
        }
        FunctionImpl(const FunctionImpl& function) : _function(function._function) {
            _parameters = function._parameters;
            _return = function._return;
        }
        ~FunctionImpl() override
        {
            _function = nullptr;
        }
        template<typename...CallArgs>
        Return call(CallArgs...args) {
            if(_function == nullptr)
            {
                Debug::Log::error("Function is null");
                return Return();
            }
            return ((Return(*)(CallArgs...))_function)(args...);
        }

        FunctionBase* clone() override {
            return new FunctionImpl<Return, Args...>(_function);
        }

        FunctionType getFunction() {
            return _function;
        }
    };

private:
    FunctionBase* _function;
public:
    explicit Function();
    explicit Function(const Function& function);
    explicit Function(FunctionBase* function);
    ~Function();

    Function& operator=(FunctionBase* function);
    Function& operator=(const Function& function);

    [[nodiscard]] size_t getParameterCount() const;
    [[nodiscard]] const std::vector<TypeInfo>& getParameters() const;
    [[nodiscard]] TypeInfo getReturn() const;

    template <typename Return, typename... Args, typename Callable>
    static Function create(Callable&& function) {
        return Function(new FunctionImpl<Return, Args...>(std::forward<Callable>(function)));
    }

    template<typename Return,typename...CallArgs>
    Return call(CallArgs...args) {
#ifdef DEBUG
        std::vector<TypeInfo> parameters = getParameters();
        std::vector<TypeInfo> callParameters = {TypeInfo::get<CallArgs>()...};

        if(parameters.size() != callParameters.size())
        {
            Debug::Log::error("Parameter count mismatch, expected:",parameters.size()," got:",callParameters.size());
        }

        for(size_t i = 0; i < std::min(parameters.size(),callParameters.size()); i++)
        {
            if(parameters[i].safeCompare(callParameters[i]))
            {
                Debug::Log::error("Parameter type mismatch, arg:",i);
            }
        }
#endif

        return (*(FunctionImpl<Return,CallArgs...>*)_function).call(args...);
    }

    template<typename Return,typename...CallArgs>
    Return(*getFunction())(CallArgs...) {
        return ((FunctionImpl<Return,CallArgs...>*)_function)->getFunction();
    }
};


#endif //ANYFUNCTIONS_FUNCTION_H