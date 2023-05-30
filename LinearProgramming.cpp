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

int soBien, soDieuKien, viTri[1001], soBienThucTe;
string mode, dieuKienBien[1001], dang;
bool isInTuVung, khongCoNghiem;
phanSo tuVung[1001][1001], tuVungPhu[1001][1001];
string bienCoSo[1001], bienKhongCoSo[1001], bienTam[1001];

int layGiaTri(string s)
{
    int x = 0;
    while (s[x] >= 'a' && s[x] <= 'z') x++;
    int ret = 0;
    for (int i=x; i<s.size(); i++)
    {
        ret = ret*10 + int(s[i] - '0');
    }
    return ret;
}

void inTuVung()
{
    if (!isInTuVung) return;
    cout << "z = " << tuVung[0][0] << " ";
    for (int j=1; j<=soBien; j++) cout << tuVung[0][j] << bienKhongCoSo[j] << " "; cout << endl;
    for (int i=1; i<=soDieuKien; i++)
    {
        cout << bienCoSo[i] << " = ";
        for (int j=0; j<=soBien; j++) cout << tuVung[i][j] << bienKhongCoSo[j] << " "; cout << endl;
    } 
}

bool kiemTraTuVung()
{
    for (int j=1; j<=soBien; j++) if (tuVung[0][j] < fromInt(0)) return true;
    return false;
}

int timBienVao()
{
    int idx = 0;
    phanSo val = fromInt(0);
    for (int j=1; j<=soBien; j++) if (tuVung[0][j] < fromInt(0) && tuVung[0][j] < val)
    {
        val = tuVung[0][j];
        idx = j;
    }
    return idx;
}

void TaoTuVungPhu()
{
    for (int i=0; i<=soDieuKien; i++) for (int j=0; j<=soBien; j++) tuVungPhu[i][j] = tuVung[i][j];
}

int timBienVaoBland()
{
    int idx = 0;
    string label = "zz";
    for (int j=1; j<=soBien; j++) if (tuVung[0][j] < fromInt(0) && bienKhongCoSo[j] < label)
    {
        idx = j;
        label = bienKhongCoSo[j];
    }
    return idx;
}

int timBienRa(int bienVao)
{
    int idx = 0;
    phanSo val = fromInt(1000000007);
    for (int i=1; i<=soDieuKien; i++)
    {
        if (tuVung[i][bienVao] < fromInt(0))
        {
            phanSo tmp = abs(tuVung[i][0] / tuVung[i][bienVao]);
            if (tmp < val)
            {
                val = tmp;
                idx = i;
            }
            else if (tmp == val && layGiaTri(bienCoSo[i]) == 0)
            {
                val = tmp;
                idx = i;
            }
        }
    }
    return idx;
}

int timBienRaPhaMot(int bienVao)
{
    int idx = 0;
    phanSo val = fromInt(1000000007);
    for (int i=1; i<=soDieuKien; i++)
    {
        if (tuVung[i][0] < val)
        {
                val = tuVung[i][0];
                idx = i;
        }
    }
    return idx;
}

void xoay(int bienVao, int bienRa)
{
    vector<phanSo>danhSachThayDoi(soDieuKien+2, fromInt(0));

    // Xoay
    for (int i=0; i<=soDieuKien; i++) if (i != bienRa)
    {
        phanSo thayDoi = tuVung[i][bienVao] / tuVung[bienRa][bienVao];
        danhSachThayDoi[i] = thayDoi;
        if (thayDoi != fromInt(0))
        {
            for (int j= 0; j<=soBien; j++)  
            {
                //cout << tuVung[i][j] << endl;
                //cout << tuVung[bienRa][j] * thayDoi  << endl;
                tuVung[i][j] = tuVung[i][j] - tuVung[bienRa][j] * thayDoi;
            }
        }
    }

    // Xu ly hang bien ra
    phanSo thayDoi = tuVung[bienRa][bienVao] * fromInt(-1);
    for (int j=0; j<=soBien; j++) tuVung[bienRa][j] = tuVung[bienRa][j] / thayDoi;
    danhSachThayDoi[bienRa] = fromInt(-1) / thayDoi;

    // Dich cac cot qua trai
    if (bienVao != soBien)
    {
        for (int i=0; i<=soDieuKien; i++)
        {
            for (int j=bienVao+1; j<=soBien; j++) tuVung[i][j-1] = tuVung[i][j];
        }
    }

    // Thay doi cot cuoi
    for (int i=0; i<=soDieuKien; i++) tuVung[i][soBien] = danhSachThayDoi[i];
    string tmp = bienKhongCoSo[bienVao];
    for (int j=bienVao+1; j<=soBien; j++) bienKhongCoSo[j-1] = bienKhongCoSo[j];
    bienKhongCoSo[soBien] = bienCoSo[bienRa];
    bienCoSo[bienRa] = tmp;
}

