#include "quickmaths.hpp"
#include <iostream>
#include <format>
#include <vector>
#include "ai.hpp"
#include "exprtk.hpp"

extern "C"
{
    double GetResult(std::string query)
    {
        auto result = AI::Generate(query);
        return Calculate(result);
    }

    double Calculate(std::string expression)
    {
        exprtk::parser<double> parser;
        exprtk::expression<double> expr;
        parser.compile(expression, expr);
        return expr.value();
    }

    void Shutdown()
    {
        AI::Shutdown();
    }

    void Initialize()
    {
        AI::Initialize();
    }
}