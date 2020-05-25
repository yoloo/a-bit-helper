# 自制lua绑定库
Lua（英语发音：/ˈluːə/）是一个简洁、轻量、可扩展的程序设计语言，
由Roberto Ierusalimschy、Waldemar Celes 和 Luiz Henrique de Figueiredo所组成的研究小组于1993年开发而成，
其设计目的是为了嵌入应用程序中，从而为应用程序提供灵活的扩展和定制功能。

## 起因
c++作为编译型语言，我们修改一点点小代码， 编译一次的时间，可以起身抽根烟，遛个弯，说不定还能去看个电影。
lua是一种解释型语言，运行时才将代码放入解释器运行，热更新非常方便

所以大量的前端/后端 开始将lua作为一种内嵌的脚本语言，与c++相结合， 发挥他们各自的优势

将异变的，非规则化的代码，用lua来实现
将不容易变的，规则化的，需要计算性能的代码，由c++来实现
动静结合，十分强大啊

## lua与c的交互
lua与c之间的交互主要是通过 lua栈，c闭包函数，userdata 3大块内容来实现的

我们先来看几个函数与定义

```typedef int (*lua_CFunction) (lua_State *L);```
能够被Lua调用的C函数都必须是这种规则。函数的返回的int值表示C函数返回值的个数。

```void lua_pushcclosure (lua_State *L, lua_CFunction fn, int n);```
将一个C闭包压栈；
首先将upvalues依次压栈，然后调用该函数，将c函数压栈，并将n个upvalues出栈；
参数fn：C函数指针
参数n：函数关联的upvalue的个数。

```void lua_register (lua_State *L, const char *name, lua_CFunction f);```
注册C函数为一个全局变量；
```#define lua_register(L,n,f) (lua_pushcfunction(L, f), lua_setglobal(L, n))```



我们再来看看这个lua_CFuntion是怎么与lua栈交互的
```
extern "C" int add(lua_State* L) 
{
    double op1 = luaL_checknumber(L,1);
    double op2 = luaL_checknumber(L,2);
    lua_pushnumber(L,op1 + op2);
    return 1;
}
extern "C" int sub(lua_State* L)
{
    double op1 = luaL_checknumber(L,1);
    double op2 = luaL_checknumber(L,2);
    lua_pushnumber(L,op1 - op2);
    return 1;
}
static luaL_Reg mylibs[] = { 
    {"add", add},
    {"sub", sub},
    {NULL, NULL} 
};
extern "C" __declspec(dllexport)
int luaopen_mytestlib(lua_State* L) 
{
    const char* libName = "mytestlib";
    luaL_register(L,libName,mylibs);
    return 1;
}
```

API有一系列的压栈函数，为了将C值转换成栈顶的Lua值，我们为每种类型定义一个对应的函数：
```
void lua_pushnil(Lua_State *L);
void lua_pushboolean(Lua_State *L,int bool);
void lua_pushnumber(Lua_State *L,double n);
void lua_pushstring(Lua_State*L,const char*s);
void lua_pushuserdata(Lua_State*L,void*p);
```
API有一系列的从栈中获取值的函数，为了将栈中的转换C值成Lua值
```
bool lua_toboolean(Lua_State*L,int idx);
int lua_tonumber(Lua_State*L,int idx);
const char* lua_tostring(Lua_State*L,int idx,size_t *len);
void* lua_touserdata(Lua_State*L,int idx);
```
Lua API还提供了一套lua_is*来检查一个元素是否是一个指定的类型
check系列函数就是先判断is,再来取值
```
int lua_isboolean(Lua_State*L,int idx);
int lua_isnumber(Lua_State*L,int idx);
int lua_isstring(Lua_State*L,int idx);
```
此外还提供了API函数来人工控制堆栈:
```
int lua_gettop(luaState *L);
void lua_settop(luaState *L,int idx);
void lua_pushvalue(luaState *L,int idx);
void lua_remove(luaState *L,int idx);
void lua_insert(luaState *L,int idx);
void lua_replace(luaState *L,int idx);
```


## 向lua注册

### 全局函数
我们希望能够直接向lua中注册c++的全局函数，且不需要手写lua_CFunction来检查各种参数
所以我们需要设计了一个通用的lua_CFunction

假设我们需要注册一个c++的全局函数
`bool IsSigned(int val);`

我们先定义一个warp函数
```
int Warp(lua_State* L)
{
    auto param1 = get_param_fom_lua_stack<int>(0);
    auto result = IsSigned(param1);
    push_result_to_lua<bool>( result );
    return 1;
}
```
我们发现好像可以根据函数的签名中的参数个数/类型， 返回类型， 来推导出一个模板函数
```
template<R,Param1,Param2...ParamN>
int Warp(lua_State* L)
{
    using FuncT = R(Param1,Param2,...,ParamN);
    auto func = (FuncT*)get_val_from_lua("IsSigned");
    auto result = func(get_param_fom_lua_stack<Param1Type>(0), 
                        get_param_fom_lua_stack<Param2Type>(1), 
                        get_param_fom_lua_stack<Param3Type>(2),
                        ... 
                        get_param_fom_lua_stack<ParamNType>(N-1));
    push_result_to_lua<R>( result );
    return result_size;
}
```

这个好像很容易通过C++11/14/17的 **可变参数模板**（Variadic Template）来实现


```
template<typename Func, typename ... DefaultArgs>
void def(lua_State* L, const char* name, Func&& func)
{
    _push_functor(L, std::forward<Func>(func));
    lua_setglobal(L, name);
}
template<typename R, typename ...ARGS>
void _push_functor(lua_State* L, R(func)(ARGS...))
{
    using Functor_Warp = functor<R, ARGS...>;
    lua_pushlightuserdata(L, (void*)func);  //把c++函数作为upval存入c闭包中
    lua_pushcclosure(L, &Functor_Warp::invoke, 1);
}
```

当我们实现这个通用回调的时候， 我们只需要考虑， 
1. 返回值
2. 有几个参数
3. 参数类型是什么
4. 如何依次将参数从lua的栈上取得
5. 如何将c++函数调用的结果push到lua的栈上
   

