#include<iostream>
#include<stdio.h>
#include<string.h>
#include<fstream>
#include<cstdlib>
#include<iomanip>

using namespace std;


struct admin
{
	char usr[20];
	char pass[30];
	
	bool get_from_file()
	{
		fstream f;
		int a,b;
		
		f.open("admin.txt", ios::in);
		struct admin *ad = new admin;
		f.read((char *)ad, sizeof(*ad));
		
		a = strcmp(usr, ad->usr);
		b = strcmp(pass, ad->pass);
		
		f.close();
		if(a == 0 && b == 0)
			return true;
		else 
			return false;
	}
};


const char *filename = "bus_data.dat";
const char *userfile = "userfile.dat";
const char *seatfile = "seatfile.dat";
const char *places = "places.txt";

class bus;
class seat_availability;
class user;
class boarding_point;


int dayofweek(int d, int m, int y);

int dayofweek(int d, int m, int y)
{
	int day;
	static int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
	y -= m < 3;
	day =  ( y + y/4 - y/100 + y/400 + t[m-1] + d) % 7;

 	
	
	switch(day)
	{
		case 0:
			cout<<endl<<"Day : Sunday";
			break;
		case 1:
			cout<<endl<<"Day : Monday";
			break;
		case 2:
			cout<<endl<<"Day : Tuesday";
			break;
		case 3:
			cout<<endl<<"Day : Wednesday";
			break;
		case 4:
			cout<<endl<<"Day : Thursday";
			break;
		case 5:
			cout<<endl<<"Day : Friday";
			break;
		case 6:
			cout<<endl<<"Day : Saturday";
	}
	
	return day;
	
}


struct TIME
{
	int h,m;
	
	void input()
	{
		cout<<" Hour : Min :: ";
		cin>>h;
		cin>>m;
	}
	
	void display()
	{
		cout<<h<<" : "<<m;
	}
};


struct DATE
{
	int d,m,y;
	
	void input()
	{
		cout<<"\nEnter the date : (dd mm yyyy)  ";
		cin>>d>>m>>y;
	}
	
	bool match_date(struct DATE dt)
	{
		if(dt.d == d && dt.m == m && dt.y == y)
			return true;
			
		else return false;
	}
};

class boarding_point
{
	char place[30];
	int id;
	
	void write_on_file(boarding_point *bp)
	{
		fstream fp;
		fp.open(places, ios::app);
		fp.write((char *)bp, sizeof(*bp));
		
		fp.close();
	}
		
	public:
		
	void add_places()
	{
		cout<<"\nEnter the place : ";
		cin>>place;
		
		cout<<"\nEnter place id : ";
		cin>>id;
		
		write_on_file(this);
		
	}

	void remove_places()
	{
		int id, fg = 0;
		display_boarding_points();
		
		cout<<"\nEnter place id to remove : ";
		cin>>id;
		
		fstream f;
		fstream f2;
		boarding_point *p = new boarding_point();
		f.open(places, ios::in);
		f2.open("places_bac.dat", ios::app);

		while(f.read((char *)p, sizeof(*p)))
		{
			if(id != p->id)
				f2.write((char *)p, sizeof(*p));
			else
				fg++;
		}
		
		f.close();
		f2.close();
		remove(places);
	  	rename("places_bac.dat",places);
	  	
	  	if(fg == 0)
	  		cout<<"\nThere is no ID "<<id<<" in the database";
	  	else
	  		cout<<"\nDeletion successful";
	  	cin.ignore().get();
	}

	

	char *getplacebyid(int id)
	{
		fstream f;
		boarding_point *p = new boarding_point();
		f.open(places, ios::in);
		
		while(f.read((char *)p, sizeof(*p)))
		{
			if(id == p->id)
				return p->place;
		}
		f.close();
	}

