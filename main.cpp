#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <cstdlib>
using namespace std;

// delete space and \n or something else
// by ChatGPT lmao
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \r\n\t");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \r\n\t");
    return str.substr(first, last - first + 1);
}

int main(){
    string filename, line;
    bool data = false;
    bool finans = true;
    /*
    題目編號定義為：(章節數)-(題號)
    例如第一章第二題即為 1-2
    */
    cout << "請輸入題目編號:";
    cin >> filename;

    string path = "problems/" + filename;
    
    if (filesystem::exists(path)){
        cout << "有該題資料:" << filename << endl;
        data = true;
    }else{
        cout << "無該題資料" << endl;
    }

    if (data){
        cout << "請輸入程式碼(換行並輸入q結束):";
        ofstream file("user_code.cpp");

        while(getline(cin, line)){
            if (line == "q"){
                break;
            }else{
                file << line << endl;
            }
            
        }

        int com = system("g++ user_code.cpp -o user_code");    // compile
        if (com != 0){
            cout << "編譯失敗!";
            return 1;
        }
        
        
        // cout << "test";

        // execute the program

        for (int i=1; i<=5; i++){
            string n = to_string(i);
            string input_path = path + "/input" + n + ".txt";
            string ans_path = path + "/answer" + n + ".txt";
            string command = "user_code.exe<" + input_path + ">output.txt";
            system(command.c_str());

            // check the answer and the user output
            string output, ans;
            ifstream outputfile("output.txt");
            ifstream ansoutput(ans_path);

            finans = true;
            while (getline(outputfile, output) && getline(ansoutput, ans)){
                // cout << "【DEBUG】" << trim(output) << " vs " << trim(ans) << endl;
                if (trim(output) != trim(ans)){
                    finans = false;
                    break;
                }
            }
            // check the number of line
            if ((getline(outputfile, output) || getline(ansoutput, ans))) {
                finans = false;
            }

            if (finans == true){
                cout << "第" << i << "筆測資:AC" << endl;
            }else{
                cout << "第" << i << "筆測資:WA" << endl;
            }

            // deal with next round
            ofstream clearfile("output.txt", ios::trunc);  // clean
            clearfile.close();
            finans = true;
        }


    }
    data = false;

}