```
template<typename T>
T upvalue_(lua_State *L)
{
    return user2type<T>(L, lua_upvalueindex(1));
}

//direct invoke func
template<int nIdxParams, typename RVal, typename Func, typename ...Args, std::size_t... index>
RVal direct_invoke_invoke_helper(Func&& func, lua_State *L, std::index_sequence<index...>)
{
    return stdext::invoke(std::forward<Func>(func), read<Args>(L, index + nIdxParams)...);
}

template<int nIdxParams, typename RVal, typename Func, typename ...Args>
RVal direct_invoke_func(Func&& func, lua_State *L)
{
    return direct_invoke_invoke_helper<nIdxParams, RVal, Func, Args...>(std::forward<Func>(func), L, std::make_index_sequence<sizeof...(Args)>{});
}

template <typename RVal, typename ... Args>
struct functor
{
    static int invoke(lua_State *L)
    {
        //从upval上取第一个，变为函数指针
        FuncWarpType* pFuncWarp = upvalue_<FuncWarpType*>(L);
        //调用
        _invoke<RVal>(L);
        return 1;     
    }

    template<typename T, typename F>
    static void _invoke_function(lua_State* L, F&& func)
    {
        if constexpr(!std::is_void<T>::value)
        {
            auto rv = direct_invoke_func<1, RVal, FunctionType, Args...>(std::forward<FunctionType>(func), L);
            push_rv<RVal>(L, std::move(rv));
        }
        else
        {
            direct_invoke_func<1, RVal, FunctionType, Args...>(std::forward<FunctionType>(func), L);
        }
    }
}
```
**因为if constexpr，只在c++17被支持**
**所有if constexpr函数可以通过enable_if实现SFINAE**

接下来我们需要实现如何与lua的栈打交道
我们要来实现read_from_lua_stack和push_val_to_lua_stack
```
//read_weap
template<typename T>
auto read(lua_State *L, int index)
    ->decltype(_stack_help<T>::_read(L, index))
{
    return _stack_help<T>::_read(L, index);
}

//push warp
template<typename T>
void push(lua_State *L, T ret)
{
    _stack_help<T>::_push(L, std::forward<T>(ret));
}
```

然后我们就可以来实现不同类型的stack_help类了

```
// lua stack help to read/push
template<typename T, typename Enable>
struct _stack_help
{
    static T _read(lua_State *L, int index)
    {
        return lua2type<T>(L, index);
    }

    //get userdata ptr from lua, can handle nil an 0
    template<typename _T>
    static typename std::enable_if<std::is_pointer<_T>::value, _T>::type lua2type(lua_State *L, int index)
    {
        if (lua_isnoneornil(L, index))
        {
            return nullptr;
        }
        else if (lua_isnumber(L, index) && lua_tonumber(L, index) == 0)
        {
            return nullptr;
        }
        return _lua2type<_T>(L, index);
    }

    //get userdata from lua 
    template<typename _T>
    static typename std::enable_if<!std::is_pointer<_T>::value, _T>::type lua2type(lua_State *L, int index)
    {
        return _lua2type<_T>(L, index);
    }

    
    //obj to lua
    template<typename _T>
    static void _push(lua_State *L, _T&& val)
    {
        _type2lua(L, std::forward<_T>(val));
    }

};

template<>
struct _stack_help<char*>
{
    static char* _read(lua_State *L, int index);
    static void  _push(lua_State *L, char* ret);
};
//integral
template<typename T>
struct _stack_help<T, typename std::enable_if<std::is_integral<T>::value>::type>
{
    static T _read(lua_State *L, int index)
    {
        return (T)lua_tointeger(L, index);
    }
    static void  _push(lua_State *L, T ret)
    {
        lua_pushinteger(L, ret);
    }
};
//float pointer
template<typename T>
struct _stack_help<T, typename std::enable_if<std::is_floating_point<T>::value>::type>
{
    static T _read(lua_State *L, int index)
    {
        return (T)lua_tonumber(L, index);
    }
    static void  _push(lua_State *L, T ret)
    {
        lua_pushnumber(L, ret);
    }
};
template<>
struct _stack_help<std::string>
{
    static std::string _read(lua_State *L, int index);
    static void _push(lua_State *L, const std::string& ret);
};
template<>
struct _stack_help<const std::string&> : public _stack_help<std::string>
{
};
//enum
template<typename T>
struct _stack_help<T, typename std::enable_if<std::is_enum<T>::value>::type>
{
    static constexpr int cover_to_lua_type() { return CLT_INT; }
    static T _read(lua_State *L, int index)
    {
        return (T)lua_tointeger(L, index);
    }
    static void  _push(lua_State *L, T ret)
    {
        lua_pushinteger(L, (int)ret);
    }
};

```

_stack_help::_read/_push实现了从c++类型->lua_value的过程
那么lua不认识的那些类型怎么办？  比如指针/引用/某个struct-class
让我们来看看 _type2lua / _lua2type的实现

先来看push2lua
```
// to lua
// userdata pointer to lua 
template<typename T>
typename std::enable_if<std::is_pointer<T>::value, void>::type object2lua(lua_State *L, T&& input)
{
    //place new
    if (input) 
        new(lua_newuserdata(L, sizeof(ptr2user<base_type<T>>))) ptr2user<base_type<T>>(std::forward<T>(input)); 
    else 
        lua_pushnil(L);
}
// userdata reference to lua
template<typename T>
typename std::enable_if<std::is_reference<T>::value, void>::type object2lua(lua_State *L, T&& input)
{
    //place new
    new(lua_newuserdata(L, sizeof(ref2user<T>))) ref2user<T>(std::forward<T>(input));
}
// userdata val to lua
template<typename T>
typename std::enable_if<!std::is_pointer<T>::value && !std::is_reference<T>::value, void>::type object2lua(lua_State *L, T&& input)
{
    //place new
    new(lua_newuserdata(L, sizeof(val2user<T>))) val2user<T>(std::forward<T>(input));
}
template<typename _T>
static void _type2lua(lua_State *L, _T&& val)
{
    object2lua(L, std::forward<_T>(val));   //将对象推入lua
    push_meta(L, get_class_name<_T>());     //找到对应meta表
    lua_setmetatable(L, -2);                //设置meta表
}
```

