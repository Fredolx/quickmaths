#include "quickmaths.hpp"
#include <iostream>
#include <format>
#include <vector>
#include "ai.hpp"
#include "exprtk.hpp"

extern "C"
{
    StringDouble GetResult(const char *query)
    {
        try
        {
            std::string queryStr(query);
            auto result = AI::Generate(queryStr);
            auto resultNum = Calculate(result);
            return {strdup(result.c_str()), resultNum};
        }
        catch (const std::exception &e)
        {
            std::cerr << "Caught exception: " << e.what() << std::endl;
        }
        catch (...)
        {
            std::cerr << "Caught unknown exception" << std::endl;
        }
        return {"No dice", -1};
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