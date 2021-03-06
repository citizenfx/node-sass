// sass.hpp must go before all system headers to get the
// __EXTENSIONS__ fix on Solaris.
#include "sass.hpp"

#include "ast2c.hpp"
#include "ast.hpp"

namespace Sass {

  union Sass_Value* AST2C::operator()(Boolean_Ptr b)
  { return sass_make_boolean(b->value()); }

  union Sass_Value* AST2C::operator()(Number_Ptr n)
  { return sass_make_number(n->value(), n->unit().c_str()); }

  union Sass_Value* AST2C::operator()(Custom_Warning_Ptr w)
  { return sass_make_warning(w->message().c_str()); }

  union Sass_Value* AST2C::operator()(Custom_Error_Ptr e)
  { return sass_make_error(e->message().c_str()); }

  union Sass_Value* AST2C::operator()(Color_RGBA_Ptr c)
  { return sass_make_color(c->r(), c->g(), c->b(), c->a()); }

  union Sass_Value* AST2C::operator()(Color_HSLA_Ptr c)
  { return operator()(c->toRGBA()); }

  union Sass_Value* AST2C::operator()(String_Constant_Ptr s)
  {
    if (s->quote_mark()) {
      return sass_make_qstring(s->value().c_str());
    } else {
      return sass_make_string(s->value().c_str());
    }
  }

  union Sass_Value* AST2C::operator()(String_Quoted_Ptr s)
  { return sass_make_qstring(s->value().c_str()); }

  union Sass_Value* AST2C::operator()(List_Ptr l)
  {
    union Sass_Value* v = sass_make_list(l->length(), l->separator(), l->is_bracketed());
    for (size_t i = 0, L = l->length(); i < L; ++i) {
      sass_list_set_value(v, i, (*l)[i]->perform(this));
    }
    return v;
  }

  union Sass_Value* AST2C::operator()(Map_Ptr m)
  {
    union Sass_Value* v = sass_make_map(m->length());
    int i = 0;
    for (auto key : m->keys()) {
      sass_map_set_key(v, i, key->perform(this));
      sass_map_set_value(v, i, m->at(key)->perform(this));
      i++;
    }
    return v;
  }

  union Sass_Value* AST2C::operator()(Arguments_Ptr a)
  {
    union Sass_Value* v = sass_make_list(a->length(), SASS_COMMA, false);
    for (size_t i = 0, L = a->length(); i < L; ++i) {
      sass_list_set_value(v, i, (*a)[i]->perform(this));
    }
    return v;
  }

  union Sass_Value* AST2C::operator()(Argument_Ptr a)
  { return a->value()->perform(this); }

  // not strictly necessary because of the fallback
  union Sass_Value* AST2C::operator()(Null_Ptr n)
  { return sass_make_null(); }

};