再来看read4lua
```
// param to pointer
template<typename T>
typename std::enable_if<std::is_pointer<T>::value, T>::type void2type(void* ptr)
{
    return (base_type<T>*)ptr;
}
//to reference
template<typename T>
typename std::enable_if<std::is_reference<T>::value, base_type<T>&>::type void2type(void* ptr)
{
    return *(base_type<T>*)ptr;
}

//to val
template<typename T>
typename std::enable_if<!std::is_pointer<T>::value && !std::is_reference<T>::value, base_type<T>>::type void2type(void* ptr)
{
    return *(base_type<T>*)ptr;
}
//userdata to T，T*，T&
template<typename T>
T user2type(lua_State *L, int index)
{
    return void2type<T>(lua_touserdata(L, index));
}
template<typename _T>
static _T _lua2type(lua_State *L, int index)
{
    if (!lua_isuserdata(L, index))
    {
        lua_pushfstring(L, "can't convert argument %d to class %s", index, get_class_name<_T>());
        lua_error(L);
        
    }
    UserDataWapper* pWapper = user2type<UserDataWapper*>(L, index);
    UserDataCheckType<_T>(pWapper,L,index);
    return void2type<_T>(pWapper->m_p);
}
```



```
//userdata holder
struct UserDataWapper
{
    template<typename T>
    explicit UserDataWapper(T* p)
        : m_p(p)
    {}

    //push a const to lua will lost constant qualifier
    template<typename T>
    explicit UserDataWapper(const T* p)
        : m_p(const_cast<T*>(p))
    {}

    virtual ~UserDataWapper() {}

    void* m_p;
};

template<typename T>
struct val2user : UserDataWapper
{
    val2user() : UserDataWapper(new T) { }
    val2user(const T& t) : UserDataWapper(new T(t)) {}
    val2user(T&& t) : UserDataWapper(new T(std::forward<T>(t))) {}
    val2user(lua_State* L, class_tag<void> tag)
        : val2user()
    {}
    ~val2user() { delete ((T*)m_p); }

};

template<typename T>
struct ptr2user : UserDataWapper
{
    ptr2user(T* t) : UserDataWapper(t) {}
    ptr2user(const T* t) : UserDataWapper(t) {}

};

template<typename T>
struct ref2user : UserDataWapper
{
    ref2user(T& t) : UserDataWapper(&t) {}

};
```


我们已经准备好了如何向lua push/read任何数据类型 
同时我们可以 push/read c++函数了

到这里我们已经可以向lua注册全局函数了
使用的时候如下
```
script::def(L, "somefunc", &somefunc);
```

我们将一个c++函数指针，放入一个通用交互函数的c闭包的第一个upval中，然后将这个c闭包放入到lua的全局表中，给他一个名字
那么在lua中调用
```
result = somefunc(a,b,c,d);
```






### 全局变量

前面我们已经实现了c++类型与luaValue的交互
那么实现一个全局变量就非常容易了
```
template<typename T>
void set(lua_State* L, const char* name, T&& object)
{
    push(L, std::forward<T>(object));
    lua_setglobal(L, name);
}

template<typename T>
T get(lua_State* L, const char* name)
{
    lua_getglobal(L, name);
    return pop<T>::apply(L);
}
```

```
// pop a value from lua stack
template<typename T>
struct pop
{
    static constexpr const int nresult = 1;
    static T apply(lua_State *L) 
    { 
        stack_delay_pop  _dealy(L, nresult); 
        return read_nocheck<T>(L, -1); 
    }
};

template<typename ...TS>
struct pop< std::tuple<TS...> >
{
    static constexpr const int nresult = sizeof...(TS);

    static std::tuple<TS...> apply(lua_State* L)
    {
        stack_delay_pop  _dealy(L, nresult);
        return apply_help(L, std::make_index_sequence<nresult>{});
    }

    template<std::size_t... index>
    static std::tuple<TS...> apply_help(lua_State *L, std::index_sequence<index...>)
    {
        return std::make_tuple(read_nocheck<TS>(L, (int)(index - nresult))...);
    }
};

template<>
struct pop<void>
{
    static constexpr const int nresult = 0;
    static void apply(lua_State *L);
};
```


我们在c++中可以这样注册
```
script::set(L, "name",  &g_value);
```

也可以这样获得一个全局变量
```
auto g_value_copy = script::get(L, "name");
```

### 类

#### lua原表
在实现类相关注册之前，我们需要先来了解一下lua的原表

Lua 中的每个值都可以有一个 元表。 这个 元表 就是一个普通的 Lua 表， 它用于定义原始值在特定操作下的行为。 如果你想改变一个值在特定操作下的行为，你可以在它的元表中设置对应域。 例如，当你对非数字值做加操作时， Lua 会检查该值的元表中的 "__add" 域下的函数。 如果能找到，Lua 则调用这个函数来完成加这个操作。

在元表中事件的键值是一个双下划线（__）加事件名的字符串； 键关联的那些值被称为 元方法。 在上一个例子中，__add 就是键值， 对应的元方法是执行加操作的函数。

你可以用 getmetatable 函数 来获取任何值的元表。 Lua 使用直接访问的方式从元表中查询元方法（参见rawget）。 所以，从对象 o 中获取事件 ev 的元方法等价于下面的代码：

     rawget(getmetatable(o) or {}, "__ev")
你可以使用 setmetatable 来替换一张表的元表。在 Lua 中，你不可以改变表以外其它类型的值的元表 （除非你使用调试库（参见§6.10））； 若想改变这些非表类型的值的元表，请使用 C API。

表和完全用户数据有独立的元表 （当然，多个表和用户数据可以共享同一个元表）。 其它类型的值按类型共享元表； 也就是说所有的数字都共享同一个元表， 所有的字符串共享另一个元表等等。 默认情况下，值是没有元表的， 但字符串库在初始化的时候为字符串类型设置了元表 （参见 §6.4）。

