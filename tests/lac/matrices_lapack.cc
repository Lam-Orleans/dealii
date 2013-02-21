//----------------------------------------------------------------------
//    $Id$
//
//    Copyright (C) 2005, 2013 by the deal.II authors
//
//    This file is subject to QPL and may not be  distributed
//    without copyright and license information. Please refer
//    to the file deal.II/doc/license.html for the  text  and
//    further information on this license.
//
//----------------------------------------------------------------------

// Test LAPACKFullMatrix::copy_from

#include "../tests.h"

#include <deal.II/base/logstream.h>

#include <deal.II/lac/full_matrix.h>
#include <deal.II/lac/lapack_full_matrix.h>
#include <deal.II/lac/sparse_matrix.h>
#include <deal.II/lac/sparse_matrix_ez.h>

#include <fstream>

int main()
{
  std::ofstream logfile("matrices_lapack/output");
  logfile.setf(std::ios::fixed);
  deallog << std::setprecision(3);
  deallog.attach(logfile);
  deallog.depth_console(0);
  deallog.threshold_double(1.e-10);

  SparseMatrixEZ<double> ez(5,4);
  ez.set(0,0,2.);
  ez.set(0,2,3.);
  ez.set(0,3,4.);
  ez.set(1,0,5.);
  ez.set(1,1,6.);
  ez.set(1,3,7.);
  ez.set(2,0,8.);
  ez.set(2,1,9.);
  ez.set(2,2,10.);
  ez.set(2,3,11.);
  ez.set(4,0,12.);
  ez.set(4,2,13.);
  ez.set(4,3,14.);

  deallog << "SparseMatrixEZ<float>::copy_from  SparseMatrixEZ<double>"
	  << std::endl;
  LAPACKFullMatrix<float> ff;
  ff.copy_from(ez);
  ff.print_formatted(logfile, 0, false, 5, "~");
}