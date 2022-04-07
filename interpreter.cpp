#include<iostream>
#include<vector>
#include<fstream>
#include<algorithm>

void translate(std::string & str_line) ;
void do_loop(std::string & str_line) ;
std::vector<std::string> &swap_value(std::vector<std::string> &v) ;
void do_condition(std::string & str_line) ;
bool bool_action(std::vector<std::string> &vec) ;
void creat_variable(const std::string & str_line, const std::string & type_name) ;
void do_output(const std::string & str_line) ;
std::string math_action(std::vector<std::string> vec_action) ;
void assignment(const std::string & str_var , const std::string & str_value, const std::string & type_name) ;
bool valid_var(const std::string & v) ;
std::pair<std::string,std::string> look_for(std::string & v) ;

struct myType
{
    std::pair<std::string,std::string> type_value ;
    std::string var ;
}; 

std::vector<myType> var ;

std::ifstream ifile("txt.txt") ;

std::vector<std::string> keyWords { "tpi" , "ete" , "fra" , "integer" , "str" , "doub" } ;

int main()
{
    std::string str_line;
    if(!ifile.is_open()) {
        std::cout << "error file is not open:\n" ;
        exit(1) ;
    }

    while(!ifile.eof()) 
    {
        str_line = "" ; 
        std::getline(ifile,str_line) ;
        translate(str_line) ;
    }
    return 0 ;
}

void translate(std::string & str_line)
{
    std::string tmp_str ;
    for(int i = 0; i < str_line.size() ; ++i) 
    {
        if(str_line[i] >= 65 && str_line[i] <= 122) {
            tmp_str.push_back(str_line[i]) ;
        } else if(std::find(keyWords.cbegin(),keyWords.cend(), tmp_str) != keyWords.end()) {

            if(tmp_str == keyWords[0]) {
                do_output(std::string(str_line.begin() + i + 1,str_line.end())) ;
                tmp_str = "" ;
                break ;
            }
            else if(tmp_str == keyWords[3] || tmp_str == keyWords[4] || tmp_str == keyWords[5] ) {
                creat_variable(std::string(str_line.begin() + i + 1 , str_line.end()), tmp_str) ;
                tmp_str = "" ;
                break ;
            }
            else if(tmp_str == keyWords[1]) {
                do_condition(str_line) ;
                tmp_str = "" ;
                break ;
            }
            else if(tmp_str == keyWords[2]){
                do_loop(str_line) ;
                tmp_str = "" ;
                break ;
            }
        }
        else if(valid_var(tmp_str)) {
            creat_variable(str_line,look_for(tmp_str).first) ;
            tmp_str = "" ;
            break ;
        }
    }
    return ;
}

void do_output(const std::string & str_line) 
{
    std::string tmp_str ;
    bool shift = true ;
    for(int i = 0 ; i < str_line.size() ; ++i) {
        if(str_line[i] != ' ')
        tmp_str.push_back(str_line[i]) ;
        if(tmp_str == "<<") {
            shift = false ;
        }
    }
    tmp_str = {} ;
    if(shift) {
        std::cout << "error: line " << str_line << " chka << \n" ;
        exit(0);
    } 
    if (std::find(str_line.begin() , str_line.end(), '"') != str_line.end()) 
    {
        auto it = std::find(str_line.cbegin() , str_line.cend(), '"') ;
        it++;
        while(*it != '"') 
        {
            std::cout << *it ;
            it++ ;
        }
        std::cout << std::endl ;
        return ;
    }
    std::string _str(std::find(str_line.cbegin() , str_line.cend(), '<') + 2 , std::find(str_line.cbegin() , str_line.cend(), ';')) ;
    std::vector<std::string> vec ;
    tmp_str = {} ;
    for(int i = 0; i < _str.size() ; ++i)
    {
        if(_str[i] >= 65 && _str[i] <= 122 || _str[i] >= '0' && _str[i] <= '9') {
            tmp_str.push_back(_str[i]) ;
        } else {
            if(tmp_str != "")
            vec.push_back(tmp_str) ;
            tmp_str.clear() ;
        }
        if(_str[i] == '+' || _str[i] == '*' || _str[i] == '-' || _str[i] == '/' ) {
            std::string s ; 
            s.push_back(_str[i]) ;
            vec.push_back(s) ;
            s.clear() ;
        }
    }
    if(vec.size() == 1) {
        if(vec[0][0] >= 65 && vec[0][0] <= 122) {
            std::cout << look_for(vec[0]).second << std::endl ;
        } else {
            std::cout << vec[0] << std::endl ;
        }
    }
    else if(vec.size() > 2) {
        std::cout << math_action(vec) << std::endl ;
    }
    else {
        std::cout << "error: undifaend " << std::endl ;
        exit(0) ;
    }
    return ;
}

std::pair<std::string,std::string> look_for(std::string & v)
{
    for(const auto & it: var) {
        if(v == it.var) {
            return std::pair<std::string,std::string>(it.type_value.first,it.type_value.second) ;
        }
    }
    std::cout << "error: you dont have " << v << std::endl ;
    exit(0) ;
}

