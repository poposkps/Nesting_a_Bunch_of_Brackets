#include <iostream>
#include <string>
#include <vector>

#ifndef ONLINE_JUDGE
#include <boost/assert.hpp>
#include <sstream>
#endif


struct pos_range
{
    pos_range()
        : begin_(0), end_(0){}
    pos_range(size_t begin, size_t end)
        : begin_(begin), end_(end){}

    operator bool() const { return !empty(); }
    bool empty() const { return begin_ >= end_; }
    size_t back() const { return end_ - 1; }
    size_t begin_;
    size_t end_;
};

pos_range find_matched_bracket_pos(const std::string & txt, size_t from)
{
    if (from >= txt.length())
        return pos_range();

    for(size_t i = from; i < txt.length() -1; ++i)
    {
        if (txt[i] == '<' || txt[i] == '>' 
            || txt[i] == '{' || txt[i] == '}' 
            || txt[i] == ')')
        {
            return pos_range(i, i+1);
        }

        if (txt[i] == '(')
        {
            if (txt[i + 1] == '*')
            {
                return pos_range(i, i + 2);
            }
            else
            {
                return pos_range(i, i + 1);
            }
        }

        if (txt[i] == '*' && txt[i + 1] == ')')
        {
            return pos_range(i, i + 2);
        }
    }

    if (txt.back() == '<' || txt.back() == '>'
        || txt.back() == '{' || txt.back() == '}'
        || txt.back() == '(' || txt.back() == ')')
    {
        return pos_range(txt.length() - 1, 1);
    }

    return pos_range();
}

bool is_open_bracket(const std::string & bracket)
{
    return (bracket == "<" || bracket == "{" || bracket == "(" || bracket == "(*");
}

std::string get_matched_open_bracket(const std::string & close_bracket)
{
    if (close_bracket == ">")
        return "<";
    if (close_bracket == ")")
        return "(";
    if (close_bracket == "*)")
        return "(*";
    if (close_bracket == "}")
        return "{";

    return "";
}

bool check_brackets_matched(const std::string & txt, size_t & first_unmatched_pos)
{
    struct bracket_info_t
    {
        std::string bracket;
        pos_range pos;
    };
    std::vector<bracket_info_t> brackets;

    size_t current_pos(0);
    while (pos_range bracket_pos = find_matched_bracket_pos(txt, current_pos))
    {
        std::string bracket = txt.substr(bracket_pos.begin_, bracket_pos.end_ - bracket_pos.begin_);
        
        if (is_open_bracket(bracket))
        {
            bracket_info_t bracket_info;
            bracket_info.bracket = bracket;
            bracket_info.pos = bracket_pos;
            brackets.push_back(bracket_info);
        }
        else
        { // is close bracket
            if (brackets.empty())
            {
                first_unmatched_pos = bracket_pos.back();
                return false;
            }
            else
            {
                if (get_matched_open_bracket(bracket) == brackets.back().bracket)
                {
                    brackets.pop_back();
                }
                else
                {
                    first_unmatched_pos = brackets.back().pos.back();
                    return false;
                }
            }
        }


        // to find next
        current_pos = bracket_pos.end_;
    }

    if (brackets.size() != 0)
    {
        first_unmatched_pos = brackets.back().pos.back();
        return false;
    }

    return true;
}

void run(std::istream & in, std::ostream & out)
{
    std::string input;

    while (in >> input)
    {
        size_t first_unmatched_pos(0);
        if (check_brackets_matched(input, first_unmatched_pos))
        {
            out << "YES" << std::endl;
        }
        else
        {
            out << "NO " << first_unmatched_pos << std::endl;
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
