#[derive(PartialEq)]
enum TokenType {
    Number,
    Identifier,
    Operator,
    LeftParentheses,
    RightParentheses,
}

struct Token {
    text: String,
    token_type: TokenType,
}

fn tokenize(input: &str) -> Vec<Token> {
    let mut chars = input.chars().peekable();
    let mut tokens: Vec<Token> = Vec::new();
    while let Some(c) = chars.next() {
        let token_type: TokenType;
        let mut text = String::new();
        text.push(c);
        if c.is_ascii_digit() {
            // Number
            token_type = TokenType::Number;
            while let Some(nc) = chars.peek() {
                if nc.is_ascii_digit() || *nc == '.' {
                    text.push(chars.next().unwrap());
                } else {
                    break;
                }
            }
        } else if c.is_ascii_alphabetic() {
            // Identifier
            token_type = TokenType::Identifier;
            while let Some(nc) = chars.peek() {
                if nc.is_alphanumeric() {
                    text.push(chars.next().unwrap());
                } else {
                    break;
                }
            }
        } else if ['+', '-', '*', '/', '^'].contains(&c) {
            // Operator
            token_type = TokenType::Operator;
        } else if c == '(' {
            // Left parentheses
            token_type = TokenType::LeftParentheses;
        } else if c == ')' {
            // Right parentheses
            token_type = TokenType::RightParentheses;
        } else if c.is_whitespace() {
            // Skip whitespace
            continue;
        } else {
            // 🤷
            panic!("Unexpected symbol {}", c);
        }
        tokens.push(Token { text, token_type });
    }

    tokens
}

fn precedence(token: &Token) -> u8 {
    match token.text.as_str() {
        "^" => 2,
        "*" | "/" => 1,
        _ => 0,
    }
}

fn shunting_yard<'a>(tokens: &'a Vec<Token>) -> Vec<&'a Token> {
    // ☝️ signature shows that returned values live as long as the input.
    let mut queue: Vec<&Token> = Vec::new();
    let mut stack: Vec<&Token> = Vec::new();

    for token in tokens {
        match token.token_type {
            TokenType::Number | TokenType::Identifier => queue.push(token),
            TokenType::Operator => {
                while !stack.is_empty()
                    && stack.last().unwrap().token_type != TokenType::LeftParentheses
                    && precedence(stack.last().unwrap()) > precedence(token)
                {
                    queue.push(stack.pop().unwrap());
                }
                stack.push(token);
            }
            TokenType::LeftParentheses => stack.push(token),
            TokenType::RightParentheses => {
                while !stack.is_empty()
                    && stack.last().unwrap().token_type != TokenType::LeftParentheses
                {
                    queue.push(stack.pop().unwrap());
                }
                if !stack.is_empty() {
                    stack.pop();
                }
            }
        }
    }

    while !stack.is_empty() {
        queue.push(stack.pop().unwrap());
    }

    queue
}

fn evaluate(tokens: Vec<&Token>) -> f64 {
    // Stack holds only numeric values
    let mut stack: Vec<f64> = Vec::new();

    for token in tokens {
        // Push numbers to stack.
        // When operator is found, pop values from stack,
        // compute the result and push it back to stack.
        if token.token_type == TokenType::Number {
            let value = match token.text.parse::<f64>() {
                Ok(value) => value,
                Err(err) => panic!("Error parsing number {}: {}", token.text, err),
            };
            stack.push(value);
        } else if token.token_type == TokenType::Operator {
            if stack.len() < 2 {
                panic!("Stack does not have enough elements");
            }
            let rhs = stack.pop().unwrap();
            let lhs = stack.pop().unwrap();
            stack.push(match token.text.as_str() {
                "+" => lhs + rhs,
                "-" => lhs - rhs,
                "*" => lhs * rhs,
                "/" => lhs / rhs,
                "^" => lhs.powf(rhs),
                _ => panic!("Unexpected operator {}", token.text),
            });
        }
    }

    *stack.first().unwrap()
}

fn main() {
    let args: Vec<String> = std::env::args().collect();
    if args.len() != 2 {
        eprintln!("Input is missing or malformed");
        std::process::exit(1);
    }
    let input = &args[1];

    let tokens = tokenize(input);
    let reverse_polish_notation = shunting_yard(&tokens);
    let result = evaluate(reverse_polish_notation);
    println!("{}", result);
}
