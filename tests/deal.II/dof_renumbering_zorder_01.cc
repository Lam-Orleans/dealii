//----------------------------------------------------------------------
//    $Id: dof_renumbering_02.cc 23710 2011-05-17 04:50:10Z bangerth $
//    Version: $Name$
//
//    Copyright (C) 2000, 2001, 2003, 2004, 2007, 2008, 2010 by the deal.II authors
//
//    This file is subject to QPL and may not be  distributed
//    without copyright and license information. Please refer
//    to the file deal.II/doc/license.html for the  text  and
//    further information on this license.
//
//----------------------------------------------------------------------

// Check DoFRenumbering::hierachical changes nothing for a regular refined mesh

#include "../tests.h"
#include <deal.II/base/logstream.h>
#include <deal.II/base/function_lib.h>
#include <deal.II/lac/vector.h>
#include <deal.II/grid/tria.h>
#include <deal.II/grid/tria_iterator.h>
#include <deal.II/grid/grid_generator.h>
#include <deal.II/dofs/dof_accessor.h>
#include <deal.II/dofs/dof_handler.h>
#include <deal.II/dofs/dof_renumbering.h>
#include <deal.II/fe/fe_q.h>
#include <deal.II/fe/fe_dgq.h>
#include <deal.II/fe/fe_dgp.h>
#include <deal.II/fe/fe_system.h>
#include <deal.II/fe/fe_values.h>

#include <fstream>
#include <sstream>


template <int dim, class stream>
void
print_dofs (const DoFHandler<dim> &dof, stream & out)
{
  const FiniteElement<dim>& fe = dof.get_fe();
  std::vector<unsigned int> v (fe.dofs_per_cell);
  std_cxx1x::shared_ptr<FEValues<dim> > fevalues;

  if (fe.has_support_points())
    {
      Quadrature<dim> quad(fe.get_unit_support_points());
      fevalues = std_cxx1x::shared_ptr<FEValues<dim> >(new FEValues<dim>(fe, quad, update_q_points));
    }

  for (typename DoFHandler<dim>::active_cell_iterator cell=dof.begin_active();
       cell != dof.end(); ++cell)
    {
      Point<dim> p = cell->center();
      if (fevalues.get() != 0)
	fevalues->reinit(cell);

      cell->get_dof_indices (v);
      for (unsigned int i=0; i<v.size(); ++i)
	if (fevalues.get() != 0)
	  out << fevalues->quadrature_point(i) << '\t' << v[i] << std::endl;
	else
	  out << p << '\t' << v[i] << std::endl;
      out << std::endl;
    }
}




template <int dim>
void
check ()
{
  Triangulation<dim> tr;
  GridGenerator::hyper_cube(tr, -1., 1.);
  tr.refine_global (1);

  FE_Q<dim> fe(1);
  DoFHandler<dim> dof(tr);
  dof.distribute_dofs(fe);

  std::ostringstream o1, o2;
  
  print_dofs(dof, o1);
  deallog << "**" << endl;

  DoFRenumbering::hierarchical(dof);

  print_dofs(dof, o2);

  if (o1.str()==o2.str())
	deallog << "OK" << endl;
  
}

int main ()
{
  std::ofstream logfile ("dof_renumbering_zorder_01/output");
  deallog << std::setprecision (2);
  deallog << std::fixed;
  deallog.attach(logfile);
  deallog.depth_console (0);

  deallog.push ("1d");
  check<1> ();
  deallog.pop ();
  deallog.push ("2d");
  check<2> ();
  deallog.pop ();
  deallog.push ("3d");
  check<3> ();
  deallog.pop ();
}