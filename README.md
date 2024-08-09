# Qwistys Lib

Qwistys Lib is a collection of commonly used data structures and utilities designed for efficient and easy usage. This library aims to provide basic implementations for usual stuff.
I alwasy reiplment those ... So .... here we are.

## Features
- (Flex Array)[docs/flexa.mb]
- (Allocator)[docs/alloc.md]
- (Avl Tree)[docs/avltree.md]

## Table of Contents

- [Installation](#installation)
- [Usage](#usage)
- [API Documentation](#api-documentation)
- [Customization](#customization)
- [Contributing](#contributing)
- [License](#license)

## Installation

To use Qwistys Lib in your project, simply include the header files and define `QWISTYS_ARRAY_IMPLEMENTATION` in one of your source files.

### Example

1. Clone the repository or download the header files.
2. Include `.h` you need and dont forget the QWISTYS_*_IMPLEMENTATION flag.

```c
#include "qwistys_lib.h"

#define QWISTYS_ALLOC_IMPLEMENTATION
#include "qwistys_alloc.h"

