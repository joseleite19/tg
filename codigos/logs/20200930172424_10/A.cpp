#include <bits/stdc++.h>

using namespace std;

void process(string filename) {
	ifstream in(filename);
	string tmp;
	double sum = 0;
	int cnt = 0;
	while(getline(in, tmp)) { // read "Processing file ..."
		getline(in, tmp); // read "... seconds"
		tmp.erase(tmp.begin() + tmp.find(' '), tmp.end());
		double t = stod(tmp);
		getline(in, tmp); // read Sat/Unsat/Unable
		while(isspace(tmp.back())) tmp.pop_back();
		if(tmp != "Unable") {
			sum += t;
			cnt += 1;
		}
		getline(in, tmp); // read "Infere.."
		getline(in, tmp); // read empty line
	}
	printf("%s %.2lf\n", filename.c_str(), sum / cnt);
}

int main() {

	string filename;

	while(cin >> filename) {
		process(filename);
	}
}
