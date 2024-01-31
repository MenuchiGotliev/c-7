#include <iostream>
#include <fstream>
#include <string>
#include "Family.h"
#include <queue>

using namespace std;
void setFile(fstream& outFile) {

	// אתחול כל המשפחות בקובץ בינארי
	for (int i = 0; i < 100; ++i) {
		Family f;
		// כתיבת משפחה ריקה לקובץ הנתונים מאותחלים במחלקת Family
		outFile.write((char*)&f, sizeof(Family));
	}

}

void add(fstream& file) {

	// קליטת נתוני המשפחה מהמשתמש
	Family newFamily;
	cout << "Enter number of family: ";
	cin >> newFamily.familyNum;
	// בדיקה האם האינדקס תקין
	if (newFamily.familyNum < 1 || newFamily.familyNum > 100) {
		throw "ERROR:Invalid family number";
		return;
	}
	cout << "Enter family name: ";
	cin >> newFamily.familyName;
	cout << "Enter number of children: ";
	cin >> newFamily.childrenNum;
	cout << "Enter phone: ";
	cin >> newFamily.phone;



	// מעבר למיקום בקובץ
	file.seekg((newFamily.familyNum-1) * sizeof(Family));

	// קריאת המשפחה במיקום הנתון בקובץ
	Family existingFamily;
	file.read((char*)(&existingFamily), sizeof(Family));

	// בדיקה האם המשפחה כבר קיימת
	if (existingFamily.familyNum != 0) {
		throw "ERROR: Family is already in the file";
		return;
	}

	// כתיבת המשפחה החדשה לקובץ
	file.seekp((newFamily.familyNum-1) * sizeof(Family));
	file.write((char*)&newFamily, sizeof(Family));

	
}
void del(fstream& file, int famNum)
{
	file.seekp((famNum-1) * sizeof(Family));
	Family f;
	f.familyNum = 0;
	file.write((char*)&f, sizeof(Family));
}


void update(fstream& file, int familyIndex, queue<Family> waitingList) {
	
	

	// בדיקת תקינות האינדקס של המשפחה
	if (familyIndex < 1 || familyIndex > 100) {
		throw "ERROR:Invalid family number";
		return;
	}

	// מעבר למיקום בקובץ של המשפחה הרצויה
	file.seekg((familyIndex-1) * sizeof(Family));
	Family fam;
	file.read((char*)(&fam), sizeof(Family));

	if (fam.familyNum == 0) {
		throw"ERROR: family is not in the file";
		return;
	}

	bool f;
	int count = 0, index;
	Family newF = fam;
	cout << "Enter boolean values for 6 courses (0/1): ";

	for (int i = 0; i < 6; ++i) {
		cin >> f;
		if (f)
		{
			file.seekg(0, ios::beg);
			for (int j = 0; j < 100; j++)
			{
				
				Family fa;
				file.read((char*)(&fa), sizeof(Family));

				if (fa.courses[i])
				{
					count++;
				}

			}
			if (count > 2)
			{
				newF.courses[i] = true;
				waitingList.push(newF);
			}
			else
			{
				fam.courses[i] = true;
				newF.courses[i] = false;
			}
			count = 0;
		}
	}
	
	file.seekp((familyIndex-1) * sizeof(Family));
	file.write((char*)(&fam), sizeof(Family));
	
}

void waiting(fstream& file, queue<Family> q)
{
	Family  f, x;
	queue<Family> temp;
	int count = 0;
	while (!q.empty())
	{
		x = q.front();
		q.pop();
		//בדיקה האם המשפחה נמחקה
		if (x.familyNum != 0)
		{
			for (int i = 0; i < 6; i++)
			{
				if (x.courses[i])
				{
					file.seekg(0, ios::beg);
					for (int j = 0; j < 100; j++)
					{
						Family fa;
						file.read((char*)(&fa), sizeof(Family));

						if (fa.courses[i])
						{
							count++;
						}

					}

					if (count<= 2)
					{
						char answer;
						cout << " name: " << x.familyName << " phone: " << x.phone << endl;
						cin >> answer;
						if (answer == 'Y')
							x.courses[i] = true;

					}
					else {
						temp.push(x);
					}
					count = 0;

				}
			}

		}
		file.seekp((x.familyNum-1) * sizeof(Family));
		file.write((char*)&x, sizeof(Family));
		x = q.front();
		q.pop();
	}
	q = temp;
	

}

