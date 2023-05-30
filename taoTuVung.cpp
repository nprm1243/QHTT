#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <math.h>
#include <stdio.h>
#include <assert.h>
using namespace std;

struct phanSo
{
    long long tu, mau;
};

phanSo rutGon(phanSo x)
{
    int UCLN = __gcd(x.tu, x.mau);
    x.tu /= UCLN;
    x.mau /= UCLN;
    if (x.mau < 0) x.tu *= -1, x.mau *= -1;
    return x;
}

void quyDong(phanSo &x, phanSo &y)
{
    int UCLN = __gcd(x.mau, y.mau);
    int BCNN = x.mau * y.mau / UCLN;
    x = {x.tu * (BCNN / x.mau), BCNN};
    y = {y.tu * (BCNN / y.mau), BCNN};
}

phanSo fromInt(int x)
{
    phanSo ret = {x, 1};
    return ret;
}

phanSo operator+ (phanSo x, phanSo y)
{
    quyDong(x, y);
    phanSo res = {x.tu + y.tu, x.mau};
    return rutGon(res);
}

phanSo operator- (phanSo x, phanSo y)
{
    quyDong(x, y);
    phanSo res = {x.tu - y.tu, x.mau};
    return rutGon(res);
}

phanSo operator* (phanSo x, phanSo y)
{
    phanSo res = {x.tu * y.tu, x.mau * y.mau};
    return rutGon(res);
}

phanSo operator/ (phanSo x, phanSo y)
{
    phanSo res = {x.tu * y.mau, x.mau * y.tu};
    return rutGon(res);
}

phanSo abs(phanSo x)
{
    phanSo ret = {abs(x.tu), abs(x.mau)};
    return ret;
}

bool operator> (phanSo x, phanSo y)
{
    quyDong(x, y);
    return x.tu > y.tu;
}

bool operator< (phanSo x, phanSo y)
{
    quyDong(x, y);
    return x.tu < y.tu;
}

bool operator== (phanSo x, phanSo y)
{
    quyDong(x, y);
    return x.tu == y.tu;
}

bool operator!= (phanSo x, phanSo y)
{
    quyDong(x, y);
    return x.tu != y.tu;
}

bool operator>= (phanSo x, phanSo y)
{
    quyDong(x, y);
    return x.tu >= y.tu;
}

bool operator<= (phanSo x, phanSo y)
{
    quyDong(x, y);
    return x.tu <= y.tu;
}

ostream &operator<<(ostream &cout, const phanSo &a)
{
    if (a.mau == 0) printf("0");
    // else if (a.mau != 1) cout << R"(\frac{)" << a.tu << "}{" << a.mau << "}";
    else if (a.mau != 1) printf("%d/%d", a.tu, a.mau);
    else printf("%d", a.tu);
    return cout;
}

istream &operator>>(istream &iss, phanSo &a) 
{
    int tu = 0, mau = 0;
    string s;
    cin >> s;
    int i = 0;
    int am = 1;
    if (s[0] == '-') am = -1, i++;
    while (i < s.size() && s[i] != '/')
    {
        tu = tu * 10 + int(s[i] - '0');
        i++;
    }
    if (i == s.size())
    {
        a = {tu*am, 1};
        return a, iss;
    }
    else
    {
        i++;
        while (i < s.size() && s[i] != '/')
        {
            mau = mau * 10 + int(s[i] - '0');
            i++;
        }
        a = {tu * am, mau};
        return a, iss;
    }
}

// Struct để biểu diễn một ràng buộc
struct Constraint {
    vector<phanSo> coefficients;
    phanSo constant;
    string sense; // "<=", ">=", "="
};

// int BienDauVao[] = {1, 1}, soBien = 2;
// string dieuKienBien[] = {">=", "freedom"};

phanSo BienDauVao[1001];
int soBien, soRangBuoc;
string dang;
string dieuKienBien[1001];

