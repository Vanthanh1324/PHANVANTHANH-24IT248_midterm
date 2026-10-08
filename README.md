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


## 10. Kiểm thử

Project có script:

    ./test_full.sh

Script kiểm tra:

- Biên dịch bằng Makefile.
- Các option `-A -a -c -d -F -f -h -i -k -l -n -q -R -r -S -s -t -u -w`.
- Các tổ hợp option.
- Các option có tính chất override như `-q/-w`, `-c/-u`, `-R/-d`, `-k/-h`, `-S/-t`.
- Sắp xếp xuôi/ngược, theo thời gian và kích thước.
- Nhiều file/thư mục operand.
- Đệ quy.
- File thường, executable, symbolic link và FIFO.
- File ẩn với `-a` và `-A`.
- Xử lý đường dẫn không tồn tại và option không hợp lệ.
- Kiểm tra output của `-l`, `-i` và output cơ bản.

Chạy:

    sh test_full.sh

## 11. Thiết kế chương trình

Chương trình được chia thành các module:

- `main.c`: xử lý operand, thư mục và đệ quy.
- `options.c`: phân tích command-line options.
- `fileinfo.c`: lấy thông tin file bằng `lstat()`.
- `sort.c`: sắp xếp theo tên, thời gian hoặc kích thước.
- `display.c`: định dạng output, long format, inode, block, owner/group và ký hiệu `-F`.

Các file header tương ứng nằm trong thư mục `include/`.

## 12. Tham khảo

Hành vi của chương trình được đối chiếu với yêu cầu trong đề bài Midterm Project và tài liệu `ls(1)` của NetBSD.
