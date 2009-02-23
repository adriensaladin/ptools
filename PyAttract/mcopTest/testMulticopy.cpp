#include <iostream>
#include <vector>

#include <rigidbody.h>
#include <attractrigidbody.h>
#include <derivify.h>
#include <minimizers/lbfgs_interface.h>


// #include <pdbio.h>
#include <mcopff.h>



using namespace PTools;
using namespace std;



dbl func(dbl x)
{
return 4*x*x;
}







int main()
{





Rigidbody m("mainR.pdb");
AttractRigidbody am (m);
Rigidbody c1("copy1R.pdb");
Rigidbody c2("copy2R.pdb");

AttractRigidbody ac1(c1);
AttractRigidbody ac2 (c2);


Region reg ;
reg.addCopy(ac1);
reg.addCopy(ac2);


Mcoprigid mcrec ;
mcrec.setMain(am);
mcrec.addEnsemble(reg);



Rigidbody lig("mainR.pdb");
AttractRigidbody alig (lig);

Mcoprigid mclig ;
mclig.setMain(alig);



AttractForceField1 ff ("aminon.par", 12.);


// BaseAttractForceField* mff = attractforceFieldCreator<AttractForceField1>("aminon.par", 12);

FFcreator fcreator =  (attractforceFieldCreator<AttractForceField1>);


McopForceField FF ( attractforceFieldCreator<AttractForceField1>  , "aminon.par", 12., mcrec, alig);


vector<dbl> v(6);
vector<dbl> d(6);

for (uint i=0; i<6; i++)
{
v[i]=0.0; d[i]=0.0;
}


FF.Function(v);
FF.calculate_weights(true);

std::cout << FF.Function(v) << std::endl;


FF.dbgPlayWithFF();









for (int i=0; i<3; i++)
{
v[4]+=100.0;
// v[2]+=100;
// v[4]+=20;
// v[5]+=120;

std::cout << FF.Function(v) << std::endl;


}

cout << "**************\n";
std::cout << FF.Function(v) << std::endl;

// for (int i=0; i<6; i++)
//  {
//     Vdouble v2(v);
//     v2[i]=v2[i]+surreal(0,1);
//     cout << FF.Function(v)<< std::endl;
//  }

FF.Derivatives(v,d);
for (int i=0; i<6; i++) 
   cout << d[i] << "  " ;


Lbfgs minim(FF);
minim.minimize(500,v);





return 0;
}