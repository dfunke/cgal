// ============================================================================
//
// Copyright (c) 2001-2006 Max-Planck-Institut Saarbruecken (Germany).
// All rights reserved.
//
// This file is part of EXACUS (http://www.mpi-inf.mpg.de/projects/EXACUS/);
// you may redistribute it under the terms of the Q Public License version 1.0.
// See the file LICENSE.QPL distributed with EXACUS.
// 
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// ----------------------------------------------------------------------------
//
// Library       : CGAL 
// File          : test/Chinese_remaminder_test.C
// CGAL_release   : $Name:  $
// Revision      : $Revision$
// Revision_date : $Date$
//
// Author(s)     : Dominik H�lse <dominik.huelse@gmx.de>
//                 Michael Hemmer <hemmer@mpi-inf.mpg.de>
//
// ============================================================================

#undef NDEBUG
#include <CGAL/basic.h>
#include <CGAL/Chinese_remainder_traits.h>
#include <CGAL/Arithmetic_kernel.h>
#include <CGAL/Polynomial.h>
#include <CGAL/Modular.h>
#include <CGAL/Sqrt_extension.h>
#include <CGAL/primes.h>

#include <cassert>

//#define WITH_OUTPUT 1

template <class NT>
struct Get_max_coefficient{            
    typedef NT result_type;        
    result_type operator () (const NT& a) {                
        return CGAL::abs(a);
    }
};

template <class NT, class Root>
struct Get_max_coefficient<CGAL::Sqrt_extension<NT, Root> >{        
    typedef Get_max_coefficient<NT> GMC;
    typedef typename GMC::result_type result_type;
    
    result_type operator () (const CGAL::Sqrt_extension<NT, Root>& a) {             
        GMC gmc;
        return std::max(std::max(gmc(a.a0()), gmc(a.a1())), gmc(NT(a.root())));            
    }
};

template <class NT>
struct Get_max_coefficient<CGAL::Polynomial<NT> >{        
    typedef Get_max_coefficient<NT> GMC;
    typedef typename GMC::result_type result_type;
    
    result_type operator () (const CGAL::Polynomial<NT>& a) {             
        GMC gmc;
        result_type m=0;
        for(int i=0; i<=a.degree(); i++){
            m = std::max(gmc(a[i]),m);
        }
        return m;           
    }
};

template<class NT>
void test_CR_for(const NT& f){

    typedef CGAL::Modular_traits<NT> MT;
    typedef typename CGAL::Modular_traits<NT>::Modular_NT MNT;
    typename MT::Modular_image modular_image;
    typename MT::Modular_image_inv modular_image_inv; 
    

    typedef CGAL::Chinese_remainder_traits<NT> CRT; 
    typedef typename CRT::Scalar_type Scalar_type; 
    typename CRT::Chinese_remainder chinese_remainder;
    
    Scalar_type max_coeff = Get_max_coefficient<NT>()(f);
    
    NT  g, g_old;
    Scalar_type p,q,pq;
    
    int prime_index, current_prime;
    bool test = true;
    prime_index = 0;
    
    // init chinese remainder
    q = current_prime = CGAL::CGALi::primes[prime_index];
    CGAL::Modular::set_current_prime(current_prime); 
    g_old = modular_image_inv(modular_image(f));
    pq= p = q;
    
        
    // try chinese remainder
    do{
        // chinese remainder failed if q > 2*max_coeff(f) 
         CGAL_postcondition_msg(pq < p*(2*max_coeff), " chinese remainder failed ");
         
        prime_index++;
        if(prime_index < 1000){
            current_prime = CGAL::CGALi::primes[prime_index]; 
        }
        else{
            std::cerr<<"primes in the array exhausted"<<std::endl;
            current_prime = CGAL::CGALi::get_next_lower_prime(current_prime);
        }    
        CGAL::Modular::set_current_prime(current_prime); 
        p = current_prime;
        
        chinese_remainder(q,g_old,p,modular_image_inv(modular_image(f)),pq,g);
       
        try{
            test = g != g_old;
        }catch(...){}
        
        q = pq; 
        g_old = g;
        
       
    }while(test); 

    assert( g == f );
}



template<class AT>
void test_CR() {
    typedef typename AT::Integer Integer; 
    typedef CGAL::Sqrt_extension<Integer,Integer> EXT;
    typedef CGAL::Polynomial<Integer> Poly;
    test_CR_for(Integer("13472398473248763214987"));
    test_CR_for(EXT(
                        Integer("13472398473248763214987"), 
                        Integer("43857430987524589"), 
                        Integer("84397984359843")));
    test_CR_for(EXT(
                        Integer("13472398473248763214987"), 
                        Integer("0"), 
                        Integer("84397984359843")));
    test_CR_for(Poly(
                        Integer("13472398473248763214987"), 
                        Integer("-43857430987524589"), 
                        Integer("84397984359843")));
}

int main(){
    
#ifdef CGAL_USE_LEDA
    test_CR<CGAL::LEDA_arithmetic_kernel>();
#endif // CGAL_USE_LEDA    

#ifdef CGAL_USE_CORE  
    test_CR<CGAL::CORE_arithmetic_kernel>();
#endif // Lis_HAVE_CORE
     
    return 0;
}


// EOF
