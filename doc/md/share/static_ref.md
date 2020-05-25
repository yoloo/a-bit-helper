# 静态反射

## 前因

在行为树/或者一些flow-ctrl结构需要实现大量节点的反序列化，
其中节点中往往还存在大量的嵌套结构，
为了实现结构体的反序列化, 我们有可能会写下如下的代码

```
struct Pos3D
{
    float x,y,z;
};

struct TestNodeA
{
    int32_t TestNodeA_a;
    int32_t TestNodeA_b;
    int32_t TestNodeA_c;
    Pos3D   TestNodeA_d;
};
struct TestNodeB
{
    int32_t   TestNodeB_a;
    TestNodeA TestNodeB_b;
};
struct AllNode
{
    union
    {
        TestNodeA AllNode_a;
        TestNodeB AllNode_b;
    }
};
```


```
OnLoadPos3D(xml_node* xml_node, Pos3D* pNode)
{
    while(xml_node->has_next())
    {
        auto* pE = xml_node->next_element();
        if(pE->GetName() == "x")
        {
            pNode->x = pE->GetFloat();
        }
        else if(pE->GetName() == "y")
        {
            pNode->y = pE->GetFloat();
        }
        else if(pE->GetName() == "z")
        {
            pNode->z = pE->GetFloat();
        }
    }
}

OnLoadTestNodeA(xml_node* xml_node, TestNodeA* pNode)
{
    while(xml_node->has_next())
    {
        auto* pE = xml_node->next_element();
        if(pE->GetName() == "TestNodeA_a")
        {
            pNode->TestNodeA_a = pE->GetInt32();
        }
        else if(pE->GetName() == "TestNodeB_a")
        {
            pNode->TestNodeA_b = hash_code(pE->GetString());
        }
        else if(pE->GetName() == "TestNodeA_c")
        {
            pNode->TestNodeA_c = pE->GetInt32();
        }
        else if(pE->GetName() == "TestNodeA_d")
        {
            OnLoadTestNodeA(pE, &pNode->TestNodeA_d);
        }
    }
    
}
OnLoadTestNodeB(xml_node* xml_node, TestNodeB* pNode)
{
    while(xml_node->has_next())
    {
        auto* pE = xml_node->next_element();
        if(pE->GetName() == "TestNodeB_a")
        {
            //从String转HASH_CODE
            pNode->TestNodeB_a = hash_code(pE->GetString());
        }
        else if(pE->GetName() == "TestNodeA")
        {
            OnLoadTestNodeA(pE, &pNode->TestNodeA);
        }
    }
}
```

当这样的结构体内的数据端从1变成20, 数据从简单的int/double，变成复合类型int3,float3之类的
我们的重复代码就会充斥于反序列化函数中，而且很容易因为CP大法，出现BUG


## 思考
既然代码逻辑非常简单，那让我们先简单的梳理一下具体的反序列化逻辑
1.找到一个xml/json节点
2.通过节点的name来查找具体的字段名
3.调用该字段的反序列化函数来反序列化
4.反序列化函数要能支持 普通类型/复合类型/嵌套结构体/特定的转换函数
5.能不能加速string的比较过程
6.因为有些结构不是我们自己的代码， 不能写出侵入式的代码



懒惰是程序员的天性：

1. “勤奋” 的程序员选择： [人工手写]
2. “懒惰” 的程序员选择： 构建代码生成器（例如 protobuf、chromium/mojo）   或 [编译器生成]

代码生成器虽然功能强大，但依赖复杂，不易于和已有系统集成。所以我们主要讨论如何用 C++ 11/14/17 提供的 元编程 (meta-programming) 技巧，让编译器帮你写代码。

1. 基于 C++ 原生语法，不需要引入第三方库
2. 提供 声明式 (declarative) 的方法，只需要声明格式，不需要写逻辑语句
3. 不会带来 额外的运行时开销，能达到和手写代码一样的运行时效率

## c++11/14/17 如何来实现静态反射

### 首先我们介绍一下constexpr
在c++新的标准中出现了1个新的关键字constexpr
这个关键字可以修饰变量或者函数
如果修饰变量，那么这个变量必须在编译期可以求值
如果修饰函数，那么函数可以在编译期求值，也可使在运行期求值

```
constexpr int f() { return 5; }
constexpr int array[f()];
```