void donHinhDantzig()
{
    cout << "DON HINH DANTZIG" << endl;
    cout << "------------------------------------------------" << endl;
    if (isInTuVung) cout << "Tu vung xuat phat:" << endl;
    inTuVung();
    int soLanLap = 0;
    while (kiemTraTuVung())
    {
        int bienVao = timBienVao();
        int bienRa = timBienRa(bienVao);
        if (!bienRa)
        {
            cout << R"(BAI TOAN KHONG GIOI NOI, $\min z = - \infty$)" << endl;
            khongCoNghiem = true;
            return;
        }
        xoay(bienVao, bienRa);
        if (isInTuVung) cout << endl;
        inTuVung();
        soLanLap++;
        // if (soLanLap >= 10)
        // {
        //     cout << "KHONG DUNG CACH NAY DUOC";
        //     break;
        // }
    }
}

void donHinhBland()
{
    cout << "DON HINH BLAND" << endl;
    cout << "------------------------------------------------" << endl;
    if (isInTuVung) cout << "Tu vung xuat phat:" << endl;
    inTuVung();
    int soLanLap = 0;
    while (kiemTraTuVung())
    {
        int bienVao = timBienVaoBland();
        int bienRa = timBienRa(bienVao);
        if (!bienRa)
        {
            cout << R"(BAI TOAN KHONG GIOI NOI, $\min z = - \infty$)" << endl;
            khongCoNghiem = true;
            return;
        }
        xoay(bienVao, bienRa);
        if (isInTuVung) cout << endl;
        inTuVung();
        soLanLap++;
        // if (soLanLap >= 10)
        // {
        //     cout << "KHONG DUNG CACH NAY DUOC";
        //     break;
        // }
    }
}

void donHinhHaiPha()
{
    cout << "DON HINH HAI PHA" << endl;
    cout << "------------------------------------------------" << endl;
    if (isInTuVung) cout << "Tu vung xuat phat:" << endl;
    inTuVung();
    if (isInTuVung) cout << endl << "PHA 1:" << endl;
    TaoTuVungPhu();
    for (int j=0; j<=soBien; j++) tuVung[0][j] = fromInt(0);
    soBien++;
    bienKhongCoSo[soBien] = "x0";
    for (int i=0; i<=soDieuKien; i++) tuVung[i][soBien] = fromInt(1);
    inTuVung();
    int bienRa = timBienRaPhaMot(soBien);
    xoay(soBien, bienRa);
    if (isInTuVung) cout << endl;
    inTuVung();
    while (kiemTraTuVung())
    {
        int bienVao = timBienVao();
        int bienRa = timBienRa(bienVao);
        xoay(bienVao, bienRa);
        if (isInTuVung) cout << endl;
        inTuVung();
    }

    for (int j=0; j<soBien; j++) if (tuVung[0][j] != fromInt(0))
    {
        cout << endl << "BAI TOAN VO NGHIEM" << endl;
        khongCoNghiem = true;
        return;
    }

    if (isInTuVung) cout << endl << "PHA 2:" << endl;
    soBien--;
    for (int i=1; i<=soDieuKien; i++) if (bienCoSo[i][0] == 'x') viTri[layGiaTri(bienCoSo[i])] = i;
    for (int i=1; i<=soBien; i++) if (viTri[i])
    {
        int loc = viTri[i];
        for (int j=0; j<=soBien; j++) tuVung[0][j] = tuVung[0][j] + tuVungPhu[0][i] * tuVung[loc][j];
    }
    for (int i=1; i<=soBien; i++) if (!viTri[i])
    {
        int loc = 1;
        vector<int>tmp(soBien+2, 0);
        for (int j=1; j<=soBien; j++) if (bienKhongCoSo[j][0] == 'x') if (layGiaTri(bienKhongCoSo[j]) == i) 
        {
            tmp[j] = 1;
            break;
        }
        // for (int j=0; j<=soBien; j++) cout << tmp[j] << " "; cout << endl;
        for (int j=0; j<=soBien; j++) tuVung[0][j] = tuVung[0][j] + tuVungPhu[0][i] * fromInt(tmp[j]);
    }
    if (isInTuVung) cout << endl;
    inTuVung();

    while (kiemTraTuVung())
    {
        int bienVao = timBienVao();
        int bienRa = timBienRa(bienVao);
        if (bienRa == 0)
        {
            cout << "BAI TOAN KHONG GIOI NOI; min = -inf" << endl;
            khongCoNghiem = true;
            return;
        }
        xoay(bienVao, bienRa);
        if (isInTuVung) cout << endl;
        inTuVung();
    }
}

bool isCoNghiem()
{
    for (int j=1; j<=soBien; j++) if (tuVung[0][j] <= fromInt(0)) return false;
    return true;
}

