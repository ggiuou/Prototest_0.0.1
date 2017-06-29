#include "stdafx.h"
#include "svm_regcognition.h"
#include "Manage.h"

using namespace dlib;
using namespace std;
typedef probabilistic_decision_function<kernel_type> probabilistic_funct_type;
typedef normalized_function<probabilistic_funct_type> pfunct_type;

SvmRegcognition::SvmRegcognition(char* path)
{
	all_test = Manage::get_all_file(path);
	string path_file(strcat(path, "\\name.dat"));
	deserialize(path_file) >> name;	
}

void SvmRegcognition::SvmRegcognizer(matrix<float, 0, 1> test)
{
	
	double *count = new double[name.size()];
	for (int i = 0; i < name.size(); i++)
		count[i] = 0;
	for (int i = 0; i < all_test.size(); i++)
	{		
		pfunct_type learned_pfunct;
		if (Manage::get_name(all_test[i]).compare("name"))
		{
			deserialize(all_test[i]) >> learned_pfunct;

			string data_name = Manage::get_name(all_test[i]);
			string nameA = data_name.substr(0, data_name.find_first_of("&"));
			string nameB = data_name.substr(data_name.find_last_of("&") + 1);

			double prob = learned_pfunct(test);

			count[atoi(nameA.c_str())] += (prob - 0.5) * 2;
			count[atoi(nameB.c_str())] += (1 - prob - 0.5) * 2;
		}
	}
	int index;
	double max = 0;
	double size = name.size() - 1;
	for (int i = 0; i < name.size(); i++)
	{
		if ((count[i] = count[i]/size) > max)
		{
			max = count[i];
			index = i;
		}
	}
	cout << name[index] + "---> confidence: " + cast_to_string(count[index])<< endl;
}
