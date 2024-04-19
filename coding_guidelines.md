# Coding Guidelines
# Appligent, Inc.

This document outlines the coding standards and best practices for the TriState C++ project. All contributors are encouraged to follow these guidelines to maintain code quality and consistency across the project.

## 1. File and Header Information

- **Header Block**: Each source file should start with a header block that includes the filename, a description of its purpose, author information, company details, copyright notice, and licensing information.
- **Include Guards**: Use `#pragma once` in all header files to prevent multiple inclusions.

## 2. Naming Conventions

- **Variables and Functions**: 
  - Use `camelCase` for local variables and function names, with local variables prefixed with `the`, e.g., `theResult`.
  - Prefix global variables with `g` and static variables with `s`.
- **Classes and Structures**: Use `PascalCase` for class and structure names.
- **Constants and Enumerators**: Use `k` prefix followed by `PascalCase`, e.g., `kZoom100Icon`.
- **Macros**: Use `ALL_CAPS` with underscores for macros and constant definitions.
- **Function Parameters**:
  - Prefix parameters with `in`, `out`, or `io` to indicate whether they are for input, output, or both, respectively.

## 3. Formatting and Style

- **Indentation**: Use 2 spaces per indentation level.
- **Braces**: Place opening braces on its own line following the statement and indented one level. Place closing braces on their own line, aligned with the opening brace.
- **Line Length**: Aim to keep lines under 80 characters where possible.
- **Spacing**: Include spaces around operators and after commas. Avoid spaces between function names and parentheses.

## 4. Commenting and Documentation

- **Function Comments**: Document the purpose, parameters, and expected results of functions immediately above the function definition.
- **Inline Comments**: Use inline comments sparingly and only when the code is not self-explanatory.
- **TODO and FIXME**: Mark incomplete implementations or areas for improvement with `TODO` or `FIXME` comments.

## 5. File Structure

- **Logical Grouping**: Group related functionalities into files and use directories to separate different modules or layers (UI, utilities, backend).
- **Separate Interface and Implementation**: Use header files for declarations and `.cpp` files for definitions.

## 6. Error Handling

- **Exception Handling Style**: Use the `DURING` and `HANDLER` macros for exception handling as required by the Acrobat SDK. Do not mix with C++ exception handling. This ensures compatibility and stability across the SDK's environment.

## 7. Performance Practices

- **Avoid Unnecessary Copies**: Use references and pointers appropriately to avoid unnecessary copying of objects.
- **Resource Management**: Implement RAII (Resource Acquisition Is Initialization) for managing resources like memory, files, etc.

## 8. Security Practices

- **Input Validation**: Ensure that all inputs to functions are validated before use.
- **Safe Functions**: Prefer using safe versions of functions to avoid common vulnerabilities like buffer overflows.

## Conclusion

Adherence to these guidelines is essential for maintaining the quality and integrity of the codebase. Review and incorporate these practices into your daily coding activities.

