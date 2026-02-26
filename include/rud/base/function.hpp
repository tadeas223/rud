#ifndef RUD_BASE_FUNCTION_HPP
#define RUD_BASE_FUNCTION_HPP

#include <algorithm>
#include <cstdlib>
#include <cstring>
namespace rud {
    template<typename>
    struct Function;

    template<typename R, typename ...Args>
    struct Function<R(Args...)> {
        struct _ICallable {
            R (*call)(void*, Args...);
        };

        template<typename F>
        struct _Callable : _ICallable{
            F p_func;
        };
        
        _ICallable p_icallable;
        void* p_callable;
        
        template<typename F>
        inline static Function<R(Args...)> make(F f) {
            _Callable<F>* callable = reinterpret_cast<_Callable<F>*>(malloc(sizeof(_Callable<F>)));
            memmove(&callable->p_func, &f, sizeof(F));
            
            _ICallable icallable = {[](void* self, Args... args){
                reinterpret_cast<_Callable<F>*>(self)->p_func(args...);
            }};

            return {icallable, static_cast<void*>(callable)};
        }

        inline R call(Args... args) {
            return p_icallable.call(p_callable, args...);
        }

        inline void destroy() {
            free(p_callable);
        }
    };
}

#endif
