#include"fung_ExpressionNode.hpp"
#include"fung_function.hpp"
#include"fung_cursor.hpp"
#include"fung_variable.hpp"
#include<new>




namespace fung{




ExpressionNode::
ExpressionNode(Identifier&&  id):
kind(ExpressionNodeKind::identifier)
{
  new(&data) Identifier(std::move(id));
}


ExpressionNode::
ExpressionNode(Value&&  v):
kind(ExpressionNodeKind::value)
{
  new(&data) Value(std::move(v));
}


ExpressionNode::
ExpressionNode(ExpressionList&&  ls):
kind(ExpressionNodeKind::list)
{
  new(&data) ExpressionList(std::move(ls));
}


ExpressionNode::
ExpressionNode(StatementList&&  stmtls):
kind(ExpressionNodeKind::statement_list)
{
  new(&data) StatementList(std::move(stmtls));
}


ExpressionNode::
ExpressionNode(Mnemonic  mn, Expression&&  l, Expression&&  r):
kind(l? ExpressionNodeKind::operation
      : ExpressionNodeKind::operator_),
left(std::move(l)),
right(std::move(r))
{
  data.mnemonic = mn;
}




ExpressionNode&
ExpressionNode::
operator=(ExpressionNode const&  rhs) noexcept
{
    if(&rhs != this)
    {
      clear();

      kind = rhs.kind;

        switch(kind)
        {
      case(ExpressionNodeKind::null):
          break;
      case(ExpressionNodeKind::operator_):
      case(ExpressionNodeKind::operation):
          data.mnemonic = rhs.data.mnemonic;
          break;
      case(ExpressionNodeKind::value):
          new(&data) Value(rhs.data.value);
          break;
      case(ExpressionNodeKind::identifier):
          new(&data) Identifier(rhs.data.identifier);
          break;
      case(ExpressionNodeKind::statement_list):
          new(&data) StatementList(rhs.data.statement_list);
          break;
      case(ExpressionNodeKind::list):
          new(&data) ExpressionList(rhs.data.list);
          break;
        }


      left  = rhs.left ;
      right = rhs.right;
    }


  return *this;
}


ExpressionNode&
ExpressionNode::
operator=(ExpressionNode&&  rhs) noexcept
{
    if(&rhs != this)
    {
      clear();

      std::swap(kind,rhs.kind);

        switch(kind)
        {
      case(ExpressionNodeKind::null):
          break;
      case(ExpressionNodeKind::operator_):
      case(ExpressionNodeKind::operation):
          data.mnemonic = rhs.data.mnemonic;
          break;
      case(ExpressionNodeKind::value):
          new(&data) Value(std::move(rhs.data.value));
          break;
      case(ExpressionNodeKind::identifier):
          new(&data) Identifier(std::move(rhs.data.identifier));
          break;
      case(ExpressionNodeKind::list):
          new(&data) ExpressionList(std::move(rhs.data.list));
          break;
      case(ExpressionNodeKind::statement_list):
          new(&data) StatementList(std::move(rhs.data.statement_list));
          break;
        }


      left  = std::move(rhs.left );
      right = std::move(rhs.right);
    }


  return *this;
}




void
ExpressionNode::
clear()
{
    switch(kind)
    {
  case(ExpressionNodeKind::null):
  case(ExpressionNodeKind::operator_):
  case(ExpressionNodeKind::operation):
      break;
  case(ExpressionNodeKind::value):
      data.value.~Value();
      break;
  case(ExpressionNodeKind::identifier):
      data.identifier.~Identifier();
      break;
  case(ExpressionNodeKind::list):
      data.list.~vector();
      break;
  case(ExpressionNodeKind::statement_list):
      data.statement_list.~vector();
      break;
    }


   left.reset();
  right.reset();

  kind = ExpressionNodeKind::null;
}


bool
ExpressionNode::
is_unary_operator() const
{
  auto&  mn = data.mnemonic;

  return((mn == Mnemonic::neg    ) ||
         (mn == Mnemonic::der    ) ||
         (mn == Mnemonic::bit_not) ||
         (mn == Mnemonic::log_not));
}


bool
ExpressionNode::
is_binary_operator() const
{
  auto&  mn = data.mnemonic;

  return((mn == Mnemonic::add    ) ||
         (mn == Mnemonic::sub    ) ||
         (mn == Mnemonic::mul    ) ||
         (mn == Mnemonic::div    ) ||
         (mn == Mnemonic::rem    ) ||
         (mn == Mnemonic::shl    ) ||
         (mn == Mnemonic::shr    ) ||
         (mn == Mnemonic::bit_or ) ||
         (mn == Mnemonic::bit_and) ||
         (mn == Mnemonic::bit_xor) ||
         (mn == Mnemonic::log_or ) ||
         (mn == Mnemonic::log_and) ||
         (mn == Mnemonic::eq     ) ||
         (mn == Mnemonic::neq    ) ||
         (mn == Mnemonic::lt     ) ||
         (mn == Mnemonic::lteq   ) ||
         (mn == Mnemonic::gt     ) ||
         (mn == Mnemonic::gteq   ) ||
         (mn == Mnemonic::cho    ) ||
         (mn == Mnemonic::eth    ) ||
         (mn == Mnemonic::acc    ) ||
         (mn == Mnemonic::ina    ) ||
         (mn == Mnemonic::sus    ) ||
         (mn == Mnemonic::cal    ));
}


bool
ExpressionNode::
is_operand() const
{
  return((kind == ExpressionNodeKind::operation ) ||
         (kind == ExpressionNodeKind::value     ) ||
         (kind == ExpressionNodeKind::identifier));
}


void
ExpressionNode::
print() const
{
    switch(kind)
    {
  case(ExpressionNodeKind::null):
      printf("NULL ");
      break;
  case(ExpressionNodeKind::operator_):
      print_mnemonic(data.mnemonic);
      break;
  case(ExpressionNodeKind::operation):
        if(is_unary_operator())
        {
          print_mnemonic(data.mnemonic);

          if(left){left->print();}
        }

      else
        {
          printf("(");

          if(left){left->print();}

          print_mnemonic(data.mnemonic);

          if(right){right->print();}

          printf(")");
        }
      break;
  case(ExpressionNodeKind::value):
      data.value.print();
      break;
  case(ExpressionNodeKind::identifier):
      printf("%s",data.identifier->data());
      break;
  case(ExpressionNodeKind::list):
      printf("(");

        {
          auto   it = data.list.cbegin();
          auto  end = data.list.cend();

            if(it != end)
            {
              (*it++)->print();

                while(it != end)
                {
                  printf(",");

                  (*it++)->print();
                }
            }
        }

      printf(")");
      break;
  case(ExpressionNodeKind::statement_list):
      printf("{");

        {
          auto   it = data.statement_list.cbegin();
          auto  end = data.statement_list.cend();

            if(it != end)
            {
              (*it++)->print();

                while(it != end)
                {
                  printf(",");

                  (*it++)->print();
                }
            }
        }

      printf("}");
      break;
    }
}


void
ExpressionNode::
print_mnemonic(Mnemonic  mn)
{
  char const*  s = "";

    switch(mn)
    {
  case(Mnemonic::nop): s = "_";break;
  case(Mnemonic::add): s = "+";break;
  case(Mnemonic::sub): s = "-";break;
  case(Mnemonic::mul): s = "*";break;
  case(Mnemonic::div): s = "/";break;
  case(Mnemonic::rem): s = "%";break;
  case(Mnemonic::shl): s = "<<";break;
  case(Mnemonic::shr): s = ">>";break;
  case(Mnemonic::bit_and): s = "&";break;
  case(Mnemonic::bit_or): s = "|";break;
  case(Mnemonic::bit_xor): s = "^";break;
  case(Mnemonic::bit_not): s = "~";break;
  case(Mnemonic::eq): s = "==";break;
  case(Mnemonic::neq): s = "!=";break;
  case(Mnemonic::lt): s = "<";break;
  case(Mnemonic::lteq): s = "<=";break;
  case(Mnemonic::gt): s = ">";break;
  case(Mnemonic::gteq): s = ">=";break;
  case(Mnemonic::log_or): s = "||";break;
  case(Mnemonic::log_and): s = "&&";break;
  case(Mnemonic::log_not): s = "!";break;
  case(Mnemonic::neg): s = "-";break;
  case(Mnemonic::cho): s = "?";break;
  case(Mnemonic::eth): s = ":";break;
  case(Mnemonic::sus): s = "";break;
  case(Mnemonic::cal): s = "";break;
  case(Mnemonic::acc): s = ".";break;
  case(Mnemonic::der): s = "*";break;
  case(Mnemonic::ina): s = "->";break;
    }


  printf("%s",s);
}




}




