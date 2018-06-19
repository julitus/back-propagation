#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <ctime>
#include <cmath>

using namespace std;

typedef double flt;
typedef vector<flt> row;
typedef vector<row> matrix;
typedef flt (*func)(flt);

#define ALPHA 0.5
#define TOLERANCE 0.1
#define MAXIMUM 0.5

int nInputs;
int nOutputs;
matrix dTraining;
matrix dTest;

row getRandomWeights(int);

class Neuron
{
	public:
		row w;
		Neuron(int n_weights) { 
			w = getRandomWeights(n_weights); 
		}
		flt getOut(row x) {
			flt s = 0;
			for (int i = 0; i < x.size(); i++)
				s += (x[i] * w[i]);
			return s;
		}
};

class Layer
{
	public:
		func f;
		func df;
		vector<Neuron *> neurons;
		Layer(int n_layers, int n_inputs, func fu, func dfu) {
			f = fu;
			df = dfu;
			for (int i = 0; i < n_layers; i++)
				neurons.push_back(new Neuron(n_inputs));
		}
		row getOut(row x, row &s) {
			row o;
			for (int i = 0; i < neurons.size(); i++) {
				flt s_t = neurons[i]->getOut(x);
				s.push_back(s_t);
				o.push_back(f(s_t));
			}
			return o;
		}
};

void printRow(row);
void printMatrix(matrix);
void printUnordererMap(unordered_map<string, matrix>);
string idToRow(row, int);
int getData(string, string, flt);
flt getError(row, row);
void training(vector<Layer *> &);
matrix forward(row, vector<Layer *> &, matrix &);
void backward(row, matrix, matrix, vector<Layer *> &);

/* f1 */
flt fExp(flt u) { return 1.0 / (1 + exp(-u)); }
flt dfExp(flt u) { flt f = fExp(u); return f * (1 - f); }
/******/

/* f2 */

/******/

int main(int argc, char *argv[])
{
	if (argc <= 4)
		cout << "Los argumentos son: perceptron_multi <n_inputs> <n_outputs> <file> <layer_1> <f_1> <layer_2> <f_2>... <layer_n> <f_n>" << endl;
	else {
		srand (time(NULL));
  		unordered_map<int, pair<func, func> > functions({{0, make_pair(fExp, dfExp)}});
		nInputs = atoi(argv[1]);
		nOutputs = atoi(argv[2]);
		string nameFile = argv[3];

		getData(nameFile, ",", 1);

		vector<Layer *> network;

		int nInputsTemp = nInputs + 1;
		for (int i = 4; i < argc; i += 2) {
			network.push_back(new Layer(atoi(argv[i]), nInputsTemp, functions[atoi(argv[i + 1])].first, functions[atoi(argv[i + 1])].second));
			nInputsTemp = atoi(argv[i]) + 1;
		}

		training(network);
		
	}
	return 0;
}

row getRandomWeights(int size)
{
	row weights;
	for (int i = 0; i < size; i++)
		weights.push_back((flt) (rand()) / (RAND_MAX / MAXIMUM));
	return weights;
}

int getData(string file, string delimeter, flt porc)
{
	unordered_map<string, matrix> d;
	string line;
	ifstream infile(file);
	while (getline(infile, line)){
		size_t pos = 0;
		string token, idRow;
		row lineRow;
		while ((pos = line.find(delimeter)) != string::npos) {
		    token = line.substr(0, pos);
		    lineRow.push_back(strtof((token).c_str(), 0));
		    line.erase(0, pos + delimeter.length());
		}
		lineRow.push_back(strtof((line).c_str(), 0));
		idRow = idToRow(lineRow, nInputs);
		d[idRow].push_back(lineRow);
		dTraining.push_back(lineRow);
	}
}

flt getError(row rs, row outs)
{	
	flt sum = 0.0;
	for (int i = 0; i < outs.size(); i++) {
		sum += (0.5 * pow(rs[i] - outs[i], 2));
	}
	return sum;
}

matrix forward(row x, vector<Layer *> &network, matrix &ues)
{	
	row xTemp(1, 1.0);
	xTemp.insert(xTemp.end(), x.begin(), x.end());
	matrix outs(1, xTemp);
	for (int i = 0; i < network.size(); i++) {
		row u;
		x = network[i]->getOut(xTemp, u);
		row xTemp(1, 1.0);
		xTemp.insert(xTemp.end(), x.begin(), x.end());
		ues.push_back(u);
		outs.push_back(xTemp);
	}
	return outs;
}

void backward(row d, matrix u, matrix o, vector<Layer *> &network)
{	
	row d_e_ans;
	for (int i = network.size() - 1; i >= 0; i--) {
		row d_e;
		for (int j = 0; j < network[i]->neurons.size(); j++) {
			flt d_e_j;
			if (i == network.size() - 1) {
				d_e_j = (d[j] - o[i + 1][j + 1]) * network[i]->df(u[i][j]);
			} else {
				d_e_j = 0.0;
				for (int e = 0; e < d_e_ans.size(); e++) {
					d_e_j += (d_e_ans[e] * network[i + 1]->neurons[e]->w[j + 1]);
				}
				d_e_j *= network[i]->df(u[i][j]);
			}
			d_e.push_back(d_e_j);
		}
		for (int j = 0; j < network[i]->neurons.size(); j++) {
			for (int k = 0; k < network[i]->neurons[j]->w.size(); k++) {
				network[i]->neurons[j]->w[k] += ALPHA * o[i][k] * d_e[j];
			}
		}
		d_e_ans = d_e;
	}
}

void training(vector<Layer *> &network)
{
	int epoch = 0;
	matrix outs(dTraining.size(), row(1, 0));
	while (true) {
		int r = 0;
		flt er, error = 0.0;
		for (int i = 0; i < dTraining.size(); i++) {
			matrix u;
			matrix outLayers = forward(row(dTraining[i].begin(), dTraining[i].begin() + nInputs), network, u);
			outs[r] = row(outLayers.back().begin() + 1, outLayers.back().end());
			er = getError(row(dTraining[i].begin() + nInputs, dTraining[i].end()), outs[r]);
			
			error += er;

			r++;
			backward(row(dTraining[i].begin() + nInputs, dTraining[i].end()), u, outLayers, network);
		}
		
		cout << "\n >> error: " << error << endl;

		if (error < TOLERANCE) {
			cout << "\n-- Weights --" << endl;
			for (int l = 0; l < network.size(); l++) {
				cout << "Layer " << l + 1 << endl;
				for (int n = 0; n < network[l]->neurons.size(); n++) {
					cout << "Neuron " << n + 1 << endl;
					printRow(network[l]->neurons[n]->w);
				}
			}
			cout << "\n-- Outs --" << endl;
			printMatrix(outs);
			cout << "\n EPOCH: " << epoch << endl;
			break;
		}

		epoch++;
	}
}

void printRow(row r)
{
	for(int j = 0; j < r.size(); j++)
		cout << r[j] << " ";
	cout << endl;
}

void printMatrix(matrix m)
{
	for(int i = 0; i < m.size(); i++)
		printRow(m[i]);
	cout << endl;
}

void printUnordererMap(unordered_map<string, matrix> dict)
{
	for (auto& x: dict) {
		cout << x.first << endl;
		printMatrix(x.second);
	}
}

string idToRow(row r, int ini)
{
	string id = "";
	for(; ini < r.size(); ini++)
		id += to_string(int(r[ini]));
	return id;
}