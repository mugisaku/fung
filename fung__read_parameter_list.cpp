#include"fung.hpp"
#include"fung_identifier.hpp"
#include"fung_parameter.hpp"
#include"fung_error.hpp"




namespace fung{



namespace{


Parameter
read_parameter(Cursor&  cur)
{
  auto  type_name = read_identifier(cur);

  auto  vk = Value::to_kind(type_name);

  skip_spaces_and_newline(cur);

  auto  const c = *cur;

    if(!isident0(c))
    {
      throw Error(cur,"仮引数の変数名が無い");
    }


  auto  variable_name = read_identifier(cur);

  return Parameter(vk,std::move(variable_name));
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

  else
    {
      cur += 1;
    }


  return std::move(ls);
}


}