元表决定了一个对象在数学运算、位运算、比较、连接、 取长度、调用、索引时的行为。 元表还可以定义一个函数，当表对象或用户数据对象在垃圾回收 （参见§2.5）时调用它。

对于一元操作符（取负、求长度、位反）， 元方法调用的时候，第二个参数是个哑元，其值等于第一个参数。 这样处理仅仅是为了简化 Lua 的内部实现 （这样处理可以让所有的操作都和二元操作一致）， 这个行为有可能在将来的版本中移除。 （使用这个额外参数的行为都是不确定的。）

接下来是元表可以控制的事件的详细列表。 每个操作都用对应的事件名来区分。 每个事件的键名用加有 '__' 前缀的字符串来表示； 例如 "add" 操作的键名为字符串 "__add"。。

|元方法|操作|
|:-|:-|
|add| +|
|sub| - |
|mul| *|
|div| /|
|mod| %|
|pow| ^|
|unm| 一元操作-|
|concat| 连接符...|
|len| 长度|
|eq| ==|
|lt| <|
|le| <=|
|index|取下标操作用于访问 table[key]|
|newindex|赋值给指定下标 table[key] = value|
|call|当 Lua 调用一个值时调用|
|gc|垃圾回收回调|
|idiv|//向下取整除法|
|band| & （按位与）操作|
|bor| \| （按位或）操作|
|bxor| ~ （按位异或）操作|
|bnot| ~ （按位非）操作|
|shl| << （左移）操作|
|shr| >> （右移）操作|

这样我们就可以给一个lua对象一个meta表来实现一个类似c++类的功能

```
template<typename T>
void class_add(lua_State* L, const char* name)
{
    detail::class_name<T>::name(name);
    lua_createtable(L, 0, 4);

    lua_pushstring(L, "__name");
    lua_pushstring(L, name);
    lua_rawset(L, -3);

    lua_pushstring(L, "__index");
    lua_pushcclosure(L, detail::meta_get, 0);
    lua_rawset(L, -3);

    lua_pushstring(L, "__newindex");
    lua_pushcclosure(L, detail::meta_set, 0);
    lua_rawset(L, -3);

    lua_pushstring(L, "__gc");
    lua_pushcclosure(L, detail::destroyer<detail::UserDataWapper>, 0);
    lua_rawset(L, -3);

    lua_setglobal(L, name);
}
```
```
template<typename T>
struct class_name
{
    // global name
    static const char* name(const char* name = NULL)
    {
        return name_str(name).c_str();
    }
    static const std::string& name_str(const char* name = NULL)
    {
        static std::string s_name;
        if (name != NULL) s_name.assign(name);
        return s_name;
    }
};
```


这样我们就能通过
```
script::class_add<CUser>(L, "CUser");
```
来向lua注册一个类



#### 类成员函数

我们可以向meta表中继续push key=函数名 value=函数warp的方式来注册我们的类成员函数
```
// Tinker Class Functions
template<typename T, typename Func>
void class_def(lua_State* L, const char* name, Func&& func)
{
    using namespace detail;
    stack_scope_exit scope_exit(L);
    if (push_meta(L, get_class_name<T>()) == LUA_TTABLE)
    {
        //register functor
        lua_pushstring(L, name);
        _push_class_functor(L, std::forward<Func>(func));
        lua_rawset(L, -3);
    }
}
template<typename T, typename Func>
void class_def_static(lua_State* L, const char* name, Func&& func)
{
    using namespace detail;
    stack_scope_exit scope_exit(L);
    if (push_meta(L, get_class_name<T>()) == LUA_TTABLE)
    {
        //register functor
        lua_pushstring(L, name);
        _push_functor(L, std::forward<Func>(func));
        lua_rawset(L, -3);
    }
}
```

当然因为类成员函数在被inovke时，需要第一个参数是class_ptr,我们的warp函数也需要做一些特殊的修改，但这问题不大

这样我们就可以在c++中这样注册成员函数
```
script::class_def<CUser>(L, "Login", CUser::Login);
```

在lua中这样调用

```
function OnAccountSucc(pUser)
    pUser:Login(xxx,xxx);
end
```

注意这里我们在调用时使用的修饰符是 ":", 这个会把pUser作为第一个参数压入栈中，就像是c++类函数那样
如果我们向调用一个静态类成员函数，那么就应该是`pUser.SomeStaticFunc(xxx); `

#### 类成员变量

```
// Tinker Class Variables
template<typename T, typename BASE, typename VAR>
void class_mem(lua_State* L, const char* name, VAR BASE::*val)
{
    using namespace detail;
    stack_scope_exit scope_exit(L);
    if (push_meta(L, get_class_name<T>()) == LUA_TTABLE)
    {
        lua_pushstring(L, name);
        new(lua_newuserdata(L, sizeof(mem_var<BASE, VAR>))) mem_var<BASE, VAR>(val);
        lua_rawset(L, -3);
    }
}
```
```
// member variable
struct var_base
{
    virtual ~var_base() {};
    virtual void get(lua_State *L) = 0;
    virtual void set(lua_State *L) = 0;
};

template<typename T, typename V>
struct mem_var : var_base
{
    V T::*_var;
    mem_var(V T::*val) : _var(val) {}
    virtual void get(lua_State *L) override
    {
        push(L, _read_classptr_from_index1<T, true>(L)->*(_var));
    }
    virtual void set(lua_State *L) override
    {
        _read_classptr_from_index1<T, false>(L)->*(_var) = read<V>(L, 3);
    }
};
```

如此类成员变量就容易多了
我们还可以扩展出
static_mem_var 静态成员变量
member_property 用一对get/set函数来模拟property


### 能不能更强~~~~
当然， 我们还可以继续扩充我们绑定库

