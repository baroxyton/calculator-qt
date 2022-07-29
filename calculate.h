#include <string>
#include <iostream>
#include <vector>
#include <codecvt>
#include <locale>
#include <cmath>
namespace calculation
{
    std::string trigMode = "deg";

    double deg2rad(double degree)
    {
        return degree * ((double)3.141592653589793238 / 180);
    }
    double rad2deg(double radiants)
    {
        return radiants / ((double)3.141592673789793238 / 180);
    }
    double grad2rad(double gradient)
    {
        return deg2rad((double)gradient * 1.8 / 2);
    }
    double toRad(double input)
    {
        if (trigMode == "deg")
        {
            return deg2rad(input);
        }
        if (trigMode == "rad")
        {
            return input;
        }
        else
        {
            return grad2rad(input);
        }
    }
    double minifyDegrees(double degrees)
    {
        if (trigMode == "deg")
        {
            while (degrees > 360)
            {
                degrees -= 360;
            }
            while (degrees < -360)
            {
                degrees += 360;
            }
            return degrees;
        }
        if (trigMode == "rad")
        {
            while (degrees > (double)6.283185307179586)
            {
                degrees -= (double)6.283185307179586;
            }
            while (degrees < (double)-6.283185307179586)
            {
                degrees += (double)6.283185307179586;
            }
            return degrees;
        }
        else
        {
            while (degrees > 400)
            {
                degrees -= 400;
            }
            while (degrees < -400)
            {
                degrees += 400;
            }
            return degrees;
        }
    }
    double log(double base, double num)
    {
        return ::log(num) / ::log(base);
    }
    double xrooty(double x, double y)
    {
        return pow(y, 1 / x);
    }
    double factorial(int num)
    {
        double result = 1;
        for (int i = 0; i < num; i++)
        {
            result *= (i + 1);
        }
        return result;
    }
    double nCr(double n, double r)
    {
        return factorial(n) / (factorial(r) * factorial(n - r));
    }
    double nPr(double n, double r)
    {
        return factorial(n) / factorial(n - r);
    }
    // 1, 0, -1, 0, ...
    // https://socratic.org/questions/how-do-you-find-the-taylor-series-of-f-x-cos-x
    double costyler(double degrees, int perceison = 15)
    {
        degrees = minifyDegrees(degrees);
        double result = 1;
        bool is_positive = false;
        double radiant = toRad(degrees);
        for (int i = 1; i < perceison; i++)
        {
            double denominater = factorial(i * 2);
            double numerator = pow(radiant, i * 2);
            double multiplier = is_positive ? 1 : -1;
            result += (numerator / denominater) * multiplier;
            is_positive = !is_positive;
        }
        return result;
    };
    // 0, 1, 0, -1
    double sintyler(double degrees, int perceison = 15)
    {
        degrees = minifyDegrees(degrees);
        bool is_positive = false;
        double radiant = toRad(degrees);
        double result = radiant;
        for (int i = 1; i < perceison; i++)
        {
            double denominater = factorial((i * 2) + 1);
            double numerator = pow(radiant, (i * 2) + 1);
            double multiplier = is_positive ? 1 : -1;
            result += (numerator / denominater) * multiplier;
            is_positive = !is_positive;
        }
        return result;
    }
    double tantyler(double degrees)
    {
        return sintyler(degrees) / costyler(degrees);
    };
    double asin(double degrees)
    {

        double radiant = toRad(degrees);
        return ::asin(radiant);
    }
    double acos(double degrees)
    {
        double radiant = toRad(degrees);
        return ::acos(radiant);
    }
    double atan(double degrees)
    {
        double radiant = toRad(degrees);
        return ::atan(degrees);
    }

    std::string ws2s(const std::wstring &utf16Str)
    {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
        return conv.to_bytes(utf16Str);
    }
    std::wstring s2ws(const std::string &utf8Str)
    {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
        return conv.from_bytes(utf8Str);
    }