	void display_boarding_points()
	{
		fstream f;
		boarding_point *p = new boarding_point();
		f.open(places, ios::in);
		
		
		cout<<"\n    "<<left<<setw(10)<<"ID"<<left<<setw(20)<<"Place";
		cout<<"\n___________________________";
		while(f.read((char *)p, sizeof(*p)))
			cout<<"\n    "<<left<<setw(10)<<p->id<<left<<setw(20)<<p->place;
		
		f.close();
	}
};

class seat_availability
{
	char reg_no[10];
	struct DATE dt;
	int rem_seat;
	
	seat_availability *check_obj(char *regno, struct DATE d)
	{
		seat_availability *p = new seat_availability();
		fstream f;
		f.open(seatfile, ios::in);
		int chk = 1;
		while(f.read((char *)p, sizeof(*p)))
		{
			chk = strcmp(p->reg_no, regno);
			if(d.match_date(p->dt) && chk == 0)
			{
				return p;
				f.close();
			}
		}
		f.close();
		return NULL;
			
	}
	
		
	void modify_file(seat_availability *b)
	{
		fstream f;
		fstream f2;
		seat_availability *p = new seat_availability();
		f.open(seatfile, ios::in);
		f2.open("seat_bac.dat", ios::app);

		while(f.read((char *)p, sizeof(*p)))
		{
			if(strcmp(b->reg_no, p->reg_no) != 0)
				f2.write((char *)p, sizeof(*p));
			else
				f2.write((char *)b, sizeof(*b));
		}
		
		f.close();
		f2.close();
		remove(seatfile);
	  	rename("seat_bac.dat",seatfile);
	}
	
	
	void write_on_file(seat_availability *b)
	{
		fstream f;
		f.open(seatfile, ios::app);
		f.write((char *)b, sizeof(*b));
		f.close();
	}
	
	
	public:
	seat_availability()
	{
	}
	seat_availability(char *regno, struct DATE d, int n_seat)
	{
		strcpy(reg_no, regno);
		dt = d;
		rem_seat = n_seat;
	}
	
	int reserve_seat(char *regno, struct DATE d, int n_seat)
	{
		seat_availability *p = check_obj(regno, d);
		
		if(p == NULL)
		{
			p = new seat_availability(regno, d, n_seat-1);
			write_on_file(p);
		}
		
		else
		{
			if(p->rem_seat > 0)
			{
				p->rem_seat--;
				cout<<"\nSeat Reserved\n";
				modify_file(p);
				return 1;
			}
			
			else
			{
				cout<<"\nSeats Full";
				return 0;
			}
		}	
			
	}
	
	void cancel_seat(char *regno, struct DATE d)
	{
		seat_availability *p = check_obj(regno, d);
		p->rem_seat++;
		modify_file(p);
		
	}
	
	int getrem_seat(char *regno, struct DATE d, int n_seat)
	{
		seat_availability *p = check_obj(regno, d);
		
		if(p == NULL)
		{
			p = new seat_availability(regno, d, n_seat);
			write_on_file(p);
			return n_seat;
		}
	
		return p->rem_seat;
	}

	
	void del_obj(char *regno)
	{
		fstream f;
		fstream f2;
		seat_availability *p = new seat_availability();
		f.open(seatfile, ios::in);
		f2.open("seat_bac.dat", ios::app);

		while(f.read((char *)p, sizeof(*p)))
		{
			if(strcmp(regno, p->reg_no) != 0)
				f2.write((char *)p, sizeof(*p));
		}
		
		f.close();
		f2.close();
		remove(seatfile);
	  	rename("seat_bac.dat",seatfile);
	}
	
	void display_all()
	{
		fstream f;
		seat_availability *p = new seat_availability();
		f.open(seatfile, ios::in);
		cout<<endl<<endl;
		cout<<"   "<<left<<setw(25)<<"Reg No."<<left<<setw(30)<<"Remaining seats"<<left<<setw(25)<<"Date";
		cout<<"\n________________________________________________________________________________\n";
		while(f.read((char *)p, sizeof(*p)))
		{
			cout<<endl;
			cout<<"   "<<left<<setw(25)<<(p->reg_no)<<left<<setw(30)<<(p->rem_seat)<<left<<setw(2)<<p->dt.d<<"/"<<p->dt.m<<"/"<<p->dt.y;

		}
		f.close();
	}
	
	
	
};


