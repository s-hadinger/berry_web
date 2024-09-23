/********************************************************************
** Copyright (c) 2018-2024 Guan Wenliang & Stephan Hadinger
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#include "be_object.h"
#include "be_gc.h"
#include <string.h>

#include <emscripten.h>

#if BE_USE_JS_MODULE


EM_JS(int, _js_dispatch, (const char *arg_t, size_t arg_t_len, int32_t arg_i), {
    return berry_dispatch(UTF8ToString(arg_t, arg_t_len), arg_i);
})


static int m_dispatch(bvm *vm)
{
    int top = be_top(vm);
    const char * arg_t = "";
    int32_t arg_i = 0;
    if (top >= 1) {
        arg_t = be_tostring(vm, 1);
    }
    if (top >= 2) {
        arg_i = be_toint(vm, 2);
    }
    int32_t ret = _js_dispatch(arg_t, strlen(arg_t), arg_i);
    be_pushint(vm, ret);
    be_return(vm);
}

#if !BE_USE_PRECOMPILED_OBJECT
be_native_module_attr_table(gc){
    be_native_module_function("dispatch", m_dispatch),
};

be_define_native_module(gc, NULL);
#else
/* @const_object_info_begin
module js (scope: global, depend: BE_USE_JS_MODULE) {
    dispatch, func(m_dispatch)
}
@const_object_info_end */
#include "../generate/be_fixed_js.h"
#endif

#endif /* BE_USE_SYS_MODULE */