std::string math_action(std::vector<std::string> vec_action) 
{
    swap_value(vec_action) ;
    for(int i = 0; i < vec_action.size() ; ++i)
    {
        double x = 0 ;
        if(vec_action[i] == "*" ) {
            double operand1 = std::stod(vec_action[i - 1]) ;
            double operand2 = std::stod(vec_action[i + 1]) ;
            x = operand1 * operand2 ;
            vec_action.insert(vec_action.begin() + i,std::to_string(x)) ;
            ++i ;
            vec_action.erase(vec_action.begin() + i) ;
            vec_action.erase(vec_action.begin() + i ) ;
            vec_action.erase(vec_action.begin() + i - 2) ;
            i = 0 ;
        } else if(vec_action[i] == "/") {
            double operand1 = std::stod(vec_action[i - 1]) ;
            double operand2 = std::stod(vec_action[i + 1]) ;
            x = operand1 / operand2 ;
            vec_action.insert(vec_action.begin() + i, std::to_string(x)) ;
            ++i ;
            vec_action.erase(vec_action.begin() + i) ;
            vec_action.erase(vec_action.begin() + i) ;
            vec_action.erase(vec_action.begin() + i - 2) ;
            i = 0 ;
        }
    }
    double value = std::stod(vec_action[0]) ;
    for(int i = 1 ; i < vec_action.size() ; ++i) 
    {
        if(vec_action[i] == "+") {
            double x = std::stod(vec_action[i + 1]) ;
            value += x ;
        } else if (vec_action[i] == "-") {
            double x = std::stod(vec_action[i + 1]) ;
            value -= x ;
        }
    }
    return std::to_string(value) ;
}

void creat_variable(const std::string & str_line, const std::string & type_name)
{
    std::vector<std::string> vec ;
    std::string tmp_str ;
    for(int i = 0; i < str_line.size() ; ++i)
    {
        if(str_line[i] >= 65 && str_line[i] <= 122 || str_line[i] >= '0' && str_line[i] <= '9') {
            tmp_str.push_back(str_line[i]) ;
        } else {
            if(tmp_str != "")
            vec.push_back(tmp_str) ;
            tmp_str.clear() ;
        }
        if(str_line[i] == '+' || str_line[i] == '*' || str_line[i] == '-' || str_line[i] == '/' ) {
            std::string s ; 
            s.push_back(str_line[i]) ;
            vec.push_back(s) ;
            s.clear() ;
        }
    }
    if(type_name == "str") {
        for(auto it = vec.begin() + 1 ; it != vec.end() ; ++it) {
            tmp_str += *it += " " ;
        }
        assignment(vec[0] , tmp_str , type_name) ;
        return ;
    }
    if(vec.size() == 2 ) {
        assignment(vec[0] , vec[1] , type_name) ;
    } else if(vec.size() == 1 ) {
        assignment(vec[0], {} , type_name) ;
    }
    else if(vec.size() > 2 ) {
       assignment(vec[0] , math_action(std::vector<std::string>(vec.begin() + 1,vec.end())) , type_name ) ;
    } else {
        std::cout << "error: undifaend " << std::endl ;
        exit(0) ;
    }
    return ;
}

void assignment(const std::string & str_var , const std::string & str_value, const std::string & type_name)
{
   if(!valid_var(str_var)) {
        myType obj ;
        obj.var = str_var ;
        obj.type_value.first = type_name ;
        obj.type_value.second = str_value ;
        var.push_back(obj) ;
    } else {
        for(auto & it: var) 
        {
            if(str_var == it.var) {
                it.type_value.first = type_name ;
                it.type_value.second = str_value ;
            }
        }
    }
    return ;
}

bool valid_var(const std::string & v)
{
    for(const auto & it: var) {
        if(v == it.var) {
            return true ;
        }
    }
    return false ;
}

void do_condition(std::string & str_line)
{
    std::string tmp ;
    std::getline(ifile,tmp,'}') ;
    tmp.push_back('}') ;
    str_line += tmp ;
    
    std::vector<std::string> vec ;
    std::string::iterator pos1  = std::find(str_line.begin(), str_line.end(),'(') ;
    std::string::iterator pos2  = std::find(str_line.begin(), str_line.end(),')') + 1 ;
    std::string tmp_str ;
    std::string s ; 
    for(;pos1 != pos2;++pos1) {
        if(*pos1 >= 65 && *pos1 <= 122 || *pos1 >= '0' && *pos1 <= '9') {
            tmp_str.push_back(*pos1) ;
        } else {
            if(tmp_str != "")
            vec.push_back(tmp_str) ;
            tmp_str.clear() ;
        }
        if(*pos1 == '<' || *pos1 == '>' || *pos1 == '=' || *pos1 == '|' || *pos1 == '&') {
            s.push_back(*pos1) ;
        } else {
            if(s != "")
            vec.push_back(s) ;
            s.clear() ;
        }
    }
    
    if( bool_action(vec) ) {
        pos1  = std::find(str_line.begin(), str_line.end(),'{') + 1 ;
        pos2  = std::find(str_line.begin(), str_line.end(),'}') - 1 ;
        while(pos1 != pos2)
        {
            std::string::iterator pos3  = std::find(pos1, str_line.end(),';') + 1 ;
            std::string s(pos1 ,pos3) ;
            pos1 = pos3 ;
            translate(s) ;
        }
        
    } else {
        return ;
    }
    
}