constexpr函数从c++11开始不停的被放松对其编写的要求，
至c++17时，已经可以出多个return语句，if分支，auto返回值推导
到了c++20时，甚至允许出现try-catach块(stl将在c++23中被修改支持constexp，即编译期stl)


### 然后我们再介绍一下tuple
类模板 std::tuple 是固定大小的异类值汇集。它是 std::pair 的推广。
pair只支持first/second， tuple支持N个值

```
#include <tuple>
#include <iostream>
#include <string>
#include <stdexcept>
 
std::tuple<double, char, std::string> get_student(int id)
{
    if (id == 0) return std::make_tuple(3.8, 'A', "Lisa Simpson");
    if (id == 1) return std::make_tuple(2.9, 'C', "Milhouse Van Houten");
    if (id == 2) return std::make_tuple(1.7, 'D', "Ralph Wiggum");
    throw std::invalid_argument("id");
}
 
int main()
{
    auto student0 = get_student(0);
    std::cout << "ID: 0, "
              << "GPA: " << std::get<0>(student0) << ", "
              << "grade: " << std::get<1>(student0) << ", "
              << "name: " << std::get<2>(student0) << '\n';
 
    double gpa1;
    char grade1;
    std::string name1;
    std::tie(gpa1, grade1, name1) = get_student(1);
    std::cout << "ID: 1, "
              << "GPA: " << gpa1 << ", "
              << "grade: " << grade1 << ", "
              << "name: " << name1 << '\n';
 
    // C++17 结构化绑定：
    auto [ gpa2, grade2, name2 ] = get_student(2);
    std::cout << "ID: 2, "
              << "GPA: " << gpa2 << ", "
              << "grade: " << grade2 << ", "
              << "name: " << name2 << '\n';
}
```

### 来看看我们如何实现静态反射


**核心原理** 是：利用 $\color{blue}{访问者模式}$ (visitor pattern)，使用 元组 std::tuple 记录结构体所有的字段信息，通过 $\color{blue}{编译期多态/静态分派}$(compile-time polymorphism) 针对具体的 字段类型 进行转换操作。


```
struct Pos3D
{
    float x,y,z;
};

struct TestNodeA
{
    int32_t TestNodeA_a;
    int32_t TestNodeA_b;
    int32_t TestNodeA_c;
    Pos3D   TestNodeA_d;
};
DEFINE_STRUCT_SCHEMA(TestNodeA,
                     DEFINE_STRUCT_FIELD(TestNodeA_a, "TestNodeA_a"),
                     DEFINE_STRUCT_FIELD_TAG(TestNodeA_b, "TestNodeA_b", Str2HashCode_Tag()),   //带入一个tag用来特化
                     DEFINE_STRUCT_FIELD(TestNodeA_c, "TestNodeA_c"),
                     DEFINE_STRUCT_FIELD_FUNC(TestNodeA_d, "TestNodeA_d", OnLoadPos3D));        //带入一个函数，用来特殊处理

struct TestNodeB
{
    int32_t   TestNodeB_a;
    TestNodeA TestNodeB_b;
};

DEFINE_STRUCT_SCHEMA(TestNodeB,
                     DEFINE_STRUCT_FIELD(TestNodeB_a, "TestNodeB_a"),
                     DEFINE_STRUCT_FIELD(TestNodeB_b, "TestNodeB_b"));

struct AllNode
{
    union
    {
        TestNodeA AllNode_a;
        TestNodeB AllNode_b;
    }
};

DEFINE_STRUCT_SCHEMA(AllNode, 
                    DEFINE_STRUCT_FIELD(AllNode_a, "AllNode_a"), 
                    DEFINE_STRUCT_FIELD(AllNode_b, "AllNode_b"));
```