    struct token
    {
        token(std::string type = "", double value = -1, std::string value_string = "") : type(type), value(value), value_string(value_string) {}
        std::string type;
        double value = -1;
        std::string value_string;
    };
    std::vector<token> tokenizer(std::wstring calculation)
    {
        bool is_error = false;
        bool in_number = false;
        bool in_operator = false;
        bool in_scientific = false;
        int paranthesesLayer = 0;

        std::string current_token;
        std::vector<token> result;

        for (int i = 0; i < calculation.size(); i++)
        {
            if (is_error)
            {
                return std::vector<token>{};
            }
            std::string curr = ws2s(std::wstring{calculation[i]});
            std::string prev = i > 0 ? ws2s(std::wstring{calculation[i - 1]}) : "START";
            std::string next = ((i + 1) < calculation.size()) ? ws2s(std::wstring{calculation[i + 1]}) : "END";

            current_token += curr;
            // Is currently parsing number
            if (in_number)
            {
                if (curr == "E" && (next == "+" || next == "-"))
                {
                    in_scientific = true;
                    continue;
                }
                if ((curr == "+" || curr == "-") && in_scientific && prev == "E")
                {
                    continue;
                }
                if ((std::string{"0123456789.E"}).find(next) == std::string::npos)
                {
                    in_scientific = false;
                    in_number = false;
                    double parsed_num = atof(current_token.c_str());
                    current_token = "";
                    result.push_back(token{"number", parsed_num});
                }
                continue;
            }

            // Is in parantheses
            if (paranthesesLayer > 0 && curr != ")")
            {
                continue;
            }

            // Is in multi-char operator (eg [COS])
            if (in_operator)
            {
                if (curr == "]")
                {
                    current_token.pop_back();
                    in_operator = false;
                    result.push_back(token{current_token});
                    current_token = "";
                }
                continue;
            }

            // Check for number
            if ((std::string{"0123456789"}).find(curr) != std::string::npos)
            {
                in_number = true;

                // Check for 1 character number
                if ((std::string{"0123456789.E"}).find(next) == std::string::npos)
                {
                    in_number = false;
                    double parsed_num = atof(current_token.c_str());
                    current_token = "";
                    result.push_back(token{"number", parsed_num});
                }
                continue;
            }

            // Check for basic operations

            if (curr == "+")
            {
                current_token = "";
                result.push_back(token{"plus"});
                continue;
            }
            if (curr == "-")
            {
                if (i == 0)
                {
                    in_number = true;
                    continue;
                }
                current_token = "";
                result.push_back(token{"minus"});
                continue;
            }
            if (curr == "×")
            {
                current_token = "";
                result.push_back(token{"multiply"});
                continue;
            }
            if (curr == "÷")
            {
                current_token = "";
                result.push_back(token{"divide"});
                continue;
            }
            if (curr == "^")
            {
                current_token = "";
                result.push_back(token{"power"});
                continue;
            }
            if (curr == "√")
            {
                current_token = "";
                result.push_back(token{"sqrt"});
            }

            // Factorial
            if (curr == "!")
            {
                current_token = "";
                result.push_back(token{"factorial"});
            }

            // Check for PI and euler number
            if (curr == "π")
            {
                current_token = "";
                result.push_back(token{"number", (double)3.141592653589793238});
                continue;
            }
            if (curr == "e")
            {
                current_token = "";
                result.push_back(token{"number", (double)2.718281828459045});
                continue;
            }

            // Check for special operators (eg [COS])
            if (curr == "[")
            {
                current_token = "";
                in_operator = true;

                continue;
            }

            // Check for parentheses
            if (curr == "(")
            {
                paranthesesLayer++;
                continue;
            }
            if (curr == ")")
            {
                paranthesesLayer--;
                if (paranthesesLayer == 0)
                {
                    result.push_back(token{"parantheses", 0, current_token});
                    current_token = "";
                }
                continue;
            }
            if (curr == ",")
            {
                result.push_back(token{"average"});
                current_token = "";
                continue;
            }
            if (curr == "%")
            {
                result.push_back(token{"mod"});
                current_token = "";
                continue;
            }
        }
        return result;
    }
    double calculate(std::wstring calculation)
    {
        auto tokenized = tokenizer(calculation);
        bool is_error = tokenized.size() == 0;
        int average_num = 0;
        double average_value = 0;
        int last_average = 0;

        if (is_error)
        {
            return NAN;
        }
        // Start with first number
        double result = tokenized[0].value;
        // Follow PEMDAS
        for (int i = 0; i < tokenized.size(); i++)
        {
            // first do parantheses
            // replace parantheses in vector with number
            if (tokenized[i].type == "parantheses")
            {
                std::string without_parantheses = tokenized[i].value_string.substr(1, tokenized[i].value_string.size() - 2);
                tokenized[i].value = calculate(s2ws(without_parantheses));
                tokenized[i].type = "number";
                tokenized[i].value_string = "";
            }
        }
        // Averages
        for (int i = 0; i < tokenized.size(); i++)
        {
            if (tokenized[i].type == "average")
            {
                bool is_first = (average_num == 0);
                if (is_first)
                {
                    double avrg = (tokenized[i - 1].value + tokenized[i + 1].value) / 2;
                    average_num = 2;
                    tokenized[i + 1].value = avrg;
                    average_value = avrg;
                    tokenized.erase(tokenized.begin() + i - 1);
                    tokenized.erase(tokenized.begin() + i - 1);
                    i--;
                    continue;
                }
                double average_expanded = average_value * average_num;
                average_expanded += tokenized[i + 1].value;
                average_num++;
                double average_new = average_expanded / average_num;
                tokenized[i + 1].value = average_new;
                average_value = average_new;
                tokenized.erase(tokenized.begin() + i - 1);
                tokenized.erase(tokenized.begin() + i - 1);
                i--;
                continue;
            }
        }
        // Exponents
        for (int i = 0; i < tokenized.size(); i++)
        {
            if (tokenized[i].type == "power")
            {
                tokenized[i + 1].value = (double)pow(tokenized[i - 1].value, tokenized[i + 1].value);
                tokenized[i + 1].type = "number";
                tokenized.erase(tokenized.begin() + i - 1);
                tokenized.erase(tokenized.begin() + i - 1);
                i--;
            }
            if (tokenized[i].type == "sqrt")
            {
                tokenized[i - 1].value = xrooty(2, tokenized[i - 1].value);
                tokenized.erase(tokenized.begin() + i);
                i--;
            }
            if (tokenized[i].type == "LOG")
            {
                tokenized[i - 1].value = log(10, tokenized[i - 1].value);
                tokenized.erase(tokenized.begin() + i);
                i--;
            }
            if (tokenized[i].type == "LOG_2")
            {
                tokenized[i - 1].value = log(2, tokenized[i - 1].value);
                tokenized.erase(tokenized.begin() + i);
                i--;
            }
            if (tokenized[i].type == "LN")
            {
                tokenized[i - 1].value = log((double)2.718281828459045, tokenized[i - 1].value);
                tokenized.erase(tokenized.begin() + i);
                i--;
            }
            if (tokenized[i].type == "y√x")
            {
                tokenized[i + 1].value = xrooty(tokenized[i + 1].value, tokenized[i - 1].value);
                tokenized[i + 1].type = "number";
                tokenized.erase(tokenized.begin() + i - 1);
                tokenized.erase(tokenized.begin() + i - 1);
                i--;
            }
        }
        // Factorial, cos, nCr, nPr
        for (int i = 0; i < tokenized.size(); i++)
        {
            if (tokenized[i].type == "factorial")
            {
                tokenized[i - 1].value = factorial(tokenized[i - 1].value);
                tokenized.erase(tokenized.begin() + i);
            }
            if (tokenized[i].type == "COS")
            {
                tokenized[i - 1].value = costyler(tokenized[i - 1].value);
                tokenized.erase(tokenized.begin() + i);
            }
            if (tokenized[i].type == "SIN")
            {
                tokenized[i - 1].value = sintyler(tokenized[i - 1].value);
                tokenized.erase(tokenized.begin() + i);
            }
            if (tokenized[i].type == "TAN")
            {
                tokenized[i - 1].value = tantyler(tokenized[i - 1].value);
                tokenized.erase(tokenized.begin() + i);
            }
            if (tokenized[i].type == "ACOS")
            {
                tokenized[i - 1].value = acos(tokenized[i - 1].value);
                tokenized.erase(tokenized.begin() + i);
            }
            if (tokenized[i].type == "ASIN")
            {
                tokenized[i - 1].value = asin(tokenized[i - 1].value);
                tokenized.erase(tokenized.begin() + i);
            }
            if (tokenized[i].type == "ATAN")
            {
                tokenized[i - 1].value = atan(tokenized[i - 1].value);
                tokenized.erase(tokenized.begin() + i);
            }
            if (tokenized[i].type == "nCr")
            {

                tokenized[i + 1].value = nCr(tokenized[i - 1].value, tokenized[i + 1].value);
                tokenized[i + 1].type = "number";
                tokenized.erase(tokenized.begin() + i - 1);
                tokenized.erase(tokenized.begin() + i - 1);
                i--;
            }
            if (tokenized[i].type == "nPr")
            {
                tokenized[i + 1].value = nPr(tokenized[i - 1].value, tokenized[i + 1].value);
                tokenized[i + 1].type = "number";
                tokenized.erase(tokenized.begin() + i - 1);
                tokenized.erase(tokenized.begin() + i - 1);
                i--;
            }
        }

        // Multiply, divide, mod,
        for (int i = 0; i < tokenized.size(); i++)
        {
            if (tokenized[i].type == "multiply")
            {
                tokenized[i + 1].value = tokenized[i - 1].value * tokenized[i + 1].value;
                tokenized[i + 1].type = "number";
                tokenized.erase(tokenized.begin() + i - 1);
                tokenized.erase(tokenized.begin() + i - 1);
                i--;
            }
            if (tokenized[i].type == "divide")
            {
                tokenized[i + 1].value = tokenized[i - 1].value / tokenized[i + 1].value;
                tokenized[i + 1].type = "number";
                tokenized.erase(tokenized.begin() + i - 1);
                tokenized.erase(tokenized.begin() + i - 1);
                i--;
            }
            if (tokenized[i].type == "mod")
            {
                tokenized[i + 1].value = (int)tokenized[i - 1].value % (int)tokenized[i + 1].value;
                tokenized[i + 1].type = "number";
                tokenized.erase(tokenized.begin() + i - 1);
                tokenized.erase(tokenized.begin() + i - 1);
                i--;
            }
        }

        // Plus and minus
        for (int i = 0; i < tokenized.size(); i++)
        {
            if (tokenized[i].type == "plus")
            {
                tokenized[i + 1].value = tokenized[i - 1].value + tokenized[i + 1].value;
                tokenized[i + 1].type = "number";
                tokenized.erase(tokenized.begin() + i - 1);
                tokenized.erase(tokenized.begin() + i - 1);
                i--;
            }
            if (tokenized[i].type == "minus")
            {
                tokenized[i + 1].value = tokenized[i - 1].value - tokenized[i + 1].value;
                tokenized[i + 1].type = "number";
                tokenized.erase(tokenized.begin() + i - 1);
                tokenized.erase(tokenized.begin() + i - 1);
                i--;
            }
        }
        if (tokenized.size() > 1)
        {
            return NAN;
        }
        result = tokenized[0].value;
        return result;
    }
}