class bus
{
	char bus_name[20],reg_no[20];
	struct TIME dept_time, arr_time;
	
	int tot_no_seats;
	int fare;
	char act_days[8];
	
	int src, dst;
	
	
	void modify_bus_file(bus *b)
	{
		fstream f;
		fstream f2;
		bus *p = new bus();
		f.open(filename, ios::in);
		f2.open("bac.dat", ios::app);

		while(f.read((char *)p, sizeof(*p)))
		{
			if(strcmp(b->getregno(), p->getregno()) != 0)
				f2.write((char *)p, sizeof(*p));
			else
				f2.write((char *)b, sizeof(*b));
		}
		
		f.close();
		f2.close();
		remove(filename);
	  	rename("bac.dat",filename);
	}
	
	
	void write_on_file(bus *b)
	{
		fstream f;
		f.open(filename, ios::app);
		f.write((char *)b, sizeof(*b));
		f.close();
	}
	
	void display_bus(bus *b)
	{
		int i;
		boarding_point *bp = new boarding_point();
		cout<<endl;
		cout<<left<<setw(25)<<(b->bus_name)<<left<<setw(25)<<(bp->getplacebyid(b->src))<<left<<setw(25)<<(bp->getplacebyid(b->dst))<<left<<setw(15)<<(b->fare)<<left<<setw(15)<<(b->reg_no)<<left<<setw(2)<<b->dept_time.h<<left<<setw(3)<<" : "<<left<<setw(15)<<b->dept_time.m<<left<<setw(2)<<b->arr_time.h<<left<<setw(3)<<" : "<<left<<setw(15)<<b->arr_time.m<<left<<setw(20)<<b->tot_no_seats;
		
		cout<<"\nBus available on days    : ";
		for(i=0;i<7;i++)
		{
			
			if(b->act_days[i] == '1')
			{
				switch(i)
				{
					case 0:
						cout<<"Sunday";
						break;
					case 1:
						cout<<"Monday";
						break;
					case 2:
						cout<<"Tuesday";
						break;
					case 3:
						cout<<"Wednesday";
						break;
					case 4:
						cout<<"Thursday";
						break;
					case 5:
						cout<<"Friday";
						break;
					case 6:
						cout<<"Saturday";
						break;
				}
				cout<<"    ";
			}
		}
		cout<<"\n\n- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n";
		 	
	}
	
	
	public:
	
	
	void add_bus()
	{
		boarding_point *bp = new boarding_point();
		bp->display_boarding_points();
		int i, inp;
		cout<<"\nBus Name : "; 
		cin>>bus_name;
		cout<<"\nSource (enter the correspoding no): "; 
		cin>>src;
		cout<<"\nDestination (enter the correspoding no): "; 
		cin>>dst;
		cout<<"\nEnter the Fare : Rs. ";
		cin>>fare;
		cout<<"\nRegistration No. : "; 
		cin>>reg_no;													
		cout<<"\nEnter Departure Time  "; 
		dept_time.input();
		cout<<"\nEnter Arrival Time  ";
		arr_time.input();
		cout<<"\nEnter the No of Seats : "; 
		cin>>tot_no_seats;
		
		cout<<"\nEnter the weekday\n(input manner : there should be 7 inputs, either 0 or 1) \n";
		for(i = 0;i<7; i++)
		{
			cin>>inp;
			if(inp == 1)
				act_days[i] = '1';
			else 
				act_days[i] = '0';
		}
		
		act_days[7] = '\0';
			
		write_on_file(this);
		cout<<("\n The bus has been added.\n");
		
	}
	
	
	void modify_bus()
	{
		bus *b;
		boarding_point *bp = new boarding_point();
		char reg[10];
		seat_availability st;
		
		display_all_buses(1);
		cout<<"\nEnter the Registration No of the Bus to be Modified : ";
		cin>>reg;
		
		b = get_bus_by_reg(reg);
		if(b == NULL)
		{
			cout<<"\nNo bus with Registration No. :"<<reg<<" exist in Database";
			return;
		}
		system("clear");
		cout<<"\nModifying details of Bus with Reg No : "<<reg;
		bp->display_boarding_points();
		
		cout<<"\nSource (enter the correspoding no) : "; 
		b->setsrc();
		cout<<"\nDestination (enter the correspoding no) : "; 
		b->setdst();
		cout<<"\nEnter the Fare : Rs. ";
		b->setfare();
		cout<<"\nEnter Departure Time : "; 
		b->setdept_time();
		cout<<"\nEnter Arrival Time : "; 
		b->setarr_time();
		
		cout<<"\nEnter the weekday\n(input manner : there should be 7 inputs, either 0 or 1) ";
		for(int i = 0;i<7; i++)
		{
			int inp;
			cin>>inp;
			if(inp == 1)
				act_days[i] = '1';
			else 
				act_days[i] = '0';
		}
		
		act_days[7] = '\0';
		
		modify_bus_file(b);
		st.del_obj(reg);
	}
	

