#include <iostream>
#include <string>
#include <vector>

#ifndef ONLINE_JUDGE
#include <boost/assert.hpp>
#include <sstream>
#endif


typedef std::string expression_char_t;
typedef std::vector<expression_char_t> expression_t;

expression_t convert_to_expression(const std::string & txt)
{
    expression_t result;
    for (size_t i = 0 ; i < txt.length() ; ++i)
    {
        if(txt[i] == '(')
        {
            if (i+1<txt.length() && txt[i+1] == '*')
            {
                result.push_back(txt.substr(i, 2));
                ++i;
            }
            else
            {
                result.push_back(txt.substr(i, 1));
            }
        }
        else if(txt[i] == '*')
        {
            if (i + 1 < txt.length() && txt[i + 1] == ')')
            {
                result.push_back(txt.substr(i, 2));
                ++i;
            }
            else
            {
                result.push_back(txt.substr(i, 1));
            }
        }
        else
        {
            result.push_back(txt.substr(i, 1));
        }
    }
    return result;
}

bool is_open_bracket(const expression_char_t & bracket)
{
    return (bracket == "<" || bracket == "{" || bracket == "(" || bracket == "[" || bracket == "(*");
}

bool is_close_bracket(const expression_char_t & bracket)
{
    return (bracket == ">" || bracket == "}" || bracket == ")" || bracket == "]" || bracket == "*)");
}

expression_char_t get_matched_open_bracket(const expression_char_t & close_bracket)
{
    if (close_bracket == ">")
        return "<";
    if (close_bracket == ")")
        return "(";
    if (close_bracket == "*)")
        return "(*";
    if (close_bracket == "}")
        return "{";
    if (close_bracket == "]")
        return "[";

    return "";
}

bool check_expression_nested(const expression_t & expression, size_t & error_pos)
{
    std::vector<expression_char_t> umached_open_brackets;
    for (size_t i = 0 ; i < expression.size() ; ++i)
    {
        if (is_open_bracket(expression[i]))
        {
            umached_open_brackets.push_back(expression[i]);
        }
        else if(is_close_bracket(expression[i]))
        {
            if (umached_open_brackets.empty())
            {
                error_pos = i;
                return false;
            }
            else
            {
                if (get_matched_open_bracket(expression[i]) == umached_open_brackets.back())
                {
                    umached_open_brackets.pop_back();
                }
                else
                {
                    error_pos = i;
                    return false;
                }
            }
        }
    }

    if (umached_open_brackets.size() != 0)
    {
        error_pos = expression.size();
        return false;
    }

    return true;
}

void run(std::istream & in, std::ostream & out)
{
    std::string input;

    while (getline(in, input))
    {
        size_t error_pos(0);
        if (check_expression_nested(convert_to_expression(input), error_pos))
        {
            out << "YES" << std::endl;
        }
        else
        {
            out << "NO " << error_pos + 1 << std::endl;
        }
    }
}

#ifdef ONLINE_JUDGE

int main(int argc, char** argv)
{
    run(std::cin, std::cout);
    return 0;
}

#else

const std::string question = R"StringLiteral((*a++(*)
(*a{+}*)
    <************)>
    ()(***)(**)
   ()(***)(*)
({{}{}}[{(){}[]}
   ([))
 ()(**)
    ()*
 aaaaaaa
    aaa(aaaa
 *******
)StringLiteral";

const std::string answser = R"StringLiteral(NO 6
YES
NO 17
YES
NO 10
NO 17
NO 6
YES
YES
YES
NO 13
YES
)StringLiteral";

int main(int argc, char** argv)
{
    std::istringstream stream_in(question);
    std::stringstream stream_out;
    run(stream_in, stream_out);

    std::string my_answer = stream_out.str();
	BOOST_ASSERT(my_answer == answser);
    return 0;
}
#endif

