
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

bool cmpr(char a, char b) {
  return a < b;
}

class Pack{
  public:
    Pack(string in) {
      int i = in.length();

      CmptA = in.substr(0,i >> 1);
      CmptB = in.substr((i >> 1),i);
      sort(CmptA.begin(), CmptA.end(), cmpr);
      sort(CmptB.begin(), CmptB.end(), cmpr);
    }

    const char Rummage() const {
      for(auto a : CmptA) {
        for(auto b: CmptB) {
          if(a == b) {
            return a;
          } else if( b > a) {
            break;
          }
        }
      }
      return 0;
    }

    const char Badges(const Pack& b, const Pack& c) const {
      string sA = CmptA + CmptB;
      string sB = b.CmptA + b.CmptB;
      string sC = c.CmptA + c.CmptB;

      sort(sA.begin(), sA.end(), cmpr);
      sort(sB.begin(), sB.end(), cmpr);
      sort(sC.begin(), sC.end(), cmpr);

      for(auto ca : sA) {
        for(auto cb : sB) {
          if(cb > ca) break;
          if( cb == ca) {
            for(auto cc : sC) {
              if(cc > cb) break;
              if(cc == cb) {
                return cc;
              }
            }
          }
        }
      }

      return 0;
    }

    string CmptA,CmptB;
};

inline static int Pri(char in) {
  return (in < 97) ? in - 38 : in - 96;
}

int main()
{
  vector<Pack> packs;

  string line;
  ifstream myfile;
  myfile.open("input.txt");

  while(getline(myfile, line)) {
    packs.push_back(Pack(line));
  }

  {// part 1
    int sum = 0;
    for(auto& p : packs) {
      const char t = p.Rummage();
      //cout << p.CmptA << " " << p.CmptB << " " << t << " " << Pri(t) << endl;
      sum += Pri(t);
    }
    cout << sum << endl;
  }

  { // part 2
    int sum = 0;
    for(int i=0; i<packs.size(); i+=3) {
      const char t = packs[i].Badges(packs[i+1], packs[i+2]);
      //cout << t << " " << Pri(t) << endl;
      sum += Pri(t);
    }
    cout << sum << endl;
  }

  return 0;
}
