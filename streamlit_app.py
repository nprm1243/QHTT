import streamlit as st
import time
import os

st.title('Linear Programming Solver')

col1, col2 = st.columns(2)

with col1:
    soBien = st.slider('Chọn số biến', 0, 100, 2)
with col2:
    soRangBuoc = st.slider('Chọn số ràng buộc', 0, 100, 2)

st.markdown("## Đầu vào")
baiToan = st.text_area("Nhập bài toán:", """min 1x1 +1x2
1x1 +0x2  <= 4
1x1 -1x2 <= 6
x1 >= 0
x2 freedom""", height = 200)

col1, col2 = st.columns(2)
with col1:
    choice = st.radio("Lựa chọn thuật toán sử dụng", ("Auto", "Dantzig", "Bland", "2-pha"))
with col2:
    inTuVung = st.radio("Lựa chọn xuất từ vựng", ("có", "không"))

run = False

def listToString(ls):
    res = f"{ls[0]}"
    for _ in range(1, len(ls)):
        res += f" {ls[_]}"
    res += '\n'
    return res

if st.button("Chạy"):
    baiToan = baiToan.split('\n')
    # split bai toan
    tmp = baiToan[0].split(' ')
    dang = tmp[0]
    heSoBaiToan = []
    for _ in range(1, soBien+1):
        print(tmp[_].split('x')[0])
        heSoBaiToan.append(int(tmp[_].split('x')[0]))
    # split cac rang buoc
    danhSachRangBuoc = []
    for _ in range(1, soRangBuoc+1):
        rangBuoc = []
        for item in baiToan[_].split(' '):
            try:
                rangBuoc.append(int(item.split('x')[0]))
            except:
                rangBuoc.append(item.split('x')[0])
        danhSachRangBuoc.append(rangBuoc)
    # split rang buoc dau
    danhSachRangBuocDau = []
    for _ in range(soRangBuoc+1, len(baiToan)):
        try:
            danhSachRangBuocDau.append(baiToan[_].split(' ')[1])
        except:
            danhSachRangBuocDau.append(baiToan[_].split(' ')[0])
    fo = open("test.inp", "w")
    fo.write(f"{choice}\n")
    if (inTuVung == "có"):
        fo.write(f"1\n")
    else:
        fo.write(f"0\n")
    fo.write(f"{soBien} {soRangBuoc}\n")
    fo.write(f"{dang}\n")
    fo.write(listToString(heSoBaiToan))
    for item in danhSachRangBuoc:
        fo.write(listToString(item))
    for item in danhSachRangBuocDau:
        fo.write(f"{item}\n")
    fo.close()
    os.startfile("taoTuVung.exe")
    time.sleep(1)
    os.startfile("LinearProgramming.exe")
    time.sleep(1)
    run = True
else:
    pass

st.markdown("## Kết quả")

if (run):
    fi = open("test.out", "rU")
    result = fi.read()
    fi.close()
    # for item in result:
    #     st.text(item)
    st.text(result)