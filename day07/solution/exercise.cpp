#include "exercise.hpp"

static int log_counter = 0;

Widget::Widget() : name(""), value(0) {}

Widget::Widget(std::string n, int v) : name(std::move(n)), value(v) {}

void increment_log_count() { ++log_counter; }

int get_log_count() { return log_counter; }

void reset_log_count() { log_counter = 0; }
