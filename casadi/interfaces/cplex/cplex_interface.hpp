/*
 *    This file is part of CasADi.
 *
 *    CasADi -- A symbolic framework for dynamic optimization.
 *    Copyright (C) 2010-2014 Joel Andersson, Joris Gillis, Moritz Diehl,
 *                            K.U. Leuven. All rights reserved.
 *    Copyright (C) 2011-2014 Greg Horn
 *
 *    CasADi is free software; you can redistribute it and/or
 *    modify it under the terms of the GNU Lesser General Public
 *    License as published by the Free Software Foundation; either
 *    version 3 of the License, or (at your option) any later version.
 *
 *    CasADi is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public
 *    License along with CasADi; if not, write to the Free Software
 *    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifndef CASADI_CPLEX_INTERFACE_HPP
#define CASADI_CPLEX_INTERFACE_HPP

#include "casadi/core/function/qpsol.hpp"
#include <casadi/interfaces/cplex/casadi_qpsol_cplex_export.h>
#include "ilcplex/cplex.h"

#include <string>

/** \defgroup plugin_Qpsol_cplex

      Interface to Cplex solver for sparse Quadratic Programs
*/

/** \pluginsection{Qpsol,cplex} */

/// \cond INTERNAL

namespace casadi {

  struct CASADI_QPSOL_CPLEX_EXPORT CplexMemory : public Memory {
    /// Indicates if we have to warm-start
    bool is_warm;

    /// Nature of problem (always minimization)
    int objsen;

    /// Determines relation >,<, = in the linear constraints
    std::vector<char> sense;

    /// Coefficients of matrix A (constraint Jacobian)
    std::vector<int> matcnt;

    /// Right-hand side of constraints
    std::vector<double> rhs;

    /// Range of constraints
    std::vector<double> rngval;

    /// Coefficients of matrix H (objective Hessian)
    std::vector<int> qmatcnt;

    /// Storage for basis info of primal variables
    std::vector<int> cstat;

    /// Storage for basis info of slack variables
    std::vector<int> rstat;

    /// CPLEX environment
    CPXENVptr env;
    CPXLPptr lp;

    /// Constructor
    CplexMemory();

    /// Destructor
    virtual ~CplexMemory();
  };

  /** \brief \pluginbrief{Qpsol,cplex}

      @copydoc Qpsol_doc
      @copydoc plugin_Qpsol_cplex

      \author Attila Kozma, Joel Andersson
      \date 2012
  */
  class CASADI_QPSOL_CPLEX_EXPORT CplexInterface : public Qpsol {
  public:
    /** \brief  Create a new QP Solver */
    static Qpsol* creator(const std::string& name,
                                     const std::map<std::string, Sparsity>& st) {
      return new CplexInterface(name, st);
    }

    /// Constructor using sparsity patterns
    explicit CplexInterface(const std::string& name,
                            const std::map<std::string, Sparsity>& st);

    /// Destructor
    virtual ~CplexInterface();

    // Get name of the plugin
    virtual const char* plugin_name() const { return "cplex";}

    // Initialize the solver
    virtual void init();

    /** \brief Create memory block */
    virtual Memory* memory() const { return new CplexMemory();}

    /** \brief Initalize memory block */
    virtual void init_memory(Memory& mem) const;

    // Solve the QP
    virtual void eval(Memory& mem, const double** arg, double** res, int* iw, double* w) const;

    // OPTIONS
    /** Which algorithm to use
     * 0 -> Automatic (default)
     * 1 -> Primal simplex
     * 2 -> Dual simplex
     * 3 -> Network optimizer
     * 4 -> Barrier
     * 5 -> Sifting
     * 6 -> Concurrent
     * 7 -> Crossover
     */
    /// Stores which QP algorithm to use
    int qp_method_;

    /// Print to file (for later use)
    bool dump_to_file_;

    /// Accuracy
    double tol_;

    /// A documentation string
    static const std::string meta_doc;

  };
} // end namespace casadi
/// \endcond
#endif // CASADI_CPLEX_INTERFACE_HPP
