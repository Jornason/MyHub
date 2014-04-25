#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

bool readInputMesh(char *filename,vector<float>& positions,vector<int>& triangles);
float getVolume(const vector<float>& positions,const vector<int>& triangles);
float getVolumeOfTriangle(float triangle[3][3]);

int main(int argc,char *argv[])
{
	if (argc!=2)
	{
		cout << "Please enter the name of the input file" << endl;
		return 0;
	}
	
	vector<float> positions;
	vector<int> triangles;
	
	bool success = readInputMesh(argv[1],positions,triangles);
	
	if (!success)
	{
		cout << "Input file " << argv[1] << " could not be read successfully. " << endl;
		return 0;
	}
	
	float volume = getVolume(positions,triangles);
	
	cout << "The volume is " << std::fixed << volume << endl;
	
	return 1;
}

//----------------------------------------------------------------------------

float getVolumeOfTriangle(float p[3][3])
{
	float volume;
	
	volume = (p[0][0]*(p[1][1]*p[2][2]-p[1][2]*p[2][1]) - p[0][1]*(p[1][0]*p[2][2]-p[2][0]*p[1][2]) + p[0][2]*(p[1][0]*p[2][1]-p[2][0]*p[1][1]))/6;	
	
	return volume;
}

//----------------------------------------------------------------------------

float getVolume(const vector<float>& positions,const vector<int>& triangles)
{
	float tri[3][3];
	int i,j,k;
	float volume;
	
	volume = 0;
	
	for (i=0;i<triangles.size();i+=3)
	{
		for (j=0;j<3;j++)
		{
			for (k=0;k<3;k++)
			{
				tri[j][k] = positions[3*triangles[i+j]+k];
			}
		}
		volume += getVolumeOfTriangle(tri);
	}
	
	return volume;
}

//-----------------------------------------------------------------------------

bool readInputMesh(char *filename,vector<float>& positions,vector<int>& triangles)
{
	int i,j;
	string line;
	
	positions.clear();
	triangles.clear();
	
	ifstream ifile(filename);
	
	if (!ifile.is_open())
	{
		return false;
	}
	
	while (getline(ifile,line))
	{
		stringstream linestr;
		
		linestr << line;
		
		string code;
		
		linestr >> code;
		
		if (code == "v")
		{
			//the next three are positions;
			float a;
			
			linestr >> a;
			positions.push_back(a);
			
			linestr >> a;
			positions.push_back(a);


			linestr >> a;
			positions.push_back(a);
		}
		else if (code == "f")
		{
			//the next three are triangle indices
			int a;
			
			linestr >> a;
			a--;
			triangles.push_back(a);

			linestr >> a;
			a--;
			triangles.push_back(a);

			linestr >> a;
			a--;
			triangles.push_back(a);
		}
		else
		{
			ifile.close();
			return false;
		}
	}
	ifile.close();
	return true;
}
						
			

			
		
