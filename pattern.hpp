#ifndef PATTERN_HPP
#define PATTERN_HPP

#include <vector>
#include <algorithm>
#include <string>

template <typename element>
struct pattern_t
{
    int count;
    std::vector<pattern_t<element>> sub_patterns;
    std::vector<element> elements;
    pattern_t() : count(1) {}
    pattern_t(std::vector<pattern_t> l) : count(1), sub_patterns(l) {}
    inline const std::string make_unique() const noexcept 
    {
        std::string unique_identification;
        if(sub_patterns.empty())
        {
            for(const element& e : elements)
                unique_identification += e.unique_identification();
            return unique_identification;
        }
        for(const pattern_t<element>& p : sub_patterns)
        {
            unique_identification += p.make_unique() + "|";
        }
        return std::to_string(count) + "*[" + unique_identification + "]";
    }
    inline bool operator==(const pattern_t<element>& rhs) const noexcept {return make_unique() == rhs.make_unique();}
    inline bool operator!=(const pattern_t<element>& rhs) const noexcept {return make_unique() != rhs.make_unique();}
};

template <typename iterable_list, typename element>
void initialize_first_pattern(const iterable_list& list_of_elements, pattern_t<element>& list_of_pattern_in)
{
    for(const element& e : list_of_elements)
    {
        pattern_t<element> p;
        p.elements.push_back(e);
        list_of_pattern_in.sub_patterns.push_back(p);
    }
}
template <typename iterable_list, typename element>
void factorize_messages_patterns(const iterable_list& list_of_elements, pattern_t<element>& list_of_pattern_in)
{
    initialize_first_pattern<iterable_list, element>(list_of_elements, list_of_pattern_in);
    pattern_t<element> list_of_pattern_out;
    pattern_t<element> pattern_window;
    int level_iteration = 0;
    while(list_of_pattern_in != list_of_pattern_out)
    {
        if(level_iteration > 0)
        {
            list_of_pattern_in = pattern_t<element>(list_of_pattern_out);
            list_of_pattern_out.sub_patterns.clear();
            pattern_window.sub_patterns.clear();
        }
        level_iteration++;
        for(const pattern_t<element>& it : list_of_pattern_in.sub_patterns)
        {
            const auto first_occurence_of_it_in_pattern = std::find_if(pattern_window.sub_patterns.begin(), pattern_window.sub_patterns.end(), [&it](const pattern_t<element>& p){return p == it;});
            if(first_occurence_of_it_in_pattern != pattern_window.sub_patterns.end())
            {
                if(list_of_pattern_out.sub_patterns.empty())
                {
                    if(pattern_window.sub_patterns.begin() < first_occurence_of_it_in_pattern)
                    {
                        for(const pattern_t<element>& p : std::vector<pattern_t<element>>(pattern_window.sub_patterns.begin(), first_occurence_of_it_in_pattern))
                            list_of_pattern_out.sub_patterns.push_back(p);
                        list_of_pattern_out.sub_patterns.push_back(pattern_t<element>(std::vector<pattern_t<element>>(first_occurence_of_it_in_pattern, pattern_window.sub_patterns.end())));
                    }
                    else
                    {
                        list_of_pattern_out.sub_patterns.push_back(pattern_window);
                    }
                }
                else
                {
                    auto last_pattern_length = list_of_pattern_out.sub_patterns.back().sub_patterns.size();
                    if(last_pattern_length == 0) last_pattern_length = list_of_pattern_out.sub_patterns.back().elements.size();
                    if(pattern_window.sub_patterns.begin() < first_occurence_of_it_in_pattern)
                    {
                        if(last_pattern_length <= pattern_window.sub_patterns.size())
                        {
                        	if(std::vector<pattern_t<element>>(pattern_window.sub_patterns.begin(), pattern_window.sub_patterns.begin() + last_pattern_length) == list_of_pattern_out.sub_patterns.back().sub_patterns)
                        	{
	                            list_of_pattern_out.sub_patterns.back().count++;
	                            if(pattern_window.sub_patterns.begin() + last_pattern_length < first_occurence_of_it_in_pattern)
	                            {
	                                for(const pattern_t<element>& p : std::vector<pattern_t<element>>(pattern_window.sub_patterns.begin() + last_pattern_length, first_occurence_of_it_in_pattern))
	                                    list_of_pattern_out.sub_patterns.push_back(p);
	                            }
	                            else
	                            {
	                                list_of_pattern_out.sub_patterns.push_back(pattern_t<element>(std::vector<pattern_t<element>>(pattern_window.sub_patterns.begin() + last_pattern_length, pattern_window.sub_patterns.end())));
	                            }
	                        }
	                        else
	                        {
	                        	for(const pattern_t<element>& p : std::vector<pattern_t<element>>(pattern_window.sub_patterns.begin(), first_occurence_of_it_in_pattern))
                                	list_of_pattern_out.sub_patterns.push_back(p);
                            	list_of_pattern_out.sub_patterns.push_back(pattern_t<element>(std::vector<pattern_t<element>>(first_occurence_of_it_in_pattern, pattern_window.sub_patterns.end())));
	                        }
                        }
                        else
                        {
                            for(const pattern_t<element>& p : std::vector<pattern_t<element>>(pattern_window.sub_patterns.begin(), first_occurence_of_it_in_pattern))
                                list_of_pattern_out.sub_patterns.push_back(p);
                            list_of_pattern_out.sub_patterns.push_back(pattern_t<element>(std::vector<pattern_t<element>>(first_occurence_of_it_in_pattern, pattern_window.sub_patterns.end())));
                        }
                    }
                    else
                    {
                        if(last_pattern_length <= pattern_window.sub_patterns.size())
                        {
                        	if(std::vector<pattern_t<element>>(pattern_window.sub_patterns.begin(), pattern_window.sub_patterns.begin() + last_pattern_length) == list_of_pattern_out.sub_patterns.back().sub_patterns)
                        	{
	                            list_of_pattern_out.sub_patterns.back().count++;
	                            if(pattern_window.sub_patterns.begin() + last_pattern_length < pattern_window.sub_patterns.end())
	                            {
	                                for(const pattern_t<element>& p : std::vector<pattern_t<element>>(pattern_window.sub_patterns.begin() + last_pattern_length, pattern_window.sub_patterns.end()))
	                                    list_of_pattern_out.sub_patterns.push_back(p);
	                            }
	                        }
	                        else
	                        {
	                        	list_of_pattern_out.sub_patterns.push_back(pattern_window);
	                        }
                        }
                        else
                        {
                            list_of_pattern_out.sub_patterns.push_back(pattern_window);
                        }
                    }
                }
                pattern_window.sub_patterns.clear();
            }
            pattern_window.sub_patterns.push_back(it);
        }
        if(!pattern_window.sub_patterns.empty())
        {
            if(!list_of_pattern_out.sub_patterns.empty())
            {
                auto last_pattern_length = list_of_pattern_out.sub_patterns.back().sub_patterns.size();
                if(last_pattern_length == 0) last_pattern_length = list_of_pattern_out.sub_patterns.back().elements.size();
                if(last_pattern_length <= pattern_window.sub_patterns.size() && std::vector<pattern_t<element>>(pattern_window.sub_patterns.begin(), pattern_window.sub_patterns.begin() + last_pattern_length) == list_of_pattern_out.sub_patterns.back().sub_patterns)
                {
                    list_of_pattern_out.sub_patterns.back().count++;
                    if(pattern_window.sub_patterns.begin() + last_pattern_length < pattern_window.sub_patterns.end())
                    {
                        for(const pattern_t<element>& p : std::vector<pattern_t<element>>(pattern_window.sub_patterns.begin() + last_pattern_length, pattern_window.sub_patterns.end()))
                                    list_of_pattern_out.sub_patterns.push_back(p);
                    }
                }
                else
                {
                    for(const pattern_t<element> p : pattern_window.sub_patterns)
                        list_of_pattern_out.sub_patterns.push_back(p);
                }
            }
            else
            {
                for(const pattern_t<element> p : pattern_window.sub_patterns)
                        list_of_pattern_out.sub_patterns.push_back(p);
            }
        }
    }
}

#endif