让我们先来看看这些宏定义
```
#define DEFINE_STRUCT_SCHEMA(ThisStruct, ...)    \
    template<>                                   \
    struct StructSchema<ThisStruct>              \
    {                                            \
        using _Struct = ThisStruct;              \
        template<typename E = void>              \
        static inline constexpr auto GetSchema() \
        {                                        \
            return std::make_tuple(__VA_ARGS__); \
        }                                        \
    };

#define MAKE_FIELD_INFO(FieldName) std::make_tuple(FieldName, FieldName##_H)

#define BIND_FIELD(StructField) std::make_tuple(&_Struct::StructField)

#define BIND_FIELD_TAG(StructField, Tag) std::make_tuple(&_Struct::StructField, Tag)

#define BIND_FIELD_FUNC(StructField, Func) std::make_tuple(&_Struct::StructField, Func)

#define DEFINE_STRUCT_FIELD_BIND(FieldName, ...) std::make_tuple(MAKE_FIELD_INFO(FieldName), __VA_ARGS__)

#define DEFINE_STRUCT_FIELD(StructField, FieldName) std::make_tuple(MAKE_FIELD_INFO(FieldName), BIND_FIELD(StructField))

#define DEFINE_STRUCT_FIELD_TAG(StructField, FieldName, Tag) \
    std::make_tuple(MAKE_FIELD_INFO(FieldName), BIND_FIELD_TAG(StructField, Tag))

#define DEFINE_STRUCT_FIELD_FUNC(StructField, FieldName, Func) \
    std::make_tuple(MAKE_FIELD_INFO(FieldName), BIND_FIELD_FUNC(StructField, Func))
```




让我们来展开一个宏定义
```
DEFINE_STRUCT_SCHEMA(TestNodeB,
                     DEFINE_STRUCT_FIELD(TestNodeB_a, "TestNodeB_a"),
                     DEFINE_STRUCT_FIELD(TestNodeB_b, "TestNodeB_b"));
```

```
template<>
struct StructSchema<TestNodeB>
{
    using _Struct = TestNodeB;
    template<typename E = void>
    static inline constexpr auto GetSchema()
    {
        return std::make_tuple(
            std::make_tuple(std::make_tuple("TestNodeB_a", "TestNodeB_a"_H), std::make_tuple(&_Struct::TestNodeB_a)),
            std::make_tuple(std::make_tuple("TestNodeB_b", "TestNodeB_b"_H), std::make_tuple(&_Struct::TestNodeB_b)));
    }
};


// std::tuple< FieldInfo, MemberData>
FieldInfo  = std::tuple<FieldName, FieldNameHash>
MemberData = std::tuple<MemberPtr> / std::tuple<MemberPtr,Tag> /   std::tuple<MemberPtr,Func>
```

这样我们就实现了一堆的constexpr函数， 来输出一个常量的tuple， 这个tuple内含了这个结构体每个成员的定义信息
每个成员的定义 由 std::tuple< FieldInfo, MemberData> 组成
FieldInfo 是一个 const char* + hash_code 组成的tuple
MemberData 是一个 函数成员指针 + Tag/Func 组成的tuple

请记住这个超大的tuple 是一个常量， 既然是常量，我们再将其作为参数输入另外一个constexpr函数， 那么这个函数也会是constexpr,可以在编译期来实现

```

template<typename T, typename Fn>
inline constexpr void FindInField(T&& value, Fn&& fn)
{
    constexpr auto struct_schema = StructSchema<std::decay_t<T>>::GetSchema();
    static_assert(std::tuple_size<decltype(struct_schema)>::value != 0,
                    "StructSchema<T>() for type T should be specialized to return "
                    "FieldSchema tuples, like ((&T::field, field_name), ...)");
    detail::FindInStructSchemaLambda<T, Fn> lambda{std::forward<T>(value), std::forward<Fn>(fn)};
    detail::find_if_tuple(struct_schema, std::move(lambda));
}
```

```
//////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Lambda, typename Tuple>
inline constexpr bool find_if_tupleImpl(Tuple&& tuple, Lambda&& lambda, std::index_sequence<>)
{
    return false;
}

template<typename Lambda, typename Tuple, std::size_t first, std::size_t... is>
inline constexpr bool find_if_tupleImpl(Tuple&& tuple, Lambda&& lambda, std::index_sequence<first, is...>)
{
    // if any lambda return true, return turel
    return lambda(std::get<first>(std::forward<Tuple>(tuple))) ||
            find_if_tupleImpl(std::forward<Tuple>(tuple),
                                std::forward<Lambda>(lambda),
                                std::index_sequence<is...>{});
}

template<typename Lambda, typename Tuple>
inline constexpr bool find_if_tuple(Tuple&& tuple, Lambda&& lambda)
{
    return find_if_tupleImpl(std::forward<Tuple>(tuple),
                                std::forward<Lambda>(lambda),
                                std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>{});
}
```



