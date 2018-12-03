# SSM - A Speedy, SIMD-enabled Math library.

SSM is an easy-to-use, header-only vector math library targeted primarily to game development and quick applications.
It features:
* Generic types for vectors, matrices and quaternions of any size and type.
* SIMD speedups for available sizes and types. (E.g. vec4f, vec2d)
* Vector and quaternion normalization taken care of by the type system (see below for details).
* A similar language structure to GSL, for easy porting of programs.

## Getting Started

Clone this repo in your project's external dependencies folder, and add the project folder to your include directories.
If you use CMake, you can add `ssm` as a library to your target instead, which will automatically take care of include folders.

## Example usage
See the wiki for more details.
```
// Template vector declaration
ssm::vector<float, 3> vec3(1.0f, 2.0f, 3.0f);

// Utility typedefs for common types
ssm::vec4 vec; //vec is ssm::vector<float, 4>


```

## Testing

This project uses catch2 and CTest for testing. To run the tests, generate the testing targets by running CMake with the BUILD_TESTS option turned on, and then run `make && make test` in your build folder, or build the RUN_TESTS target if you're using Visual Studio.

## Authors

* **Carlo Refice** - [CRefice](https://github.com/CRefice)

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

As far as performance is concerned, this library is as fast as it could be while still using generic types and being general-purpose. As per the usual, if you need to squeeze every last bit of performance out of your hardware, a specific solution might be what you're looking for instead.