### 继承可以吗?
c++的继承方式有多种， 总体归纳来说分为, 单重继承， 多重继承， 虚继承 以及他们的结合
虚继承一般情况下是不会使用，也不被允许使用的，那么我们就只需要考虑 单重继承， 多重继承
单重继承最简单，只要我们在index/newindex时 查找自己的meta表发现没有找到该函数时，向自己的parent节点继续查找即可
我们只需要在meta表中插入一个parent的key，指向父类的Meta表即可

多重继承，有两种方式去查找， 广度优先/深度优先  这个就看大家自己的喜好了，甚至你都可以不去支持多重继承

```
// Tinker Class Inheritance
template<typename T, typename P>
void class_inh(lua_State* L)
{
    using namespace detail;
    stack_scope_exit scope_exit(L);
    if (push_meta(L, get_class_name<T>()) == LUA_TTABLE)
    {
        stack_obj class_meta = stack_obj::get_top(L);
#ifndef LUATINKER_MULTI_INHERITANCE
        lua_pushstring(L, "__parent");
        push_meta(L, get_class_name<P>());
        class_meta.rawset();// set class_meta["__parent"] = __parent
#else
        stack_obj parent_table = class_meta.rawget("__parent");
        if (parent_table.is_nil())
        {
            parent_table.remove();
            lua_pushstring(L, "__parent");
            push_meta(L, get_class_name<P>());
            class_meta.rawset();// set class_meta["__parent"] = __parent
        }
        else
        {
            stack_obj parent_table = class_meta.rawget("__multi_parent");
            if (parent_table.is_nil())
            {
                parent_table.remove();
                lua_pushstring(L, "__multi_parent");
                lua_createtable(L, 1, 0);
                parent_table = stack_obj::get_top(L);
                {
                    push_meta(L, get_class_name<P>());
                    parent_table.rawseti(1);// set __multi_parent[1]=table
                }
                class_meta.rawset();// set class_meta["__multi_parent"] = __multi_parent
            }
            else
            {
                int nLen = parent_table.get_rawlen() + 1;
                push_meta(L, get_class_name<P>());
                parent_table.rawseti(nLen); // set __multi_parent[n]=table
                parent_table.remove();//pop __multi_parent table
            }
        }
#endif

    }

}
```

### stl容器可以吗?
Ofcourse.

因为容器本身一般占用的内存比较大， 如果直接转为table/从table转换， 会带来大量的性能损耗
这里我们用enable_if， 判断，发现stack_help的对象是容器类型的时候， 我们传入一个container_meta表


```
template<typename _T>
void _type2lua(lua_State* L, _T&& val)
{
    object2lua(L, std::forward<_T>(val));
    if(get_class_name<_T>() == std::string(""))
    {
        if constexpr(!std::is_same<base_type<_T>, std::string>::value && is_container<base_type<_T>>::value)
        {
            // container warp
            add_container_mate<base_type<_T>>(L);
            push_meta(L, get_class_name<_T>());
            lua_setmetatable(L, -2);
        }
        else if(!std::is_reference<_T>::value && !std::is_pointer<_T>::value)
        {
            // val2usr must add _gc
            push_meta(L, "__onlygc_meta");
            lua_setmetatable(L, -2);
        }
    }
    else
    {
        push_meta(L, get_class_name<_T>());
        lua_setmetatable(L, -2);
    }
}
```
```
template<typename T>
void add_container_mate(lua_State* L)
{
    std::string name = std::string("container_") + std::to_string(get_type_idx<base_type<T>>());
    detail::class_name<base_type<T>>::name(name.c_str());
    lua_createtable(L, 0, 8);

    lua_pushstring(L, "__name");
    lua_pushstring(L, name.c_str());
    lua_rawset(L, -3);

    lua_pushstring(L, "__index");
    lua_pushcclosure(L, detail::meta_container_get<base_type<T>>, 0);
    lua_rawset(L, -3);

    lua_pushstring(L, "__newindex");
    lua_pushcclosure(L, detail::meta_container_set<base_type<T>>, 0);
    lua_rawset(L, -3);

    lua_pushstring(L, "__pairs");
    lua_pushcclosure(L, detail::meta_container_make_range<base_type<T>>, 0);
    lua_rawset(L, -3);

    lua_pushstring(L, "__len");
    lua_pushcclosure(L, detail::meta_container_get_len<base_type<T>>, 0);
    lua_rawset(L, -3);

    lua_pushstring(L, "to_table");
    lua_pushcclosure(L, detail::meta_container_to_table<base_type<T>>, 0);
    lua_rawset(L, -3);

    lua_pushstring(L, "push");
    lua_pushcclosure(L, detail::meta_container_push<base_type<T>>, 0);
    lua_rawset(L, -3);

    lua_pushstring(L, "erase");
    lua_pushcclosure(L, detail::meta_container_erase<base_type<T>>, 0);
    lua_rawset(L, -3);

    lua_pushstring(L, "__gc");
    lua_pushcclosure(L, detail::destroyer<detail::UserDataWapper>, 0);
    lua_rawset(L, -3);

    lua_setglobal(L, name.c_str());
}
```


具体实现， 有时间可以看