那么这个FindInField展开后就是
```
template<typename T, typename Fn>
inline constexpr void FindInField(T&& value, Fn&& fn)
{
    constexpr auto struct_schema = StructSchema<std::decay_t<T>>::GetSchema();
    detail::FindInStructSchemaLambda<T, Fn> lambda{std::forward<T>(value), std::forward<Fn>(fn)};
    return lambda(std::get<0>(struct_schema)) ||
            lambda(std::get<1>(struct_schema)) ||
            lambda(std::get<2>(struct_schema)) ||
            ...
            lambda(std::get<N>(struct_schema)) );
}

```


我们再来看这个仿函数对象的实现

```
template<typename Value, typename Func>
struct FindInStructSchemaLambda
{
    Value&      value;
    const Func& fn;

    template<typename Info, typename Tuple>
    bool invoke_one_impl(Info&& info, Tuple&& t, std::index_sequence<>) const
    {
        // invoke(FieldName, MemberPtr);
        return fn(std::forward<Info>(info), value.*(std::get<0>(std::forward<Tuple>(t))));
    }

    template<typename Info, typename Tuple, std::size_t... Index>
    bool invoke_one_impl(Info&& info, Tuple&& t, std::index_sequence<Index...>) const
    {
        // invoke(FieldName, MemberPtr,Tag);
        // invoke(FieldName. MemberPtr,Func);
        return fn(std::forward<Info>(info),
                    value.*(std::get<0>(std::forward<Tuple>(t))),
                    std::get<Index + 1>(std::forward<Tuple>(t))...);
    }

    template<typename Info, typename Tuple>
    bool invoke_one(Info&& info, Tuple&& t) const
    {
        constexpr auto size = std::tuple_size<typename std::decay<Tuple>::type>::value;
        return invoke_one_impl(std::forward<Info>(info),
                                std::forward<Tuple>(t),
                                std::make_index_sequence<size - 1>{});
    }

    template<typename Info, typename Tuple>
    bool invoke_impl(Info&& info, Tuple&& t, std::index_sequence<>) const
    {
        return false;
    }
    template<typename Info, typename Tuple, std::size_t first, std::size_t... is>
    bool invoke_impl(Info&& info, Tuple&& t, std::index_sequence<first, is...>) const
    {
        // invoke(FieldName, std::tuple<MemberPtr>);
        // invoke(FieldName, std::tuple<MemberPtr,Tag>);
        // invoke(FieldName. std::tuple<MemberPtr,Func>);
        bool bInvoke = invoke_one(std::forward<Info>(info), std::get<first + 1>(std::forward<Tuple>(t)));

        // first invoke all, then all result ||;
        return invoke_impl(std::forward<Info>(info), std::forward<Tuple>(t), std::index_sequence<is...>{}) ||
                bInvoke;
    }

    template<typename Tuple>
    bool operator()(Tuple&& field_schema) const
    {
        using FieldSchema         = std::decay_t<decltype(field_schema)>;
        constexpr auto size_tuple = std::tuple_size<FieldSchema>::value;
        static_assert(size_tuple >= 2, "FieldSchema<T>() must have 1 field");
        // tuple like std::tuple<FieldName, std::tuple<MemberPtr>, std::tuple<MemberPtr,Tag> ,
        // std::tuple<MemberPtr,Func> >
        return invoke_impl(std::get<0>(std::forward<Tuple>(field_schema)),
                            std::forward<Tuple>(field_schema),
                            std::make_index_sequence<size_tuple - 1>{});
    }
};


```


我们也把这个labmda展开来看

```
invoke(FieldInfo, MemberData) -> 
    invoke(FieldInfo, MemberPtr)
    invoke(FieldInfo, MemberPtr,Tag)
    invoke(FieldInfo, MemberPtr,Func)

```




我们传递给FindInField的有一个参数是fn，这就是我们入口回调函数


