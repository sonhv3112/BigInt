=============================================================================
                    Đồ án kỹ thuật lập trình: Big Integer
                            Sinh viên thực hiện: 
                         Hồ Văn Sơn, Trần Hữu Thiên 
=============================================================================

Nội dung cài đặt của các file: 

BigInt.h
	Header file: Khai báo struct BigInt, các hàm operator của BigInt và các
	hàm tính năng của BigInt. 

BigInt.cpp 
	Cài đặt các hàm đã được khai báo ở file header BigInt.h, bao gồm các
	phép tính: +, -, *, /, % và các hàm như pow, digits, to_base cho BigInt.

Command.h  
	Header file: Khai báo các hàm nhằm phân loại và thực hiện các phép tính 
	BigInt ở file input. 

Command.cpp 
	Cài đặt các hàm đã được khai báo ở file header Command.h. 

main.cpp 
	Chương trình chính, làm nhiệm vụ mở file input, output được truyền vào
	ở Command line và thực hiện các phép tính theo yêu cầu.

//////////////////////////////////////////////////////////////////////////////

Biên dịch chương trình: 

- Mở Command Prompt tại thư mục chứa các file mã nguồn của chương trình (folder 
Program). 
- Dùng lệnh dịch: "g++ -o 20120021_20120584.exe *.cpp -lpsapi" (không chứa dấu 
ngoặc kép). Ở đây do chương trình có sử dụng thư viện Psapi.h nên ta cần liên 
kết đến thư viện bằng lệnh "-lpsapi" như lệnh dịch trên. 

//////////////////////////////////////////////////////////////////////////////

Sử dụng chương trình dạng Command line: 

- Mở Command Prompt tại thư mục chứa các file thực thi của chương trình
(20120021_20120584.exe). 
- Dùng lệnh: "20120021_20120584.exe <input> <output>" với input và output lần 
lượt là file input và file output. 

//////////////////////////////////////////////////////////////////////////////

Các truy vấn có thể truyền vào file input: 

- Các tên gọi: 
	<base>, <base1>, <base2>: Hệ cơ số (bằng 10 là hệ thập phân, bằng 2 là
	hệ nhị phân).
	<x>, <y>: giá trị của toán hạng x, y (đúng với base). 
	<operator>: phép tính. 
	<function>: Tên các hàm hỗ trợ (abs, min, max, pow, digits, to_string, 
	to_base32, to_base58, to_base64, is_prime).

- Số nhị phân: Các số biểu diễn ở dạng nhị phân được biểu diễn theo dạng bù 2 
và quy ước rằng bit cao nhất là bit dấu. 

- Phép tính số học: "<base> <x> <operator> <y>" (không chứa dấu ngoặc kép), với
<operator> là một trong những phép tính thuộc tập {'-', '+', '*', '/', '%'}. 

- Lấy số đối: "<base> - <x>" (không chứa dấu ngoặc kép). 

- Phép toán thao tác bit: "<base> <x> <operator> <y>" hoặc "<base> ~ <x>" (không
chứa dấu ngoặc kép), với <operator> là một trong những phép tính thuộc tập 
{'&','^', '|', '<<', '>>'}.

- Các hàm hỗ trợ: "<base> <function>(<x>)" (không có dấu ngoặc kép). 
Ví dụ truy vấn "10 is_prime(5)" ở file input thì chương trình sẽ in ra true ở 
file output. 

- Chuyển đổi hệ cơ số: "<base1> <base2> <x>" (không có dấu ngoặc kép). Điều kiện: 
<base1> khác <base2> và <x> cần được biểu diễn dưới dạng <base1>. Chương trình 
sẽ tiến hành chuyển đổi hệ cơ số của <x> sang hệ <base2> và in ra file output. 

- Để xuất ra thời gian, bộ nhớ đã dùng hiện tại của chương trình ta dùng truy
vấn: "get_info" (không có dấu ngoặc kép).

//////////////////////////////////////////////////////////////////////////////



	