bool rishum(fstream& file, int familyIndex, int courseNum)
{
	
	Family fam;
	if (courseNum < 1 || courseNum>6)
	{
		throw "ERROR:Invalid class number";
		return false;
	}
	if (familyIndex < 1 || familyIndex > 100) {
		throw "ERROR:Invalid family number";
		return false;
	}
	file.seekg((familyIndex - 1) * sizeof(Family));
	file.read((char*)(&fam), sizeof(Family));
	// בדיקת תקינות האינדקס של המשפחה
	
	if (fam.familyNum == 0) {
		throw"ERROR: family is not in the file";
		return false;
	}
	if (fam.courses[courseNum - 1])
		return true;
	return false;
}
void print(fstream& file, int familyIndex)
{
	Family fam;
	if (familyIndex < 1 || familyIndex > 100) {
		throw "ERROR:Invalid family number";
		return;
	}
	file.seekg((familyIndex - 1) * sizeof(Family));
	file.read((char*)(&fam), sizeof(Family));
	// בדיקת תקינות האינדקס של המשפחה

	if (fam.familyNum == 0) {
		throw"ERROR: family is not in the file";
		return;
	}
	cout << "family name:" << fam.familyName << endl;
	cout << "number of children:" << fam.childrenNum << endl;
	cout << "phone:" << fam.phone << endl;
	for (int i = 0; i < 6; i++)
	{
		if (fam.courses[i])
			cout << "in course" << i + 1 << "Y" << endl;
		else
			cout << "in course" << i + 1 << "N" << endl;
	}
}
void inClass(fstream& file, int courseNum)
{
	
	if (courseNum < 1 || courseNum>6)
	{
		throw "ERROR:Invalid class number";
		return;
	}
	file.seekg(0, ios::beg);
	for (int i = 0; i < 100; i++)
	{
		Family fa;
		file.read((char*)(&fa), sizeof(Family));

		if (fa.courses[courseNum-1])
		{
			cout << fa.familyName << endl;
		}
	}

}
int main()
{
	queue<Family> q;
	fstream file;
	file.open("families.txt", ios::binary | ios::in | ios::out);
	if (!file)
	{
		cout << "ERROR: couldn't open file\n";
		return 0;
	}
	setFile(file);
	int choice;
	int snum;
	int cnum;
	cout << "Choices are:\n0 to exit\n1 to add a family\n2 to delete a family\n3 to update rishum to classes\n4 to update waiting to classes \n5 to check rishum for a classas\n6 to print a family\n7 to print all the families that participate in a specific class\n";
	cout << "enter 0-7:\n";
	cin >> choice;
	while (choice)
	{
		switch (choice)
		{
		case 1://add to the file
			try { add(file); }
			catch (const char* msg) { cout << msg; }
			break;
		case 2://delete from file
			cout << "enter number of family to delete:\n";
			cin >> snum;
			try { del(file, snum); }
			catch (const char* msg) { cout << msg; }
			break;
		case 3://update the list of classes of a family
			cout << "enter number of family to update:\n";
			cin >> snum;
			try { update(file, snum,q); }
			catch (const char* msg) { cout << msg; }
			break;
		case 4://update the list of classes of a waiting family
			waiting(file, q);
			break;
		case 5://check rishum to a specific class
			cout << "enter number of family to check rishum:\n";
			cin >> snum;
			cout << "enter number of class to check rishum:\n";
			cin >> cnum;
			try
			{
				cout << "The family is" << (rishum(file, snum, cnum) ? " " : " not ") << "taking the class\n";
			}
			catch (const char* msg) { cout << msg; }
			break;
		case 6://print the details of a specific family
			cout << "enter number of family to print:\n";
			cin >> snum;
			try { print(file, snum); }
			catch (const char* msg) { cout << msg; }
			break;
		case 7://print the details of all the families that are taking a specific class
			cout << "enter number of class to check rishum:\n";
			cin >> cnum;
			try { inClass(file, cnum); }
			catch (const char* msg) { cout << msg; }
			break;
		default:
			cout << "ERROR: invalid choice\n";

		}
		cout << "\nenter 0-7:\n";
		cin >> choice;
	}
	file.close();
	return 0;
}