	void display_all_buses(int chk = 0)
	{
		fstream f;
		bus *p = new bus();
		f.open(filename, ios::in);
		if(chk == 0)
		{
			while(f.read((char *)p, sizeof(*p)))
				display_bus(p);
		}
		
		else
		{
			cout<<"\n    "<<left<<setw(10)<<"Reg No."<<left<<setw(20)<<"Bus Name";
			cout<<"\n_________________________________";
			while(f.read((char *)p, sizeof(*p)))
			{
				cout<<"\n    "<<left<<setw(10)<<p->reg_no<<left<<setw(20)<<p->bus_name;
			}
		}
		
		f.close();
	}
	
	
	int display_acc_user(int src, int dst, int act_day)
	{
		fstream f;
		bus *p = new bus();
		f.open(filename, ios::in);
		int wd, count = 0;
		wd = 1;
		
		while(f.read((char *)p, sizeof(*p)))
		{
			wd = 1;
			if(p->act_days[act_day] == '1')
				wd = 0;
			if(src == p->getsrc() && dst == p->getdst() && wd == 0)
			{
				display_bus(p);
				count++;
			}
		}
		return count;
		f.close();
			
	}
		

	void remove_bus()
	{
		fstream f;
		fstream f2;
		char reg[10];
		seat_availability st;
		
		display_all_buses(1);
		
		cout<<"\nEnter the Registration No of Bus to be Removed from Database : ";
		cin>>reg;
		
		bus *b = get_bus_by_reg(reg);
		
		if(b == NULL)
		{
			cout<<"\nNo bus with Registration No. :"<<reg<<" exist in Database";
			return;
		}
		
		bus *p = new bus();
		f.open(filename, ios::in);
		f2.open("bac.dat", ios::app);

		while(f.read((char *)p, sizeof(*p)))
		{
			if(strcmp(b->getregno(), p->getregno()) != 0)
				f2.write((char *)p, sizeof(*p));
		}
		
		st.del_obj(reg);
		f.close();
		f2.close();
		remove(filename);
	  	rename("bac.dat",filename);
	  	cout<<"\n The bus has been removed.\n";
	  	system("clear");
	  	
	}

	
	bus *get_bus_by_reg(char *reg)
	{
		bus *b = new bus();
		fstream f;
		f.open(filename, ios::in);
		while(f.read((char *)b, sizeof(*b)))
		{
			if(strcmp(reg, b->getregno()) == 0)
			{
				f.close();
				return b; 
			}
		}
		
		f.close();
		return NULL;
	}
	
	
	char *getregno()
	{
		return reg_no;
	}
	
	
	int getsrc()
	{
		return src;
	}
	
	
	int getdst()
	{
		return dst;
	}
	
	
	char *getact_days()
	{
		return act_days;
	}
	
