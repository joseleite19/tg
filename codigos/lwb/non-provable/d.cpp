#include <bits/stdc++.h>

using namespace std;

int main(){
	vector<string> v; 
	
	v.push_back("k_d4_n.txt.Z.k");
	v.push_back("k_grz_n.txt.Z.k");
	v.push_back("k_path_n.txt.Z.k");
	v.push_back("k_poly_n.txt.Z.k");
	v.push_back("k_branch_n.txt.Z.k");
	v.push_back("k_dum_n.txt.Z.k");
	v.push_back("k_lin_n.txt.Z.k");
	v.push_back("k_ph_n.txt.Z.k");
	v.push_back("k_t4p_n.txt.Z.k");

	ofstream ofile;
	ifstream ifile;
	for(int i = 0; i < (int)v.size(); i++){
		ifile.open(v[i].c_str());

		vector<string> s;
		string ss;
		while(getline(ifile, ss)) s.push_back(ss);

		ifile.close();
		
		ofile.open(v[i].c_str());

		ofile << "sos(formulas).\n";

		for(int j = 0; j < (int)s.size(); j++)
			ofile << s[i] << ".\n";

		ofile << "end_of_list.\n";
		ofile.close();
	}


	return 0;
}