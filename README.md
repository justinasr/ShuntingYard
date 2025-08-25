# Shunting Yard

A mathematical expression evaluator implemented in Rust using the Shunting Yard algorithm to convert infix notation to Reverse Polish Notation (RPN) and then evaluate the result.

## Features

- **Tokenization**: Parses mathematical expressions into tokens (numbers, operators, parentheses)
- **Shunting Yard Algorithm**: Converts infix notation (e.g., `2 + 3 * 4`) to postfix/RPN notation
- **Expression Evaluation**: Evaluates RPN expressions to produce numeric results
- **Memory Efficient**: Uses Rust lifetimes to avoid unnecessary cloning of tokens
- **Error Handling**: Descriptive error messages for invalid input

## Supported Operations

- **Arithmetic Operators**: `+`, `-`, `*`, `/`, `^` (exponentiation)
- **Parentheses**: `(` and `)` for grouping expressions
- **Decimal Numbers**: Supports floating-point numbers (e.g., `3.14`)

## Usage

```bash
# Build the project
cargo build --release

# Run with an expression
cargo run "2 + 3 * 4"
# Output: 14

cargo run "(2 + 3) * 4"
# Output: 20

cargo run "2^3^2"
# Output: 64 (currently left-associative, evaluates as (2^3)^2)

cargo run "3.14 * 2"
# Output: 6.28
```

## Known Limitations

- Exponentiation (`^`) is currently left-associative instead of the mathematically correct right-associative behavior
- No support for unary operators (e.g., negative numbers like `-5`)
- No variable support (identifiers are tokenized but not evaluated)
- Limited error recovery (panics on invalid input rather than graceful error handling)

## Project Structure

```
src/
├── main.rs          # Main implementation with tokenizer, shunting yard, and evaluator
Cargo.toml           # Project configuration
README.md           # This file
```