```
// stl helper
template<typename T>
int32_t meta_container_get(lua_State* L)
{
    stack_obj class_obj(L, 1);
    stack_obj key_obj(L, 2);
    stack_obj class_meta = class_obj.get_metatable();
    stack_obj val_obj    = class_meta.rawget(key_obj);
    if(val_obj.is_nil())
    {
        // val = list[key]
        UserDataWapper* pWapper    = user2type<UserDataWapper*>(L, 1);
        T*              pContainer = (T*)(pWapper->m_p);
        if constexpr(is_associative_container<T>::value)
        {
            // k,v
            auto it = pContainer->find(detail::read<typename T::key_type>(L, 2));
            if(it == pContainer->end())
            {
                lua_pushnil(L);
            }
            else
            {
                detail::push(L, it->second);
            }
        }
        else if constexpr(!is_associative_container<T>::value && has_key_type<T>::value)
        {
            // set
            auto it = pContainer->find(detail::read<typename T::value_type>(L, 2));
            if(it == pContainer->end())
            {
                lua_pushnil(L);
            }
            else
            {
                detail::push(L, *it);
            }
        }
        else
        {
            // vector

            int32_t key = detail::read<int32_t>(L, 2);
            key -= 1;
            if(key > (int32_t)pContainer->size())
            {
                lua_pushnil(L);
            }
            else
            {
                detail::push(L, pContainer->at(key));
            }
        }
    }
    return 1;
}

template<typename T>
int32_t meta_container_set(lua_State* L)
{
    // list[key] = val;
    UserDataWapper* pWapper    = user2type<UserDataWapper*>(L, 1);
    T*              pContainer = (T*)(pWapper->m_p);
#ifdef LUATINKER_USERDATA_CHECK_CONST
    if(pWapper->is_const())
    {
        lua_pushfstring(L, "container is const");
        lua_error(L);
    }
#endif
    if constexpr(is_associative_container<T>::value)
    {
        // k,v
        (*pContainer)[detail::read<typename T::key_type>(L, 2)] = detail::read<typename T::mapped_type>(L, 3);
    }
    else if constexpr(!is_associative_container<T>::value && has_key_type<T>::value)
    {
        // set
        pContainer->insert(detail::read<typename T::value_type>(L, 2));
    }
    else
    {
        // vector
        int32_t key = detail::read<int32_t>(L, 2);
        key -= 1;
        if(key > (int32_t)pContainer->size())
        {
            lua_pushfstring(L, "set to vector : %d out of range", key);
            lua_error(L);
        }
        else
        {
            (*pContainer)[key] = detail::read<typename T::value_type>(L, 3);
        }
    }

    return 0;
}

template<typename T>
int32_t meta_container_push(lua_State* L)
{
    // list[key] = val;
    UserDataWapper* pWapper    = user2type<UserDataWapper*>(L, 1);
    T*              pContainer = (T*)(pWapper->m_p);
#ifdef LUATINKER_USERDATA_CHECK_CONST
    if(pWapper->is_const())
    {
        lua_pushfstring(L, "container is const");
        lua_error(L);
    }
#endif
    if constexpr(is_associative_container<T>::value)
    {
        // k,v
        pContainer->emplace(detail::read<typename T::key_type>(L, 2),
                            detail::read<typename T::mapped_type>(L, 3));
    }
    else if constexpr(!is_associative_container<T>::value && has_key_type<T>::value)
    {
        // set
        pContainer->emplace(detail::read<typename T::value_type>(L, 2));
    }
    else if constexpr(has_allocator_type<T>::value)
    {
        // vector or string
        pContainer->emplace_back(detail::read<typename T::value_type>(L, 2));
    }
    else
    {
        // array
    }

    return 0;
}

template<typename T>
int32_t meta_container_erase(lua_State* L)
{
    // list[key] = val;
    UserDataWapper* pWapper    = user2type<UserDataWapper*>(L, 1);
    T*              pContainer = (T*)(pWapper->m_p);
#ifdef LUATINKER_USERDATA_CHECK_CONST
    if(pWapper->is_const())
    {
        lua_pushfstring(L, "container is const");
        lua_error(L);
    }
#endif
    if constexpr(is_associative_container<T>::value)
    {
        // k,v
        pContainer->erase(detail::read<typename T::key_type>(L, 2));
    }
    else if constexpr(!is_associative_container<T>::value && has_key_type<T>::value)
    {
        // set
        pContainer->erase(detail::read<typename T::value_type>(L, 2));
    }
    else if constexpr(has_allocator_type<T>::value)
    {
        // vector or string
        pContainer->erase(
            std::find(pContainer->begin(), pContainer->end(), detail::read<typename T::value_type>(L, 2)));
    }
    else
    {
        // maybe std::array
    }

    return 0;
}

template<typename T>
struct lua_iterator
{
    using Iter = typename base_type<T>::iterator;

    lua_iterator(Iter beg, Iter end)
        : m_Iter(std::move(beg))
        , m_End(std::move(end))
    {
    }
    bool HasMore() const { return m_Iter != m_End; }
    void MoveNext()
    {
        m_Iter++;
        m_nMoveDistance++;
    }
    static int32_t Next(lua_State* L)
    {
        UserDataWapper*  pWapper = user2type<UserDataWapper*>(L, 1);
        lua_iterator<T>* pIter   = (lua_iterator<T>*)(pWapper->m_p);
        if(pIter->HasMore() == false)
        {
            lua_pushnil(L);
            return 1;
        }
        if constexpr(is_associative_container<T>::value)
        {
            detail::push(L, pIter->m_Iter->first);
            detail::push(L, pIter->m_Iter->second);
            pIter->MoveNext();
            return 2;
        }
        else
        {
            detail::push(L, pIter->m_nMoveDistance);
            detail::push(L, *(pIter->m_Iter));
            pIter->MoveNext();
            return 2;
        }
    }
    size_t m_nMoveDistance = 1;
    Iter   m_Iter;
    Iter   m_End;
};

template<typename T>
int32_t meta_container_make_range(lua_State* L)
{
    UserDataWapper* pWapper    = user2type<UserDataWapper*>(L, 1);
    T*              pContainer = (T*)(pWapper->m_p);
    lua_pushcfunction(L, &lua_iterator<base_type<T>>::Next);
    detail::push(L, lua_iterator<decltype(*pContainer)>(pContainer->begin(), pContainer->end()));
    lua_pushnil(L);
    return 3;
}

template<typename T>
int32_t meta_container_get_len(lua_State* L)
{
    UserDataWapper* pWapper    = user2type<UserDataWapper*>(L, 1);
    T*              pContainer = (T*)(pWapper->m_p);
    detail::push(L, pContainer->size());
    return 1;
}

template<typename T>
int32_t meta_container_to_table(lua_State* L)
{
    UserDataWapper* pWapper    = user2type<UserDataWapper*>(L, 1);
    T*              pContainer = (T*)(pWapper->m_p);

    _pushtotable(L, *pContainer);
    return 1;
}
```

### shared_ptr/weak_ptr可以吗?
Absolutely.

