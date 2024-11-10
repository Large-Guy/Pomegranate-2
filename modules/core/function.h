#ifndef ANYFUNCTIONS_FUNCTION_H
#define ANYFUNCTIONS_FUNCTION_H
#include <vector>
#include "type.h"

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

        template <typename T>
        void addArg() {
            TypeInfo param{};
            param.size = sizeof(T);
            param.type = typeid(T).hash_code();
            param.name = typeid(T).name();
            _parameters.push_back(param);
        }
    public:
        explicit FunctionImpl(FunctionType function) : _function(function) {
            (addArg<Args>(), ...);

            if constexpr (!std::is_void_v<Return>) {
                _return.size = sizeof(Return);          // Size of Return type
                _return.type = typeid(Return).hash_code();  // Type identifier hash
                _return.name = typeid(Return).name();       // Type name
            } else {
                _return.size = 0;          // No size for void type
                _return.type = typeid(void).hash_code();  // Use void's type hash
                _return.name = "void";     // Explicitly indicate "void"
            }
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
        return (*(FunctionImpl<Return,CallArgs...>*)_function).call(args...);
    }

    template<typename Return,typename...CallArgs>
    Return(*getFunction())(CallArgs...) {
        return ((FunctionImpl<Return,CallArgs...>*)_function)->getFunction();
    }
};


#endif //ANYFUNCTIONS_FUNCTION_H
