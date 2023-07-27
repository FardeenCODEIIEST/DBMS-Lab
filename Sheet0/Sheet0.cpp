#include <iostream>
#include <vector>
#include <unordered_map>
#include <iomanip>
#include <fstream>

using namespace std;

// Structure defined for each record
struct record
{
  int regionNo;
  int salesManNo;
  string _id;
  int unitsSold;
};

record toRecord(string &row)
{
  stringstream s(row);
  string word;
  record rec;

  getline(s, word, ',');
  rec.regionNo = stoi(word);

  getline(s, word, ',');
  rec._id = word;

  getline(s, word, ',');
  rec.salesManNo = stoi(word);

  getline(s, word, ',');
  rec.unitsSold = stoi(word);

  return rec;
}

// DATABASE handles all the queries
class Database
{
private:
  ifstream finput;
  ofstream foutput;
  string companyName;
  vector<vector<int>> salesCost;
  vector<record> rows;
  unordered_map<string, int> priceMap;

public:
  Database(string, string &);
  string formatCost(int n);
  void readData();
  void fetchPrice();
  void calculateSales();
  void displaySummary(string &);
};

Database::Database(string company, string &inputFile)
{
  this->companyName = company;
  // 4 regions, each region has 6 salesmen --> 1 based indexing
  salesCost.resize(5);
  for (int i = 1; i <= 4; i++)
  {
    salesCost[i].resize(7);
  }
  this->finput.open(inputFile, ios::in);
  this->fetchPrice();
  this->readData();
}

void Database::readData()
{
  string row;
  this->finput >> row; // ignore heading
  while (this->finput >> row)
  {
    rows.push_back(toRecord(row));
  }
}

void Database::fetchPrice()
{
  cout << "Please enter the product id, followed by its corresponding price\n";
  for (int i = 0; i < 8; i++)
  {
    int price;
    string _id;
    cout << "Enter the product id: \n";
    cin >> _id;
    cout << "Enter the price: \n";
    cin >> price;
    this->priceMap[_id] = price;
  }
}

void Database::calculateSales()
{
  for (auto record : this->rows)
  {
    salesCost[record.regionNo][record.salesManNo] += this->priceMap[record._id] * record.unitsSold;
  }
}

// 12,34,56,780--> n=9
// 5,3,1

string Database::formatCost(int num)
{
  string cost = to_string(num);
  string ans = "";
  int n = cost.size();
  int t = n - 4;
  for (int i = n - 1; i >= 0; i--)
  {
    if (i == t)
    {
      ans = "," + ans;
      ans = cost[i] + ans;
      t -= 2;
    }
    else
    {
      ans = cost[i] + ans;
    }
  }
  return ans;
}

void Database::displaySummary(string &outputFile)
{
  this->foutput.open(outputFile, ios::out);
  this->calculateSales();
  this->foutput << setw(45) << this->companyName << "\n";
  for (int i = 0; i < 90; i++)
  {
    this->foutput << "-";
  }
  this->foutput << "\n\n\n";
  int regSales;
  for (int i = 1; i <= 4; i++)
  {
    this->foutput << "Region " + to_string(i) << "\n";
    for (int i = 0; i < 90; i++)
    {
      this->foutput << "-";
    }
    this->foutput << "\n\n";
    regSales = 0;
    for (int j = 1; j <= 6; j++)
    {
      this->foutput << "Salesman " << j << "\t\t\t\t\t\t"
                    << "Rs. " << this->formatCost(this->salesCost[i][j]) << "/-\n\n";
      regSales += this->salesCost[i][j];
    }
    this->foutput << "Total sales at Region " + to_string(i) << "\t\t\t\t\t\t"
                  << "Rs. " << this->formatCost(regSales) << "/-\n";
    for (int i = 0; i < 90; i++)
    {
      this->foutput << "-";
    }
    this->foutput << "\n\n\n";
  }
}

string INPUT_FILE = "./data.csv";
string OUTPUT_FILE = "./output.txt";

int main()
{
  Database db("ABC Company", INPUT_FILE);
  db.displaySummary(OUTPUT_FILE);

  return 0;
}