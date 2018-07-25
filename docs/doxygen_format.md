# Xmax doxygen doc format

websit: http://www.stack.nl/~dimitri/doxygen/manual/docblocks.html

## Copyright Header
c++ copyright head of all c++ files.

```c++
/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
```




## Class Format
```c++
/**
*  brief-description.
*/

```

## Function Format

Default format
```c++
/**
* fuction-description.
* @param[in] name param-description
* @return return-description
*/

```
Simple format
```c++
/**
*  fuction-description.
*/

```

## Value Format
```c++
/*!< value-description */  
```

Example:
```c++
int this_is_value;/*!< this is a value */  
```