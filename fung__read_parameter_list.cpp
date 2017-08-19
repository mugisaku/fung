#include"fung.hpp"
#include"fung_identifier.hpp"
#include"fung_variable.hpp"




namespace fung{



namespace{


ValueKind
to_value_kind(std::string const&  id)
{
       if(id == "integer" ){return ValueKind::integer;}
  else if(id == "boolean" ){return ValueKind::boolean;}
  else if(id == "string"  ){return ValueKind::string;}
  else if(id == "function"){return ValueKind::function;}


  return ValueKind::undefined;
}


Variable
read_parameter(Cursor&  cur)
{
  auto  type_name = read_identifier(cur);

  auto  vk = to_value_kind(type_name);

  skip_spaces_and_newline(cur);

  auto  const c = *cur;

    if(!isident0(c))
    {
      throw Error(cur,"仮引数の変数名が無い");
    }


  auto  variable_name = read_identifier(cur);

  return Variable(vk,std::move(variable_name));
}


}


ParameterList
read_parameter_list(Cursor&  cur)
{
  ParameterList  ls;

  skip_spaces_and_newline(cur);

  auto  const first_c = *cur;

    if(isident0(first_c))
    {
      ls.emplace_back(read_parameter(cur));

        for(;;)
        {
          skip_spaces_and_newline(cur);

          auto  const c = *cur;

            if(c == ')')
            {
              cur += 1;

              break;
            }

          else
            if(c == ',')
            {
              cur += 1;

              skip_spaces_and_newline(cur);

                if(isident0(*cur))
                {
                  ls.emplace_back(read_parameter(cur));
                }

              else
                {
                  throw Error(cur,"コンマの後に識別子が無い");
                }
            }

          else
            {
              throw Error(cur,"仮引数の途中に不明な文字");
            }
        }
    }

  else
    if(first_c != ')')
    {
      throw Error(cur,"仮引数のリストが閉じられていない");
    }


  return std::move(ls);
}


}