// Hàm chuyển đổi các ràng buộc thành dạng chuẩn
vector<Constraint> chuyen_dang_chuan(vector<Constraint>& constraints) 
{
    vector<Constraint> chuan_constraints;
    // Xử lý biến
    // Xử lý biến <= 0
    for (int bien = 0; bien < soBien; bien++) if (dieuKienBien[bien] == "<=")
        for (int i=0; i< int(constraints.size()); i++)
        {
            constraints[i].coefficients[bien] = constraints[i].coefficients[bien] * fromInt(-1);
        }
    // Xử lý biến tự do
    int soLuongBienTuDo = 0, bienDangXet = 0;
    for (int i=0; i<soBien; i++) if (dieuKienBien[i] == "freedom") soLuongBienTuDo++;
    for (int bien = 0; bien < soBien; bien++) if (dieuKienBien[bien] == "freedom")
    {
        for (int i=0; i<int(constraints.size()); i++)
        {
            constraints[i].coefficients.push_back(fromInt(0));
        }
        for (int i=0; i<int(constraints.size()); i++)
        {
            for (int j=constraints[i].coefficients.size()-1; j>bien+bienDangXet; j--) 
                constraints[i].coefficients[j] = constraints[i].coefficients[j-1];
            constraints[i].coefficients[bien+1+bienDangXet] = constraints[i].coefficients[bien+1+bienDangXet] * fromInt(-1);
        }
        soBien++;
        for (int j=soBien-1; j>bien+bienDangXet; j--) 
            BienDauVao[j] = BienDauVao[j-1];
        BienDauVao[bien+1+bienDangXet] = BienDauVao[bien+1+bienDangXet] * fromInt(-1);
        bienDangXet++;
    }

    // Xử lý ràng buộc
    for (Constraint& constraint : constraints) {
        if (constraint.sense == ">=") {
            // Chuyển đổi ràng buộc ax >= b thành -ax <= -b
            Constraint chuan_constraint;
            chuan_constraint.coefficients = constraint.coefficients;
            for (phanSo& coefficient : chuan_constraint.coefficients) {
                coefficient = coefficient * fromInt(-1);
            }
            chuan_constraint.constant = constraint.constant * fromInt(-1);
            chuan_constraint.sense = "<=";
            
            chuan_constraints.push_back(chuan_constraint);
        }
        else if (constraint.sense == "=") {
            // Chuyển đổi ràng buộc ax = b thành hai ràng buộc ax <= b và -ax <= -b
            Constraint chuan_constraint1;
            chuan_constraint1.coefficients = constraint.coefficients;
            chuan_constraint1.constant = constraint.constant;
            chuan_constraint1.sense = "<=";
            
            Constraint chuan_constraint2;
            chuan_constraint2.coefficients = constraint.coefficients;
            for (phanSo& coefficient : chuan_constraint2.coefficients) {
                coefficient = coefficient * fromInt(-1);
            }
            chuan_constraint2.constant = constraint.constant * fromInt(-1);
            chuan_constraint2.sense = "<=";
            
            chuan_constraints.push_back(chuan_constraint1);
            chuan_constraints.push_back(chuan_constraint2);
            soRangBuoc++;
        }
        else {
            // Giữ nguyên ràng buộc nếu là dạng ax <= b
            chuan_constraints.push_back(constraint);
        }
    }
    // Xử lý max - min
    if (dang == "max")
    {
        // dang = "min";
        for (int i=0; i<soBien; i++) BienDauVao[i] = BienDauVao[i] * fromInt(-1);
    }
    
    return chuan_constraints;
}

int main() {

    freopen("test.inp", "r", stdin);
    freopen("tg.txt", "w", stdout);
    // Đọc data từ file
    string mode = "";
    bool isInTuVung = 0;

    cin >> mode;
    cin >> isInTuVung;
    cin >> soBien >> soRangBuoc;
    cin >> dang;

    int soBienThucTe = soBien;
    vector<Constraint> constraints;

    for (int i=0; i<soBien; i++) cin >> BienDauVao[i];
    for (int i=0; i<soRangBuoc; i++)
    {
        Constraint tmp;
        phanSo ps;
        for (int j=0; j<soBien; j++) 
        {
            cin >> ps;
            tmp.coefficients.push_back(ps);
        }
        cin >> tmp.sense;
        cin >> tmp.constant;
        constraints.push_back(tmp);
    }
    for (int i=0; i<soBien; i++) cin >> dieuKienBien[i];

    // Mảng các ràng buộc ban đầu
    // vector<Constraint> constraints = {
    //     {{1, 2}, 5, "<="},
    //     {{-1, 3}, 4, "<="},
    //     {{2, -1}, 3, "="}
    // };
    
    // Chuyển đổi thành dạng chuẩn
    vector<Constraint> chuan_constraints = chuyen_dang_chuan(constraints);
    
    cout << mode << endl;
    cout << isInTuVung << endl;
    cout << soBien << " " << soRangBuoc << " " << soBienThucTe << endl;
    cout << dang << endl;
    // In bai toan
    for (int i=0; i<soBien; i++) cout << BienDauVao[i] << " "; cout << endl;
    // In ra các ràng buộc chuẩn
    for (int i = 0; i < chuan_constraints.size(); ++i) {
        const Constraint& constraint = chuan_constraints[i];
        // cout << "Rang buoc thu " << i + 1 << ": ";

        cout << constraint.constant << " ";

        for (int j = 0; j < constraint.coefficients.size(); ++j) {
            cout << constraint.coefficients[j] * fromInt(-1) << " ";// << "x" << j + 1;
            // if (j != constraint.coefficients.size() - 1) {
            //     cout << " + ";
            // }
        }
        cout << endl;
    }
    for (int i=0; i<soBien; i++) cout << dieuKienBien[i] << endl;
    return 0;
}