bool bool_action(std::vector<std::string> &vec)
{
    swap_value(vec) ;
    for(int it = 0; it < vec.size(); ++it) 
    {
        if(vec[it] == "==") {
            double x = std::stod(vec[it - 1]) ;
            double y = std::stod(vec[it + 1]) ;
            bool equal = x == y ;
            vec.insert(vec.begin() + it,std::to_string(equal) ) ;
            ++it ;
            vec.erase(vec.begin() + it) ;
            vec.erase(vec.begin() + it) ;
            vec.erase(vec.begin() + it - 2) ;
            it = 0 ;
        } else if(vec[it] == ">") {
            double x = std::stod(vec[it - 1]) ;
            double y = std::stod(vec[it + 1]) ;
            bool equal = x > y ;
            vec.insert(vec.begin() + it,std::to_string(equal) ) ;
            ++it ;
            vec.erase(vec.begin() + it) ;
            vec.erase(vec.begin() + it) ;
            vec.erase(vec.begin() + it - 2) ;
            it = 0 ;
        }
        else if(vec[it] == "<") {
            double x = std::stod(vec[it - 1]) ;
            double y = std::stod(vec[it + 1]) ;
            bool equal = x < y ;
            vec.insert(vec.begin() + it,std::to_string(equal) ) ;
            ++it ;
            vec.erase(vec.begin() + it) ;
            vec.erase(vec.begin() + it) ;
            vec.erase(vec.begin() + it - 2) ;
            it = 0 ;
        }
    }
    for(int it = 0; it < vec.size(); ++it) 
    {
        if(vec[it] == "&&") {
            bool x = std::stoi(vec[it - 1]) ;
            bool y = std::stoi(vec[it + 1]) ;
            bool equal = x && y ;
            vec.insert(vec.begin() + it,std::to_string(equal)) ;
            ++it;
            vec.erase(vec.begin() + it) ;
            vec.erase(vec.begin() + it) ;
            vec.erase(vec.begin() + it - 2) ;
            it = 0 ;
        } else if(vec[it] == "||") {
            bool x = std::stoi(vec[it - 1]) ;
            bool y = std::stoi(vec[it + 1]) ;
            bool equal = x || y ;
            vec.insert(vec.begin() + it,std::to_string(equal)) ;
            ++it;
            vec.erase(vec.begin() + it) ;
            vec.erase(vec.begin() + it) ;
            vec.erase(vec.begin() + it - 2) ;
            it = 0 ;
        }
    }
    return vec[0] == "1" ? true : false ;
}

void do_loop(std::string & str_line)
{
    std::string tmp ;
    std::getline(ifile,tmp,'}') ;
    tmp.push_back('}') ;
    str_line += tmp ;
    std::vector<std::string> vec ;
    std::string::iterator pos1  = std::find(str_line.begin(), str_line.end(),'(') ;
    std::string::iterator pos2  = std::find(str_line.begin(), str_line.end(),')') + 1 ;
    std::string tmp_str ;
    std::string s ; 
    for(;pos1 != pos2;++pos1) {
        if(*pos1 >= 65 && *pos1 <= 122 || *pos1 >= '0' && *pos1 <= '9') {
            tmp_str.push_back(*pos1) ;
        } else {
            if(tmp_str != "")
            vec.push_back(tmp_str) ;
            tmp_str.clear() ;
        }
        if(*pos1 == '<' || *pos1 == '>' || *pos1 == '=' || *pos1 == '|' || *pos1 == '&') {
            s.push_back(*pos1) ;
        } else {
            if(s != "")
            vec.push_back(s) ;
            s.clear() ;
        }
    }   
    std::vector<std::string> vec_tmp = vec ;
    bool whil = bool_action(vec_tmp) ;
    while( whil ) {
        
        pos1  = std::find(str_line.begin(), str_line.end(),'{') + 1 ;
        pos2  = std::find(str_line.begin(), str_line.end(),'}') - 1 ;
        while(pos1 != pos2)
        {
            std::string::iterator pos3  = std::find(pos1, str_line.end(),';') + 1 ;
            std::string s(pos1 ,pos3) ;
            pos1 = pos3 ;
            translate(s) ;
        }
        vec_tmp = vec ;
        whil = bool_action(vec_tmp) ;        
    }   
}

std::vector<std::string>& swap_value(std::vector<std::string> &v) 
{
    for(auto& it: v) {
        if(it[0] >= 65 && it[0] <= 122) it = look_for(it).second ;
    }
    return v ;
}