void ketLuanNghiem()
{
    vector<phanSo> nghiem(soBien+2, fromInt(0));
    vector<phanSo> nghiemReal(soBien+2, fromInt(0));
    vector<int> vsn(soBien+2, 0);
    vector<vector<phanSo> > vsnn;
    vector<phanSo> huhu;
    bool btvsn = false;
    int n = max(soBien, soDieuKien) + 2;
    for (int i=1; i<=n; i++) huhu.push_back(fromInt(0));
    for (int i=1; i<=n; i++) vsnn.push_back(huhu);
    if (isCoNghiem && !khongCoNghiem)
    {
        for (int i=1; i<=soBien; i++)
        {
            int x = layGiaTri(bienKhongCoSo[i]);
            if(tuVung[0][i] == fromInt(0))
            {
                vsn[x] = 1;
                btvsn = true;
            }
        }
        for (int i=1; i<=soDieuKien; i++) if (bienCoSo[i][0] == 'x')
        {
            nghiem[layGiaTri(bienCoSo[i])] = tuVung[i][0];
            int x = layGiaTri(bienCoSo[i]);
            for (int j=1; j<=soBien; j++) if (tuVung[0][j] == fromInt(0)) vsnn[x][j] = tuVung[i][j]; 
        }
        phanSo ret = fromInt(0);
        for (int i=1; i<=soBien; i++) ret = ret + nghiem[i] * tuVungPhu[0][i];
        int x = 1;
        for (int i=1; i<=soBienThucTe; i++, x++)
        {
            if (dieuKienBien[x] == ">=") nghiemReal[i] = nghiem[x];
            else if (dieuKienBien[x] == "<=")
            {
                nghiemReal[i] = nghiem[x] * fromInt(-1);
                for (int j=1; j<=soBien; j++) vsnn[x][j] = vsnn[x][j] * fromInt(-1);
            }
            else
            {
                nghiemReal[i] = nghiem[x] + nghiem[x+1];
                // Cong don vsnn
                for (int j=1; j<=soBien; j++) vsnn[i][j] = vsnn[i][j] + vsnn[i+1][j];
                // Don vsnn len
                for (int k=i+1; k<n-1; k++) for (int j=1; j<=soBien; j++) vsnn[k][j] = vsnn[k+1][j];
                x++;
            }
        }

        for (int i=1; i<=soBienThucTe; i++) if (vsn[i])
        {
            for (int j=1; j<=soBien; j++) if (vsnn[i][j] != fromInt(0))
            {
                vsn[i] = 0;
                break;
            }
        }

        if (soBien > soBienThucTe || btvsn)
        {
            cout << "Nghiem bai toan ban dau" << endl;
            for (int i=1; i<=soBienThucTe; i++) if (!vsn[i])
            {
                cout << "x" << i << " = " << nghiemReal[i];
                for (int j=1; j<=soBien; j++) if (vsnn[i][j] != fromInt(0))
                {
                    if (vsnn[i][j] < fromInt(0)) cout << " " << vsnn[i][j] << bienKhongCoSo[j];
                    else cout << " +" << vsnn[i][j] << bienKhongCoSo[j];
                }
                cout << endl;
            }
        }
        else
        {
            cout << "Nghiem cua bai toan:" << endl;
            for (int i=1; i<=soBien; i++) cout << "x" << i << " = " << nghiem[i] << endl;
        }
        cout << "Gia tri toi uu cua bai toan la: " << (dang == "min" ? ret : ret * fromInt(-1)) << endl;
    }
}

int main()
{
    freopen("tg.txt", "r", stdin);
    freopen("test.out", "w", stdout);

    cin >> mode;
    cin >> isInTuVung;

    cin >> soBien >> soDieuKien >> soBienThucTe;
    cin >> dang;
    tuVung[0][0] = fromInt(0);
    for (int j=1; j<=soBien; j++) cin >> tuVung[0][j];
    for (int i=1; i<=soDieuKien; i++)
    {
        for (int j=0; j<=soBien; j++) cin >> tuVung[i][j];
    }
    for (int i=1; i<=soBien; i++) bienTam[i] = bienKhongCoSo[i] = "x" + to_string(i) + "";
    for (int i=1; i<=soDieuKien; i++) bienCoSo[i] = "w" + to_string(i) + "";
    for (int i=1; i<=soBien; i++) cin >> dieuKienBien[i];

    TaoTuVungPhu();
    if (mode == "Dantzig") donHinhDantzig();
    else if (mode == "Bland") donHinhBland();
    else if (mode == "2-pha") donHinhHaiPha();
    else
    {
        phanSo minOfb = fromInt(100);
        phanSo maxOfb = fromInt(-100);
        bool isZero = 0;

        for (int i=1; i<=soDieuKien; i++) 
        {
            minOfb = min(minOfb, tuVung[i][0]);
            maxOfb = max(minOfb, tuVung[i][0]);
            isZero = (tuVung[i][0] == fromInt(0));
        }

        if (minOfb < fromInt(0)) donHinhHaiPha();
        else if (isZero == 1) donHinhBland();
        else donHinhDantzig();
    }

    ketLuanNghiem();

    // cin >> soBien >> soDieuKien;
    // // tuVung[0][0] = fromInt(0);
    // for (int j=0; j<=soBien; j++) cin >> tuVung[0][j];
    // for (int i=1; i<=soDieuKien; i++)
    // {
    //     for (int j=0; j<=soBien; j++) cin >> tuVung[i][j];
    // }

    // for (int i=0; i<=soDieuKien; i++) 
    // {
    //     for (int j=0; j<=soBien; j++) cout << tuVung[i][j] << " "; cout << endl;
    // }
    // isInTuVung = true;
    // xoay(1, 1);
    // inTuVung();
}