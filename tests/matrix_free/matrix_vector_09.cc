//------------------  matrix_vector_09.cc  ------------------------
//    $Id$
//    Version: $Name$
//
//------------------  matrix_vector_09.cc  ------------------------


// same test as matrix_vector_06 (quite large mesh, hanging nodes, different
// cell types), but tiny domain of size 1e-20 to test correctness of
// relative scaling in mapping info

#include "../tests.h"
#include <deal.II/base/function.h>
#include "create_mesh.h"

std::ofstream logfile("matrix_vector_09/output");

#include "matrix_vector_common.h"

template <int dim, int fe_degree>
void test ()
{
  if (fe_degree > 1)
    return;

  Triangulation<dim> tria;
  create_mesh (tria, 1e-20);
  tria.begin_active ()->set_refine_flag();
  tria.execute_coarsening_and_refinement();
  typename Triangulation<dim>::active_cell_iterator cell, endc;
  cell = tria.begin_active ();
  endc = tria.end();
  for (; cell!=endc; ++cell)
    if (cell->center().norm()<0.5*1e-20)
      cell->set_refine_flag();
  tria.execute_coarsening_and_refinement();
  tria.begin(tria.n_levels()-1)->set_refine_flag();
  tria.last()->set_refine_flag();
  tria.execute_coarsening_and_refinement();
  cell = tria.begin_active ();
  for (unsigned int i=0; i<10-3*dim; ++i)
    {
      cell = tria.begin_active ();
      endc = tria.end();
      unsigned int counter = 0;
      for (; cell!=endc; ++cell, ++counter)
        if (counter % (7-i) == 0)
          cell->set_refine_flag();
      tria.execute_coarsening_and_refinement();
    }

  FE_Q<dim> fe (fe_degree);
  DoFHandler<dim> dof (tria);
  dof.distribute_dofs(fe);
  ConstraintMatrix constraints;
  DoFTools::make_hanging_node_constraints(dof, constraints);
  VectorTools::interpolate_boundary_values (dof, 0, ZeroFunction<dim>(),
                                            constraints);
  constraints.close();

  do_test<dim, fe_degree, double> (dof, constraints);
}