	int getfare()
	{
		return fare;
	}
	
	int gettotseats()
	{
		return tot_no_seats;
	}
	
	
	void setsrc()
	{
		cin>>src;
	}
	
	
	void setdst()
	{
		cin>>dst;
	}
	
	
	void setfare()
	{
		cin>>fare;
	}
	
	
	void setdept_time()
	{
		dept_time.input();
	}
	
	
	void setarr_time()
	{
		arr_time.input();
	}

};




class user
{
	char fname[30], lname[30], email[30], mob[11], gender, reg_no[10], from[30], to[30];
	int age, fare;
	DATE doj;
	
	
	void write_on_file(user *b)
	{
		fstream f;
		f.open(userfile, ios::app);
		f.write((char *)b, sizeof(*b));
		f.close();
	}
	
	
	user *get_user_by_mob(char *mob)
	{
		user *b = new user();
		fstream f;
		f.open(userfile, ios::in);
		while(f.read((char *)b, sizeof(*b)))
		{
			if(strcmp(mob, b->mob) == 0)
			{
				f.close();
				return b; 
			}
		}
		
		f.close();
		return NULL;
	}
	
	void generate_bill()
	{
		cout<<"\n\nName 			       : 	"<<fname<<" "<<lname;
		cout<<"\nEmail   		       : 	"<<email;
		cout<<"\nContact number 	               :        "<<mob;
		cout<<"\nGender(M/F)                    : 	"<<gender;
		cout<<"\nAge 			       : 	"<<age;
		cout<<"\nRegistration No. of the bus    : 	"<<reg_no;
		cout<<"\nDate 			       :        "<<doj.d<<"/"<<doj.m<<"/"<<doj.y;
		cout<<"\nTravelling From 	       : 	"<<from<<"   To : "<<to;
		cout<<"\nFare in INR  		       :        "<<fare;
		
	}
	public:
	
	void book_ticket(int src, int dst, char *reg, struct DATE dt, int price)
	{
		boarding_point *bp = new boarding_point();
		
		int i, inp;
		cout<<"\nFirst Name : "; 
		cin>>fname;
		cout<<"\nLast Name : "; 
		cin>>lname;
		cout<<"\nE-mail : "; 
		cin>>email;
		cout<<"\nMobile : ";
		cin>>mob;
		cout<<"\nGender(M/F) : "; 
		cin>>gender;
		cout<<"\nAge : "; 
		cin>>age;
		system("clear");
		fare = price;
		doj = dt;
		strcpy(from, bp->getplacebyid(src));
		strcpy(to, bp->getplacebyid(dst));
		strcpy(reg_no, reg);

		generate_bill();
		cout<<"\n\nTHANK YOU FOR BOOKING\n";
		write_on_file(this);
		
		
	}
	
	
	void cancel_ticket()
	{
		fstream f;
		fstream f2;
		char mob[11];
		char ch;

		seat_availability *st = new seat_availability();
		
		cout<<"\nEnter your Mobile No : ";
		cin>>mob;
		
		user *b = get_user_by_mob(mob);
		if(b == NULL)
		{
			cout<<"\nMobile Number could not be found";
			return;
		}
		
		b->generate_bill();
		
		cout<<"\nConfirm Cancel ticket Y/N : ";
		cin>>ch;
		
		if(ch == 'N' || ch == 'n')
			return;
		
		
			
		user *p = new user();
		f.open(userfile, ios::in);
		f2.open("user_bac.dat", ios::app);

		while(f.read((char *)p, sizeof(*p)))
		{
			if(strcmp(b->mob, p->mob) != 0)
				f2.write((char *)p, sizeof(*p));
		}
		
		st->cancel_seat(b->reg_no, b->doj);
		
		f.close();
		f2.close();
		remove(userfile);
	  	rename("user_bac.dat",userfile);
	  	cout<<"\nTictket is successfully cancelled.\n";
	  	cin.ignore().get();
	  
	}
	
