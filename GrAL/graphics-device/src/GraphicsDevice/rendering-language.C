
// $LICENSE

#include <stack>
#include <vector>

#include "GraphicsDevice/rendering-language.h"
#include "GraphicsDevice/triangle.h"
#include "GraphicsDevice/segment.h"

class rlang_internals {
public:
  std::ostream* out;
  std::stack<Transformation, std::vector<Transformation> > todo_trafo;
  rlang_internals(std::ostream* o) : out(o), todo_trafo() {}
};

rlang_internals*  copy_traits<rlang_internals>::clone(rlang_internals const& t)
{ return new rlang_internals(t); }
void  copy_traits<rlang_internals>::destroy(rlang_internals      * t)
{ delete t;}


rendering_language::rendering_language(ostream* out) : state(new rlang_internals(out)) {}
rendering_language::~rendering_language() {}

void rendering_language::set_stream(ostream* out) { state->out = out;}
ostream& rendering_language::the_stream()
{  return (*(state->out)); }

Transformation rendering_language::ToDoTrafo() const
{return state->todo_trafo.top();}

void rendering_language::PushToDoTrafo(const Transformation& T)
{
  if (state->todo_trafo.empty())
    state->todo_trafo.push(T);
  else
    state->todo_trafo.push(ToDoTrafo()(T)); // compose ToDo with T
}

void rendering_language::PopToDoTrafo() 
{ state->todo_trafo.pop(); }

bool rendering_language::IsToDoTrafo() const
{ return (!state->todo_trafo.empty());}



void rendering_language::filter(const RenderableGeom& Obj) 
{ 
  if (Obj.HasAttr())  begin_attribute(Obj.attr());
  if (Obj.HasTrafo()) begin_transformation(Obj.trafo());
  begin_object(Obj);
  end_object();
  if( Obj.HasTrafo()) end_transformation();
  if( Obj.HasAttr())  end_attribute();

  the_stream() << std::endl;
}


// default definitions
void rendering_language::write_triangle(const geom_triangle& tr)
{
  write_segment(geom_segment(tr.corner(1),tr.corner(2)));
  write_segment(geom_segment(tr.corner(2),tr.corner(3)));
  write_segment(geom_segment(tr.corner(3),tr.corner(1)));
}

