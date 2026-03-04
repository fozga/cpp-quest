#include "exercise.hpp"

#include <sstream>

Node make_int(int v) {
    return Node{IntegerLiteral{v}};
}

Node make_add(Node left, Node right) {
    return Node{Add{std::make_unique<Node>(std::move(left)),
                    std::make_unique<Node>(std::move(right))}};
}

Node make_mul(Node left, Node right) {
    return Node{Mul{std::make_unique<Node>(std::move(left)),
                    std::make_unique<Node>(std::move(right))}};
}

struct EvalVisitor {
    int operator()(const IntegerLiteral& lit) const {
        return lit.value;
    }
    int operator()(const Add& a) const {
        return evaluate(*a.left) + evaluate(*a.right);
    }
    int operator()(const Mul& m) const {
        return evaluate(*m.left) * evaluate(*m.right);
    }
};

int evaluate(const Node& n) {
    return std::visit(EvalVisitor{}, n.data);
}

struct ToStringVisitor {
    std::string operator()(const IntegerLiteral& lit) const {
        return std::to_string(lit.value);
    }
    std::string operator()(const Add& a) const {
        return "(" + to_string(*a.left) + " + " + to_string(*a.right) + ")";
    }
    std::string operator()(const Mul& m) const {
        return "(" + to_string(*m.left) + " * " + to_string(*m.right) + ")";
    }
};

std::string to_string(const Node& n) {
    return std::visit(ToStringVisitor{}, n.data);
}
