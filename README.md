# Contemplator
Easy to use Template based compiler


# How to install in existing projects
Include this directory in your cmake file and use "CONTEMPLATOR" as target library. 
Include "contemplator.hpp" in your code


# How to use
Code gen example: 

1. Define an input template file
```cpp

char component_template[] = R"(
    $#
    struct $(STRUCT_NAME)$_component 
    {
        int id;
        int owner;

        $<
        $[FIELD_TYPE]$ $[FIELD_NAME]$;>$
    };
    #$
)";
```
(can also be a file)


2. Gather argument input

```cpp

// Transform component arguments
template_key_values transform_data = {};
transform_data.argument_instances["STRUCT_NAME"] = "transform";

transform_data.list_arguments_instances["FIELD_TYPE"].emplace_back("vector3<float>");
transform_data.list_arguments_instances["FIELD_NAME"].emplace_back("position");

transform_data.list_arguments_instances["FIELD_TYPE"].emplace_back("vector3<float>");
transform_data.list_arguments_instances["FIELD_NAME"].emplace_back("scale");

transform_data.list_arguments_instances["FIELD_TYPE"].emplace_back("quaternion<float>");
transform_data.list_arguments_instances["FIELD_NAME"].emplace_back("rotation");

// Test component arguments
template_key_values test_data = {};
test_data.argument_instances["STRUCT_NAME"] = "test";

test_data.list_arguments_instances["FIELD_TYPE"].emplace_back("int");
test_data.list_arguments_instances["FIELD_NAME"].emplace_back("hello");


// Create final template key day
template_key_values component_datas = {};
component_datas.matrix_arguments_instances.emplace_back(transform_data);
component_datas.matrix_arguments_instances.emplace_back(test_data);
```

3. Generate templated file!
```cpp
std::vector<std::string> out = contemplator::generate( { component_datas }, component_template);
```

4. Output result
```cpp

struct transform_component 
{
    int id;
    int owner;

    vector3<float> position;
    vector3<float> scale;
    quaternion<float> rotation;
};


struct test_component 
{
    int id;
    int owner;

    int hello;
};

```
