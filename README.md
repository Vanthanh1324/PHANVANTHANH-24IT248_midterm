# PHANVANTHANH-24IT248_midterm

## 1. Giới thiệu

Đây là bài Midterm Project: Implement ls(1).

Chương trình được viết bằng ngôn ngữ C và chạy trên NetBSD 10.1.

## 2. Cấu trúc project

PHANVANTHANH-24IT248_midterm/
├── README.md
├── Makefile
├── .gitignore
├── include/
│   ├── display.h
│   ├── fileinfo.h
│   ├── options.h
│   └── sort.h
└── src/
    ├── main.c
    ├── options.c
    ├── fileinfo.c
    ├── sort.c
    └── display.c

## 3. Biên dịch

Sử dụng Makefile:

    make

Sau khi biên dịch, chương trình thực thi có tên:

    ls

## 4. Xóa file biên dịch

    make clean

Lệnh này xóa các file object và chương trình ls.

## 5. Cách sử dụng

Cú pháp:

    ./ls [options] [file ...]

Nếu không truyền file hoặc thư mục:

    ./ls

Hiển thị nội dung thư mục hiện tại.

Ví dụ:

    ./ls src

Hiển thị nội dung thư mục src.

## 6. Các option

-A    Hiển thị file ẩn, ngoại trừ . và ..
-a    Hiển thị tất cả file, bao gồm . và ..
-c    Sử dụng thời gian thay đổi trạng thái file
-d    Hiển thị thư mục như một file
-F    Thêm ký hiệu phân loại file
-f    Không sắp xếp
-h    Hiển thị kích thước dễ đọc
-i    Hiển thị inode
-k    Hiển thị kích thước theo KB
-l    Hiển thị thông tin dạng long format
-n    Hiển thị UID/GID dạng số
-q    Thay ký tự không in được bằng ?
-R    Hiển thị đệ quy các thư mục
-r    Đảo ngược thứ tự sắp xếp
-S    Sắp xếp theo kích thước
-s    Hiển thị số block
-t    Sắp xếp theo thời gian
-u    Sử dụng thời gian truy cập
-w    Hiển thị ký tự không in được ở dạng raw

## 7. Ví dụ sử dụng

Hiển thị file:

    ./ls

Hiển thị file ẩn:

    ./ls -a

Long format:

    ./ls -l

Hiển thị inode:

    ./ls -i

Sắp xếp ngược:

    ./ls -r

Sắp xếp theo thời gian:

    ./ls -t

Sắp xếp theo kích thước:

    ./ls -S

Hiển thị đệ quy:

    ./ls -R

Kết hợp nhiều option:

    ./ls -lR

    ./ls -lah

## 8. Môi trường phát triển

Operating System: NetBSD 10.1
Compiler: GCC
Programming Language: C
Build tool: Make

## 9. Tác giả

PHAN VAN THANH

Mã sinh viên: 24IT248
