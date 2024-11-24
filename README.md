# Contemplator  
**A Simple Template-Based Stuff Generator**

---

## Installation in Existing Projects  
1. Add this directory to your CMake file.  
2. Use `CONTEMPLATOR` as the target library.  
3. Include the header `contemplator.hpp` in your code.  

---

## Usage Guide  

### Template Context  
```cpp
$< stuff >$ 
```  
Defines a context block for templates. If any templates inside the context fail to match, the whole context is discarded.


### Template Matrix
```cpp
$[ stuff ]$
```
Same as context, but used to generate repeated content based on multiple input arguments. It iterates through the entries in `template_key_values::matrix_arguments_instances`. For each entry in the matrix, the following occurs:

 1. The Template Matrix evaluates and generates content using the provided arguments.
 2. Each iteration inherits the current context, which includes matched keys and their associated values.

The Template Matrix enables recursion, meaning nested Matrix contexts inside the matrix can be processed for each entry.

### Template Key Argument  
```cpp
$(argument_name)$  
```  
Searches for `argument_name` in the current context and replaces it with the corresponding value.  

### Template Key List Argument  
```cpp
$[list_name]$  
```  
Searches for `list_name` in the current context. For every match, it:  
- Repeats the current context (including matched arguments).  

---

## Example: Code Generation  

### Step 1: Define an Input Template File  
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
Templates can also be loaded from external files.  

---

### Step 2: Gather Input Arguments  

```cpp
// Transform component arguments
template_key_values transform_data;
transform_data.argument_instances["STRUCT_NAME"] = "transform";

transform_data.list_arguments_instances["FIELD_TYPE"].emplace_back("vector3<float>");
transform_data.list_arguments_instances["FIELD_NAME"].emplace_back("position");

transform_data.list_arguments_instances["FIELD_TYPE"].emplace_back("vector3<float>");
transform_data.list_arguments_instances["FIELD_NAME"].emplace_back("scale");

transform_data.list_arguments_instances["FIELD_TYPE"].emplace_back("quaternion<float>");
transform_data.list_arguments_instances["FIELD_NAME"].emplace_back("rotation");

// Test component arguments
template_key_values test_data;
test_data.argument_instances["STRUCT_NAME"] = "test";

test_data.list_arguments_instances["FIELD_TYPE"].emplace_back("int");
test_data.list_arguments_instances["FIELD_NAME"].emplace_back("hello");

// Combine template arguments
template_key_values component_datas;
component_datas.matrix_arguments_instances.emplace_back(transform_data);
component_datas.matrix_arguments_instances.emplace_back(test_data);
```

---

### Step 3: Generate the Output  
```cpp
std::vector<std::string> output = contemplator::generate({ component_datas }, component_template);
```  

---

### Step 4: View the Result  

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