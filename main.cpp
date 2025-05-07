#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <cstdlib>
using namespace std;

// delete space and \n or something else
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \r\n\t");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \r\n\t");
    return str.substr(first, last - first + 1);
}

void edit(string name){
    // idk how it works:( but i know the most important rule:
    // "if the code can work, then don't change it!!"
    bool creat = false;
    string path = "./problems/";
    if (!filesystem::exists(path + name)){
        filesystem::create_directory(path + name);
        cout << "===============================" << endl;
        cout << "題目建立中..." << endl;
        path += name + "/";
        creat = true;
    }else{
        cout << "該題已存在！" << endl;
    }

    if (creat){
        string nowinputpath, nowanspath, input, inputans;
        for (int i=1; i<=5; i++){
            string n = to_string(i);
            nowinputpath = path + "input" + n + ".txt";
            nowanspath = path + "answer" + n + ".txt";
            ofstream nowinputfile(nowinputpath);
            ofstream nowansfile(nowanspath);

            cout << "第" << i << "筆測資input(換行q結束):" << endl;
            while (getline(cin, input)){
                if (input != "q"){
                    nowinputfile << input << endl;
                }else{
                    break;
                }
            }

            cout << "第" << i << "筆測資答案(換行q結束):" << endl;
            while (getline(cin, inputans)){
                if (inputans != "q"){
                    nowansfile << inputans << endl;
                }else{
                    break;
                }
            }
        }
    }
    creat = false;
    

}

int main(){
    string filename, line;
    bool data = false;
    bool finans = true;
    cout << "請輸入題目編號:";
    cin >> filename;

    if (filename == "creat"){    // decide edit or just test the code
        cout << "請輸入題目名稱:";
        cin >> filename;
        edit(filename);
    }else{
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
}