我们可以储存任何类型的对象，shared_ptr/weak_ptr更是不在话下
通过使用对应的stack_help,我们完成对shared_ptr/weak_ptr的支持

```
template<typename T>
struct _stack_help<std::shared_ptr<T>>
{
    static std::shared_ptr<T> _read(lua_State* L, int32_t index)
    {
        if(!lua_isuserdata(L, index))
        {
            lua_pushfstring(L, "can't convert argument %d to class %s", index, get_class_name<T>());
            lua_error(L);
        }

        UserDataWapper* pWapper = user2type<UserDataWapper*>(L, index);
        if(pWapper->isSharedPtr() == false)
        {
            lua_pushfstring(L, "can't convert argument %d to class %s", index, get_class_name<T>());
            lua_error(L);
        }

        if(pWapper->haveOwership())
        {
            sharedptr2user<T>* pSharedWapper = static_cast<sharedptr2user<T>*>(pWapper);
            return pSharedWapper->m_holder;
        }
        else
        {
            weakptr2user<T>* pSharedWapper = static_cast<weakptr2user<T>*>(pWapper);
            return pSharedWapper->m_holder.lock();
        }
    }
    // shared_ptr to lua
    static void _push(lua_State* L, std::shared_ptr<T>&& val)
    {
        if(val)
        {
            if(val.use_count() == 1) // last count,if we didn't hold it, it will lost
            {
                new(lua_newuserdata(L, sizeof(sharedptr2user<T>)))
                    sharedptr2user<T>(std::forward<std::shared_ptr<T>>(val));
            }
            else
            {
                new(lua_newuserdata(L, sizeof(weakptr2user<T>))) weakptr2user<T>(val);
            }
        }
        else
            lua_pushnil(L);

        push_meta(L, get_class_name<std::shared_ptr<T>>());
        lua_setmetatable(L, -2);
    }
};
template<typename T>
struct _stack_help<const std::shared_ptr<T>&> : public _stack_help<std::shared_ptr<T>>
{
    static void _push(lua_State* L, const std::shared_ptr<T>& val)
    {
        if(val)
        {
            // if (val.use_count() == 1)	//last count,if we didn't hold it, it will lost
            //{
            //	new(lua_newuserdata(L, sizeof(sharedptr2user<T>))) sharedptr2user<T>(val);
            //}
            // else
            {
                new(lua_newuserdata(L, sizeof(weakptr2user<T>))) weakptr2user<T>(val);
            }
        }
        else
            lua_pushnil(L);

        push_meta(L, get_class_name<std::shared_ptr<T>>());
        lua_setmetatable(L, -2);
    }
};

```
数据存储
```
template<typename T>
struct weakptr2user : UserDataWapper
{
    weakptr2user(const std::shared_ptr<T>& rht)
        : UserDataWapper(&m_holder)
        , m_holder(rht)
    {
    }

    virtual bool isSharedPtr() const override { return true; }
    // use weak_ptr to hold it
    ~weakptr2user() { m_holder.reset(); }

    std::weak_ptr<T> m_holder;
};

template<typename T>
struct sharedptr2user : UserDataWapper
{
    sharedptr2user(const std::shared_ptr<T>& rht)
        : UserDataWapper(&m_holder)
        , m_holder(rht)
    {
    }
    sharedptr2user(std::shared_ptr<T>&& rht)
        : UserDataWapper(&m_holder)
        , m_holder(std::forward<std::shared_ptr<T>>(rht))
    {
    }
    virtual bool isSharedPtr() const override { return true; }
    virtual bool haveOwership() const override { return true; }
    // use weak_ptr to hold it
    ~sharedptr2user() { m_holder.reset(); }

    std::shared_ptr<T> m_holder;
};
```
### tuple可以吗?
tuple我们可以直接将之解开到table， 也可以只存储一个Tuple对象

