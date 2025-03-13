#include "ai.hpp"
#include <iostream>
#include <format>
#include <ort_genai_c.h>
#include <ort_genai.h>

std::unique_ptr<OgaGenerator> AI::generator = nullptr;
std::unique_ptr<OgaModel> AI::model = nullptr;
std::unique_ptr<OgaTokenizer> AI::tokenizer = nullptr;
std::unique_ptr<OgaGeneratorParams> AI::params = nullptr;

void AI::Initialize()
{
    model = OgaModel::Create("model");
    tokenizer = OgaTokenizer::Create(*model);
    params = OgaGeneratorParams::Create(*model);
    params->SetSearchOption("max_length", 200);
    generator = OgaGenerator::Create(*model, *params);
}

std::string AI::Generate(std::string input)
{
    auto sequences = OgaSequences::Create();
    input = std::format(R"(
        Convert the following text into a mathematical equation.
        Output only the equation, without any explanation.

        Example:
        Input: 500 from 250 in %
        Output: 500 / 250 * 100

        Input: 7 to 10 in percentage
        Output: (10 - 7) / 7 * 100

        Input: 4.5 LB for 20$, what is it in $ per pound
        Output: 20 / 4.5

        Input: 7 oz for 20$ in $/lb
        Output: 20 * (16 / 7)

        Input: {}
        Output: )",
                        input);
    input = std::format("<|user|>\n{} <|end|>\n<|assistant|>", input);
    tokenizer->Encode(input.c_str(), *sequences);
    generator->AppendTokenSequences(*sequences);
    while (!generator->IsDone())
    {
        generator->GenerateNextToken();
    }
    auto output_sequence = generator->GetSequenceData(0);
    auto output_string = tokenizer->Decode(output_sequence, generator->GetSequenceCount(0));
    return GetOutput(std::string(output_string.p_));
}

std::string AI::Trim(const std::string &str)
{
    auto start = std::find_if_not(str.begin(), str.end(), ::isspace);
    auto end = std::find_if_not(str.rbegin(), str.rend(), ::isspace).base();
    return (start < end) ? std::string(start, end) : "";
}

std::string AI::GetOutput(const std::string &input)
{
    const std::string marker = "Output: ";
    size_t pos = input.rfind(marker);
    if (pos != std::string::npos)
    {
        return Trim(input.substr(pos + marker.length()));
    }
    return "";
}

void AI::Shutdown()
{
    generator.reset();
    params.reset();
    tokenizer.reset();
    model.reset();
    OgaShutdown();
}