	void display_all()
	{
		fstream f;
		user *p = new user();
		f.open(userfile, ios::in);
		
		
		char full_name[60];
		
		
		cout<<endl;
		cout<<left<<setw(40)<<"NAME"<<left<<setw(20)<<"MOBILE NO."<<left<<setw(7)<<"M/F"<<left<<setw(17)<<"REG NO."<<left<<setw(20)<<"FROM"<<left<<setw(20)<<"TO"<<left<<setw(10)<<"FARE"<<left<<setw(20)<<"Date of journey";
		cout<<"\n___________________________________________________________________________________________________________________________________________________";
		while(f.read((char *)p, sizeof(*p)))
		{
			full_name[0] = '\0';
			strcpy(full_name, p->fname);
			strcat(full_name," ");	
			strcat(full_name, p->lname);
			cout<<endl;
			
			cout<<left<<setw(40)<<full_name<<left<<setw(20)<<(p->mob)<<left<<setw(7)<<(p->gender)<<left<<setw(17)<<(p->reg_no)<<left<<setw(20)<<(p->from)<<left<<setw(20)<<(p->to)<<left<<setw(10)<<(p->fare)<<left<<setw(2)<<(p->doj.d)<<"/"<<(p->doj.m)<<"/"<<(p->doj.y);                                  
			
		}
		f.close();
	}
	
};