让我们来看一下范例代码, 从main函数一点一点来看调用
```
// forward decal
template<class T>
inline void xmlElement_to_struct(tinyxml2::XMLElement* pE, T& refStruct);

template<class FieldType>
inline void xml_value_to_field(tinyxml2::XMLElement* pVarE, FieldType* field)
{
    if constexpr(std::is_integral<FieldType>::value || std::is_floating_point<FieldType>::value ||
                 std::is_same<bool, FieldType>::value)
    {
        pVarE->QueryAttribute("val", field);
    }
    else if constexpr(std::is_same<std::string, FieldType>::value)
    {
        const char* pVal = pVarE->Attribute("val");
        if(pVal)
        {
            *field = pVal;
        }
    }
    else
    {
        xmlElement_to_struct(pVarE, *field);
    }
}

template<class FieldType>
inline void xml_value_to_field(tinyxml2::XMLElement*                                   pVarE,
                               FieldType*                                              field,
                               std::function<void(tinyxml2::XMLElement*, FieldType*)>& after_func)
{
    xml_value_to_field(pVarE, field);
    after_func(pVarE, field);
}

struct ForEachXMLLambda
{
    tinyxml2::XMLElement* pVarE;
    const char*           field_name;
    std::size_t           field_name_hash;
    template<typename FieldInfo, typename Field>
    bool operator()(FieldInfo&& this_field_info, Field&& this_field) const
    {
        printf("%s test:%s\n", field_name, std::get<0>(this_field_info));
        if(field_name_hash != std::get<1>(this_field_info))
            return false;
        printf("%s vist:%s\n", field_name, std::get<0>(this_field_info));
        xml_value_to_field(pVarE, &this_field);
        return true;
    }

    template<typename FieldInfo, typename Field, typename Tag>
    bool operator()(FieldInfo&& this_field_info, Field&& this_field, Tag&& tag) const
    {
        printf("%s test:%s\n", field_name, std::get<0>(this_field_info));
        if(field_name_hash != std::get<1>(this_field_info))
            return false;
        printf("%s vist:%s\n", field_name, std::get<0>(this_field_info));
        xml_value_to_field(pVarE, &this_field, std::forward<Tag>(tag));
        return true;
    }
};

template<class T>
inline void xmlElement_to_struct(tinyxml2::XMLElement* pE, T& refStruct)
{
    tinyxml2::XMLElement* pVarE = pE->FirstChildElement();
    while(pVarE != NULL)
    {
        const char* pStrName = pVarE->Attribute("name");
        if(pStrName != NULL)
        {
            std::string field_name      = pStrName;
            std::size_t field_name_hash = hash::MurmurHash3::shash(field_name.c_str(), field_name.size(), 0);
            static_reflection::FindInField_Index(refStruct,
                                                 field_name_hash,
                                                 ForEachXMLLambda{pVarE, field_name.c_str(), field_name_hash});
        }

        pVarE = pVarE->NextSiblingElement();
    }
}

int main()
{
    __ENTER_FUNCTION

    tinyxml2::XMLDocument doc;
    auto                  errZ_code = doc.Parse(xml_txt.c_str(), xml_txt.size());
    if(errZ_code != tinyxml2::XMLError::XML_SUCCESS)
    {
        return -1;
    }

    auto pRootE = doc.FirstChildElement("Root");
    if(pRootE == NULL)
    {
        return -1;
    }

    auto pNodesE = pRootE->FirstChildElement("Nodes");
    if(pNodesE == NULL)
    {
        return -1;
    }
    std::vector<AllNode> testNodeList;
    auto                 pNodeE = pNodesE->FirstChildElement();
    while(pNodeE)
    {

        std::string struct_name = pNodeE->Name();
        AllNode     testNode;
        xmlElement_to_struct(pNodeE, testNode);
        testNodeList.emplace_back(std::move(testNode));
        pNodeE = pNodeE->NextSiblingElement();
    }

    return 0;
    __LEAVE_FUNCTION
    return -1;
}
```





这里

