#pragma once

#include <string>
#include <ort_genai_c.h>
#include <ort_genai.h>

class AI
{
public:
    static void Initialize();
    static void Shutdown();
    static std::string Generate(std::string);

private:
    static std::unique_ptr<OgaGenerator> generator;
    static std::unique_ptr<OgaModel> model;
    static std::unique_ptr<OgaTokenizer> tokenizer;
    static std::unique_ptr<OgaGeneratorParams> params;
    static std::string GetOutput(const std::string &input);
    static std::string Trim(const std::string &input);
};