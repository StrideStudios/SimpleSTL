# Simple STL

A Library designed to wrap around the convoluted c++ standard containers and make them easier to use, especially in a generic context.

## Usage

For some basic usage and testing, see [Test.cpp](./test/Test.cpp).  There are Three different types of containers in sstl.
 - Sequence Containers
   - Ex: Vector, Deque, List
 - Associative Containers
   - Ex: Map, MultiMap
 - Single Associative Containers
   - Ex: Set, MultiSet

### Function

All Container types have similar functions, but have different implementations.  For specifics, refer to [Test.cpp](./test/Test.cpp).

Containers can generally be accessed by the following:
```
# Gets value from a specified index
value = container.get(index)

# Gets the first element of a container
begin = container.top()

# Gets the last element of a container
end = container.bottom()

# Returns true if the container has a certain index
container.contains(0)

# Returns true if the container has a certain value
# Note that pointer types will compare pointers, not the actual value
container.contains(obj)
```

They have a few functions useful for adding and replacing values, which can be used like so:
```
# Adds a defaulted value
container.push()

# Adds a value by copy
obj = String("Hello")
container.push(obj)

# Adds a value by moving
obj = String("Hello")
container.push(std::move(obj))

# Inserts a value after an index by copy
obj = String("Hello")
container.push(0, obj)

# Inserts a value after an index by moving
obj = String("Hello")
container.push(0, std::move(obj))

# Replaces a value at an index by copy
obj = String("Hello")
container.replace(0, obj)

# Replaces a value at an index by moving
obj = String("Hello")
container.replace(0, std::move(obj))
```

There are also useful resizing functions for the containers, which can be used like so:
```
# Adds amt defaulted elements to the container
container.resize(10)

# Fills the container with amt elements, supplied by the function lambda
container.resize(10, [](obj, index) {
   obj = String("Hello")
})

# In a static array, this can be used instead
container.resize([](obj) {
   obj = String("Hello")
})
```

There are also a few functions useful for removing values, which can be used like so (Note that it will always invalidate the object in the array, unless copied out):
```
# Removes a value from the top of the container
container.pop()

# Removes a value from a specified index
container.pop(0)
```

Lastly, there are functions that can iterate through the container.  They can be used like so:
```
# Iterates through the container forward, is guaranteed to be implemented
container.forEach([](index, obj) {
   print(obj)
})

# Iterates through the container in reverse, is NOT guaranteed to be implemented 
# Ex: TForwardList cannot reverse iterate
container.forEachReverse([](index, obj) {
   print(obj)
})
```

## Setup

The bare minimum setup is C++11, though it is recommended to use at least C++17 for if constexpr.  The easiest method is to use git submodules, but copying into your project should work fine.

### Copying

Copy the sstl folder somewhere inside of your project, include the directory (preferably the one above sstl, to avoid include clutter), then use and compile.

### Submodule

The recommended method is to load as a submodule, this setup may be more complex initially, but allows for a lot of freedom.  I recommend setting up the project as a subdirectory to avoid too much clutter in your main project.

Run git submodule like so, replace third_party/simplestl with any name you want.

```
git submodule add https://github.com/StrideStudios/SimpleSTL third_party/simplestl
```

If using CMake, add the subdirectory into your project, Link the library, then use!
```
add_subdirectory(simplestl)
target_link_libraries(your_target SimpleSTL)
```

You can also compile separately in this directory by using the copy method of adding the source files, but this is not recommended as the source may change.