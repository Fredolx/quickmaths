#pragma once

#include <string>
#include <llama.h>
class AI
{
public:
    static void Initialize();
    static void Shutdown();
    static std::string Generate(std::string);
    static constexpr int ngl = 99;
    static constexpr int n_predict = 100;

private:
    static std::string GetOutput(const std::string &input);
    static std::string Trim(const std::string &input);
    static llama_model *model;
    static const llama_vocab *vocab;
    static constexpr char systemMessage[] = "You are an equation parser. From a user prompt, you give an equation. For each user prompt, you only give a single equation in a machine readable format. For example:\nInput: 10 pounds to ounces\nOutput: 10 * 16\nJust like this example, you do not give any explanation, you just give an equation with nothing else attached to it, no result, no punctuation.\n\n";
};