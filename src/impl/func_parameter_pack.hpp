#pragma once

namespace composite::_impl {

    template<typename>
    struct func_parameter_pack;

    template<typename Result, typename... Args>
    struct func_parameter_pack<Result (*)(Args...)> {
        std::tuple<Args...> params;
    };

}
