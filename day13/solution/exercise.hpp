#pragma once

#include <memory>
#include <string>
#include <variant>

struct Node;

struct IntegerLiteral {
    int value;
};

struct Add {
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
};

struct Mul {
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
};

struct Node {
    std::variant<IntegerLiteral, Add, Mul> data;
};

Node make_int(int v);
Node make_add(Node left, Node right);
Node make_mul(Node left, Node right);
int evaluate(const Node& n);
std::string to_string(const Node& n);
