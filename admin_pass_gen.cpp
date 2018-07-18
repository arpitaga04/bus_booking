#include<fstream>
#include<iostream>
using namespace std;

struct admin
{
	char usr[20], pass[30];
	
	void write_on_file()
	{
		fstream f;
		int a,b;
		
		remove("admin.txt");
		f.open("admin.txt", ios::out);
		
		cout<<"\nEnter new Username : ";
		cin>>usr;
		cout<<"\nEnter new Password : ";
		cin>>pass;
		
		f.write((char *)this, sizeof(*this));
	
	}
};

//struct admin *ad = new admin;

int main()
{
	struct admin ad;
	ad.write_on_file();
	return 1;
}