int main()
{
	bus b, *p;
	user u;
	
	boarding_point *bp = new boarding_point();
	seat_availability *st;
	char reg[10];
	int src, dst;
	
	int ch1 = 1, ch2 = 1, d = -1, ch3 = 0;
	int c, c1 = 0;
	struct DATE dt;
	
	struct admin chk;
	
		
	while(ch1 != 3)
	{
	
		ch2 = 1;
		system("clear");
		cout<<"\n1. User Panel\n2. Admin Login\n3. Exit";
		cout<<"\nSelect the option  : ";
		cin>>ch1;
		
		system("clear");
		switch(ch1)
		{
			case 1:										//User Panel
			
			while(ch2 != 3)
			{
				cout<<"\n1. Search Bus\n2. Cancel Ticket\n3. Back";
				cout<<"\nSelect the option : ";
				cin>>ch2;
				system("clear");
				switch(ch2)
				{
					case 1:
						cout<<"\nList of all the places : ";
						bp->display_boarding_points();
						cout<<"\nEnter the Boarding Point (id corresponding to the place): ";
						cin>>src;
						cout<<"\nEnter the Destination: (id corresponding to the place) : ";
						cin>>dst;
						dt.input();
						d = dayofweek(dt.d,dt.m,dt.y);
						system("clear");
						
						cout<<endl;
						cout<<left<<setw(25)<<"Bus Name"<<left<<setw(25)<<"Source"<<left<<setw(25)<<"Dest."<<left<<setw(15)<<"Fare (INR)"<<left<<setw(15)<<"Bus Reg. No"<<left<<setw(20)<<"Dept. Time"<<left<<setw(20)<<"Arr. Time"<<left<<setw(20)<<"No. of seats";
						cout<<"\n_________________________________________________________________________________________________________________________________________________________________\n";


						c = b.display_acc_user(src, dst, d);
						if(c == 0)
						{
							cout<<"\nNo buses available";
							ch3 = 2;
							cin.ignore().get();
						}
						else
						{
					        	cout<<"\n\n1. Book the ticket\n\n2. Perform new search";
							cout<<"\n\nSelect the option : ";
							cin>>ch3;
						}

						if(ch3 == 1)
						{
							
							cout<<"\nEnter the registration no of the bus : ";
							cin>>reg; 
							p = b.get_bus_by_reg(reg);
							if(p == NULL)
								cout<<"\nNo Bus with this Registration No. Please try again";
							else
							{	
								int se = p->gettotseats();
								c1 = st->reserve_seat(reg, dt, se);
								se = p->getfare();
								if(c1 != 0)
									u.book_ticket(src, dst, reg, dt, se);
							}
						}
						
						break;
					
					
					case 2:
						
						u.cancel_ticket();
						
						break;

							
					default:
						ch2 = 3;
				
				}
			
			}
			break;
			
			
			case 2:							//****************************ADMIN******************************* 
			
			cout<<"\nEnter the login Details : \n\n";
			cout<<"\nUsername : ";
			cin>>chk.usr;
			cout<<"\nPassword : ";
			cin>>chk.pass;
			
			
			if(chk.get_from_file())
			{
				system("clear");
				while(ch2 != 10)
				{
					
					cout<<"\n1. Add Bus\n2. Modify Bus\n3. Display All Bus\n4. Remove Bus\n5. Search Bus\n6. Display Seat Details\n7. Add Places\n8. Remove Places\n9. Display all Bookings\n10. Back";
					cout<<"\nSelect the option : ";
					cin>>ch2;
					
					
					switch(ch2)
					{
						case 1:	
							system("clear");
							cout<<"\nAdd Bus-\n";
							b.add_bus();
							break;
							
						case 2:	
							system("clear");
							b.modify_bus();
							break;
							
						case 3:	
							system("clear");
							cout<<endl;
							cout<<left<<setw(25)<<"Bus Name"<<left<<setw(25)<<"Source"<<left<<setw(25)<<"Dest."<<left<<setw(15)<<"Fare (INR)"<<left<<setw(15)<<"Bus Reg. No"<<left<<setw(20)<<"Dept. Time"<<left<<setw(20)<<"Arr. Time"<<left<<setw(20)<<"No. of seats";
							cout<<"\n_________________________________________________________________________________________________________________________________________________________________\n";
							b.display_all_buses();
							break;
							
						case 4:	
							system("clear");
							b.remove_bus();
							break;
							
						case 5:	
							system("clear");
							cout<<"\nList of all the places available";
							bp->display_boarding_points();
							cout<<"\nEnter the Boarding Point (id corresponding to the place) : ";
							cin>>src;
							cout<<"\nEnter the Destination: (id corresponding to the place) : ";
							cin>>dst;
							dt.input();
							d = dayofweek(dt.d,dt.m,dt.y);
							system("clear");
							
							cout<<endl;
							cout<<left<<setw(25)<<"Bus Name"<<left<<setw(25)<<"Source"<<left<<setw(25)<<"Dest."<<left<<setw(15)<<"Fare (INR)"<<left<<setw(15)<<"Bus Reg. No"<<left<<setw(20)<<"Dept. Time"<<left<<setw(20)<<"Arr. Time"<<left<<setw(20)<<"No. of seats";
							cout<<"\n_________________________________________________________________________________________________________________________________________________________________\n";
							c = b.display_acc_user(src, dst, d);
							if(c == 0)
							{
								cout<<"\nNo buses available";
								ch3 = 2;
								
							}
							cin.ignore().get();
							break;

						case 6:
							system("clear");
							cout<<"\nDisplay Seat Details";
							st->display_all();
							break;
							
						case 7:	
							system("clear");
							cout<<"\nAdd Place-\n";
							bp->add_places();
							break;
								
						case 8:	
							system("clear");
							cout<<"\nRemove Place-\n";
							bp->remove_places();
							break;
							
						case 9:
							system("clear");
							u.display_all();
							break;
			
						default:
							ch2 = 10;
							
					}
					cout<<"\n- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n";
					
				}
			
			}
			else
			{
				cout<<"\nWrong Username or Password";
				int ch;
				cout<<"\n\nPress any key to go back : ";
				cin.ignore().get();
				system("clear");
			}
			break;
			
			default:
				ch1 = 3;
		
		}
	}
	
	return 1;
}