```
template<typename... Args>
struct _stack_help<std::tuple<Args...>>
{
    using TupleType = std::tuple<Args...>;

    template<typename T, std::size_t I>
    static T _read_tuple_element_fromtable_helper(lua_State* L, int32_t table_stack_pos)
    {
        T t;
        push(L, I);
        lua_gettable(L, table_stack_pos);
        t = read<T>(L, -1);

        return t;
    }

    template<typename Tuple, std::size_t... index>
    static Tuple _read_tuple_fromtable(lua_State* L, int32_t table_stack_pos, std::index_sequence<index...>)
    {
        return std::make_tuple(
            _read_tuple_element_fromtable_helper<typename std::tuple_element<index, Tuple>::type, index + 1>(
                L,
                table_stack_pos)...);
    }

    template<typename Tuple>
    static Tuple _read_tuple_fromtable(lua_State* L, int32_t index)
    {
        stack_obj table_obj(L, index);
        if(table_obj.is_table() == false)
        {
            lua_pushfstring(L, "convert set from argument %d must be a table", index);
            lua_error(L);
        }

        return _read_tuple_fromtable<Tuple>(L,
                                            table_obj._stack_pos,
                                            std::make_index_sequence<std::tuple_size<Tuple>::value>{});
    }

    static TupleType _read(lua_State* L, int32_t index)
    {
        if(lua_istable(L, index))
            return _read_tuple_fromtable<TupleType>(L, index);
        else
        {
            return _lua2type<TupleType>(L, index);
        }
    }

    template<typename Tuple>
    static void _push_tuple_totable_helper(lua_State* L,
                                            int32_t    table_stack_pos,
                                            Tuple&&    tuple,
                                            std::index_sequence<>)
    {
    }

    template<typename Tuple, std::size_t first, std::size_t... is>
    static void _push_tuple_totable_helper(lua_State* L,
                                            int32_t    table_stack_pos,
                                            Tuple&&    tuple,
                                            std::index_sequence<first, is...>)
    {
        push(L, first + 1); // lua table index from 1~x
        push(L, std::get<first>(std::forward<Tuple>(tuple)));
        lua_settable(L, table_stack_pos);
        _push_tuple_totable_helper(L,
                                    table_stack_pos,
                                    std::forward<Tuple>(tuple),
                                    std::index_sequence<is...>{});
    }

    static void _push(lua_State* L, TupleType&& tuple)
    {
        stack_obj        table_obj  = stack_obj::new_table(L, std::tuple_size<TupleType>(), 0);
        constexpr size_t tuple_size = std::tuple_size<TupleType>::value;
        _push_tuple_totable_helper(L,
                                    table_obj._stack_pos,
                                    std::forward<TupleType>(tuple),
                                    std::make_index_sequence<tuple_size>());
    }

    static void _push(lua_State* L, const TupleType& val) { return _type2lua(L, val); }

    static void _push(lua_State* L, TupleType& val) { return _type2lua(L, val); }
};
```
### functional对象可以吗?
没问题， 我们可以通过将一个c函数闭包放入lua的注册表中，来长期保持对他的引用
```
template<typename RVal, typename... Args>
struct _stack_help<std::function<RVal(Args...)>>
{
    // func must be release before lua close.....user_conctrl
    static std::function<RVal(Args...)> _read(lua_State* L, int32_t index)
    {
        if(lua_isfunction(L, index) == false)
        {
            lua_pushfstring(L, "can't convert argument %d to function", index);
            lua_error(L);
        }

        // copy idx to top
        lua_pushvalue(L, index);
        // make ref
        int32_t lua_callback = luaL_ref(L, LUA_REGISTRYINDEX);

        lua_function_ref<RVal> callback_ref(L, lua_callback);

        return std::function<RVal(Args...)>(callback_ref);
    }

    static void _push(lua_State* L, std::function<RVal(Args...)>&& func)
    {
        _push_functor(L, std::forward<std::function<RVal(Args...)>>(func));
    }
    static void _push(lua_State* L, const std::function<RVal(Args...)>& func)
    {
        _push_functor(L, std::forward<std::function<RVal(Args...)>>(func));
    }
};
template<typename RVal, typename... Args>
struct _stack_help<const std::function<RVal(Args...)>&> : public _stack_help<std::function<RVal(Args...)>>
{
};

```
通过一个helper类来将lua中的函数，放入lua注册表，来长期持有该对象
```
template<typename RVal>
struct lua_function_ref : public detail::lua_ref_base
{
using lua_ref_base::lua_ref_base;

template<typename... Args>
RVal operator()(Args&&... args) const
{
    lua_getglobal(m_L, ERROR_CALLBACK_NAME);
    int32_t errfunc = lua_gettop(m_L);

    if(lua_rawgeti(m_L, LUA_REGISTRYINDEX, m_regidx) == LUA_TFUNCTION)
    {
        detail::push_args(m_L, std::forward<Args>(args)...);
        if(lua_pcall(m_L, sizeof...(Args), detail::pop<RVal>::nresult, errfunc) != 0)
        {
            // stack have a nil string from on_error
            if constexpr(detail::pop<RVal>::nresult == 0)
            {
                // not need it, pop
                lua_pop(m_L, 1);
            }
            else if constexpr(detail::pop<RVal>::nresult > 1)
            {
                // push nil to pop result
                for(int32_t i = 0; i < detail::pop<RVal>::nresult - 1; i++)
                {
                    lua_pushnil(m_L);
                };
            }
            else
            {
                //==1, leave it for pop resuslt
            }
        }
    }
    else
    {
        print_error(m_L, "lua_tinker::lua_function_ref attempt to call (not a function)");
    }

    lua_remove(m_L, errfunc);
    return detail::pop<RVal>::apply(m_L);
}
};
```
### namespace呢?
将namespace作为一个普通的的meta表来处理即可， 其内部注册的函数，不再放入global表而是注册在namespace的子表下
```
static void namespace_add(lua_State* L, const char* namespace_name)
{
    lua_createtable(L, 0, 3);

    lua_pushstring(L, "__name");
    lua_pushstring(L, namespace_name);
    lua_rawset(L, -3);

    lua_pushstring(L, "__index");
    lua_pushcclosure(L, detail::meta_get, 0);
    lua_rawset(L, -3);

    lua_pushstring(L, "__newindex");
    lua_pushcclosure(L, detail::meta_set, 0);
    lua_rawset(L, -3);

    lua_setglobal(L, namespace_name);
}
```
### const能判断吗?
UserDataWapper在保持对象时将对象的const属性保存入内
当调用类函数时，我们也可以从函数签名上判断该函数是否是const的
自然可以达到判断的目的，当然这会增加额外的数据存储/性能开销（我们可以只在debug版本中加入类型判断检查/const判断检查）

### 函数重载可以吗?
让我们来想想c++是如何实现函数重载的
1. 根据参数个数
2. 根据参数的类型

我们也可以将一个函数签名， 转换为lua参数类型签名
当lua回调c++时，我们根据栈上的参数个数， 栈上的参数类型，生成1个参数签名
与该名字重名的N个函数的签名进行比对， 最接近/完全一致的就是我们想要的重载函数

## 需要自己手动写binding代码吗?
### 认识libclang
现在的Clang，不仅仅是一个编译器前端，同时也可以作为一个库使用。作为一个库使用的时候，可以用它去分析C/C++/ObjectC语言代码，可以分析源码得到AST，也可以获取已经分析好的AST，也可以遍历AST，还可以获取AST中基本元素的物理源码位置。这就是libclang。

libclang提供了一系列的C语言的接口，但是这些接口并不能完全提供存储在Clang C++ AST中的所有信息，只能提供部分基本信息，但是这些基本信息已经可以满足一般情况下的使用。主要目的是为了稳定，并且可以支持开发工具的基本功能

### 自动化生产binding代码
我们可以先
` #define lua_export`
定义一个空的宏定义， 它不产生任何额外的运行开销
然后我们在所有想要到处给lua的函数/class/enum前加入这个宏，帮我们定位这些源代码

最后我们可以使用libclang来分析我们代码， 在宏定义转换阶段， 找到所有需要宏展开 lua_export的地方，一一记录
在后面解析完ast树后， 我们遍历整个ast树， 只要找到任何一个我们前面记录下来的lua_export的地方， 根据具体的类型， 转换为我们需要的binding代码