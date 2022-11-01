#include <ilcplex/ilocplex.h>
ILOSTLBEGIN
int main()
{
    IloEnv env;
    try
    {
        IloModel model(env, "exemple2");
        // déclaration d'un tableau de 3 variables (réelles) de borne inf. 0 et sup. 50
        IloNumVarArray x(env, 3, 0.0, 50.0);
        // ajout au modèle de la fonction objectif (somme des variables)
        model.add(IloMinimize(env, x[0] + x[1] + x[2]));
        // ajout au modèle des contraintes
        model.add(-x[0] + x[1] + 3 * x[2] == 15);
        model.add(x[0] + 5 * x[1] <= 20);
        model.add(6 * x[1] - x[2] >= 15);
        // associe le modèle a une instance d'IloCplex pour résolution
        IloCplex cplex(model);
        // resolution
        cplex.solve();

        // résultats
        cout << " obj = " << cplex.getObjValue() << endl;
        IloNumArray vals(env);
        cplex.getValues(vals, x);
        cout << " variables = " << vals << endl;
        cplex.exportModel("test.lp");
    }
    catch (IloException &e)
    {
        cerr << " ERREUR : exception = " << e << endl;
    }
    env.end();
    return 0;
}