```
template<class FieldType>
inline void xml_value_to_field(tinyxml2::XMLElement* pVarE, FieldType* field)
```
就是我们的静态分派函数
我们可以通过重载的方式将普通类型和复合类型都实现
```

template<class FieldType >
void xml_value_to_field(tinyxml2::XMLElement* pVarE, FieldType* field)
{
	xmlElement_to_struct(pVarE, *field);
}

template<>
void xml_value_to_field(tinyxml2::XMLElement* pVarE, uint64_t* field)
{
	pVarE->QueryAttribute("val", (int64_t*)field);
}

template<>
void xml_value_to_field(tinyxml2::XMLElement* pVarE, unsigned* field)
{
	pVarE->QueryAttribute("val", field);
}

template<>
void xml_value_to_field(tinyxml2::XMLElement* pVarE, int* field)
{
	pVarE->QueryAttribute("val", field);
}


void xml_value_to_field(tinyxml2::XMLElement* pVarE, int* field, ElfHash_tag tag)
{
	pVarE->QueryAttribute("val", field);
}


void xml_value_to_field(tinyxml2::XMLElement* pVarE, int* field, NameHash_tag tag)
{
	pVarE->QueryAttribute("val", field);
}

void xml_value_to_field(tinyxml2::XMLElement* pVarE, int* field, std::function<void(tinyxml2::XMLElement*, int*)> after_func)
{
	pVarE->QueryAttribute("val", field);
	after_func(pVarE, field);
}



template<>
void xml_value_to_field(tinyxml2::XMLElement* pVarE, int64_t* field)
{
	pVarE->QueryAttribute("val", field);
}

template<>
void xml_value_to_field(tinyxml2::XMLElement* pVarE, bool* field)
{
	pVarE->QueryAttribute("val", field);
}

template<>
void xml_value_to_field(tinyxml2::XMLElement* pVarE, double* field)
{
	pVarE->QueryAttribute("val", field);
}

template<>
void xml_value_to_field(tinyxml2::XMLElement* pVarE, float* field)
{
	pVarE->QueryAttribute("val", field);
}
```




我们再来将前面的函数展开

```
return lambda(std::get<0>(struct_schema)) ||
        lambda(std::get<1>(struct_schema)) ||
        lambda(std::get<2>(struct_schema)) ||
        ...
        lambda(std::get<N>(struct_schema)) );

invoke(FieldInfo, MemberData) -> 
    invoke(FieldInfo, MemberPtr)
    invoke(FieldInfo, MemberPtr,Tag)
    invoke(FieldInfo, MemberPtr,Func)


return lambda(FieldInfo_0, MemberData_0) ||
        lambda(FieldInfo_1, MemberData_1) ||
        lambda(FieldInfo_2, MemberData_2) ||
        ...
        lambda(FieldInfo_N, MemberData_N) );

return if(field_name_hash == std::get<1>(this_field_info)){ xml_value_to_field(pVarE, &this_field); } ||
        if(field_name_hash == std::get<1>(this_field_info)){ xml_value_to_field(pVarE, &this_field); } ||
        if(field_name_hash == std::get<1>(this_field_info)){ xml_value_to_field(pVarE, &this_field, std::forward<Tag>(tag)); };


field_name_hash是这个pVarE的name的 hash_code
与field_info中的hash_code比较， 如果一致，则 执行 xml_value_to_field

实际代码就是
return if(field_name_hash == 0xABCDEFG){ xml_value_to_field(pVarE, &this_field); } ||
        if(field_name_hash == 0xABCD){ xml_value_to_field(pVarE, &this_field); } ||
        if(field_name_hash == 0xABCDE){ xml_value_to_field(pVarE, &this_field, std::forward<Tag>(tag)); };

最终代码就会被优化为一行 xml_value_to_field(pVarE, &this_field); 

```



使用编译时静态反射，相对于运行时动态反射，有许多优点：

|比较|动态反射|静态反射|
|:-|:-|:-|
|使用难度|	（难）需要 编写注册代码，调用 RegisterField| 动态绑定字段信息	（易）可以通过 声明式 的方法，静态定义字段信息|
|运行时开销|	（有）需要动态构造 converter 对象，需要通过 虚函数表 (virtual table) 实现面向对象的多态|	（无）编译时 静态展开代码，和直接手写一样|
|可复用性|	（差）每个 converter 对象绑定了各个 字段类型 的具体 映射方法；如果需要进行不同转换操作，则需要另外创建 converter 对象|	（好）在调用 ForEachField 时，映射方法 作为参数传入；利用 编译时多态 的机制，为不同的 字段类型 选择合适的操作|



## c++2x 静态反射库
让我们高兴一下reflection库已经被c++标准接受，他的ts实现将加入到c++20,  IS实现将加入到c++23

```
struct person{
    int id;
    std::string name;
};

using MetaPerson = reflexpr(person);
using Members = std::reflect::get_data_members_t<MetaPerson>;

using Meta_id = std::reflect::get_element_t<0,Members>;
constexpr bool is_public = std::reflect::is_public_v<Meta_id>;

using Field0_t = std::reflect::get_reflected_type_t<Meta_id>;// int
using Field1_t = std::reflect::get_reflected_type_t<std::reflect::get_element_t<1,Members>>;// string
```

#让我们期待未来！！