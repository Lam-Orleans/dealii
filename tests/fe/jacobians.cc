//----------------------------  jacobians.cc  ---------------------------
//    $Id$
//    Version: $Name$
//
//    Copyright (C) 2012 by the deal.II authors
//
//    This file is subject to QPL and may not be  distributed
//    without copyright and license information. Please refer
//    to the file deal.II/doc/license.html for the  text  and
//    further information on this license.
//
//----------------------------  jacobians.cc  ---------------------------

// Show the Jacobians and inverse Jacobians on hyperball with one quadrature
// point

#include "../tests.h"
#include <deal.II/base/quadrature_lib.h>
#include <deal.II/fe/fe_nothing.h>
#include <deal.II/fe/fe_values.h>
#include <deal.II/fe/mapping_q.h>
#include <deal.II/grid/tria.h>
#include <deal.II/grid/grid_generator.h>
#include <deal.II/grid/tria_boundary_lib.h>

template<int dim>
void test()
{
  Triangulation<dim> tria;
  GridGenerator::hyper_ball (tria);
  static const HyperBallBoundary<dim> boundary;
  tria.set_boundary (0, boundary);

  MappingQ<dim> mapping(3);
  FE_Nothing<dim> dummy;
				// choose a point that is not right in the
				// middle of the cell so that the Jacobian
				// contains many nonzero entries
  Point<dim> quad_p;
  for (int d=0; d<dim; ++d)
    quad_p(d) = 0.42 + 0.11 * d;
  Quadrature<dim> quad(quad_p);
  FEValues<dim> fe_val (mapping, dummy, quad,
			update_jacobians | update_inverse_jacobians);
  deallog << dim << "d Jacobians:" << std::endl;
  typename Triangulation<dim>::active_cell_iterator
    cell = tria.begin_active(), endc = tria.end();
  for ( ; cell != endc; ++cell)
    {
      fe_val.reinit (cell);
      
      for (unsigned int d=0; d<dim; ++d)
	for (unsigned int e=0; e<dim; ++e)
	  deallog << fe_val.jacobian(0)[d][e] << " ";
      deallog << std::endl;
    }
  deallog << std::endl;

  deallog << dim << "d inverse Jacobians:" << std::endl;
  cell = tria.begin_active();
  endc = tria.end();
  for ( ; cell != endc; ++cell)
    {
      fe_val.reinit (cell);
      
      for (unsigned int d=0; d<dim; ++d)
	for (unsigned int e=0; e<dim; ++e)
	  deallog << fe_val.inverse_jacobian(0)[d][e] << " ";
      deallog << std::endl;
    }
  deallog << std::endl;
}


int
main()
{
  std::ofstream logfile ("jacobians/output");
  deallog << std::setprecision(4) << std::fixed;
  deallog.attach(logfile);
  deallog.depth_console(0);
  deallog.threshold_double(1.e-10);

  test<2>();
  test<3>();

  return 0;
}


