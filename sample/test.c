#include <iostream>
#include <vector>
#include "KDataFrame.h"

int main (int argc, char **argv)
{
    KDataFrame df("./test.csv");
    
    // カラムの一覧を表示(名前、年齢、身長)
    std::cout<<"Key: "<<df.GetDFInfo().Keys()<<std::endl; 

    df.Scan(); // csv内のデータの情報を表で表示

    // データの取り出し
    // 文字列でいいとき
    auto Data0 =  df["Name"];
    for(auto &v : Data0) std::cout<<v<<", ";
    std::cout<<"\n";
    
    // 上と同じ意味
    // df["Name"] <=> df.GetcolumnStr("Name")
    auto Data1 = df.GetcolumnStr("Name");
    for(auto &v : Data1) std::cout<<v<<", ";
    std::cout<<"\n";

    std::cout<<"\n";

    // 数字として取り出したい(文字列をキャストして取り出したい)とき
    auto Data = df.Get<int>("Age");
    for(auto &v : Data) std::cout<<v<<", ";
    std::cout<<"\n";

    return 0;
}